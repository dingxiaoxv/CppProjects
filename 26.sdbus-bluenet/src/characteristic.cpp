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

  object_ = sdbus::createObject(connection_, objectPath_);

  // 注册GattCharacteristic1接口
  const char *chrcInterface = "org.bluez.GattCharacteristic1";

  // 注册方法: ReadValue
  object_->registerMethod(chrcInterface, "ReadValue", "a{sv}", "ay",
                          [this](sdbus::MethodCall call) {
                            std::map<std::string, sdbus::Variant> options;
                            call >> options;

                            std::cout << "ReadValue called on " << objectPath_
                                      << std::endl;

                            auto value = this->readValue(options);
                            auto reply = call.createReply();
                            reply << value;
                            reply.send();
                          });

  // 注册方法: WriteValue
  object_->registerMethod(chrcInterface, "WriteValue", "aya{sv}", "",
                          [this](sdbus::MethodCall call) {
                            std::vector<uint8_t> value;
                            std::map<std::string, sdbus::Variant> options;
                            call >> value >> options;

                            std::cout << "WriteValue called on " << objectPath_
                                      << ", data: ";
                            for (auto byte : value) {
                              std::cout << std::hex << std::setw(2)
                                        << std::setfill('0') << (int)byte
                                        << " ";
                            }
                            std::cout << std::dec << std::endl;

                            this->writeValue(value, options);
                            auto reply = call.createReply();
                            reply.send();
                          });

  // 注册方法: StartNotify
  object_->registerMethod(
      chrcInterface, "StartNotify", "", "", [this](sdbus::MethodCall call) {
        std::cout << "StartNotify called on " << objectPath_ << std::endl;
        this->startNotify();
        auto reply = call.createReply();
        reply.send();
      });

  // 注册方法: StopNotify
  object_->registerMethod(
      chrcInterface, "StopNotify", "", "", [this](sdbus::MethodCall call) {
        std::cout << "StopNotify called on " << objectPath_ << std::endl;
        this->stopNotify();
        auto reply = call.createReply();
        reply.send();
      });

  // 注册属性
  object_->registerProperty(chrcInterface, "UUID", "s")
      .onInterface(chrcInterface)
      .withGetter([this]() { return uuid_; });

  object_->registerProperty(chrcInterface, "Service", "o")
      .onInterface(chrcInterface)
      .withGetter([this]() { return sdbus::ObjectPath(servicePath_); });

  object_->registerProperty(chrcInterface, "Value", "ay")
      .onInterface(chrcInterface)
      .withGetter([this]() { return value_; });

  object_->registerProperty(chrcInterface, "Flags", "as")
      .onInterface(chrcInterface)
      .withGetter([this]() { return flags_; });

  object_->registerProperty(chrcInterface, "Notifying", "b")
      .onInterface(chrcInterface)
      .withGetter([this]() { return notifying_; });

  object_->finishRegistration();
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
  changed["Value"] = value;

  emitPropertiesChanged(changed);

  std::cout << "Notification sent from " << objectPath_
            << ", value size: " << value.size() << std::endl;
}

void Characteristic::emitPropertiesChanged(
    const std::map<std::string, sdbus::Variant> &changed) {

  auto signal = object_->createSignal("org.freedesktop.DBus.Properties",
                                      "PropertiesChanged");

  signal << "org.bluez.GattCharacteristic1"; // interface_name
  signal << changed;                         // changed_properties
  signal << std::vector<std::string>{};      // invalidated_properties

  signal.send();
}

std::map<std::string, sdbus::Variant> Characteristic::getProperties() const {
  std::map<std::string, sdbus::Variant> props;

  props["UUID"] = uuid_;
  props["Service"] = sdbus::ObjectPath(servicePath_);
  props["Flags"] = flags_;
  props["Value"] = value_;
  props["Notifying"] = notifying_;

  return props;
}
