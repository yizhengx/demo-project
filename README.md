# Concurrent Graph/Tree Traversal

This project aims to utilize internal concurrency of SSD to accelerate *no-cache* I/O in searching/traversing graphs and trees. 

## Algorithm

For now, depth-first (DFS) and bread-first searching (BFS) on tree are implemented and tested, with both serial and parallel version with OpenMP.

## Build

Goto ./src folder, then simply run

```bash
make
```

## Usage

```bash
# Generate test data
./generator <data_path> <num_elements> <branch>

# Run on generated data
./main <data_path>
```

## Reference

The unordered parallel DFS is referred to ideas in [A work-efficient algorithm for parallel unordered depth-first search](https://dl.acm.org/doi/10.1145/2807591.2807651).
