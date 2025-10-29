#include "tree_algorithm.hpp"

bool s_dfs(Tree *tree, int &key) { return _s_dfs(tree, 0, key); }

bool _s_dfs(Tree *tree, int id, int &key) {
  S_Node *s_node = tree->read_snode(id);
  if (s_node->key == key)
    return true;
  for (auto i = 0; i < s_node->degree; i++) {
    if (_s_dfs(tree, s_node->data[i], key))
      return true;
  }
  return false;
}

bool s_dfs_2(Tree *tree, int &key) {
  std::vector<int> frontier;
  frontier.push_back(0);

  while (!frontier.empty()) {
    S_Node *s_node = tree->read_snode(frontier.back());

    if (s_node->key == key)
      return true;

    frontier.pop_back();

    frontier.insert(frontier.end(), s_node->data,
                    s_node->data + s_node->degree);
  }
  return false;
}

void _p_dfs_omp(Tree *tree, int &key, std::vector<int> &frontier, int &fSize,
                bool &isFound) {
#pragma omp taskgroup
  while (!frontier.empty()) {
    if (isFound) {
      break;
    }
    // Read stack top
    S_Node *s_node = tree->read_snode(frontier.back());
    // Found it?
    if (s_node->key == key)
      isFound = true;
    if (isFound)
      break;
    
    // Pop stack top
    frontier.pop_back();
    // Insert its children
    frontier.insert(frontier.end(), s_node->data,
                    s_node->data + s_node->degree);
    // Stack size larger than fSize?
    while (frontier.size() > fSize) {
      if (isFound)
        break;
      // Split in two
      std::vector<int> frontier_new(frontier.begin() + frontier.size() / 2,
                                    frontier.end());
      frontier.resize(frontier.size() / 2);
#pragma omp task shared(tree, key, fSize, isFound)
      // Give one half to another thread
      _p_dfs_omp(tree, key, frontier_new, fSize, isFound);
    }
  }
}

bool p_dfs_omp(Tree *tree, int &key, int f_size) {
  std::vector<int> initFrontier;
  int fSize = f_size;

  initFrontier.push_back(0);
  bool isFound = false;
#pragma omp parallel
#pragma omp single nowait
  { _p_dfs_omp(tree, key, initFrontier, fSize, isFound); }

  return isFound;
}

bool s_iddfs_worker(Tree *tree, int id, int &key, size_t depLeft) {
  S_Node *s_node = tree->read_snode(id);
  if (s_node->key == key)
    return true;
  if (depLeft == 0)
    return false;
  for (int i = 0; i < s_node->degree; i++) {
    if (s_iddfs_worker(tree, s_node->data[i], key, depLeft - 1))
      return true;
  }
  return false;
}

bool s_iddfs(Tree *tree, int &key, size_t maxDepth) {
  size_t dep = 1;
  while (dep <= maxDepth) {
    if (s_iddfs_worker(tree, 0, key, dep))
      return true;
    dep++;
  }
  return false;
}

// void p_dfs(TreeNode *curNode, int key, bool *isFound, std::queue<TreeNode *>
// *nodeQ)
// {
//   while (1) {
//     // std::cout << curNode << " " << curNode->key << std::endl;
//     if (*isFound)
//       return;
//     if (curNode->key == key) {
//       *isFound = true;
//       return;
//     }
//     for (size_t i = 1; i < curNode->degree; i++) {
//       (*nodeQ).push(curNode->edges[i]);
//     }
//     if (curNode->degree >= 1) {
//       curNode = curNode->edges[0];
//     } else
//       break;
//   }
// }

// void p_bfs(TreeNode *curNode, int key, bool *isFound, std::queue<TreeNode *>
// *nodeQ)
// {
//   // std::cout << curNode << " " << curNode->key << std::endl;
//   if (*isFound)
//     return;
//   if (curNode->key == key) {
//     *isFound = true;
//     return;
//   }
//   for (size_t i = 0; i < curNode->degree; i++) {
//     (*nodeQ).push(curNode->edges[i]);
//   }
// }

// void p_sche(TreeNode *root, int type, int key) {
//   bool isFound = false;
//   std::queue<TreeNode *> nodeQ;
//   nodeQ.push(root);
//   while (1) {
//     if (isFound)
//       break;
//     if (!nodeQ.empty()) {
//       TreeNode *t = nodeQ.front();
//       nodeQ.pop();
//       if (type == 0) {
//         std::async(p_bfs, t, key, &isFound, &nodeQ);
//       } else if (type == 1) {
//         std::async(p_dfs, t, key, &isFound, &nodeQ);
//       }
//     }
//   }
// }

