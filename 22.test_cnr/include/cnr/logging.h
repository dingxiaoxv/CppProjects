#ifndef _CNR_LOGGING_H_20140506_
#define _CNR_LOGGING_H_20140506_

#include <map>

#include "cnr/eventbase.h"
#include "cnr/mstream.h"

namespace cnr {

enum LogLevel {
  CNR_TRACE = 0,
  CNR_DEBUG = 1,
  CNR_INFO = 2,
  CNR_WARN = 3,
  CNR_ERROR = 4,
  CNR_FATAL = 5,
};

class Logging {
  DISALLOW_COPY_AND_ASSIGN(Logging);

 public:
  Logging() : ms_(buf_, 1024, true) {}
  Logging(LogLevel level, const char* file = nullptr, int line = 0);

  ~Logging();

  template <typename T>
  inline Logging& operator<<(const T t) {
    ms_ << t;
    return *this;
  }

  static void format(LogLevel level, const char* __format, ...);
  static void setLogLevel(const LogLevel level) { configured_level = level; }
  static void setLogLevel(const char* envname);
  static void verbose(bool v) { log_with_file_line = v; }

  static void enable(int fd);

  static void disable(int fd);

  static void start(FILE* fp, EventDispather* dispatcher = NULL);
  static void _flush();
  static void stop(EventDispather* dispatcher = NULL);
  static void hexDump(const uint8_t* buffer, int len);
  static void startRotate(const std::string& dir, int size);

  static LogLevel configured_level;
  static bool log_with_file_line;
  static std::map<std::string, LogLevel> levels;

 private:
  char buf_[1024];
  MStream ms_;

  // static Timer* ptimer_;
};

}  // namespace cnr

#define cLOG_PRINT cnr::Logging()
#define cLOG_TRACE                                      \
  if (cnr::Logging::configured_level <= cnr::CNR_TRACE) \
  cnr::Logging(cnr::CNR_TRACE, __FILE__, __LINE__)
#define cLOG_INFO                                      \
  if (cnr::Logging::configured_level <= cnr::CNR_INFO) \
  cnr::Logging(cnr::CNR_INFO, __FILE__, __LINE__)
#define cLOG_DEBUG                                      \
  if (cnr::Logging::configured_level <= cnr::CNR_DEBUG) \
  cnr::Logging(cnr::CNR_DEBUG, __FILE__, __LINE__)
#define cLOG_WARN                                      \
  if (cnr::Logging::configured_level <= cnr::CNR_WARN) \
  cnr::Logging(cnr::CNR_WARN, __FILE__, __LINE__)
#define cLOG_ERROR                                      \
  if (cnr::Logging::configured_level <= cnr::CNR_ERROR) \
  cnr::Logging(cnr::CNR_ERROR, __FILE__, __LINE__)
#define cLOG_FATAL cnr::Logging(cnr::CNR_FATAL, __FILE__, __LINE__)

#define _CNR_LOG(level, __format, ...) \
  if (cnr::Logging::configured_level <= level) cnr::Logging::format(level, __format, __VA_ARGS__)

#define CNR_TRACE(__format, ...) _CNR_LOG(cnr::CNR_TRACE, __format, __VA_ARGS__)
#define CNR_INFO(__format, ...) _CNR_LOG(cnr::CNR_INFO, __format, __VA_ARGS__)
#define CNR_DEBUG(__format, ...) _CNR_LOG(cnr::CNR_DEBUG, __format, __VA_ARGS__)
#define CNR_ERROR(__format, ...) _CNR_LOG(cnr::CNR_ERROR, __format, __VA_ARGS__)
#define CNR_FATAL(__format, ...) _CNR_LOG(cnr::CNR_FATAL, __format, __VA_ARGS__)

#endif  // _CNR_LOGGING_H_20140506_