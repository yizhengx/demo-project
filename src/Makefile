CC = g++
CFLAGS = -Wall -O3 -march=native -std=c++17 -fopenmp -D_GNU_SOURCE
OBJS = tree.o tree_algorithm.o serializer.o graph.o graph_algorithm.o

build: $(OBJS) main.o tree_generator.o
	$(CC) $(CFLAGS) $(OBJS) main.o -o main
	$(CC) $(CFLAGS) $(OBJS) tree_generator.o -o generator

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

serializer.o: serializer.cpp serializer.hpp
	$(CC) $(CFLAGS) -c serializer.cpp

tree.o: tree.cpp tree.hpp
	$(CC) $(CFLAGS) -c tree.cpp

tree_algorithm.o: tree_algorithm.cpp tree_algorithm.hpp
	$(CC) $(CFLAGS) -c tree_algorithm.cpp

tree_generator.o: tree_generator.cpp
	$(CC) $(CFLAGS) -c tree_generator.cpp

graph.o: graph.cpp graph.hpp
	$(CC) $(CFLAGS) -c graph.cpp

graph_algorithm.o: graph_algorithm.cpp graph_algorithm.hpp
	$(CC) $(CFLAGS) -c graph_algorithm.cpp

diotest:
	$(CC) -g $(CFLAGS) -pthread diotest.cpp -o diotest

clean:
	rm *.o main generator