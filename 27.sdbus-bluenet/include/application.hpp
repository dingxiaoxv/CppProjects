#pragma once
#include <memory>
#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <vector>

class Service;

class Application {
public:
  Application(sdbus::IConnection &connection, const std::string &objectPath);
  ~Application();

  void addService(std::shared_ptr<Service> service);
  void registerToBlueZ(const std::string &adapterPath);

  std::string getPath() const { return objectPath_; }

private:
  std::string objectPath_;
  std::unique_ptr<sdbus::IObject> object_;
  std::vector<std::shared_ptr<Service>> services_;
  sdbus::IConnection &connection_;

  // ObjectManager接口实现
  std::map<sdbus::ObjectPath,
           std::map<std::string, std::map<std::string, sdbus::Variant>>>
  getManagedObjects();
};
