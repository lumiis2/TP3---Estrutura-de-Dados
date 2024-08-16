#include "Hash.h"
#include <cstring> // Para std::strlen e std::memcpy
#include <stdexcept> // Para std::runtime_error

// Função de hash básica para strings
long Hash::hash(std::string station_idend) const {
    long ret = 0;
    for(char & c : station_idend) {
        ret += (long) c;
    }
    return (ret % this->mod);
}

Hash::Hash() : table(new Addr[MAX]), occupied(new bool[MAX]()), mod(MAX)  {
    // Inicializa todas as posições do vetor occupied como falsas
    std::fill_n(occupied, MAX, true);
}

Hash::~Hash() {
    delete[] table;
    delete[] occupied;
}

void Hash::insert(Addr s) {
    long hash = this->hash(s.idend);

    long i = hash;
    while(!this->occupied[i]) {
        i = (i + 1) % this->mod;
        if(i == hash) {
            throw std::overflow_error("Capacidade máxima da tabela hash atingida.");
        }
    }

    this->table[i] = s;
    this->occupied[i] = false;
}

Addr Hash::get(std::string station_idend) {
    long hash = this->hash(station_idend);

    long i = hash;
    while((!this->occupied[i]) && (this->table[i].idend != station_idend)) {
        i = (i + 1) % this->mod;
        if(i == hash) {
            throw std::out_of_range("Objeto almejado não encontrado.");
        }
    }

    return this->table[i];
    
}
