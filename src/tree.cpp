#include "tree.hpp"

bool Tree::add(int key, int value) {
  if (!root) {
    root = new TreeNode(maxDegree, maxValues);
    root->id = numNode++;
    root->key = key;
    root->values.push_back(value);
    root->numValues = 1;
    root->level = 0;
    nodes.push_back(root);
    return true;
  }

  std::vector<int> frontier;
  std::vector<int> next;

  frontier.push_back(0);

  while (frontier.size() > 0) {
    for (auto i = 0; i < frontier.size(); i++) {
      TreeNode *curNode = nodes[frontier[i]];
      // Found, Insert to values
      if (curNode->key == key) {
        if (curNode->numValues < curNode->maxValues) {
          curNode->values.push_back(value);
          curNode->numValues++;
        }
        return true;
      }
      next.insert(next.end(), curNode->edges.begin(), curNode->edges.end());
    }
    frontier = next;
    next.clear();
  }

  //  Not Found, new TreeNode
  TreeNode *newNode = new TreeNode(maxDegree, maxValues);
  newNode->id = numNode++;
  newNode->key = key;
  newNode->values.push_back(value);
  newNode->numValues = 1;

  frontier.clear();
  next.clear();
  frontier.push_back(0);

  while (frontier.size() > 0) {
    for (auto i = 0; i < frontier.size(); i++) {
      TreeNode *curNode = nodes[frontier[i]];
      if (curNode->degree < curNode->maxDegree) {
        curNode->edges.push_back(newNode->id);
        curNode->degree++;
        newNode->level = curNode->level + 1;
        maxLevel = std::max(maxLevel, newNode->level);
        nodes.push_back(newNode);
        return true;
      }
      next.insert(next.end(), curNode->edges.begin(), curNode->edges.end());
    }
    frontier = next;
    next.clear();
  }

  return false;
}

void Tree::fill(int numInserts, int upperBound) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, upperBound);

  std::vector<int> keys, values;
  for (auto i = 0; i < numInserts; i++) {
    int key = distrib(gen);
    int value = distrib(gen);
    keys.push_back(key);
    values.push_back(value);
    add(key, value);
  }
}

void Tree::dump_tree() {
// Dump metadata
#ifdef _WIN32
  S_MetaData *s_metadata =
      (S_MetaData *)_aligned_malloc(sizeof(S_MetaData), BLOCK_SIZE);
#else
  S_MetaData *s_metadata =
      (S_MetaData *)aligned_alloc(BLOCK_SIZE, sizeof(S_MetaData));
#endif
  s_metadata->numNode = numNode;
  s_metadata->maxDegree = maxDegree;
  s_metadata->dataType = 1;
  gs.writeMetadata(s_metadata);

#pragma omp parallel for
  // Dump each node
  for (int i = 0; i < numNode; i++) {
    dump_node(nodes[i]);
  }
}
