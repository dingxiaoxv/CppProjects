#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <string>
#include <zlib.h>

void compressFile(const char *source, gzFile dest, int level) {
  std::ifstream inFile(source, std::ios::binary);
  if (!inFile) {
    std::cerr << "Error opening file: " << source << std::endl;
    return;
  }

  char buffer[1024];
  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;
  deflateInit(&stream, level);

  do {
    inFile.read(buffer, sizeof(buffer));
    stream.avail_in = static_cast<uInt>(inFile.gcount());
    stream.next_in = reinterpret_cast<Bytef *>(buffer);

    do {
      int deflateResult = Z_OK;
      stream.avail_out = sizeof(buffer);
      stream.next_out = reinterpret_cast<Bytef *>(buffer);
      deflateResult = deflate(&stream, Z_FINISH);

      if (deflateResult == Z_STREAM_ERROR) {
        std::cerr << "Error compressing file: " << source << std::endl;
        deflateEnd(&stream);
        inFile.close();
        return;
      }

      int have = sizeof(buffer) - stream.avail_out;
      gzwrite(dest, buffer, have);

    } while (stream.avail_out == 0);

  } while (inFile);

  deflateEnd(&stream);
  inFile.close();
}

void compressFolder(const char *folderPath, const char *outputFilePath, int level) {
  DIR *dir = opendir(folderPath);
  if (!dir) {
    std::cerr << "Error opening directory: " << folderPath << std::endl;
    return;
  }

  gzFile dest = gzopen(outputFilePath, "wb");
  if (!dest) {
    std::cerr << "Error opening output file: " << outputFilePath << std::endl;
    closedir(dir);
    return;
  }

  dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    std::string filePath = std::string(folderPath) + "/" + entry->d_name;

    // Skip directories "." and ".."
    if (entry->d_type == DT_DIR &&
        (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)) {
      continue;
    }

    compressFile(filePath.c_str(), dest, level);
  }

  gzclose(dest);
  closedir(dir);
}

int main() {
  const char *folderPath = "./test/";
  const char *outputFilePath = "test.zip";
  int compressionLevel = Z_BEST_COMPRESSION;

  compressFolder(folderPath, outputFilePath, compressionLevel);

  return 0;
}
