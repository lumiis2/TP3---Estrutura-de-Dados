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

        /**
     * @brief Calcula a menor distância entre um ponto e a caixa retangular.
     *
     * Esta função calcula a distância mínima entre o ponto `p` e qualquer uma das bordas
     * da caixa definida pelos pontos `topLeft` e `bottomRight`.
     *
     * @param p Ponto do qual a distância será calculada.
     * @return A menor distância entre o ponto `p` e a caixa.
     */
    double distanceToPoint(Point p) const {
        // Calcula a coordenada x mais próxima dentro dos limites da caixa
        double x_closest = std::max(topLeft.x, std::min(p.x, bottomRight.x));
        // Calcula a coordenada y mais próxima dentro dos limites da caixa
        double y_closest = std::max(topLeft.y, std::min(p.y, bottomRight.y));
        
        // Calcula a distância euclidiana entre o ponto p e o ponto mais próximo dentro da caixa
        return std::sqrt(std::pow(p.x - x_closest, 2) + std::pow(p.y - y_closest, 2));
    }

    // double dx = std::max(rect.left - p.x, std::max(0.0, p.x - rect.right));
    // double dy = std::max(rect.bottom - p.y, std::max(0.0, p.y - rect.top));
    // return std::sqrt(dx * dx + dy * dy);

    // Box getQuadrant(int index) const;
};

#endif  // BOX_H
