#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <functional>

template<typename T>
class Graph
{
public:
  Graph() = default;

  // TODOs
  int getNumConnectedComponents();
  bool isConnected(T source, T destination, std::function<bool(T, T)> func);
  std::vector<T> topologicalSort();

  // DONE
  void addVertex(const T& v);
  void addEdge(const T& v1, const T& v2, int a, std::function<bool(T, T)> func = [](T a, T b){return a == b;});
  int getNumEdges();
  void disp(); 
  std::vector<T> bfs(int vertex);
  std::vector<T> findShortestPath(T source, T destination, std::function<bool(T, T)> func = [](T a, T b){return a == b;});
  std::vector<T> getNeighbors(T vertex, std::function<bool(T, T)> func = [](T a, T b){return a == b;});

private:
    class Node
    {
      public:
          T value;
          int weight{};
          Node* next{};
          
          //TODO: constructors and methods
          Node(T v) : value(v), weight(0), next(nullptr) {}; 
    };
    std::vector<Node*> head;
};

struct Node { //// for part 2
  int value;
  Node* left;
  Node* right;

  Node(int value) {
    this->value = value;
    left = nullptr;
    right = nullptr;
  }
};

Node* insert(Node* root, int value);
void deleteNode(Node* root, int value);
Node* find(Node* root, int value);
void printInOrder(Node* root);

#endif





