#ifndef QUADTREE_H
#define QUADTREE_H

#include <queue>
#include "Qnode.h"
#include "PriorityQueue.h"
#include "Pair.h"


class QuadTree {
public:
    static const int MAX = 1e5; // Tamanho máximo da quadtree
    
    QuadTree();
    QuadTree(Box box);
    ~QuadTree();
    
    void insert(Addr s);
    Pair<double, Addr>* KNN(Point p, int k);
    bool activate(Addr s);
    bool inactivate(Addr s);

private:
    Qnode* tree;       // Vetor que representa a quadtree
    const T root = 0;            // Índice da raiz da quadtree na implementação por vetor
    T next;            // Índice da próxima posição disponível no vetor

    void KNNRecursive(PriorityQueue<Pair<double, Addr>>& pq, T quad, Point p, int k);
    T find(Point p);
    T find(Addr s);
};

#endif // QUADTREE_H
