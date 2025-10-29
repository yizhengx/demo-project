#include "graph.hpp"
#include "serializer.hpp"
#include "tree.hpp"
#include <cstring>

#pragma once

Graph *readData(std::string path) {
  Serializer *sz = new Serializer();
  sz->openFile(path, MODE::READ);

  S_MetaData *sm = sz->readMetadata();
  if (!sm)
    return nullptr;

  Graph *g;
  switch (sm->dataType) {
  case 1:
    /* Tree */
    g = new Tree(0, 0);
    break;
  case 2:
    /* Graph */
    g = new Graph();
    break;
  default:
    return nullptr;
  }
  g->init_serializer(sz);
  return g;
}
