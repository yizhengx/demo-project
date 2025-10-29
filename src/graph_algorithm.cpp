#include "graph_algorithm.hpp"

bool s_dfs(Graph *graph, int &key) {
  std::vector<int> frontier;
  std::unordered_set<int> vis;

  frontier.push_back(0);
  vis.insert(0);

  while (!frontier.empty()) {
    int tmpNum = frontier.back();
    if (vis.find(tmpNum) == vis.end()) {
      // Not visited
      S_Node *s_node = graph->read_snode(frontier.back());

      if (s_node->key == key)
        return true;

      frontier.pop_back();
      vis.insert(tmpNum);

      frontier.insert(frontier.end(), s_node->data,
                      s_node->data + s_node->degree);
    } else {
      frontier.pop_back();
    }
  }

  return false;
}

bool s_bfs(Graph *graph, int &key) {
  std::vector<int> frontier;
  std::vector<int> next;
  std::unordered_set<int> vis;

  frontier.push_back(0);
  vis.insert(0);

  bool isFound = false;

  while (!isFound && frontier.size() > 0) {
    for (auto i = 0; i < frontier.size(); i++) {
      if (isFound)
        continue;
      int tmpNum = frontier[i];
      bool isExisted;
      isExisted = (vis.find(tmpNum) != vis.end());
      if (isExisted)
        continue;
      vis.insert(tmpNum);
      S_Node *s_node = graph->read_snode(tmpNum);
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
  return isFound;
}

bool p_bfs(Graph *graph, int &key) {
  std::vector<int> frontier;
  std::vector<int> next;
  std::unordered_set<int> vis;

  frontier.push_back(0);
  vis.insert(0);

  bool isFound = false;

  while (!isFound && frontier.size() > 0)
#pragma omp parallel
  {
#pragma omp for nowait
    for (auto i = 0; i < frontier.size(); i++) {
      if (isFound)
        continue;
      int tmpNum = frontier[i];
      bool isExisted;
#pragma omp critical
      isExisted = (vis.find(tmpNum) != vis.end());
      if (isExisted)
        continue;
#pragma omp critical
      vis.insert(tmpNum);
      S_Node *s_node = graph->read_snode(tmpNum);
      if (s_node->key == key) {
        isFound = true;
        continue;
      }
      if (s_node->degree == 0)
        continue;
#pragma omp critical
      next.insert(next.end(), s_node->data, s_node->data + s_node->degree);
    }
    frontier = next;
    next.clear();
  }
  return isFound;
}

bool s_bfs_alt(Graph *graph, int &key) {
  std::vector<int> frontier;
  std::unordered_set<int> next;
  std::unordered_set<int> vis;

  frontier.push_back(0);
  vis.insert(0);

  bool isFound = false;

  while (!isFound && frontier.size() > 0) {
    for (auto i = 0; i < frontier.size(); i++) {
      if (isFound)
        continue;
      S_Node *s_node = graph->read_snode(frontier[i]);
      if (s_node->key == key) {
        isFound = true;
        continue;
      }
      if (s_node->degree == 0)
        continue;
      for (int i = 0; i < s_node->degree; i++) {
        int nextNum = s_node->data[i];
        if (vis.find(nextNum) == vis.end()) {
          next.insert(nextNum);
          vis.insert(nextNum);
        }
      }
    }
    frontier.assign(next.begin(), next.end());
    next.clear();
  }
  return isFound;
}

bool p_bfs_alt(Graph *graph, int &key) {
  std::vector<int> frontier;
  std::unordered_set<int> next;
  std::unordered_set<int> vis;

  frontier.push_back(0);
  vis.insert(0);

  bool isFound = false;

  while (!isFound && frontier.size() > 0)
#pragma omp parallel
  {
#pragma omp for nowait
    for (auto i = 0; i < frontier.size(); i++) {
      if (isFound)
        continue;
      S_Node *s_node = graph->read_snode(frontier[i]);
      if (s_node->key == key) {
        isFound = true;
        continue;
      }
      if (s_node->degree == 0)
        continue;
      for (int i = 0; i < s_node->degree; i++) {
        int nextNum = s_node->data[i];
#pragma omp critical
        if (vis.find(nextNum) == vis.end()) {
          next.insert(nextNum);
          vis.insert(nextNum);
        }
      }
    }
    frontier.assign(next.begin(), next.end());
    next.clear();
  }
  return isFound;
}
