#ifndef CNR_JSON_BUILDER_H
#define CNR_JSON_BUILDER_H

#include <string>
#include <list>
#include <vector>
#include <typeinfo>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


namespace cnr {

class JsonBuilder {
public:
  JsonBuilder(bool array = false);

  bool load(const std::string& file);
  bool loadString(const std::string& json);
  JsonBuilder& add(rapidjson::Value& parent, const char* key, const char* value);
  JsonBuilder& add(rapidjson::Value& parent, const char* key, int value);
  JsonBuilder& add(rapidjson::Value& parent, const char* key, uint64_t value);
  JsonBuilder& add(rapidjson::Value& parent, const char* key, bool value);
  JsonBuilder& add(rapidjson::Value& parent, const char* key, double value);
  JsonBuilder& add(rapidjson::Value& parent, const char* key, const rapidjson::Value& value);
  JsonBuilder& addObject(rapidjson::Value& parent, const char* key);
  JsonBuilder& addArray(rapidjson::Value& parent, const char* key);
  JsonBuilder& pushBack(rapidjson::Value& parent, const char* value);
  JsonBuilder& pushBack(rapidjson::Value& parent, int value);
  JsonBuilder& pushBack(rapidjson::Value& parent, bool value);
  JsonBuilder& pushBack(rapidjson::Value& parent, const rapidjson::Value& value);
  JsonBuilder& pushBack(rapidjson::Value& parent, const std::vector<std::string>& values);
  JsonBuilder& pushBack(rapidjson::Value& parent, const std::vector<int>& values);
  JsonBuilder& pushBack(rapidjson::Value& parent, const std::vector<uint8_t>& values);
  rapidjson::Value& pushBack(rapidjson::Value& parent);
  JsonBuilder& append(rapidjson::Value& parent, const std::vector<int>& values);
  JsonBuilder& append(rapidjson::Value& parent, const std::vector<std::string>& values);
  JsonBuilder& append(rapidjson::Value& parent, const std::vector<uint8_t>& values);

  const std::string result();
  const std::string result(const rapidjson::Value& value);
  const std::vector<uint8_t> result_vector();
  static const std::string dumpJsonValue(const rapidjson::Value& value);

public:
  rapidjson::Document document;

private:
  rapidjson::Document::AllocatorType& allocator_;
};

}

#endif // CNR_JSON_BUILDER_H
