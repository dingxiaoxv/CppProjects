#include "application.hpp"
#include "characteristic.hpp"
#include "service.hpp"
#include <iostream>

Application::Application(sdbus::IConnection &connection,
                         const std::string &objectPath)
    : objectPath_(objectPath), connection_(connection) {

  object_ = sdbus::createObject(connection_, sdbus::ObjectPath(objectPath_));

  // 使用内置的ObjectManager支持
  object_->addObjectManager();

  std::cout << "Application created at " << objectPath_ << std::endl;
}

Application::~Application() {
  std::cout << "Application destroyed" << std::endl;
}

void Application::addService(std::shared_ptr<Service> service) {
  services_.push_back(service);
  std::cout << "Service added: " << service->getUUID() << std::endl;
}

void Application::registerToBlueZ(const std::string &adapterPath) {
  try {
    auto gattManagerProxy =
        sdbus::createProxy(connection_, sdbus::ServiceName("org.bluez"),
                          sdbus::ObjectPath(adapterPath));

    gattManagerProxy->callMethod("RegisterApplication")
        .onInterface("org.bluez.GattManager1")
        .withArguments(sdbus::ObjectPath(objectPath_),
                      std::map<std::string, sdbus::Variant>{});

    std::cout << "Application registered to BlueZ at " << adapterPath
              << std::endl;

  } catch (const sdbus::Error &e) {
    std::cerr << "Failed to register application: " << e.what() << std::endl;
    throw;
  }
}

std::map<sdbus::ObjectPath,
         std::map<std::string, std::map<std::string, sdbus::Variant>>>
Application::getManagedObjects() {
  std::cout << "GetManagedObjects called, services count: " << services_.size() << std::endl;

  std::map<sdbus::ObjectPath,
           std::map<std::string, std::map<std::string, sdbus::Variant>>>
      objects;

  // 添加所有服务
  for (const auto &service : services_) {
    std::map<std::string, std::map<std::string, sdbus::Variant>> interfaces;

    // 添加GattService1接口
    interfaces["org.bluez.GattService1"] = service->getProperties();

    objects[sdbus::ObjectPath(service->getPath())] = interfaces;
    std::cout << "  Added service: " << service->getPath() << std::endl;

    // 添加所有特征
    for (const auto &chrc : service->getCharacteristics()) {
      std::map<std::string, std::map<std::string, sdbus::Variant>>
          chrcInterfaces;
      chrcInterfaces["org.bluez.GattCharacteristic1"] = chrc->getProperties();
      objects[sdbus::ObjectPath(chrc->getPath())] = chrcInterfaces;
      std::cout << "    Added characteristic: " << chrc->getPath() << std::endl;
    }
  }

  std::cout << "GetManagedObjects returning " << objects.size() << " objects" << std::endl;
  return objects;
}
