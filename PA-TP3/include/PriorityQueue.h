#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

/**
 * @brief Functor para comparar elementos em ordem crescente.
 * 
 * @tparam T O tipo dos elementos a serem comparados.
 */
template <typename T>
struct Min {
    /**
     * @brief Operador de comparação para comparação de menor que.
     * 
     * @param lhs Operando do lado esquerdo.
     * @param rhs Operando do lado direito.
     * @return true se lhs for menor que rhs.
     * @return false caso contrário.
     */
    bool operator()(const T& lhs, const T& rhs) const {
        return lhs < rhs;
    }
};

/**
 * @brief Troca os valores de duas variáveis.
 * 
 * @tparam T O tipo das variáveis.
 * @param a A primeira variável.
 * @param b A segunda variável.
 */
template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

/**
 * @brief Fila de prioridade genérica com funcionalidade de comparação personalizável.
 * 
 * @tparam T O tipo dos elementos na fila de prioridade.
 * @tparam Compare O functor de comparação a ser usado (padrão é Min<T>).
 */
template <typename T, typename Compare = Min<T>>
class PriorityQueue {
private:
    T *heap; ///< Array que representa o heap.
    int size; ///< Tamanho atual do heap.
    const int MAX_SIZE = 1010; ///< Tamanho máximo do heap.
    Compare comp; ///< Objeto de comparação.

    /**
     * @brief Realoca memória dinâmica para armazenar o heap.
     */
    void realloc();


    /**
     * @brief Mantém a propriedade do heap movendo um elemento para cima.
     * 
     * @param idx Índice do elemento a ser movido para cima.
     */
    void heapify_up(int idx);

    /**
     * @brief Mantém a propriedade do heap movendo um elemento para baixo.
     * 
     * @param idx Índice do elemento a ser movido para baixo.
     */
    void heapify_down(int idx);

    /**
     * @brief Obtém o índice do pai do nó dado.
     * 
     * @param idx Índice do nó atual.
     * @return int Índice do nó pai.
     */
    int getParent(int idx) const;

    /**
     * @brief Obtém o índice do filho esquerdo do nó dado.
     * 
     * @param idx Índice do nó atual.
     * @return int Índice do nó filho esquerdo.
     */
    int getLeftChild(int idx) const;

    /**
     * @brief Obtém o índice do filho direito do nó dado.
     * 
     * @param idx Índice do nó atual.
     * @return int Índice do nó filho direito.
     */
    int getRightChild(int idx) const;

public:
    /**
     * @brief Constrói um novo objeto PriorityQueue.
     */
    PriorityQueue();

    /**
     * @brief Destrói o objeto PriorityQueue.
     */
    ~PriorityQueue();

    /**
     * @brief Verifica se a fila está vazia.
     * 
     * @return true se a fila estiver vazia.
     * @return false caso contrário.
     */
    bool empty() const;

    /**
     * @brief Insere um valor na fila de prioridade.
     * 
     * @param value O valor a ser inserido.
     */
    void insert(const T& value);

    /**
     * @brief Remove o valor do topo da fila de prioridade.
     */
    void remove();

    /**
     * @brief Obtém o valor do topo da fila de prioridade.
     * 
     * @return T O valor do topo da fila de prioridade.
     */
    T top() const;
};

// Construtor: inicializa o heap e define o tamanho como zero
template <typename T, typename Compare>
PriorityQueue<T, Compare>::PriorityQueue() {
    this->heap = new T[MAX_SIZE];
    this->size = 0;
}

// Destrutor: libera a memória alocada para o heap
template <typename T, typename Compare>
PriorityQueue<T, Compare>::~PriorityQueue() {
    delete[] heap;
}

// Verifica se a fila está vazia
template <typename T, typename Compare>
bool PriorityQueue<T, Compare>::empty() const {
    return size == 0;
}

// Insere um valor na fila e mantém a propriedade do heap
template <typename T, typename Compare>
void PriorityQueue<T, Compare>::insert(const T& value) {
    if (size < MAX_SIZE) {
        heap[size] = value;
        heapify_up(size);
        ++size;
    }
}

// Remove o elemento no topo da fila e mantém a propriedade do heap
template <typename T, typename Compare>
void PriorityQueue<T, Compare>::remove() {
    if (!empty()) {
        heap[0] = heap[--size];
        heapify_down(0);
    }
}

// Retorna o elemento no topo da fila
template <typename T, typename Compare>
T PriorityQueue<T, Compare>::top() const {
    if (!empty()) {
        return heap[0];
    }
    return T(); // Retorna valor padrão de T se a fila estiver vazia
}

// Ajusta a posição do elemento inserido para manter a propriedade do heap
template <typename T, typename Compare>
void PriorityQueue<T, Compare>::heapify_up(int idx) {
    while (idx > 0 && comp(heap[idx], heap[getParent(idx)])) {
        swap(heap[idx], heap[getParent(idx)]);
        idx = getParent(idx);
    }
}

// Ajusta a posição do elemento removido para manter a propriedade do heap
template <typename T, typename Compare>
void PriorityQueue<T, Compare>::heapify_down(int idx) {
    int largest = idx;
    int left = getLeftChild(idx);
    int right = getRightChild(idx);

    if (left < size && comp(heap[left], heap[largest])) {
        largest = left;
    }
    if (right < size && comp(heap[right], heap[largest])) {
        largest = right;
    }
    if (largest != idx) {
        swap(heap[idx], heap[largest]);
        heapify_down(largest);
    }
}

// Retorna o índice do pai do nó atual
template <typename T, typename Compare>
int PriorityQueue<T, Compare>::getParent(int idx) const {
    return (idx - 1) / 2;
}

// Retorna o índice do filho esquerdo do nó atual
template <typename T, typename Compare>
int PriorityQueue<T, Compare>::getLeftChild(int idx) const {
    return 2 * idx + 1;
}

// Retorna o índice do filho direito do nó atual
template <typename T, typename Compare>
int PriorityQueue<T, Compare>::getRightChild(int idx) const {
    return 2 * idx + 2;
}

template <typename T, typename Compare>
void realloc() {
    size_t newCapacity = capacity * 2;
    T* aux = this->heap;
    this->heap = new T[newCapacity];
    
    // Copia os elementos do heap antigo para o novo
    for (size_t i = 0; i < size; ++i) {
        this->heap[i] = aux[i];
    }
    // Libera a memória do heap antigo
    delete[] aux;
}

#endif // PRIORITY_QUEUE_H
