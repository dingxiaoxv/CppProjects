#include <fstream>
#include <iostream>
#include <vector>
#include <getopt.h>

const uint64_t ONE_GB = 1024 * 1024 * 1024;
const std::string OUT_FILE = "test.data";
int g_testSize = 0;

void createFile() {
  std::cout << "creating " << g_testSize << "GB file...." << std::endl;
  std::ofstream file(OUT_FILE, std::ios::binary);
  if (!file) {
    std::cerr << "Cannot open file for writing." << std::endl;
    return;
  }

  size_t writtenBytes = 0;
  const uint64_t updateInterval = g_testSize * ONE_GB / 100;

  for (size_t i = 0; i < g_testSize * ONE_GB; ++i) {
    char data = static_cast<char>(i % 256);
    file.write(&data, sizeof(data));
    ++writtenBytes;

    if (writtenBytes % updateInterval == 0) {
      int progress = static_cast<int>((static_cast<double>(writtenBytes) / g_testSize * ONE_GB) * 100);
      std::cout << "\rProgress: " << progress << "%" << std::flush;  // Use \r to return to the beginning of the line
    }
  }
  std::cout << "\nfile create completed! " << writtenBytes << std::endl;
}

int verifyFile(const std::string &toVerify) {
  std::cout << "checking file " << toVerify << "..." << std::endl;
  std::ifstream origFile(OUT_FILE, std::ios::binary);
  std::ifstream verifyFile(toVerify, std::ios::binary);

  if (!origFile || !verifyFile) {
    std::cerr << "Cannot open files for reading." << std::endl;
    return -1;
  }

  int errorCount = 0;
  for (size_t i = 0; i < ONE_GB; ++i) {
    char origData, verifyData;
    origFile.read(&origData, sizeof(origData));
    verifyFile.read(&verifyData, sizeof(verifyData));

    if (origData != verifyData) {
      errorCount++;
    }
  }

  return errorCount;
}

int main(int argc, char *argv[]) {
  const char *short_options = "c:v:";
  const struct option long_options[] = {{"create", required_argument, nullptr, 'c'},
                                        {"verify", required_argument, nullptr, 'v'},
                                        {nullptr, 0, nullptr, 0}};

  int opt = 0;
  

  while ((opt = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
    switch (opt) {
    case 'c':
      g_testSize = std::stoi(optarg);
      createFile();
      return 0;
    case 'v': {
      std::string filename = optarg;
      int errors = verifyFile(filename);
      std::cout << "Number of errors in file: " << errors << std::endl;
      return 0;
    }
    default:
      std::cerr << "Usage: " << argv[0] << " --create filename/--verify filename" << std::endl;
      return 1;
    }
  }

  return 0;
}