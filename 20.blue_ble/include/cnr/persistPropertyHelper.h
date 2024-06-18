#pragma once

#include <string>
#include <stdexcept>
#include <functional>
#include <vector>
#include <map>
#include <mutex>
#include <condition_variable>
#include <type_traits>
#include <typeinfo>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "cnr/logging.h"

#define DEFAULT_FILE_PATH "/data/persist/"

namespace cnr {
    class PersistPropertyHelper {
        public:
            class Config {
                friend class PersistPropertyHelper;
                public:
                    Config() {}
                    ~Config() {}

                    Config& setName(std::string name) {
                        if (submited_) {
                            throw std::runtime_error("can't set name after submit");
                        }

                        name_ = name;
                        return *this;
                    }

                    Config& setFilePath(std::string filePath) {
                        if (submited_) {
                            throw std::runtime_error("can't set name after submit");
                        }
                        filePath_ = filePath;
                        return *this;
                    }

                    Config& setBackupFilePath(std::string backupFilePath) {
                        if (submited_) {
                            throw std::runtime_error("can't set name after submit");
                        }
                        backupFilePath_ = backupFilePath;
                        return *this;
                    }

                    Config& disableBackup() {
                        if (submited_) {
                            throw std::runtime_error("can't set name after submit");
                        }
                        enableBackup_ = false;
                        return *this;
                    }

                    Config& enableRawData() {
                        if (submited_) {
                            throw std::runtime_error("can't set name after submit");
                        }
                        rawData_ = true;
                        return *this;
                    }

                    Config& setInitCallback(std::function<std::vector<uint8_t>()> callback) {
                        if (submited_) {
                            throw std::runtime_error("can't set name after submit");
                        }
                        rawDataInitCallback_ = callback;
                        return *this;
                    }

                    Config& setInitCallback(std::function<void(PersistPropertyHelper&)> callback) {
                        if (submited_) {
                            throw std::runtime_error("can't set name after submit");
                        }
                        initCallback_ = callback;
                        return *this;
                    }

                    Config& submit() {
                        if (name_.empty()) {
                            throw std::runtime_error("name is empty");
                        }

                        if (filePath_.empty()) {
                            filePath_ = DEFAULT_FILE_PATH + name_ + ".dat";
                        }

                        if (backupFilePath_.empty() && enableBackup_) {
                            backupFilePath_ = DEFAULT_FILE_PATH + name_ + ".bak";
                        }

                        submited_ = true;

                        return *this;
                    }

                private:
                    std::string name_;
                    std::string filePath_;
                    std::string backupFilePath_;
                    std::function<std::vector<uint8_t>()> rawDataInitCallback_;
                    std::function<void(PersistPropertyHelper&)> initCallback_;

                    bool rawData_ = false;
                    bool enableBackup_ = true;

                    bool submited_ = false;
            };

            static std::shared_ptr<PersistPropertyHelper> getOrCreate(Config& config);

            ~PersistPropertyHelper() {}

            template<typename T> void set(const std::string& group, const std::string& key, T value, bool saveImmediately = false)
            {
                std::unique_lock<std::mutex> lock(writeMutex_);

                if (config_.rawData_) {
                    throw std::runtime_error("raw data not support set");
                } else {
                    rapidjson::Value jsonValue;
                    if (std::is_same<T, const char*>::value) {
                        const char** __v = (const char**)&value;
                        jsonValue.SetString(*__v, doc_.GetAllocator());
                    }
                    else if (std::is_same<T, std::string>::value) {
                        std::string* __v = (std::string*)&value;
                        jsonValue.SetString(__v->c_str(), doc_.GetAllocator());
                    }
                    else if (std::is_same<T, bool>::value) {
                        bool* __v = (bool*)&value;
                        jsonValue.SetBool(!!*__v);
                    }
                    else if (std::is_same<T, int>::value) {
                        int* __v = (int*)&value;
                        jsonValue.SetInt(*__v);
                    }
                    else if (std::is_same<T, double>::value) {
                        double* __v = (double*)&value;
                        jsonValue.SetDouble(*__v);
                    }
                    else {
                        throw std::runtime_error("not support type");
                    }

                    if (doc_.HasMember(rapidjson::StringRef(group.c_str()))) {
                        if (!doc_[group.c_str()].IsObject()) {
                            throw std::runtime_error("group is not object" + group);
                        }
                    } else {
                        doc_.AddMember(rapidjson::Value(group.c_str(), doc_.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), doc_.GetAllocator());
                    }

                    if (!doc_[group.c_str()].HasMember(rapidjson::StringRef(key.c_str()))) {
                        doc_[group.c_str()].AddMember(rapidjson::Value(key.c_str(), doc_.GetAllocator()), jsonValue, doc_.GetAllocator());
                    } else {
                        if (doc_[group.c_str()][key.c_str()] == jsonValue)
                            return;
                            
                        doc_[group.c_str()][key.c_str()] = jsonValue;
                    }
                }

                notifyToStoreToFile(saveImmediately, false);
            }

