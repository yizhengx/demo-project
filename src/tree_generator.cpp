#include "tree_algorithm.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 4) {
    std::cerr << "[ERROR]: Usage is ./generator <data_path> <num_elements> "
                 "<max_branch> (<max_values)"
              << std::endl;
    exit(1);
  }
  int num_elements = std::stol(argv[2]);
  printf("# of elements: %d\n", num_elements);

  int max_branch = std::stoi(argv[3]);
  printf("Max Branch: %d\n", max_branch);

  int max_values = num_elements;
  if (argc >= 5) {
    max_values = std::stoi(argv[4]);
  }

  Tree tree(max_branch, max_values);

  tree.fill(num_elements, num_elements);

  tree.init_serializer(argv[1], 1);

  tree.dump_tree();

  printf("TreeNode Count: %d\n", tree.numNode);

  return 0;
}
