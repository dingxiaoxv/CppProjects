#pragma once
#include <memory>
#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <vector>

class Advertisement {
public:
  Advertisement(sdbus::IConnection &connection, const std::string &objectPath,
                const std::string &type = "peripheral");
  ~Advertisement();

  void setLocalName(const std::string &name);
  void addServiceUUID(const std::string &uuid);
  void registerToBlueZ(const std::string &adapterPath);
  void unregister();

  std::string getPath() const { return objectPath_; }

private:
  std::string objectPath_;
  std::unique_ptr<sdbus::IObject> object_;
  sdbus::IConnection &connection_;

  std::string type_;
  std::string localName_;
  std::vector<std::string> serviceUUIDs_;
  std::vector<std::string> includes_;

  std::unique_ptr<sdbus::IProxy> advManagerProxy_;
};