            bool contains(const std::string& group, const std::string& key)
            {
                std::unique_lock<std::mutex> lock(writeMutex_);

                if (config_.rawData_) {
                    throw std::runtime_error("raw data not support get");
                } else {
                    if (doc_.HasMember(group.c_str()) && doc_[group.c_str()].IsObject() && doc_[group.c_str()].HasMember(key.c_str())) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }
            }

            template<typename T> typename std::enable_if<std::is_same<T, std::string>::value, T>::type get(const std::string& group, const std::string& key) {
                std::unique_lock<std::mutex> lock(writeMutex_);

                if (config_.rawData_) {
                    throw std::runtime_error("raw data not support get");
                } else {
                    if (doc_.HasMember(group.c_str()) && doc_[group.c_str()].IsObject() && doc_[group.c_str()].HasMember(key.c_str())) {
                        return doc_[group.c_str()][key.c_str()].GetString();
                    }
                    else {
                        return "";
                    }
                }
            }

            template<typename T> typename std::enable_if<std::is_same<T, const char *>::value, T>::type get(const std::string& group, const std::string& key) {
                std::unique_lock<std::mutex> lock(writeMutex_);

                if (config_.rawData_) {
                    throw std::runtime_error("raw data not support get");
                } else {
                    if (doc_.HasMember(group.c_str()) && doc_[group.c_str()].IsObject() && doc_[group.c_str()].HasMember(key.c_str())) {
                        return doc_[group.c_str()][key.c_str()].GetString();
                    }
                    else {
                        return nullptr;
                    }
                }
            }

            template<typename T> typename std::enable_if<std::is_same<T, bool>::value, T>::type get(const std::string& group, const std::string& key) {
                std::unique_lock<std::mutex> lock(writeMutex_);

                if (config_.rawData_) {
                    throw std::runtime_error("raw data not support get");
                } else {
                    if (doc_.HasMember(group.c_str()) && doc_[group.c_str()].IsObject() && doc_[group.c_str()].HasMember(key.c_str())) {
                        return doc_[group.c_str()][key.c_str()].GetBool();
                    }
                    else {
                        return false;
                    }
                }
            }

            template<typename T> typename std::enable_if<std::is_same<T, int>::value, T>::type get(const std::string& group, const std::string& key) {
                std::unique_lock<std::mutex> lock(writeMutex_);

                if (config_.rawData_) {
                    throw std::runtime_error("raw data not support get");
                } else {
                    if (doc_.HasMember(group.c_str()) && doc_[group.c_str()].IsObject() && doc_[group.c_str()].HasMember(key.c_str())) {
                        return doc_[group.c_str()][key.c_str()].GetInt();
                    }
                    else {
                        return 0;
                    }
                }
            }

            template<typename T> typename std::enable_if<std::is_same<T, double>::value, T>::type get(const std::string& group, const std::string& key) {
                std::unique_lock<std::mutex> lock(writeMutex_);

                if (config_.rawData_) {
                    throw std::runtime_error("raw data not support get");
                } else {
                    if (doc_.HasMember(group.c_str()) && doc_[group.c_str()].IsObject() && doc_[group.c_str()].HasMember(key.c_str())) {
                        return doc_[group.c_str()][key.c_str()].GetDouble();
                    }
                    else {
                        return 0.0;
                    }
                }
            }

            void setRawData(std::vector<uint8_t>& data);

            std::vector<uint8_t>& getRawData();

            void updateRawData();

            [[deprecated("This function is deprecated, do not to use this.")]]
            void release() {
                needRelease_ = true;

                notifyToStoreToFile(true);
            }

        private:
            PersistPropertyHelper(Config &config);
            
            PersistPropertyHelper(const PersistPropertyHelper&) = delete;
            PersistPropertyHelper& operator=(const PersistPropertyHelper&) = delete;

            bool loadFromFile();
            bool loadFromFile(std::string path);

            void saveToFile();
            void saveToFile(std::string path);

            void generateRawContents();

            void updateCrc();

            void notifyToStoreToFile(bool immediately = false, bool lockAcquired = true);

            uint32_t crc_;
            std::vector<uint8_t> rawContents_;

            rapidjson::Document doc_;
            std::mutex writeMutex_;
            std::condition_variable writeCondition_;
            bool immediateWrite_ = false;
            uint32_t lastWriteCrc_;
            bool writeRequested_ = false;

            bool needRelease_ = false;

            
            Config config_;
    };
}