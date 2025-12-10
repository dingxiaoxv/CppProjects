#include <getopt.h>
#include <sdbus-c++/sdbus-c++.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <optional>
#include <string>

#include "dbus/usermanager-client.h"

struct UserOptions {
  std::string name;
  uint32_t age;
  std::string email;
};

std::optional<UserOptions> parseCommandLineArgs(int argc, char *argv[]) {
  UserOptions opts;
  opts.age = 0;

  const char *short_options = "n:a:e:h";
  const struct option long_options[] = {{"name", required_argument, nullptr, 'n'},
                                        {"age", required_argument, nullptr, 'a'},
                                        {"email", required_argument, nullptr, 'e'},
                                        {"help", no_argument, nullptr, 'h'},
                                        {nullptr, 0, nullptr, 0}};

  int opt;
  while ((opt = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
    switch (opt) {
    case 'n':
      opts.name = optarg;
      break;
    case 'a':
      try {
        opts.age = static_cast<uint32_t>(std::stoul(optarg));
      } catch (const std::exception &e) {
        spdlog::error("Invalid age: {}", optarg);
        return std::nullopt;
      }
      break;
    case 'e':
      opts.email = optarg;
      break;
    case 'h':
      spdlog::info("Usage: {} [OPTIONS]\n"
                   "Options:\n"
                   "  -n, --name NAME      User name\n"
                   "  -a, --age AGE        User age\n"
                   "  -e, --email EMAIL    User email\n"
                   "  -h, --help           Show this help message\n",
                   argv[0]);
      return std::nullopt;
    default:
      spdlog::info("Use --help for usage information");
      return std::nullopt;
    }
  }

  if (opts.name.empty() || opts.age == 0 || opts.email.empty()) {
    spdlog::error("Error: --name, --age, and --email are required");
    spdlog::info("Use --help for usage information");
    return std::nullopt;
  }

  return opts;
}

class UserManagerProxy : public sdbus::ProxyInterfaces<com::example::UserManager_proxy> {
public:
  UserManagerProxy(sdbus::IConnection &connection, sdbus::ServiceName destination,
                   sdbus::ObjectPath path)
    : ProxyInterfaces(connection, destination, std::move(path)) {
    registerProxy();
  }

  ~UserManagerProxy() { unregisterProxy(); }

  void onUserAdded(const uint64_t &userId, const std::string &username) override {
    spdlog::info("Signal received: UserAdded - ID: {}, Name: {}", userId, username);
  }
};

int main(int argc, char *argv[]) {
  spdlog::set_default_logger(spdlog::stdout_color_mt("user_manager"));
  spdlog::set_level(spdlog::level::info);

  auto options = parseCommandLineArgs(argc, argv);
  if (!options) {
    return 1;
  }

  auto connection = sdbus::createSessionBusConnection();
  sdbus::ServiceName destination{"com.example.UserManager"};
  sdbus::ObjectPath objectPath{"/com/example/UserManager"};
  auto managerProxy =
      std::make_unique<UserManagerProxy>(*connection, std::move(destination), std::move(objectPath));
  uint64_t userId = managerProxy->AddUser(options->name, options->age, options->email);
  spdlog::info("Added user with ID: {}", userId);
  connection->enterEventLoop();
  return 0;
}