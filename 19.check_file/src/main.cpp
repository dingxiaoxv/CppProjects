#include <fstream>
#include <iostream>
#include <vector>
#include <getopt.h>

const size_t ONE_GB = 1024 * 1024 * 1024;
const std::string OUT_FILE = "test.bin";

void createFile() {
  std::ofstream file(OUT_FILE, std::ios::binary);
  if (!file) {
    std::cerr << "Cannot open file for writing." << std::endl;
    return;
  }

  for (size_t i = 0; i < ONE_GB; ++i) {
    char data = static_cast<char>(i % 256);
    file.write(&data, sizeof(data));
  }
}

int verifyFile(const std::string &toVerify) {
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
  const char *short_options = "cv:";
  const struct option long_options[] = {{"create", no_argument, nullptr, 'c'},
                                        {"verify", required_argument, nullptr, 'v'},
                                        {nullptr, 0, nullptr, 0}};

  int opt = 0;
  std::string filename;

  while ((opt = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
    switch (opt) {
    case 'c':
      createFile();
      break;
    case 'v':
      filename = optarg;
      int errors = verifyFile(filename);
      std::cout << "Number of errors in file: " << errors << std::endl;
      break;
    default:
      std::cerr << "Usage: " << argv[0] << " --create filename/--verify filename" << std::endl;
      exit(0);
    }
  }

  return 0;
}