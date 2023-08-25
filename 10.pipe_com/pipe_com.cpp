#include <iostream>
#include <sys/types.h>  
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  const char *FIFO_NAME = "/tmp/my_fifo";
  unlink(FIFO_NAME); // 删除命名管道
  // 创建命名管道
  if (mkfifo(FIFO_NAME, 0666) == -1)
  {
    perror("mkfifo failed!");
    return 1;
  }

  // 对于以只读方式（O_RDONLY）打开的FIFO文件，如果open调用是阻塞的（即第二个参数为O_RDONLY），除非有一个进程以写方式打开同一个FIFO，否则它不会返回
  size_t cnt = 0;
  while (cnt < 10) {
    int read_fd = open(FIFO_NAME, O_RDONLY);
    if (read_fd == -1)
    {
      perror("open failed!");
      return 1;
    }
    std::cout << "open successfully" << std::endl;

    char buffer[256];
    ssize_t bytes_read = read(read_fd, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("read failed!");
        close(read_fd);
        return 1;
    }

    buffer[bytes_read] = '\0';
    std::cout << "C++ received: " << buffer << std::endl;
    ++cnt;
    close(read_fd);
  }

  unlink(FIFO_NAME); // 删除命名管道
  return 0;
}
