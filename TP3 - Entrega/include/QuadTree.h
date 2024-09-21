#ifndef QUADTREE_H
#define QUADTREE_H

#include <queue>
#include "Qnode.h"
#include "PriorityQueue.h"
#include "Pair.h"

/**
 * @brief Classe que representa uma quadtree.
 * 
 * A quadtree é uma estrutura de dados usada para dividir o espaço em regiões
 * e armazenar pontos geográficos para consultas eficientes, como KNN (k-nearest neighbors).
 */
class QuadTree {
public:
    static const int MAX = 1e5; // Máximo número de nós na quadtree
    
    QuadTree();                ///< Construtor padrão
    QuadTree(Box box);         ///< Construtor com limites especificados
    ~QuadTree();               ///< Destruidor
    
    void insert(Addr s);       ///< Insere um endereço na quadtree
    Pair<double, Addr>* KNN(Point p, int k); ///< Encontra os k vizinhos mais próximos de um ponto
    bool activate(Addr s);    ///< Ativa um endereço (adiciona à quadtree)
    bool inactivate(Addr s);  ///< Desativa um endereço (remove da quadtree)

private:
    Qnode* tree;               ///< Vetor que armazena os nós da quadtree
    const T root = 0;         ///< Índice da raiz da quadtree no vetor
    T next;                   ///< Próximo índice disponível no vetor

    void KNNRecursive(PriorityQueue<Pair<double, Addr>>& pq, T quad, Point p, int k, double& minDistance);
    T find(Point p);          ///< Encontra o nó que contém um ponto
    T find(Addr s);           ///< Encontra o nó que contém um endereço
};

#endif // QUADTREE_H
