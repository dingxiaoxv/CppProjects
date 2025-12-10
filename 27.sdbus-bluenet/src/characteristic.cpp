#include "characteristic.hpp"
#include <iomanip>
#include <iostream>

Characteristic::Characteristic(sdbus::IConnection &connection,
                               const std::string &objectPath,
                               const std::string &uuid,
                               const std::vector<std::string> &flags,
                               const std::string &servicePath)
    : objectPath_(objectPath), uuid_(uuid), flags_(flags),
      servicePath_(servicePath), connection_(connection) {

  object_ = sdbus::createObject(connection_, sdbus::ObjectPath(objectPath_));

  // 注册GattCharacteristic1接口
  const char *chrcInterface = "org.bluez.GattCharacteristic1";

  object_->addVTable(
    // 注册方法: ReadValue
    sdbus::registerMethod("ReadValue").implementedAs(
      [this](const std::map<std::string, sdbus::Variant> &options) {
        std::cout << "ReadValue called on " << objectPath_ << std::endl;
        return this->readValue(options);
      }),

    // 注册方法: WriteValue
    sdbus::registerMethod("WriteValue").implementedAs(
      [this](const std::vector<uint8_t> &value,
             const std::map<std::string, sdbus::Variant> &options) {
        std::cout << "WriteValue called on " << objectPath_ << ", data: ";
        for (auto byte : value) {
          std::cout << std::hex << std::setw(2) << std::setfill('0')
                    << (int)byte << " ";
        }
        std::cout << std::dec << std::endl;
        this->writeValue(value, options);
      }),

    // 注册方法: StartNotify
    sdbus::registerMethod("StartNotify").implementedAs(
      [this]() {
        std::cout << "StartNotify called on " << objectPath_ << std::endl;
        this->startNotify();
      }),

    // 注册方法: StopNotify
    sdbus::registerMethod("StopNotify").implementedAs(
      [this]() {
        std::cout << "StopNotify called on " << objectPath_ << std::endl;
        this->stopNotify();
      }),

    // 注册属性
    sdbus::registerProperty("UUID").withGetter([this]() { return uuid_; }),
    sdbus::registerProperty("Service").withGetter(
      [this]() { return sdbus::ObjectPath(servicePath_); }),
    sdbus::registerProperty("Value").withGetter([this]() { return value_; }),
    sdbus::registerProperty("Flags").withGetter([this]() { return flags_; }),
    sdbus::registerProperty("Notifying").withGetter([this]() { return notifying_; })
  ).forInterface(chrcInterface);

  // 发出InterfacesAdded信号
  object_->emitInterfacesAddedSignal();

  std::cout << "Characteristic created: " << uuid_ << " at " << objectPath_
            << std::endl;
}

Characteristic::~Characteristic() {
  std::cout << "Characteristic destroyed: " << uuid_ << std::endl;
}

void Characteristic::setReadCallback(ReadCallback callback) {
  readCallback_ = callback;
}

void Characteristic::setWriteCallback(WriteCallback callback) {
  writeCallback_ = callback;
}

std::vector<uint8_t> Characteristic::readValue(
    const std::map<std::string, sdbus::Variant> &options) {

  if (readCallback_) {
    return readCallback_(options);
  }

  // 返回当前值
  return value_;
}

void Characteristic::writeValue(
    const std::vector<uint8_t> &value,
    const std::map<std::string, sdbus::Variant> &options) {

  value_ = value;

  if (writeCallback_) {
    writeCallback_(value, options);
  }
}

void Characteristic::startNotify() { notifying_ = true; }

void Characteristic::stopNotify() { notifying_ = false; }

void Characteristic::updateValue(const std::vector<uint8_t> &value) {
  value_ = value;
}

void Characteristic::notify(const std::vector<uint8_t> &value) {
  if (!notifying_) {
    std::cerr << "Notification not enabled for " << objectPath_ << std::endl;
    return;
  }

  value_ = value;

  // 发送PropertiesChanged信号
  std::map<std::string, sdbus::Variant> changed;
  changed["Value"] = sdbus::Variant(value);

  emitPropertiesChanged(changed);

  std::cout << "Notification sent from " << objectPath_
            << ", value size: " << value.size() << std::endl;
}

void Characteristic::emitPropertiesChanged(
    const std::map<std::string, sdbus::Variant> &changed) {

  object_->emitSignal("PropertiesChanged")
      .onInterface("org.freedesktop.DBus.Properties")
      .withArguments("org.bluez.GattCharacteristic1", changed,
                     std::vector<std::string>{});
}

std::map<std::string, sdbus::Variant> Characteristic::getProperties() const {
  std::map<std::string, sdbus::Variant> props;

  props["UUID"] = sdbus::Variant(uuid_);
  props["Service"] = sdbus::Variant(sdbus::ObjectPath(servicePath_));
  props["Flags"] = sdbus::Variant(flags_);
  props["Value"] = sdbus::Variant(value_);
  props["Notifying"] = sdbus::Variant(notifying_);

  return props;
}
