#include "serializer.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <random>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#pragma once

#define MAX_DEGREE 253
#define BLOCK_SIZE 512

class GraphNode {
public:
  GraphNode() : key(-1), id(0), degree(0), numValues(0) {}
  ~GraphNode();
  int key, id;
  int degree;
  uint8_t numValues;
  std::vector<int> edges;
  std::vector<uint16_t> values;
};

class Graph {
public:
  Graph(int _maxDegree = 0)
      : numNode(0), maxDegree(_maxDegree), numExtSNode(0), gs_init(false) {}
  ~Graph() {}

  void init_serializer(std::string filename, int rw);
  bool init_serializer(Serializer *sz);
  void init_metadata();

  void dump_graph();
  void dump_node(GraphNode *node);

  S_Node *read_snode(int id);
  S_MetaData *read_smetadata();

  int numNode;
  int maxDegree;
  int numExtSNode;

  Serializer gs;

private:
  S_Node *node_to_snode(GraphNode *node);
  S_Node *node_to_aligned_snode(GraphNode *node);

  std::vector<GraphNode *> nodes;
  bool gs_init;
};
