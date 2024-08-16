#ifndef HASH_H
#define HASH_H

#include <string>
#include "Addr.h"

class Hash {
private:
    static const int MAX = 1e5; // Tamanho máximo da tabela hash
    Addr* table;             // Tabela hash propriamente dita
    bool* occupied;             // Vetor auxiliar para indicar se uma posição está ocupada
    long mod;                  // Valor da divisão para cálculo do hash


    long hash(std::string station_idend) const;

public:
    Hash();                // Construtor
    ~Hash();               // Destruidor

    void insert(Addr s);
    Addr get(std::string station_idend);
};

#endif  // HASH_H
