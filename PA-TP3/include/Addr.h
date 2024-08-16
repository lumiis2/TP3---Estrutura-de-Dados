#ifndef ADDR_H
#define ADDR_H

#include <iostream>
#include <string>
#include "Point.h"

/**
 * @brief Estrutura que representa um endereço com informações detalhadas.
 */
struct Addr { 
    std::string idend;        ///< Identificador do endereço
    long id_logrado;          ///< ID do logradouro
    std::string sigla_tipo;   ///< Sigla do tipo de logradouro (ex: "R" para Rua)
    std::string nome_logra;   ///< Nome do logradouro
    int numero_imo;           ///< Número do imóvel
    std::string nome_bairr;   ///< Nome do bairro
    std::string nome_regio;   ///< Nome da região
    int cep;                  ///< Código de Endereçamento Postal (CEP)
    Point coordenadas;        ///< Coordenadas (latitude e longitude)
    bool ativo;               ///< Status de atividade (ativo ou não)

    Addr() : idend(""), id_logrado(0), sigla_tipo(""), nome_logra(""), numero_imo(0),
             nome_bairr(""), nome_regio(""), cep(0), coordenadas(Point()), ativo(true) {}

    /**
     * @brief Sobrecarga do operador de saída para imprimir detalhes do endereço.
     * @param os Fluxo de saída
     * @param s Objeto do tipo Addr
     * @return Fluxo de saída
     */
    friend std::ostream& operator<<(std::ostream& os, const Addr& s) {
        os  << s.sigla_tipo << " "
            << s.nome_logra << ", "
            << s.numero_imo << ", "
            << s.nome_bairr << ", "
            << s.nome_regio << ", "
            << s.cep;
        return os;
    }
};

#endif  // ADDR_H

