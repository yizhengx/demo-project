#include "graph.hpp"

class TreeNode : public GraphNode {
public:
  int maxDegree, level;
  uint8_t maxValues;
  TreeNode(int _maxDegree, uint8_t _maxValues)
      : GraphNode(), maxDegree(_maxDegree), maxValues(_maxValues) {}
};

class Tree : public Graph {
public:
  Tree(int _maxDegree, int _maxValues)
      : Graph(_maxDegree), root(NULL), maxLevel(0), maxValues(_maxValues) {}
  bool add(int key, int value);
  void fill(int numInserts, int upperBound = 100);
  void dump_tree();

private:
  TreeNode *root;
  std::vector<TreeNode *> nodes;
  int maxLevel, maxValues;
};
