#include "QuadTree.h"
#include <cmath>
#include <stdexcept> // Para std::runtime_error
#include <limits>    // Para std::numeric_limits

// Construtor padrão
QuadTree::QuadTree() : tree(new Qnode[MAX]), next(1) {
    Box defaultBox; // Cria um Box com valores padrão
    tree[root] = Qnode(defaultBox); // Inicializa a raiz com o Box padrão
}

// Construtor com limites específicos
QuadTree::QuadTree(Box box) : tree(new Qnode[MAX]), next(1) {
    tree[root] = Qnode(box); // Inicializa a raiz com o Box fornecido
}

// Destrutor
QuadTree::~QuadTree() {
    delete[] tree; // Libera a memória alocada para a quadtree
}

// Encontra o quadrante que contém o ponto
T QuadTree::find(Point p) {
    T node = root;
    while (node != -1 && tree[node].nw != -1) { // Continua enquanto há filhos
        if (tree[tree[node].ne].limites.contains(p))
            node = tree[node].ne; // Vai para o noroeste
        else if (tree[tree[node].nw].limites.contains(p))
            node = tree[node].nw; // Vai para o nordeste
        else if (tree[tree[node].sw].limites.contains(p))
            node = tree[node].sw; // Vai para o sudoeste
        else if (tree[tree[node].se].limites.contains(p))
            node = tree[node].se; // Vai para o sudeste
        else
            throw std::runtime_error("Ponto não encontrado na quadtree.");
    }
    return node; // Retorna o índice do quadrante que contém o ponto
}

// Encontra o quadrante que contém o endereço
T QuadTree::find(Addr st) {
    T node = root;
    while (node != -1 && tree[node].ne != -1 && tree[node].station.idend != st.idend) { // Continua enquanto há filhos e o endereço não é encontrado
        if (tree[tree[node].ne].limites.contains(st.coordenadas))
            node = tree[node].ne; // Vai para o noroeste
        else if (tree[tree[node].nw].limites.contains(st.coordenadas))
            node = tree[node].nw; // Vai para o nordeste
        else if (tree[tree[node].sw].limites.contains(st.coordenadas))
            node = tree[node].sw; // Vai para o sudoeste
        else if (tree[tree[node].se].limites.contains(st.coordenadas))
            node = tree[node].se; // Vai para o sudeste
        else
            throw std::runtime_error("Endereço não encontrado na quadtree.");
    }
    if (node == -1 || tree[node].station.idend != st.idend) {
        throw std::runtime_error("Endereço não encontrado na quadtree.");
    }
    return node; // Retorna o índice do quadrante que contém o endereço
}

// Insere um endereço na quadtree
void QuadTree::insert(Addr st) {
    if (next >= MAX) {
        throw std::runtime_error("Quadtree está cheia. Não é possível inserir mais elementos.");
    }

    T node = find(st.coordenadas); // Encontra o quadrante que deve conter o endereço
    tree[node].station = st; // Insere o endereço no nó encontrado

    // Subdivide o quadrante e cria novos filhos
    tree[node].ne = next;
    if (next < MAX) {
        tree[next++] = Qnode(Box(Point(st.coordenadas.x, tree[node].limites.topLeft.y), 
                                 Point(tree[node].limites.bottomRight.x, st.coordenadas.y)));
    } else {
        throw std::runtime_error("Quadtree está cheia. Não é possível criar novos nós.");
    }

    tree[node].nw = next;
    if (next < MAX) {
        tree[next++] = Qnode(Box(tree[node].limites.topLeft, st.coordenadas));
    } else {
        throw std::runtime_error("Quadtree está cheia. Não é possível criar novos nós.");
    }

    tree[node].sw = next;
    if (next < MAX) {
        tree[next++] = Qnode(Box(Point(tree[node].limites.topLeft.x, st.coordenadas.y), 
                                 Point(st.coordenadas.x, tree[node].limites.bottomRight.y)));
    } else {
        throw std::runtime_error("Quadtree está cheia. Não é possível criar novos nós.");
    }

    tree[node].se = next;
    if (next < MAX) {
        tree[next++] = Qnode(Box(st.coordenadas, tree[node].limites.bottomRight));
    } else {
        throw std::runtime_error("Quadtree está cheia. Não é possível criar novos nós.");
    }
}

// Função recursiva para encontrar k vizinhos mais próximos
void QuadTree::KNNRecursive(PriorityQueue<Pair<double, Addr>>& pq, T node, Point p, int k, double& minDistance) {
    if (node == -1) return; // Verifica se o quadrante é inválido

    double distance = p.euclideanDistance(tree[node].station.coordenadas); // Calcula a distância euclidiana

    if (tree[node].station.ativo) { // Verifica se o endereço está ativo
        if (pq.size < k) {
            pq.insert(Pair<double, Addr>(distance, tree[node].station)); // Adiciona no pq se não houver k elementos ainda
            // if(distance < minDistance) minDistance = distance;
        } else if (distance < minDistance) {
            pq.remove(); // Remove o elemento com maior distância
            pq.insert(Pair<double, Addr>(distance, tree[node].station)); // Adiciona o novo elemento
            minDistance = pq.top().first; // Atualiza a menor distância
        }
    }
    
    // Verifica a distância do retângulo ao ponto antes de buscar os quadrantes filhos
    double distanceToRect = tree[node].limites.distanceToPoint(p);
    if (distanceToRect < minDistance) {
        KNNRecursive(pq, tree[node].ne, p, k, minDistance);
        KNNRecursive(pq, tree[node].nw, p, k, minDistance);
        KNNRecursive(pq, tree[node].sw, p, k, minDistance);
        KNNRecursive(pq, tree[node].se, p, k, minDistance);
    }
}

// Retorna os k vizinhos mais próximos
Pair<double, Addr>* QuadTree::KNN(Point p, int k) {
    if (k <= 0) {
        throw std::invalid_argument("O valor de k deve ser maior que 0.");
    }
    PriorityQueue<Pair<double, Addr>> pq; // Cria uma fila de prioridade para armazenar os vizinhos mais próximos
    double minDistance = std::numeric_limits<double>::max(); // Inicializa a menor distância com o valor máximo possível
    KNNRecursive(pq, root, p, k, minDistance); // Preenche a fila de prioridade com os k vizinhos mais próximos

    Pair<double, Addr>* val = new Pair<double, Addr>[k]; // Aloca memória para os k vizinhos
    int i = k - 1;
    while (!pq.empty()) { // Remove os elementos da fila de prioridade e os armazena no array
        val[i--] = pq.top();
        pq.remove();
    }
    return val; // Retorna o array com os k vizinhos mais próximos
}

// Ativa um endereço e retorna o estado anterior
bool QuadTree::activate(Addr st) {
    T node = find(st); // Encontra o nó que contém o endereço
    bool was = tree[node].station.ativo; // Salva o estado anterior
    tree[node].station.ativo = true; // Ativa o endereço
    return was; // Retorna o estado anterior
}

// Desativa um endereço e retorna o estado anterior
bool QuadTree::inactivate(Addr st) {
    T node = find(st); // Encontra o nó que contém o endereço
    bool was = tree[node].station.ativo; // Salva o estado anterior
    tree[node].station.ativo = false; // Desativa o endereço
    return was; // Retorna o estado anterior
}
