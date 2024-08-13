// Usar um tipo abstrato de dados QuadTree que execute as operações de construção, inserção, as funções mencionadas
// (consultas e atualizações) e destruição da estrutura de dados.
// Fazer essa implementação possa utilizando um vetor, onde ospontadores entre as células são índices do vetor de nodos. 
// Reimplementar as funções de consulta de estações de recarga próximas, assim como a ativação e desativação de pontos de recarga. 
//  A expectativa é que a implementação dessas três funções tenha complexidade sub-linear, em particular logarítmica

#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm> // Para sort
#include <cstdlib>   // Para atof
#include <cstdio>
#include <cfloat>

using namespace std;

struct Point {
    double x, y;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}

    // Overload equality operator
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    // Overload subtraction operator to compute vector between two points
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
};

// Função para calcular a distância quadrada entre dois pontos
double square_distance(const Point& p1, const Point& p2) {
    Point diff = p1 - p2;
    return diff.x * diff.x + diff.y * diff.y;
}


struct QuadNode {
    bool is_leaf;
    Point pt;
    int children[4]; // índices para os filhos
    int capacity;
    vector<Point> points;

    QuadNode() : is_leaf(true), capacity(1) {
        fill(begin(children), end(children), -1);
    }
};

class QuadTree {
private:
    vector<QuadNode> nodes;
    int root_idx;
    int node_capacity;

    int create_node() {
        nodes.push_back(QuadNode());
        return nodes.size() - 1;
    }

public:
    QuadTree(int capacity = 1) : node_capacity(capacity) {
        root_idx = create_node();
    }

    // Função para construir a árvore
    void insert(const Point& pt, int node_idx = 0) {
        QuadNode& node = nodes[node_idx];

        if (node.is_leaf) {
            if (node.points.size() < node.capacity) {
                node.points.push_back(pt);
            } else {
                // Dividir o nó e distribuir os pontos
                node.is_leaf = false;
                for (const Point& p : node.points) {
                    // Distribuir os pontos existentes
                    int quad = get_quadrant(p);
                    if (node.children[quad] == -1) {
                        node.children[quad] = create_node();
                    }
                    insert(p, node.children[quad]);
                }
                node.points.clear();
                // Inserir o novo ponto
                int quad = get_quadrant(pt);
                if (node.children[quad] == -1) {
                    node.children[quad] = create_node();
                }
                insert(pt, node.children[quad]);
            }
        } else {
            int quad = get_quadrant(pt);
            if (node.children[quad] == -1) {
                node.children[quad] = create_node();
            }
            insert(pt, node.children[quad]);
        }
    }

    int get_quadrant(const Point& pt) {
        // Implementar lógica para determinar o quadrante de um ponto
        // baseado no centro da bounding box ou do nó atual.
        // Retorna um valor entre 0 e 3, correspondendo ao quadrante.
    }

    // Função para consultar o ponto mais próximo (distância mínima)
    double nearest(const Point& p, int node_idx = 0) {
        QuadNode& node = nodes[node_idx];

        if (node.is_leaf) {
            double min_dist = DBL_MAX;
            for (const Point& pt : node.points) {
                double dist = square_distance(p, pt);
                min_dist = min(min_dist, dist);
            }
            return min_dist;
        } else {
            double best_dist = DBL_MAX;
            for (int i = 0; i < 4; ++i) {
                if (node.children[i] != -1) {
                    best_dist = min(best_dist, nearest(p, node.children[i]));
                }
            }
            return best_dist;
        }
    }

    // Função para ativar/desativar pontos de recarga
    void update_status(const Point& pt, bool active) {
        // Implementar lógica para ativar/desativar o ponto
        // Pode envolver a remoção do ponto da árvore ou
        // a marcação do status em uma estrutura associada.
    }
};

