#include "graph.hpp"

#include <omp.h>
#include <queue>
#include <vector>
#include <unordered_set>
#include <algorithm>


bool s_bfs(Graph *graph, int &key);
bool s_dfs(Graph *graph, int &key);
bool s_dfs_alt(Graph *graph, int &key);

bool p_bfs(Graph *graph, int &key);
bool p_bfs_alt(Graph *graph, int &key);