bool s_bfs(Tree *tree, int &key) {
  std::vector<int> frontier;
  std::vector<int> next;

  frontier.push_back(0);

  bool isFound = false;
  // int rfCnt = 0;
  while (!isFound && frontier.size() > 0) {
    for (size_t i = 0; i < frontier.size(); i++) {
      if (isFound)
        continue;
      S_Node *s_node = tree->read_snode(frontier[i]);
      // rfCnt++;
      if (s_node->key == key) {
        isFound = true;
        continue;
      }
      if (s_node->degree == 0)
        continue;
      next.insert(next.end(), s_node->data, s_node->data + s_node->degree);
    }
    frontier = next;
    next.clear();
  }
  // printf("Read file count = %d\n", rfCnt);
  return isFound;
}

bool p_bfs_omp(Tree *tree, int &key) {
  std::vector<int> frontier;
  std::vector<int> next;

  frontier.push_back(0);

  bool isFound = false;
  // int rfCnt = 0;
  while (!isFound && frontier.size() > 0) {
#pragma omp parallel
    {
#pragma omp for nowait
      for (size_t i = 0; i < frontier.size(); i++) {
        if (isFound)
          continue;
        S_Node *s_node = tree->read_snode(frontier[i]);
        // rfCnt++;
        if (s_node->key == key) {
          isFound = true;
          continue;
        }
        if (s_node->degree == 0)
          continue;
#pragma omp critical
        next.insert(next.end(), s_node->data, s_node->data + s_node->degree);
      }
    }
    frontier = next;
    next.clear();
  }
  // printf("Read file count = %d\n", rfCnt);
  return isFound;
}

bool p_iddfs_worker(Tree *tree, int id, int &key, size_t depLeft) {
  S_Node *s_node = tree->read_snode(id);
  if (s_node->key == key)
    return true;
  if (depLeft == 0)
    return false;

  bool isFound = false;
  // #pragma omp parallel for
  for (int i = 0; i < s_node->degree; i++) {
    if (isFound)
      continue;
    isFound |= p_iddfs_worker(tree, s_node->data[i], key, depLeft - 1);
  }
  return isFound;
}

bool p_iddfs_omp(Tree *tree, int &key, size_t maxDepth) {
  bool isFound = false;
#pragma omp parallel for
  for (size_t i = 1; i <= maxDepth; i++) {
    if (isFound)
      continue;
    if (p_iddfs_worker(tree, 0, key, i))
      isFound = true;
  }
  return isFound;
}

bool p_hybrid_omp(Tree *tree, int id, int &key, int &brhThres) {
  std::vector<int> frontier;
  std::vector<int> next;

  frontier.push_back(id);
  bool isFound = false;
  int rfCnt = 0;
  while (!isFound && frontier.size() > 0) {
#pragma omp parallel
    {
#pragma omp for nowait
      for (size_t i = 0; i < frontier.size(); i++) {
        S_Node *s_node = tree->read_snode(frontier[i]);
        rfCnt++;
        if (s_node->key == key)
          isFound = true;
        if (isFound || s_node->degree == 0)
          continue;
        if (s_node->degree > brhThres) {
#pragma omp critical
          next.insert(next.end(), s_node->data,
                      s_node->data + s_node->degree);
        } else {
#pragma omp parallel for
          for (int i = 0; i < s_node->degree; i++) {
            if (isFound)
              continue;
            isFound |= p_hybrid_omp(tree, s_node->data[i], key, brhThres);
          }
        }
      }
    }
    frontier = next;
    next.clear();
  }
  // printf("Read file count = %d\n", rfCnt);
  return isFound;
}

bool p_test_omp(Tree *tree, int id, int &key, int &maxFSize) {
  std::vector<int> frontier;
  std::vector<int> next;

  frontier.push_back(0);

  bool isFound = false;
  int rfCnt = 0;
  while (!isFound && frontier.size() > 0) {
#pragma omp parallel
    {
#pragma omp for nowait
      for (size_t i = 0; i < frontier.size(); i++) {
        S_Node *s_node = tree->read_snode(frontier[i]);
        rfCnt++;
        if (s_node->key == key)
          isFound = true;
        if (isFound || s_node->degree == 0)
          continue;
#pragma omp critical
        next.insert(next.end(), s_node->data, s_node->data + s_node->degree);
      }
    }
    frontier = next;
    next.clear();
  }
  printf("Read file count = %d\n", rfCnt);
  return isFound;
}
