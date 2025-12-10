#include <sdbus-c++/sdbus-c++.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "dbus/usermanager-server.h"

class UserManagerAdaptor final : public sdbus::AdaptorInterfaces<com::example::UserManager_adaptor> {
  using UserInfo = sdbus::Struct<std::string, uint32_t, std::string>;
  using ConfigInfo = std::map<std::string, sdbus::Variant>;

public:
  explicit UserManagerAdaptor(sdbus::IConnection &connection, sdbus::ObjectPath path)
    : AdaptorInterfaces(connection, std::move(path)) {
    registerAdaptor();
  }

  ~UserManagerAdaptor() { unregisterAdaptor(); }

  uint64_t AddUser(const std::string &username, const uint32_t &age, const std::string &email) override {
    uint64_t userId = 1900 + ++userCount_;
    users_.emplace(userId, UserInfo{username, age, email});
    spdlog::info("Added user {}: {} ({}, {})", userId, username, age, email);
    emitUserAdded(userId, username);
    return userId;
  }

  UserInfo GetUser(const uint64_t &userId) override { return users_.at(userId); }

  std::map<uint64_t, UserInfo> GetAllUsers() override { return users_; }

  void UpdateUserTags(const uint64_t &userId, const std::vector<sdbus::Variant> &tags) override {
    auto it = users_.find(userId);
    if (it == users_.end()) {
      spdlog::error("User ID {} not found", userId);
      return;
    }

    if (tags.size() != 3) {
      spdlog::error("Expected 3 tags (username, age, email), got {}", tags.size());
      return;
    }

    try {
      std::string username = tags[0].get<std::string>();
      uint32_t age = tags[1].get<uint32_t>();
      std::string email = tags[2].get<std::string>();

      it->second = UserInfo{username, age, email};
      spdlog::info("Updated user {}: {} ({}, {})", userId, username, age, email);
    } catch (const std::exception &e) {
      spdlog::error("Failed to convert tags to UserInfo: {}", e.what());
    }
  }

  uint32_t UserCount() override { return userCount_; }

  std::map<std::string, sdbus::Variant> Config() override { return config_; }

  void Config(const std::map<std::string, sdbus::Variant> &value) override { config_ = value; }

private:
  std::map<uint64_t, UserInfo> users_;
  uint32_t userCount_ = 0;
  ConfigInfo config_;
};

int main() {
  spdlog::set_default_logger(spdlog::stdout_color_mt("user_manager"));
  spdlog::set_level(spdlog::level::info);

  sdbus::ServiceName serviceName{"com.example.UserManager"};
  auto connection = sdbus::createSessionBusConnection(serviceName);
  auto manager =
      std::make_unique<UserManagerAdaptor>(*connection, sdbus::ObjectPath{"/com/example/UserManager"});
  spdlog::info("UserManager D-Bus service is running...");
  connection->enterEventLoop();

  return 0;
}