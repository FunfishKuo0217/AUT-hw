#include "graph.h"
#include <vector>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <climits>
#include <limits>


// function 1: addVertex
template<typename T>
void Graph<T>::addVertex(const T& v) {
    for (const Node* node : head) {
        if (node && node->value == v) {
            std::cout << "Node Already Exist!\n";
            return;
        }
    }
    Node* newNode = new Node(v);
    head.push_back(newNode);
}

// function 2: addEdge
template<typename T>
void Graph<T>::addEdge(const T& v1, const T& v2, int a, std::function<bool(T, T)> func) {
    Node* node1 = nullptr;
    Node* node2 = nullptr;

    // find v1 and v2
    for (Node* node : head) {
        if (func(node->value, v1)) {node1 = node;}
        if (func(node->value, v2)) {node2 = node;}
    }

    // if v1 or v2 does not exist
    if(!node1){addVertex(v1);}
    if(!node2){addVertex(v2);}

    // confirm v1 and v2
    for (Node* node : head) {
        if (func(node->value, v1)) {node1 = node;}
        if (func(node->value, v2)) {node2 = node;}
    }

    // check if the edge is exist, update new weight
    Node* current = node1->next;
    while (current) {
        if (func(current->value, v2)) {
            current->weight = a;
            return;
        }
        current = current->next;
    }

    // otherwise, create new edges
    Node* newEdge = new Node(v2);
    newEdge->weight = a;
    newEdge->next = node1->next;
    node1->next = newEdge;
}

// function 3: display
template<typename T>
void Graph<T>::disp() {
    for (const Node* node : head) {
        std::cout << "Vertex: " << node->value << " has edges to: ";
        Node* edge = node->next;
        while (edge) {
            std::cout << "(" << edge->value << ", weight: " << edge->weight << ") ";
            edge = edge->next;
        }
        std::cout << std::endl;
    }
}

// function 4: getNumEdges 
template<typename T>
int Graph<T>::getNumEdges() {
    int numEdges = 0;
    for(const Node* node : head){
        Node* edge = node->next;
        while(edge){
            numEdges++; 
            edge = edge->next;
        }
    }
    return numEdges;
}


// function 5: getNeighbors
template<typename T>
std::vector<T> Graph<T>::getNeighbors(T vertex, std::function<bool(T, T)> func) {
    std::vector<T> neighbors;

    // find node
    Node* currNode = nullptr;
    for(Node* node : head){
        if(func(node->value, vertex)){
            currNode = node;
            break;
        }
    }

    if(currNode){
        Node* u = currNode->next;
        while(u){
            neighbors.push_back(u->value);
            u = u->next;
        }
    }

    // sorting for answer order checking
    sort(neighbors.begin(), neighbors.end());
    return neighbors;

}


// function 6: find SP
template<typename T>
std::vector<T> Graph<T>::findShortestPath(T source, T destination, std::function<bool(T, T)> func) {
    // Dijkstra
    // pq: find the shortest weight node for current node (dis, node)
    std::priority_queue<std::pair<int, T>, std::vector<std::pair<int, T>>, std::greater<>> pq;

    std::unordered_map<T, int> dist; // current shortest path
    std::unordered_map<T, T> prev; // parent node

    // initialization: dist and parent node
    for(Node* node : head){
        dist[node->value] = INT_MAX;
        prev[node->value] = T(); // default T constructor
    }

    dist[source] = 0;
    pq.push({0, source});

    while(!pq.empty()){
        T currNodeVal = pq.top().second;
        pq.pop();

        // find the destination! 
        if(func(currNodeVal, destination)){
            std::vector<T> path;
            for(T curr = destination; curr != T(); curr = prev[curr]){
                path.push_back(curr);
            }   
            std::reverse(path.begin(), path.end());
            return path;
        }

        // check neighbors for current node
        Node* currNode = nullptr;
        for(Node* node : head){
            if(func(node->value, currNodeVal)){
                currNode = node;
                break;
            }
        }

        if(currNode){
            Node* edge = currNode->next;
            // go through all edges for current node
            while(edge){
                int newDist = dist[currNodeVal] + edge->weight;
                if(newDist < dist[edge->value]){
                    dist[edge->value] = newDist;
                    prev[edge->value] = currNodeVal;
                    pq.push({newDist, edge->value});
                }
                edge = edge->next;
            }
        }
    }
    return {};
}

