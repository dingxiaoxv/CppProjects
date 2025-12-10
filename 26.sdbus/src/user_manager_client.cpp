#include <getopt.h>
#include <sdbus-c++/sdbus-c++.h>
#include <spdlog/spdlog.h>

#include <optional>
#include <string>

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

int main(int argc, char *argv[]) {
  auto options = parseCommandLineArgs(argc, argv);
  if (!options) {
    return 1;
  }

  return 0;
}