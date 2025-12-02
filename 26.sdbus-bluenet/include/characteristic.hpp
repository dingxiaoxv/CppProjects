#pragma once
#include <functional>
#include <memory>
#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <vector>

class Characteristic {
public:
  using ReadCallback = std::function<std::vector<uint8_t>(
      const std::map<std::string, sdbus::Variant> &options)>;
  using WriteCallback =
      std::function<void(const std::vector<uint8_t> &value,
                         const std::map<std::string, sdbus::Variant> &options)>;

  Characteristic(sdbus::IConnection &connection, const std::string &objectPath,
                 const std::string &uuid, const std::vector<std::string> &flags,
                 const std::string &servicePath);
  ~Characteristic();

  void setReadCallback(ReadCallback callback);
  void setWriteCallback(WriteCallback callback);

  // 发送通知给客户端
  void notify(const std::vector<uint8_t> &value);

  // 更新值但不发送通知
  void updateValue(const std::vector<uint8_t> &value);

  std::string getPath() const { return objectPath_; }
  std::string getUUID() const { return uuid_; }
  std::map<std::string, sdbus::Variant> getProperties() const;

private:
  std::unique_ptr<sdbus::IObject> object_;
  std::string objectPath_;
  std::string uuid_;
  std::vector<std::string> flags_;
  std::string servicePath_;
  std::vector<uint8_t> value_;
  bool notifying_ = false;
  sdbus::IConnection &connection_;

  ReadCallback readCallback_;
  WriteCallback writeCallback_;

  // D-Bus方法实现
  std::vector<uint8_t>
  readValue(const std::map<std::string, sdbus::Variant> &options);
  void writeValue(const std::vector<uint8_t> &value,
                  const std::map<std::string, sdbus::Variant> &options);
  void startNotify();
  void stopNotify();

  // 发送PropertiesChanged信号
  void
  emitPropertiesChanged(const std::map<std::string, sdbus::Variant> &changed);
};
