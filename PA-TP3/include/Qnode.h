#ifndef QNODE_H
#define QNODE_H

#include "Box.h"  
#include "Addr.h"

typedef long T; // Alias para `long`, usado como índice dos filhos

/**
 * @brief Representa um nó em uma árvore quadtree.
 */
struct Qnode {
    Addr station;  ///< Endereço armazenado no nó
    Box limites;   ///< Limites geográficos do nó
    
    T nw, ne, sw, se; ///< Índices para os filhos: noroeste, nordeste, sudoeste e sudeste

    Qnode() = default; ///< Construtor padrão

    /**
     * @brief Construtor com limites.
     * 
     * Inicializa os limites do nó e define índices dos filhos como -1.
     *
     * @param limites Limites do nó
     */
    Qnode(Box limites) 
        : limites(limites), nw(-1), ne(-1), sw(-1), se(-1) {}
};

#endif // QNODE_H
