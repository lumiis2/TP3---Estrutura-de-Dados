// Usar um tipo abstrato de dados QuadTree que execute as operações de construção, inserção, as funções mencionadas
// (consultas e atualizações) e destruição da estrutura de dados.
// Fazer essa implementação possa utilizando um vetor, onde ospontadores entre as células são índices do vetor de nodos. 
// Reimplementar as funções de consulta de estações de recarga próximas, assim como a ativação e desativação de pontos de recarga. 
//  A expectativa é que a implementação dessas três funções tenha complexidade sub-linear, em particular logarítmica

#include <iostream>
#include <vector>
#include <cfloat>
#include <algorithm>

using namespace std;

struct Point {
    double x, y;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
};

double square_distance(const Point& p1, const Point& p2) {
    Point diff = p1 - p2;
    return diff.x * diff.x + diff.y * diff.y;
}

struct QuadNode {
    bool is_leaf;
    vector<Point> points;
    int children[4];
    int capacity;

    QuadNode(int cap) : is_leaf(true), capacity(cap) {
        fill(begin(children), end(children), -1);
    }
};

class QuadTree {
private:
    vector<QuadNode> nodes;
    int root_idx;
    int node_capacity;

    int create_node() {
        nodes.push_back(QuadNode(node_capacity));
        return nodes.size() - 1;
    }

    Point calculate_center(const QuadNode& node) const {
        double minX = DBL_MAX, minY = DBL_MAX;
        double maxX = -DBL_MAX, maxY = -DBL_MAX;
        for (const Point& pt : node.points) {
            minX = min(minX, pt.x);
            minY = min(minY, pt.y);
            maxX = max(maxX, pt.x);
            maxY = max(maxY, pt.y);
        }
        return Point((minX + maxX) / 2, (minY + maxY) / 2);
    }

    void subdivide(QuadNode& node) {
        node.is_leaf = false;
        for (const Point& pt : node.points) {
            int quad = get_quadrant(pt, calculate_center(node));
            if (node.children[quad] == -1) {
                node.children[quad] = create_node();
            }
            insert(pt, node.children[quad]);
        }
        node.points.clear();
    }

public:
    QuadTree(int capacity = 1) : node_capacity(capacity) {
        root_idx = create_node();
    }

    void insert(const Point& pt, int node_idx = 0) {
        QuadNode& node = nodes[node_idx];

        if (node.is_leaf) {
            if (node.points.size() < node.capacity) {
                node.points.push_back(pt);
            } else {
                subdivide(node);
                insert(pt, node_idx);  // Reinserir o ponto após a subdivisão
            }
        } else {
            Point center = calculate_center(node);
            int quad = get_quadrant(pt, center);
            if (node.children[quad] == -1) {
                node.children[quad] = create_node();
            }
            insert(pt, node.children[quad]);
        }
    }

    int get_quadrant(const Point& pt, const Point& center) const {
        if (pt.x >= center.x && pt.y >= center.y)
            return 0; // Quadrante superior direito
        else if (pt.x < center.x && pt.y >= center.y)
            return 1; // Quadrante superior esquerdo
        else if (pt.x < center.x && pt.y < center.y)
            return 2; // Quadrante inferior esquerdo
        else
            return 3; // Quadrante inferior direito
    }

    double nearest(const Point& p, int node_idx = 0) const {
        const QuadNode& node = nodes[node_idx];

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

    void update_status(const Point& pt, bool active) {
        if (active) {
            insert(pt);
        } else {
            remove(pt);
        }
    }

    void remove(const Point& pt, int node_idx = 0) {
        QuadNode& node = nodes[node_idx];

        if (node.is_leaf) {
            auto it = find(node.points.begin(), node.points.end(), pt);
            if (it != node.points.end()) {
                node.points.erase(it);
            }
        } else {
            Point center = calculate_center(node);
            int quad = get_quadrant(pt, center);
            if (node.children[quad] != -1) {
                remove(pt, node.children[quad]);
            }
        }
    }
};

int main() {
    QuadTree qt(10); // Capacidade do nó: 2 pontos

    qt.insert(Point(5, 5));
    qt.insert(Point(10, 10));
    qt.insert(Point(15, 15));
    qt.insert(Point(20, 20));

    cout << "Nearest distance to (12,12): " << qt.nearest(Point(12, 12)) << endl;

    qt.update_status(Point(10, 10), false); // Desativa o ponto (15, 15)

    cout << "Nearest distance to (12,12) after deactivation: " << qt.nearest(Point(12, 12)) << endl;

    return 0;
}
