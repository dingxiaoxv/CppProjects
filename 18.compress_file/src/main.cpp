#include <boost/filesystem.hpp>
#include <iostream>
#include <zip.h>

void compress_dir(zip_t *zip, const std::string &dir_name) {
  std::cout << "compress folder " << dir_name << std::endl;
  for (const auto& entry : boost::filesystem::recursive_directory_iterator(dir_name)) {
    if (boost::filesystem::is_directory(entry)) {
      continue;
    }

    std::cout << entry.path().string() << std::endl;

    zip_entry_open(zip, entry.path().string().c_str());
    zip_entry_fwrite(zip, entry.path().string().c_str());
    zip_entry_close(zip);  
  }
}

int on_extract_entry(const char *filename, void *arg) {
  static int i = 0;
  int n = *(int *)arg;
  printf("Extracted: %s (%d of %d)\n", filename, ++i, n);

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <directory> <output.zip>\n";
    return EXIT_FAILURE;
  }

  std::string opt = argv[1];
  std::string dir_name = argv[2];
  std::string zip_filename = argv[3];

  if (opt == "c") {
    zip_t *zip = zip_open(zip_filename.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
    if (zip == nullptr) {
      std::cerr << "Could not create zip archive " << zip_filename << "\n";
      return EXIT_FAILURE;
    }
    compress_dir(zip, dir_name);
    zip_close(zip);
  } else if (opt == "e") {
    int arg = 6;
    zip_extract(zip_filename.c_str(), dir_name.c_str(), on_extract_entry, &arg);
  }

  return EXIT_SUCCESS;
}
