#ifndef PAIR_H
#define PAIR_H

/**
 * @brief Classe template que representa um par de valores.
 * 
 * @tparam T1 Tipo do primeiro valor.
 * @tparam T2 Tipo do segundo valor.
 */
template <typename T1, typename T2>
class Pair {
public:
    T1 first;  ///< Primeiro valor do par.
    T2 second; ///< Segundo valor do par.

    /**
     * @brief Construtor padrão que inicializa o par com valores padrão.
     */
    Pair();

    /**
     * @brief Construtor que inicializa o par com valores especificados.
     * 
     * @param first Primeiro valor do par.
     * @param second Segundo valor do par.
     */
    Pair(const T1 first, const T2 second) : first(first), second(second) {}

    /**
     * @brief Operador de comparação menor que.
     * 
     * @param other O outro par a ser comparado.
     * @return true Se este par for menor que o outro par.
     * @return false Caso contrário.
     */
    bool operator<(const Pair<T1, T2>& other) const {
        return this->first < other.first;
    }

    /**
     * @brief Operador de comparação maior que.
     * 
     * @param other O outro par a ser comparado.
     * @return true Se este par for maior que o outro par.
     * @return false Caso contrário.
     */
    bool operator>(const Pair<T1, T2>& other) const {
        return this->first > other.first;
    }

    /**
     * @brief Operador de igualdade.
     * 
     * @param other O outro par a ser comparado.
     * @return true Se este par for igual ao outro par.
     * @return false Caso contrário.
     */
    bool operator==(const Pair<T1, T2>& other) const {
        return this->first == other.first && this->second == other.second;
    }

    /**
     * @brief Operador de desigualdade.
     * 
     * @param other O outro par a ser comparado.
     * @return true Se este par for diferente do outro par.
     * @return false Caso contrário.
     */
    bool operator!=(const Pair<T1, T2>& other) const {
        return !(*this == other);
    }
};

#endif // PAIR_H
