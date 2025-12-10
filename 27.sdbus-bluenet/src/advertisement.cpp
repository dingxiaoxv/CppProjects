#include "advertisement.hpp"
#include <iostream>

Advertisement::Advertisement(sdbus::IConnection &connection,
                             const std::string &objectPath,
                             const std::string &type)
    : objectPath_(objectPath), connection_(connection), type_(type) {

  object_ = sdbus::createObject(connection_, sdbus::ObjectPath(objectPath_));

  // 默认包含tx-power
  includes_.push_back("tx-power");

  // 注册LEAdvertisement1接口的属性
  const char *advInterface = "org.bluez.LEAdvertisement1";

  object_->addVTable(
      sdbus::registerProperty("Type").withGetter([this]() { return type_; }),

      sdbus::registerProperty("ServiceUUIDs")
          .withGetter([this]() { return serviceUUIDs_; }),

      sdbus::registerProperty("LocalName")
          .withGetter([this]() { return localName_; }),

      sdbus::registerProperty("Includes")
          .withGetter([this]() { return includes_; }))
      .forInterface(advInterface);

  std::cout << "Advertisement created at " << objectPath_ << std::endl;
}

Advertisement::~Advertisement() {
  try {
    unregister();
  } catch (...) {
    // Ignore errors during destruction
  }
  std::cout << "Advertisement destroyed" << std::endl;
}

void Advertisement::setLocalName(const std::string &name) {
  localName_ = name;
}

void Advertisement::addServiceUUID(const std::string &uuid) {
  serviceUUIDs_.push_back(uuid);
}

void Advertisement::registerToBlueZ(const std::string &adapterPath) {
  try {
    advManagerProxy_ = sdbus::createProxy(
        connection_, sdbus::ServiceName("org.bluez"),
        sdbus::ObjectPath(adapterPath));

    advManagerProxy_->callMethod("RegisterAdvertisement")
        .onInterface("org.bluez.LEAdvertisingManager1")
        .withArguments(sdbus::ObjectPath(objectPath_),
                      std::map<std::string, sdbus::Variant>{});

    std::cout << "Advertisement registered to BlueZ at " << adapterPath
              << std::endl;

  } catch (const sdbus::Error &e) {
    std::cerr << "Failed to register advertisement: " << e.what() << std::endl;
    throw;
  }
}

void Advertisement::unregister() {
  if (advManagerProxy_) {
    try {
      advManagerProxy_->callMethod("UnregisterAdvertisement")
          .onInterface("org.bluez.LEAdvertisingManager1")
          .withArguments(sdbus::ObjectPath(objectPath_));

      std::cout << "Advertisement unregistered" << std::endl;
    } catch (const sdbus::Error &e) {
      std::cerr << "Failed to unregister advertisement: " << e.what()
                << std::endl;
    }
    advManagerProxy_.reset();
  }
}
