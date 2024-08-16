#ifndef BOX_H
#define BOX_H

#include "Point.h"

/**
 * @brief Classe que representa uma caixa retangular definida por dois pontos: 
 * o canto superior esquerdo (topLeft) e o canto inferior direito (bottomRight).
 */
class Box {
public:
    Point topLeft;        ///< Ponto representando o canto superior esquerdo da caixa.
    Point bottomRight;    ///< Ponto representando o canto inferior direito da caixa.
    
    /**
     * @brief Construtor padrão que inicializa a caixa como vazia.
     */
    Box() = default;

    /**
     * @brief Construtor que inicializa a caixa com os pontos superior esquerdo e inferior direito.
     * 
     * @param tl Ponto representando o canto superior esquerdo da caixa.
     * @param br Ponto representando o canto inferior direito da caixa.
     */
    Box(Point tl, Point br) : topLeft(tl), bottomRight(br) {}

    /**
     * @brief Verifica se um ponto está contido dentro da caixa.
     *
     * Esta função verifica se as coordenadas do ponto `p` estão dentro
     * dos limites definidos pelos pontos `topLeft` e `bottomRight`.
     *
     * @param p Ponto a ser verificado.
     * @return true se o ponto está contido dentro da caixa, false caso contrário.
     */
    bool contains(Point p) const {
        return (p.x >= topLeft.x && p.x <= bottomRight.x &&
                p.y >= topLeft.y && p.y <= bottomRight.y);
    }

    // Box getQuadrant(int index) const;
};

#endif  // BOX_H
