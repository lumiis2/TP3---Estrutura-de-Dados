#include "QuadTree.h"
#include <cmath>

// Construtor padrão
QuadTree::QuadTree() : tree(new Qnode[MAX]), next(1) {
    Box defaultBox; // Supondo que Box tem um construtor padrão
    tree[root] = Qnode(defaultBox);
}

// Construtor com limites específicos
QuadTree::QuadTree(Box box) : tree(new Qnode[MAX]), next(1) {
    tree[root] = Qnode(box);
}

// Destrutor
QuadTree::~QuadTree() {
    delete[] tree;
}

// Encontra o quadrante que contém o ponto
T QuadTree::find(Point p) {
    T quad = root;
    while (tree[quad].nw != -1) {
        if (tree[tree[quad].ne].limites.contains(p))
            quad = tree[quad].ne;
        else if (tree[tree[quad].nw].limites.contains(p))
            quad = tree[quad].nw;
        else if (tree[tree[quad].sw].limites.contains(p))
            quad = tree[quad].sw;
        else if (tree[tree[quad].se].limites.contains(p))
            quad = tree[quad].se;
    }
    return quad;
}

// Encontra o quadrante que contém o endereço
T QuadTree::find(Addr s) {
    T quad = root;
    while (tree[quad].nw != -1 && tree[quad].station.idend != s.idend) {
        if (tree[tree[quad].ne].limites.contains(s.coordenadas))
            quad = tree[quad].ne;
        else if (tree[tree[quad].nw].limites.contains(s.coordenadas))
            quad = tree[quad].nw;
        else if (tree[tree[quad].sw].limites.contains(s.coordenadas))
            quad = tree[quad].sw;
        else if (tree[tree[quad].se].limites.contains(s.coordenadas))
            quad = tree[quad].se;
    }
    return quad;
}

// Insere um endereço na quadtree
void QuadTree::insert(Addr s) {
    T quad = find(s.coordenadas);
    tree[quad].station = s;

    // Subdivide o quadrante e cria novos filhos
    tree[quad].ne = next;
    tree[next++] = Qnode(Box(Point(s.coordenadas.x, tree[quad].limites.topLeft.y), 
                             Point(tree[quad].limites.bottomRight.x, s.coordenadas.y))); 
    
    tree[quad].nw = next;
    tree[next++] = Qnode(Box(tree[quad].limites.topLeft, s.coordenadas)); 
    
    tree[quad].sw = next;
    tree[next++] = Qnode(Box(Point(tree[quad].limites.topLeft.x, s.coordenadas.y), 
                             Point(s.coordenadas.x, tree[quad].limites.bottomRight.y)));

    tree[quad].se = next;
    tree[next++] = Qnode(Box(s.coordenadas, tree[quad].limites.bottomRight)); 
}

// Função recursiva para encontrar k vizinhos mais próximos
void QuadTree::KNNRecursive(PriorityQueue<Pair>& pq, T quad, Point p, int k) {
    if (quad == -1) return; // Verifica se o quadrante é inválido

    double distance = p.euclideanDistance(tree[quad].station.coordenadas);
    
    if (tree[quad].station.ativo) {
        if (pq.size < k) {
            pq.insert(Pair(distance, tree[quad].station));
        } else if (distance < pq.top().first) {
            pq.remove();
            pq.insert(Pair(distance, tree[quad].station));
        }
    }
    
    // Recorre nos quadrantes filhos
    KNNRecursive(pq, tree[quad].ne, p, k);
    KNNRecursive(pq, tree[quad].nw, p, k);
    KNNRecursive(pq, tree[quad].sw, p, k);
    KNNRecursive(pq, tree[quad].se, p, k);
}
 
// Retorna os k vizinhos mais próximos
Pair* QuadTree::KNN(Point p, int k) {
    PriorityQueue<Pair> pq;
    KNNRecursive(pq, root, p, k);

    Pair* nn = new Pair[k];
    int i = k - 1;
    while (!pq.empty()) {
        nn[i--] = pq.top();
        pq.remove();
    }
    return nn;
}

// Ativa um endereço e retorna o estado anterior
bool QuadTree::activate(Addr s) {
    T quad = find(s);
    bool wasActive = tree[quad].station.ativo;
    tree[quad].station.ativo = true;
    return wasActive;
}

// Desativa um endereço e retorna o estado anterior
bool QuadTree::inactivate(Addr s) {
    T quad = find(s);
    bool wasInactive = !tree[quad].station.ativo;
    tree[quad].station.ativo = false;
    return wasInactive;
}
