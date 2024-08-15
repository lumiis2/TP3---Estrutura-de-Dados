#ifndef QNODE_H
#define QNODE_H

#include "Box.h"  // Inclua o cabeçalho que define a estrutura Box.
#include "Addr.h" // Inclua o cabeçalho que define a estrutura Addr.

typedef long T;

struct Qnode {
    Addr station; // Ponto armazenado no nó
    Box limites;   // Limites do nó
    T nw, ne, sw, se; // Índices para os filhos (noroeste, nordeste, sudoeste, sudeste)

    Qnode();

    Qnode(Box limites) 
        : limites(limites), nw(-1), ne(-1), sw(-1), se(-1) {};
};

#endif // QNODE_H