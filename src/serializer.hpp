#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#endif

#pragma once

#define S_NODE_SIZE 512
#define S_METADATA_SIZE 512

enum MODE { READ, WRITE, INVALID };

struct S_Node {
  int key;                       // 4 Bytes
  uint8_t degree;                // 1 Byte, degree (0~255)
  uint8_t pSize;                 // 1 Byte, payload size (0~255)
  uint16_t data[253];            // 506 Bytes, edges then payloads
} __attribute__((aligned(512))); // GCC extension to align a struct

struct S_MetaData {
  int numNode;
  int maxDegree;
  int dataType; // Tree: 1, Graph: 2, and others
} __attribute__((aligned(512)));

class Serializer {
public:
  Serializer();
  ~Serializer();

  bool writeMetadata(S_MetaData *metadata);
  S_MetaData *readMetadata();

  bool writeNode(S_Node *node, int id);
  S_Node *readNode(int id);

  void openFile(std::string filepath, MODE mode);

private:
#ifdef _WIN32
  HANDLE hf;
#else
  int fd;
#endif
  MODE mode_internal;
  bool at_eof;
  S_Node *readNodeFromOffset(size_t offset);
  bool writeNodeWithOffset(S_Node *node, size_t offset);
};
