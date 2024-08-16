#ifndef POINT_H
#define POINT_H

#include <cmath> // Para função sqrt

/**
 * @brief Uma classe que representa um ponto no espaço 2D.
 */
class Point {
public:
    double x, y; ///< Coordenadas dos pontos

    /**
     * @brief Construtor padrão que inicializa o ponto em (0.0, 0.0).
     */
    Point() : x(0.0), y(0.0) {}

    /**
     * @brief Construtor que cria um novo objeto Point com coordenadas especificadas.
     * 
     * @param x Coordenada x do ponto.
     * @param y Coordenada y do ponto.
     */
    Point(double x, double y) : x(x), y(y) {}

    /**
     * @brief Destrutor da classe Point.
     */
    ~Point() {}

    /**
     * @brief Obtém a coordenada x do ponto.
     * 
     * @return double A coordenada x.
     */
    double getX() { return x; }

    /**
     * @brief Obtém a coordenada y do ponto.
     * 
     * @return double A coordenada y.
     */
    double getY() { return y; }

    /**
     * @brief Calcula a distância Euclidiana para outro ponto.
     * 
     * @param b O outro ponto.
     * @return double A distância Euclidiana entre este ponto e o ponto b.
     */
    double euclideanDistance(const Point& b) const {
        double dx = x - b.x;
        double dy = y - b.y;
        return sqrt(dx * dx + dy * dy);
    }
};

#endif // POINT_H
 