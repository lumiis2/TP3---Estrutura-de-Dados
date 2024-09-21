#include "Hash.h"
#include <cstring>      
#include <stdexcept>   
#include <algorithm>   


/**
 * @brief Função de hash para strings.
 * 
 * Esta função calcula um valor hash simples somando os valores ASCII
 * de todos os caracteres na string fornecida.
 *
 * @param station_idend Identificador da estação.
 * @return Valor hash calculado.
 */
long Hash::hash(std::string station_idend) const {
    long val = 0;
    for (char& c : station_idend) {
        val += static_cast<long>(c);  // Somando o valor ASCII de cada caractere
    }
    return val % mod;  // Retorna o hash dentro do intervalo permitido
}

/**
 * @brief Construtor da classe `Hash`.
 */
Hash::Hash() : table(new Addr[MAX]), occupied(new bool[MAX]()), mod(MAX)  {
    // Inicializa todas as posições do vetor occupied como falsas
    for (int i = 0; i < MAX; ++i) {
        occupied[i] = false;
    }
}

/**
 * @brief Destrutor da classe `Hash`.
 */
Hash::~Hash() {
    delete[] table;
    delete[] occupied;
}

/**
 * @brief Insere um objeto `Addr` na tabela hash.
 * @param s Objeto `Addr` a ser inserido.
 * @throws std::overflow_error Se a tabela hash estiver cheia.
 */
void Hash::insert(Addr s) {
    long h = hash(s.idend);

    long i = h;
    while (occupied[i]) {
        i = (i + 1) % mod;  // Resolução linear de colisões
        if (i == h) {
            throw std::overflow_error("Erro: Tabela hash cheia, não é possível inserir o endereço.");
        }
    }

    table[i] = s;
    occupied[i] = true;
}

/**
 * @brief Recupera um objeto `Addr` da tabela hash.
 * @param station_idend Identificador da estação a ser procurada (string).
 * @return Objeto `Addr` correspondente ao identificador.
 * @throws std::out_of_range Se o identificador não for encontrado na tabela hash.
 */
Addr Hash::get(std::string station_idend) {
    long h = hash(station_idend);

    long i = h;
    while (occupied[i] && table[i].idend != station_idend) {
        i = (i + 1) % mod;  // Resolução linear de colisões
        if (i == h) {
            throw std::out_of_range("Erro: Identificador não encontrado na tabela hash.");
        }
    }

    return table[i];
}
