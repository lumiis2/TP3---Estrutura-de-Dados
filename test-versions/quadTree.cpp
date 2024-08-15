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
    QuadNode *nw, *ne, *sw, *se; // Ponteiros para os filhos

    QuadNode(int x1, int y1, int x2, int y2)
        : minX(x1), minY(y1), maxX(x2), maxY(y2), nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr) {}
};

using NodePtr = QuadNode*;

int tests, n;
Point arr[N], pts[N];
NodePtr root;
long long ans;

long long squaredDistance(Point a, Point b) {
    // Calcula a distância quadrada entre dois pontos
    return sqrt(1LL * (a.x - b.x) * (a.x - b.x) + 1LL * (a.y - b.y) * (a.y - b.y));
}

bool inBoundary(QuadNode *node, Point p) {
    // Verifica se um ponto está dentro dos limites de um nó quadtree
    return (p.x >= node->minX && p.x <= node->maxX && p.y >= node->minY && p.y <= node->maxY);
}

void insert(NodePtr &node, Point p, int minX, int minY, int maxX, int maxY) {
    if (node == nullptr) {
        node = new QuadNode(minX, minY, maxX, maxY);
        node->p = p;
        cout << "Inserido ponto (" << p.x << ", " << p.y << ") no nó com limites ["
             << minX << ", " << minY << "] a [" << maxX << ", " << maxY << "]\n";
        return;
    }

    if (p != node->p) {
        int midX = (minX + maxX) / 2;
        int midY = (minY + maxY) / 2;

        if (p.x <= midX && p.y <= midY) {
            cout << "Inserindo no quadrante NW: ponto (" << p.x << ", " << p.y
                 << ") com limites [" << minX << ", " << minY << "] a [" << midX << ", " << midY << "]\n";
            insert(node->nw, p, minX, minY, midX, midY);
        } else if (p.x > midX && p.y <= midY) {
            cout << "Inserindo no quadrante NE: ponto (" << p.x << ", " << p.y
                 << ") com limites [" << midX + 1 << ", " << minY << "] a [" << maxX << ", " << midY << "]\n";
            insert(node->ne, p, midX + 1, minY, maxX, midY);
        } else if (p.x <= midX && p.y > midY) {
            cout << "Inserindo no quadrante SW: ponto (" << p.x << ", " << p.y
                 << ") com limites [" << minX << ", " << midY + 1 << "] a [" << midX << ", " << maxY << "]\n";
            insert(node->sw, p, minX, midY + 1, midX, maxY);
        } else {
            cout << "Inserindo no quadrante SE: ponto (" << p.x << ", " << p.y
                 << ") com limites [" << midX + 1 << ", " << midY + 1 << "] a [" << maxX << ", " << maxY << "]\n";
            insert(node->se, p, midX + 1, midY + 1, maxX, maxY);
        }
    }
}

void nearestNeighbor(NodePtr node, Point q, long long &ans) {
    if (node == nullptr) return;

    long long dist = squaredDistance(node->p, q);
    if (dist != 0) {  // Evita contar a distância do ponto com ele mesmo
        cout << "Distância ao ponto (" << q.x << ", " << q.y << ") do nó (" << node->p.x << ", " << node->p.y << ") é " << dist << "\n";
        ans = min(ans, dist);
    }

    int midX = (node->minX + node->maxX) / 2;
    int midY = (node->minY + node->maxY) / 2;
    double sqrt_ans = sqrt(ans);

    cout << "Centro do nó atual: (" << midX << ", " << midY << ")\n";

    if (q.x <= midX && q.y <= midY) {
        cout << "Percorrendo quadrante NW\n";
        nearestNeighbor(node->nw, q, ans);
        if (q.x + sqrt_ans > midX) {
            cout << "Verificando quadrante NE\n";
            nearestNeighbor(node->ne, q, ans);
        }
        if (q.y + sqrt_ans > midY) {
            cout << "Verificando quadrante SW\n";
            nearestNeighbor(node->sw, q, ans);
        }
        if (q.x + sqrt_ans > midX && q.y + sqrt_ans > midY) {
            cout << "Verificando quadrante SE\n";
            nearestNeighbor(node->se, q, ans);
        }
    } else if (q.x > midX && q.y <= midY) {
        cout << "Percorrendo quadrante NE\n";
        nearestNeighbor(node->ne, q, ans);
        if (q.x - sqrt_ans <= midX) {
            cout << "Verificando quadrante NW\n";
            nearestNeighbor(node->nw, q, ans);
        }
        if (q.y + sqrt_ans > midY) {
            cout << "Verificando quadrante SE\n";
            nearestNeighbor(node->se, q, ans);
        }
        if (q.x - sqrt_ans <= midX && q.y + sqrt_ans > midY) {
            cout << "Verificando quadrante SW\n";
            nearestNeighbor(node->sw, q, ans);
        }
    } else if (q.x <= midX && q.y > midY) {
        cout << "Percorrendo quadrante SW\n";
        nearestNeighbor(node->sw, q, ans);
        if (q.x + sqrt_ans > midX) {
            cout << "Verificando quadrante SE\n";
            nearestNeighbor(node->se, q, ans);
        }
        if (q.y - sqrt_ans <= midY) {
            cout << "Verificando quadrante NW\n";
            nearestNeighbor(node->nw, q, ans);
        }
        if (q.x + sqrt_ans > midX && q.y - sqrt_ans <= midY) {
            cout << "Verificando quadrante NE\n";
            nearestNeighbor(node->ne, q, ans);
        }
    } else {
        cout << "Percorrendo quadrante SE\n";
        nearestNeighbor(node->se, q, ans);
        if (q.x - sqrt_ans <= midX) {
            cout << "Verificando quadrante SW\n";
            nearestNeighbor(node->sw, q, ans);
        }
        if (q.y - sqrt_ans <= midY) {
            cout << "Verificando quadrante NE\n";
            nearestNeighbor(node->ne, q, ans);
        }
        if (q.x - sqrt_ans <= midX && q.y - sqrt_ans <= midY) {
            cout << "Verificando quadrante NW\n";
            nearestNeighbor(node->nw, q, ans);
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
        root = nullptr;
        for (int i = 1; i <= n; i++) {
            insert(root, arr[i], minX, minY, maxX, maxY);
        }

        for (int i = 1; i <= n; i++) {
            ans = INF;
            cout << "Buscando vizinho mais próximo para o ponto (" << pts[i].x << ", " << pts[i].y << ")\n";
            nearestNeighbor(root, pts[i], ans);
            cout << "Menor distância ao ponto (" << pts[i].x << ", " << pts[i].y << ") é " << ans << "\n";
            cout << ans << "\n";
        }
    }

    return 0;
}
