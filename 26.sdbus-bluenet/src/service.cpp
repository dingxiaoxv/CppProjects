#include "service.hpp"
#include "characteristic.hpp"
#include <iostream>

Service::Service(sdbus::IConnection &connection, const std::string &objectPath,
                 const std::string &uuid, bool primary)
    : objectPath_(objectPath), uuid_(uuid), primary_(primary),
      connection_(connection) {

  object_ = sdbus::createObject(connection_, objectPath_);

  // 注册GattService1接口的属性
  const char *serviceInterface = "org.bluez.GattService1";

  object_->registerProperty(serviceInterface, "UUID", "s")
      .onInterface(serviceInterface)
      .withGetter([this]() { return uuid_; });

  object_->registerProperty(serviceInterface, "Primary", "b")
      .onInterface(serviceInterface)
      .withGetter([this]() { return primary_; });

  // Characteristics属性会在运行时动态获取
  object_->registerProperty(serviceInterface, "Characteristics", "ao")
      .onInterface(serviceInterface)
      .withGetter([this]() {
        std::vector<sdbus::ObjectPath> paths;
        for (const auto &chrc : characteristics_) {
          paths.push_back(sdbus::ObjectPath(chrc->getPath()));
        }
        return paths;
      });

  object_->finishRegistration();
  std::cout << "Service created: " << uuid_ << " at " << objectPath_
            << std::endl;
}

Service::~Service() {
  std::cout << "Service destroyed: " << uuid_ << std::endl;
}

void Service::addCharacteristic(
    std::shared_ptr<Characteristic> characteristic) {
  characteristics_.push_back(characteristic);
  std::cout << "Characteristic added to service: " << characteristic->getUUID()
            << std::endl;
}

std::map<std::string, sdbus::Variant> Service::getProperties() const {
  std::map<std::string, sdbus::Variant> props;

  props["UUID"] = uuid_;
  props["Primary"] = primary_;

  std::vector<sdbus::ObjectPath> chrcPaths;
  for (const auto &chrc : characteristics_) {
    chrcPaths.push_back(sdbus::ObjectPath(chrc->getPath()));
  }
  props["Characteristics"] = chrcPaths;

  return props;
}
