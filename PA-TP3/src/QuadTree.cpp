#include "QuadTree.h"

QuadTree::QuadTree(Box box) {
    tree = new Qnode[MAX];
    tree[root] = Qnode(box);
    next = 1;
}