#ifndef HASH_H
#define HASH_H

#include <string>
#include "Addr.h"

/**
 * @brief Classe que implementa uma tabela hash para armazenar e recuperar objetos do tipo `Addr`.
 */
class Hash {
private:
    static const int MAX = 1e5;  ///< Tamanho máximo da tabela hash.
    Addr* table;                 ///< Vetor de ponteiros para os objetos `Addr` armazenados na tabela.
    bool* occupied;              ///< Vetor auxiliar para indicar se uma posição na tabela está ocupada.
    long mod;                    ///< Valor usado na operação de modulação para cálculo do hash.

    /**
     * @brief Calcula o valor hash para uma dada string.
     * 
     * Esta função utiliza o identificador da estação (`station_idend`) para calcular
     * um valor hash, que é usado para determinar a posição na tabela hash.
     *
     * @param station_idend String representando o identificador da estação.
     * @return Valor hash calculado a partir da string.
     */
    long hash(std::string station_idend) const;

public:
    /**
     * @brief Construtor da classe `Hash`.
     */
    Hash();                

    /**
     * @brief Destrutor da classe `Hash`.
     */
    ~Hash();               

    /**
     * @brief Insere um objeto `Addr` na tabela hash.
     * @param s Objeto `Addr` a ser inserido.
     */
    void insert(Addr s);

    /**
     * @brief Recupera um objeto `Addr` da tabela hash.
     * @param station_idend String representando o identificador da estação.
     * @return Objeto `Addr` correspondente ao identificador.
     */
    Addr get(std::string station_idend);
};

#endif  // HASH_H
