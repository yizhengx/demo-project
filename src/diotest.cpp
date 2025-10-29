#include "serializer.hpp"
#include <chrono>
#include <errno.h>
#include <fcntl.h>
#include <future>
#include <iostream>
#include <omp.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <unistd.h>
#include <vector>

void testRead(char *filepath) {
  int fd = open(filepath, O_RDWR | O_CREAT | O_DIRECT);
  if (fd < 0) {
    std::cerr
        << "[ERROR]: DirectSearch fd Initialization failed\n[MESSAGE]: file"
        << filepath << " unable to be opened or created";
    exit(1);
  }
  int ret;
  char *buf;
  ret = posix_memalign((void **)&buf, 512, 512);
  if (ret) {
    perror("posix_memalign failed");
    exit(1);
  }
  ret = pread(fd, buf, 512, 0);
  // std::cout << "Bytes read: " << ret << "node id: " << ((S_Node*)buf)->key <<
  // std::endl;
  if (ret < 0)
    std::cout << "Could Not Read Offset " << 0
              << " & Reason: " + std::string(strerror(errno)) << std::endl;

  // S_Node* buf_;
  // ret = pread(fd, (char*)buf_, 512, 0);
  // std::cout << "Bytes read: " << ret << "node id: " << buf_->key <<
  // std::endl; if(ret<0) std::cout << "Could Not Read Offset "<< 0 <<" &
  // Reason: " + std::string(strerror(errno))<<std::endl;
}

void testSerial(int fd, int size) {
  for (int i = 0; i < size; i++) {
    int ret;
    char *buf;
    ret = posix_memalign((void **)&buf, 512, 512);
    if (ret) {
      perror("posix_memalign failed");
      exit(1);
    }
    ret = pread(fd, buf, 512, 512 * i);
    // std::cout << "Bytes read: " << ret << "node id: " << ((S_Node*)buf)->key
    // << std::endl;
    if (ret < 0)
      std::cout << "Could Not Read Offset " << 512 * i
                << " & Reason: " + std::string(strerror(errno)) << std::endl;
  }
}

void parallelRead(int fd, int offset) {
  int ret;
  char *buf;
  ret = posix_memalign((void **)&buf, 512, 512);
  if (ret) {
    perror("posix_memalign failed");
    exit(1);
  }
  ret = pread(fd, buf, 512, 512 * offset);
  // std::cout << "Read node id: " << ((S_Node*)buf)->key << std::endl;
  if (ret < 0)
    std::cout << "Could Not Read Offset " << 512 * offset
              << " & Reason: " + std::string(strerror(errno)) << std::endl;
}

void testParallel(int fd, int size) {
#pragma omp parallel for
  for (int i = 0; i < size; i++) {
    parallelRead(fd, i);
  }
}

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "[ERROR]: Usage is ./diotest <filepath> <NumofRandomReads>"
              << std::endl;
    exit(1);
  }

  // Test 1: use direct io to read a S_Node
  // testRead(argv[1]);

  // Test 2: multithread to read direct io

  std::cout << "Start Testing" << std::endl;
  int fd = open(argv[1], O_RDONLY | O_DIRECT);
  if (fd < 0) {
    std::cerr
        << "[ERROR]: DirectSearch fd Initialization failed\n[MESSAGE]: file"
        << argv[1] << " unable to be opened or created";
    exit(1);
  }
  auto start = std::chrono::high_resolution_clock::now();
  testSerial(fd, std::stoi(argv[2]));
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "Serial Timing: " << duration.count() << std::endl;

  std::vector<int> threads = {1, 2, 4, 8, 16, 32, 64, 80, 100, 120};
  for (int i = 0; i < threads.size(); i++) {
    std::cout << "----- Thread " << threads.at(i) << " -----" << std::endl;
    omp_set_num_threads(threads.at(i));
    start = std::chrono::high_resolution_clock::now();
    testParallel(fd, std::stoi(argv[2]));
    stop = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Parallel Timing: " << duration.count() << std::endl;
  }
}