// function 7(b): bfs -> change function to return visit path by dist
template<typename T>
std::vector<T> Graph<T>::bfs(int vertex){
    // core: dijkstra
    std::priority_queue<std::pair<int, T>, std::vector<std::pair<int, T>>, std::greater<>> pq;
    std::unordered_map<T, int> dist;
    std::vector<T> visitOrder;

    for(Node* node : head){
        dist[node->value] = INT_MAX;
    }

    dist[vertex] = 0;
    pq.push({0, vertex});

    while(!pq.empty()){
        T currNodeVal = pq.top().second;
        pq.pop();

        visitOrder.push_back(currNodeVal);
        
        Node* currNode = nullptr;
        for(Node* node : head){
            if(currNodeVal == node->value){
                currNode = node;
                break;
            }
        }
        
        if(currNode){
            Node* edge = currNode->next;
            while(edge){
                int newDist = edge->value + dist[currNodeVal];
                if(newDist < dist[edge->value]){
                    dist[edge->value] = newDist;
                    pq.push({newDist, edge->value});
                }
                edge = edge->next;
            }
        }
    }
    return visitOrder;
}

// instantiation for class
template class Graph<int>;
template class Graph<char>;



/* Part II: BST */
Node* insert(Node* root, int value) {
    if(!root){return new Node(value);}
    if(root->value > value){
        root->left = insert(root->left, value);
    }
    else if(root->value < value){
        root->right = insert(root->right, value);
    }
    return root;
}


Node* findMin(Node* root) {
    while (root->left != nullptr) {
        root = root->left;
    }
    return root;
}

Node* deleteNodeHelper(Node* root, int value) {
    if (root == nullptr) {
        return nullptr;
    }

    if (value < root->value) {
        root->left = deleteNodeHelper(root->left, value);
    } else if (value > root->value) {
        root->right = deleteNodeHelper(root->right, value);
    } else {
        if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        } else {
            Node* temp = findMin(root->right);
            root->value = temp->value;
            root->right = deleteNodeHelper(root->right, temp->value);
        }
    }
    return root;
}

void deleteNode(Node* root, int value) {
    root = deleteNodeHelper(root, value);
}

Node* find(Node* root, int value) {
    if(root == nullptr){return root;}
    if(root->value > value){
        return find(root->left, value);
    }
    if(root->value < value){
        return find(root->right, value);
    }
    return root;
}

void printInOrder(Node* root) {
    if(root == nullptr){return;}
    if(root->left){printInOrder(root->left);}
    std::cout << root->value << " ";
    if(root->right){printInOrder(root->right);}
}


// ARCHIVE
// function 7(a): bfs -> change function to return visit path
// template<typename T>
// std::vector<T> Graph<T>::bfs(int vertex) {
//     std::vector<T> visPath;
//     std::queue<T> q;
//     std::unordered_map<T, bool> vis;
//     // initialize
//     for(Node* node : head){ vis[node->value] = false; }
    
//     vis[vertex] = true;
//     q.push(vertex);

//     while(!q.empty()){
//         T currNodeVal = q.front();
//         q.pop();
//         visPath.push_back(currNodeVal);

//         Node* currNode = nullptr;
//         for(Node* node : head){
//             if(node->value == currNodeVal){
//                 currNode = node;
//                 break;
//             }
//         }

//         if(currNode){
//             Node* edge = currNode->next;
//             while(edge){
//                 if(!vis[edge->value]){
//                     vis[edge->value] = true;
//                     q.push(edge->value);
//                 }
//                 edge = edge->next;
//             }
//         }
//     }
//     return visPath;
// }