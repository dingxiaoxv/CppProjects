#ifndef _CNR_PATH_H_20140721_
#define _CNR_PATH_H_20140721_

#include <cnr/logging.h>
#include <dirent.h>
#include <libgen.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <functional>
#include <string>
#include <vector>
#include <fstream>

#define MAX_PATH 256

namespace cnr {
namespace Path {

inline bool isdir(const char* strDir) {
  struct stat s;
  stat(strDir, &s);

  return S_ISDIR(s.st_mode) ? true : false;
}

inline char* curdir(char* strPWD) { return getcwd(strPWD, MAX_PATH); }

inline std::string curdir() {
  char strPWD[256];
  return getcwd(strPWD, MAX_PATH) ? std::string(strPWD) : std::string();
}

#ifdef __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Winfinite-recursion"
#endif
inline bool chdir(const char* path) { return chdir(path) == 0; }
#ifdef __clang__
# pragma clang diagnostic pop
#endif

inline char* abspath(const char* path, char* strAbsPath) { return realpath(path, strAbsPath); }
inline std::string abspath(const char* path) {
  char strAbsPath[256];
  return realpath(path, strAbsPath) ? std::string(strAbsPath) : std::string();
}

inline char* home(char* strhome) {
  char* p = getenv("HOME");
  if (p) {
    strncpy(strhome, p, strlen(p) + 1);
    return strhome;
  }
  return NULL;
}

inline std::string home() {
  char* p = getenv("HOME");
  return p ? std::string(p) : std::string();
}

inline char* expanduser(const char* path, char* strAbsPath) {
  if (path[0] == '~' && path[1] == '/') {
    char* p = getenv("HOME");
    if (!p) {
      return NULL;
    }
    int len = strlen(p);
    strncpy(strAbsPath, p, len);
    strncpy(strAbsPath + len, path + 1, strlen(path));
    return strAbsPath;
  }

  return NULL;
}

inline std::string expanduser(const char* path) {
  char strAbsPath[MAX_PATH];
  if (expanduser(path, strAbsPath)) {
    return std::string(strAbsPath);
  }
  return std::string();
}

inline std::string dirname(const char* filename) {
  char path[MAX_PATH];
  strncpy(path, filename, MAX_PATH);
  char* res = ::dirname(path);
  return res ? std::string(res) : std::string();
}

inline std::string dirname(const std::string& filename) { return dirname(filename.c_str()); }

inline std::string basename(const char* filename) {
  char path[MAX_PATH];
  strncpy(path, filename, MAX_PATH);
  char* res = ::basename(path);
  return res ? std::string(res) : std::string();
}

inline std::string basename(const std::string& filename) { return basename(filename.c_str()); }

inline char* moudlepath(char* path) {
  if (readlink("/proc/self/exe", path, MAX_PATH) != -1) {
    ::dirname(path);
    return path;
  }

  return NULL;
}

inline std::string moudlepath() {
  char path[MAX_PATH];
  if (readlink("/proc/self/exe", path, MAX_PATH) != -1) {
    ::dirname(path);
    return std::string(path);
  }

  return std::string();
}

inline bool exists(const char* strFile) { return 0 == access(strFile, F_OK); }

inline bool mkdir(const char* path) { return 0 == ::mkdir(path, 0755); }

inline bool rmdir(const char* path) { return 0 == ::rmdir(path); }
inline bool remove(const char* path) { return 0 == ::remove(path); }

inline bool copy(const std::string& src_path, const std::string& dst_path) {
  std::ifstream src_file(src_path, std::ios::binary);
  std::ofstream dst_file(dst_path, std::ios::binary);
  
  if (!src_file.is_open() || !dst_file.is_open()) {
    return false;
  }
  
  dst_file << src_file.rdbuf();
  return true;
}

inline std::string join(const char* pre, const char* post) {
  std::string res;
  if (post[0] == '/') return res;

  res = pre;
  if (*res.rbegin() != '/') res += '/';

  res += post;
  return res;
}

inline std::string join(const std::string& pre, const std::string& post) {
  std::string res;
  if (post[0] == '/') return res;

  res = pre;
  if (*res.rbegin() != '/') res += '/';

  res += post;
  return res;
}

enum CNR_TYPE {
  CNR_UNKNOWN = DT_UNKNOWN,
  CNR_FIFO = DT_FIFO,
  CNR_CHR = DT_CHR,
  CNR_DIR = DT_DIR,
  CNR_BLK = DT_BLK,
  CNR_REG = DT_REG,
  CNR_LNK = DT_LNK,
  CNR_SOCK = DT_SOCK,
  CNR_WHT = DT_WHT,
};

inline void walk(const char* dirname, std::function<void(const char* name, CNR_TYPE type)> callback,
                 bool full = false) {
  auto dir = opendir(dirname);
  if (!dir) {
    cLOG_ERROR << "open dir " << dirname << " failed";
    return;
  }

  while (auto ptr = readdir(dir)) {
    if (!strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, "..")) continue;

    if (!full) {
      callback(join(dirname, ptr->d_name).c_str(), (CNR_TYPE)ptr->d_type);
    } else {
      if (ptr->d_type == DT_DIR) {
        walk(join(dirname, ptr->d_name).c_str(), callback, full);
      }

      callback(join(dirname, ptr->d_name).c_str(), (CNR_TYPE)ptr->d_type);
    }
  }
  closedir(dir);
}

inline void getSortedFiles(const char* dirname, std::vector<std::string>& files,
                           unsigned char filter = DT_REG) {
  walk(
      dirname,
      [&files, filter](const char* name, CNR_TYPE type) {
        if ((type & filter) || filter == DT_UNKNOWN) {
          files.push_back(name);
        }
      },
      false);

  std::sort(files.begin(), files.end());
}

inline int createtime(const std::string& file) {
  struct stat s;
  if (stat(file.c_str(), &s) == 0) {
    return s.st_ctim.tv_sec;
  }
  
  return -1;
}

inline int modifytime(const std::string& file) {
  struct stat s;
  if (stat(file.c_str(), &s) == 0) {
    return s.st_mtim.tv_sec;
  }

  return -1;
}

}  // namespace Path
}  // namespace cnr

#endif  // _CNR_PATH_H_20140721_
