#pragma once
#include <memory>
#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <vector>

class Characteristic;

class Service {
public:
  Service(sdbus::IConnection &connection, const std::string &objectPath,
          const std::string &uuid, bool primary);
  ~Service();

  void addCharacteristic(std::shared_ptr<Characteristic> characteristic);

  std::string getPath() const { return objectPath_; }
  std::string getUUID() const { return uuid_; }
  bool isPrimary() const { return primary_; }

  std::vector<std::shared_ptr<Characteristic>> getCharacteristics() const {
    return characteristics_;
  }

  std::map<std::string, sdbus::Variant> getProperties() const;

private:
  std::string objectPath_;
  std::string uuid_;
  bool primary_;
  std::unique_ptr<sdbus::IObject> object_;
  std::vector<std::shared_ptr<Characteristic>> characteristics_;
  sdbus::IConnection &connection_;
};
