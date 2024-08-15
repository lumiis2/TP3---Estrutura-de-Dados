#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

const int N = 1 << 17;
const long long INF = 2000000000000000007;

struct Point {
    int x, y;
    bool operator !=(const Point &a) const {
        return !(x == a.x && y == a.y);
    }
};

struct QuadNode {
    Point p; // Ponto armazenado no nó
    int minX, minY, maxX, maxY; // Limites do nó
    int nw, ne, sw, se; // Índices para os filhos

    QuadNode(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0)
        : minX(x1), minY(y1), maxX(x2), maxY(y2), nw(-1), ne(-1), sw(-1), se(-1) {}
};

QuadNode nodes[N]; // Array para armazenar os nós do Quadtree
int nodeCount = 0; // Contador para gerenciar a quantidade de nós usados

int tests, n;
Point arr[N], pts[N];
int root;
long long ans;

long long squaredDistance(Point a, Point b) {
    // Calcula a distância quadrada entre dois pontos
    return sqrt(1LL * (a.x - b.x) * (a.x - b.x) + 1LL * (a.y - b.y) * (a.y - b.y));
}

bool inBoundary(QuadNode &node, Point p) {
    // Verifica se um ponto está dentro dos limites de um nó quadtree
    return (p.x >= node.minX && p.x <= node.maxX && p.y >= node.minY && p.y <= node.maxY);
}

void insert(int &nodeIndex, Point p, int minX, int minY, int maxX, int maxY) {
    if (nodeIndex == -1) {
        nodeIndex = nodeCount++;
        nodes[nodeIndex] = QuadNode(minX, minY, maxX, maxY);
        nodes[nodeIndex].p = p;
        return;
    }

    if (p != nodes[nodeIndex].p) {
        int midX = (minX + maxX) / 2;
        int midY = (minY + maxY) / 2;

        if (p.x <= midX && p.y <= midY) {
            insert(nodes[nodeIndex].nw, p, minX, minY, midX, midY);
        } else if (p.x > midX && p.y <= midY) {
            insert(nodes[nodeIndex].ne, p, midX + 1, minY, maxX, midY);
        } else if (p.x <= midX && p.y > midY) {
            insert(nodes[nodeIndex].sw, p, minX, midY + 1, midX, maxY);
        } else {
            insert(nodes[nodeIndex].se, p, midX + 1, midY + 1, maxX, maxY);
        }
    }
}

void nearestNeighbor(int nodeIndex, Point q, long long &ans) {
    if (nodeIndex == -1) return;

    long long dist = squaredDistance(nodes[nodeIndex].p, q);
    if (dist != 0) {  // Evita contar a distância do ponto com ele mesmo
        ans = min(ans, dist);
    }

    int midX = (nodes[nodeIndex].minX + nodes[nodeIndex].maxX) / 2;
    int midY = (nodes[nodeIndex].minY + nodes[nodeIndex].maxY) / 2;
    double sqrt_ans = sqrt(ans);

    if (q.x <= midX && q.y <= midY) {
        nearestNeighbor(nodes[nodeIndex].nw, q, ans);
        if (q.x + sqrt_ans > midX) {
            nearestNeighbor(nodes[nodeIndex].ne, q, ans);
        }
        if (q.y + sqrt_ans > midY) {
            nearestNeighbor(nodes[nodeIndex].sw, q, ans);
        }
        if (q.x + sqrt_ans > midX && q.y + sqrt_ans > midY) {
            nearestNeighbor(nodes[nodeIndex].se, q, ans);
        }
    } else if (q.x > midX && q.y <= midY) {
        nearestNeighbor(nodes[nodeIndex].ne, q, ans);
        if (q.x - sqrt_ans <= midX) {
            nearestNeighbor(nodes[nodeIndex].nw, q, ans);
        }
        if (q.y + sqrt_ans > midY) {
            nearestNeighbor(nodes[nodeIndex].se, q, ans);
        }
        if (q.x - sqrt_ans <= midX && q.y + sqrt_ans > midY) {
            nearestNeighbor(nodes[nodeIndex].sw, q, ans);
        }
    } else if (q.x <= midX && q.y > midY) {
        nearestNeighbor(nodes[nodeIndex].sw, q, ans);
        if (q.x + sqrt_ans > midX) {
            nearestNeighbor(nodes[nodeIndex].se, q, ans);
        }
        if (q.y - sqrt_ans <= midY) {
            nearestNeighbor(nodes[nodeIndex].nw, q, ans);
        }
        if (q.x + sqrt_ans > midX && q.y - sqrt_ans <= midY) {
            nearestNeighbor(nodes[nodeIndex].ne, q, ans);
        }
    } else {
        nearestNeighbor(nodes[nodeIndex].se, q, ans);
        if (q.x - sqrt_ans <= midX) {
            nearestNeighbor(nodes[nodeIndex].sw, q, ans);
        }
        if (q.y - sqrt_ans <= midY) {
            nearestNeighbor(nodes[nodeIndex].ne, q, ans);
        }
        if (q.x - sqrt_ans <= midX && q.y - sqrt_ans <= midY) {
            nearestNeighbor(nodes[nodeIndex].nw, q, ans);
        }
    }
}

int main() {
    cin >> tests;
    while (tests--) {
        cin >> n;
        for (int i = 1; i <= n; i++) {
            cin >> arr[i].x >> arr[i].y;
            pts[i] = arr[i];
        }

        int minX = 0, minY = 0, maxX = 15, maxY = 15; // Ajuste de acordo com a faixa de valores das coordenadas
        root = -1;
        nodeCount = 0;
        for (int i = 1; i <= n; i++) {
            insert(root, arr[i], minX, minY, maxX, maxY);
        }

        for (int i = 1; i <= n; i++) {
            ans = INF;
            nearestNeighbor(root, pts[i], ans);
            cout << ans << "\n";
        }
    }

    return 0;
}
