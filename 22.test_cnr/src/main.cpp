#include <cnr/logging.h>
#include <cnr/jsonbuilder.h>

std::string outputRapidJsonData(rapidjson::Value &data) {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  data.Accept(writer);
  return buffer.GetString();
}

std::string createObject(int n) {
  cnr::JsonBuilder builder;
  builder.add(builder.document, "ssid", n);
  builder.add(builder.document, "aiid", n);
  return builder.result();
}

int main(int argc, char** argv) {
  rapidjson::Document doc;
  rapidjson::Value array(rapidjson::kArrayType);
  for (int i = 0; i < 5; ++i) {
    std::string ret = createObject(i);
    doc.Parse(rapidjson::StringRef(ret.c_str()));
    rapidjson::Value value(doc, doc.GetAllocator());
    array.PushBack(value, doc.GetAllocator());
  }
  
  cLOG_INFO << outputRapidJsonData(array);

  return 0;
}