#ifndef _CNR_LOGMAN_H_20230529_
#define _CNR_LOGMAN_H_20230529_

#include <map>

#include "cnr/ofstream.h"
#include "cnr/ifstream.h"
#include "cnr/path.h"

namespace cnr {

static constexpr int LOGFILE_COUNT = 2;

class LogMan {
  DISALLOW_COPY_AND_ASSIGN(LogMan);

public:
  explicit LogMan(const std::string& dir, int size) : dir_(dir), size_(size) {
    currentOfs_ = 0;
    open(false);
    if (ofs_[currentOfs_].raw()) {
      cnr::Logging::start(ofs_[currentOfs_].raw(), nullptr);
    }
  }

  void timerEvent() {
    ofs_[currentOfs_].flush();

    auto len = ofs_[currentOfs_].length();
    if (len > size_) {
      currentOfs_ = (currentOfs_ + 1) % 2;
      open(true);
      if (ofs_[currentOfs_].raw()) {
        cnr::Logging::start(ofs_[currentOfs_].raw());
      }
    }
  }

  void open(bool next) {
    if (!cnr::Path::exists(dir_.c_str())) {
      cnr::Path::mkdir(dir_.c_str());
    }
    auto txt = cnr::Path::join(dir_, "current");
    int current = 0;
    if (cnr::Path::exists(txt.c_str())) {
      cnr::IFStream iftxt(txt.c_str());
      char buf[4];
      iftxt.read(buf, 4);
      current = atoi(buf) + (next ? 1 : 0);
      if (current >= LOGFILE_COUNT) current = 0;
    }

    LOCAL_MS(512, ms);
    ms <<  dir_ << "/log." << current;
    ofs_[currentOfs_].reopen(ms.data(), !next);

    ms.reset();
    ms << current;
    cnr::OFStream oftxt(txt.c_str());
    oftxt.write(ms.data(), ms.size());

    cLOG_INFO << ">>>>>>>>> log to " << current;
    cnr::Logging::_flush();
  }

private:
    cnr::OFStream ofs_[2];
    int currentOfs_;
    std::string dir_;
    int size_;
};

}  // namespace cnr

#endif  // _CNR_LOGMAN_H_20230529_