#include "Hash.h"
#include <cstring> // Para std::strlen e std::memcpy
#include <stdexcept> // Para std::runtime_error

// Função de hash básica para strings
long Hash::hash(std::string station_idend) const {
    long hash = 0;
    for (char c : station_idend) {
        hash = (hash * 31 + c) % mod;
    }
    return hash;
}

Hash::Hash() : mod(MAX), table(new Addr[MAX]), occupied(new bool[MAX]()) {
    // Inicializa todas as posições do vetor occupied como falsas
    std::fill_n(occupied, MAX, false);
}

Hash::~Hash() {
    delete[] table;
    delete[] occupied;
}

void Hash::insert(const Addr& s) {
    long index = hash(s.idend); // Supondo que Addr tem um método idend que retorna a ID como string
    long startIndex = index;

    do {
        if (!occupied[index]) {
            table[index] = s;
            occupied[index] = true;
            return;
        } else if (table[index].idend == s.idend) {
            table[index] = s; // Atualiza a estação se a ID já existir
            return;
        }
        index = (index + 1) % mod;
    } while (index != startIndex);

    throw std::runtime_error("Hash is full");
}

Addr Hash::get(const std::string& station_idend) const {
    long index = hash(station_idend);
    long startIndex = index;

    do {
        if (occupied[index] && table[index].idend == station_idend) {
            return table[index];
        }
        index = (index + 1) % mod;
    } while (index != startIndex);

    throw std::runtime_error("Addr not found");
}
