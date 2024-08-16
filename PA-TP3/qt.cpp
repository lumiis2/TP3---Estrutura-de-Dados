#include "QuadTree.h"

QuadTree::QuadTree(Box box) {
    this->tree = new Quad[MAX];
    this->tree[this->root] = Quad(box);
    this->next = 1;
}

QuadTree::~QuadTree() {
    delete[] this->tree;
}

QuadAddress QuadTree::find(Point p) {
    QuadAddress quad = this->root;

    // Partindo da premissa que um vértice na árvore possui 0 ou 4 filhos exatamente, basta checar se existe um deles.
    while(this->tree[quad].northEast != NULLQUAD) {
        if(this->tree[this->tree[quad].northEast].boundary.contains(p)) quad = this->tree[quad].northEast;
        else if(this->tree[this->tree[quad].northWest].boundary.contains(p)) quad = this->tree[quad].northWest;
        else if(this->tree[this->tree[quad].southWest].boundary.contains(p)) quad = this->tree[quad].southWest;
        else if(this->tree[this->tree[quad].southEast].boundary.contains(p)) quad = this->tree[quad].southEast;
    }

    return quad;
}

QuadAddress QuadTree::find(Station s) { 
    QuadAddress quad = this->root;

    while((this->tree[quad].northEast != NULLQUAD) && (this->tree[quad].station.idend != s.idend)) {
        if(this->tree[this->tree[quad].northEast].boundary.contains(s.coord)) quad = this->tree[quad].northEast;
        else if(this->tree[this->tree[quad].northWest].boundary.contains(s.coord)) quad = this->tree[quad].northWest;
        else if(this->tree[this->tree[quad].southWest].boundary.contains(s.coord)) quad = this->tree[quad].southWest;
        else if(this->tree[this->tree[quad].southEast].boundary.contains(s.coord)) quad = this->tree[quad].southEast;
    }

    return quad;
}

void QuadTree::insert(Station s) {
    QuadAddress quad = this->find(s.coord);
    
    this->tree[quad].station = s;

    // Divisão do quadrante no ponto ocasionada pela inserção de "Station".
    
    this->tree[quad].northEast = this->next;
    this->tree[this->next++] = Box(Point(s.coord.x, this->tree[quad].boundary.topLeft.y), 
                                    Point(this->tree[quad].boundary.botRight.x, s.coord.y)); 
    
    this->tree[quad].northWest = this->next;
    this->tree[this->next++] = Box(this->tree[quad].boundary.topLeft, s.coord); 
    
    this->tree[quad].southWest = this->next;
    this->tree[this->next++] = Box(Point(this->tree[quad].boundary.topLeft.x, s.coord.y), 
                                    Point(s.coord.x, this->tree[quad].boundary.botRight.y));

    this->tree[quad].southEast = this->next;
    this->tree[this->next++] = Box(s.coord, this->tree[quad].boundary.botRight); 
}

void QuadTree::kNNRecursive(PriorityQueue<Pair<double, Station>> & pq, QuadAddress quad, Point p, int k) {
    if(quad == NULLQUAD) return;

    double distance = p.distance(this->tree[quad].station.coord);

    if(this->tree[quad].station.ativa) {
        if(pq.size() < k) {
            pq.insert(Pair<double, Station>(distance, this->tree[quad].station));
        } else if(distance < pq.top().first) {
            pq.remove();
            pq.insert(Pair<double, Station>(distance, this->tree[quad].station));
        }
    }

    kNNRecursive(pq, this->tree[quad].northEast, p, k);
    kNNRecursive(pq, this->tree[quad].northWest, p, k);
    kNNRecursive(pq, this->tree[quad].southWest, p, k);
    kNNRecursive(pq, this->tree[quad].southEast, p, k);
}

Pair<double, Station>* QuadTree::kNN(Point p, int k = 10) {
    PriorityQueue<Pair<double, Station>> pq;
    kNNRecursive(pq, this->root, p, k);

    Pair<double, Station>* nn = new Pair<double, Station>[k];

    // Max-heap em ordem invertida, da maior para a menor distância.
    for(int i = k-1; i >= 0; i--) {
        nn[i] = pq.remove();
    }

    return nn;
}

bool QuadTree::activate(Station s) {
    QuadAddress quad = this->find(s);
    bool ret = this->tree[quad].station.ativa;
    this->tree[quad].station.ativa = true;
    return ret;
}

bool QuadTree::disable(Station s) {
    QuadAddress quad = this->find(s);
    bool ret = !this->tree[quad].station.ativa;
    this->tree[quad].station.ativa = false;
    return ret;
}