# RingBuffer
c++不同方法实现环形缓冲区

# 1. RingBuffer_1

总是保持一个存储单元为空

# 2. RingBuffer_2

使用一个计数数据

# 3. RingBuffer_3

使用读/写计数数据

# 4. RingBuffer_4

使用QByteArray作为缓冲区存储数据结构

# 5. RingBuffer_5

使用QSemaphore和线程池实现多线程访问缓冲区

