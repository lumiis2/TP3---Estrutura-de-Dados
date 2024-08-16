#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "QuadTree.h"
#include "Hash.h"

using namespace std;

// Função para processar o arquivo de entrada e inserir dados no QuadTree e na HashTable
void process(ifstream& file, QuadTree& quadtree, Hash& hash) {
    string linha;
    // Lê o arquivo linha por linha
    while (getline(file, linha)) {
        istringstream sstream(linha);
        Addr temp; // Estrutura para armazenar os dados de um endereço
        double x, y; // Coordenadas
        string campo; // Variável auxiliar para ler os campos

        // Lê e atribui o id do endereço
        getline(sstream, campo, ';');
        temp.idend = campo;
        // Lê e atribui o id_logrado, tratando exceções de conversão
        getline(sstream, campo, ';');
        try {
            temp.id_logrado = stoll(campo);
        } catch (const exception& e) {
            cerr << "Erro na conversão de id_logrado: " << e.what() << " | valor: " << campo << endl;
            continue; // Pular para a próxima linha em caso de erro
        }

        // Lê e atribui a sigla do tipo de logradouro
        getline(sstream, temp.sigla_tipo, ';');
        // Lê e atribui o nome do logradouro
        getline(sstream, temp.nome_logra, ';');
        // Lê e atribui o número do logradouro, tratando exceções de conversão
        getline(sstream, campo, ';');
        try {
            temp.numero_imo = stoi(campo);
        } catch (const exception& e) {
            cerr << "Erro na conversão de numero_imo: " << e.what() << " | valor: " << campo << endl;
            continue; // Pular para a próxima linha em caso de erro
        }

        // Lê e atribui o nome do bairro
        getline(sstream, temp.nome_bairr, ';');
        // Lê e atribui o nome da região
        getline(sstream, temp.nome_regio, ';');
        // Lê e atribui o CEP, tratando exceções de conversão
        getline(sstream, campo, ';');
        try {
            temp.cep = stoi(campo);
        } catch (const exception& e) {
            cerr << "Erro na conversão de CEP: " << e.what() << " | valor: " << campo << endl;
            continue; // Pular para a próxima linha em caso de erro
        }

        // Lê e atribui a coordenada X, tratando exceções de conversão
        getline(sstream, campo, ';');
        try {
            x = stod(campo);
        } catch (const exception& e) {
            cerr << "Erro na conversão de coordenada X: " << e.what() << " | valor: " << campo << endl;
            continue; // Pular para a próxima linha em caso de erro
        }

        // Lê e atribui a coordenada Y, tratando exceções de conversão
        getline(sstream, campo, ';');
        try {
            y = stod(campo);
        } catch (const exception& e) {
            cerr << "Erro na conversão de coordenada Y: " << e.what() << " | valor: " << campo << endl;
            continue; // Pular para a próxima linha em caso de erro
        }

        // Define as coordenadas do endereço e marca como ativo
        temp.coordenadas = Point(x, y);
        temp.ativo = true;

        // Tenta inserir o endereço no QuadTree e na HashTable, tratando exceções
        try {
            quadtree.insert(temp);
        } catch (const exception& e) {
            cerr << "Erro ao inserir no QuadTree: " << e.what() << endl;
            continue; // Pular para a próxima linha em caso de erro
        }

        try {
            hash.insert(temp);
        } catch (const exception& e) {
            cerr << "Erro ao inserir na HashTable: " << e.what() << endl;
            continue; // Pular para a próxima linha em caso de erro
        }
    }
}

// Função para consultar os pontos mais próximos de uma coordenada
void consultar(double x, double y, int n, char op, QuadTree& quadtree, Hash& hash) {
    cout << fixed << setprecision(6);
    cout << op << ' ' << x << ' ' << y << ' ' << n << endl;
    cout << fixed << setprecision(3);

    // Obtém os n pontos mais próximos da coordenada (x, y) do QuadTree
    Pair<double, Addr>* stations = quadtree.KNN(Point(x, y), n);
    // Imprime cada ponto encontrado com a distância
    for(int i = 0; i < n; i++) {
        cout << stations[i].second << " (" << stations[i].first << ")" << endl;
    }

    delete[] stations; // Libera a memória alocada para os pontos
}

// Função para ativar um ponto de recarga
void ativar(string id, char op, QuadTree& quadtree, Hash& hash) {
    bool flag = quadtree.activate(hash.get(id)); // Ativa o ponto de recarga

    cout << fixed << setprecision(6);
    cout << op << ' ' << id << endl;
    cout << fixed << setprecision(3);
    // Imprime mensagem sobre o status da ativação
    cout << "Ponto de recarga " << id << (flag ? " já estava ativo." : " ativado.") << endl;
}

// Função para desativar um ponto de recarga
void desativar(string id, char op, QuadTree& quadtree, Hash& hash) {
    bool flag = quadtree.inactivate(hash.get(id)); // Desativa o ponto de recarga

    cout << fixed << setprecision(6);
    cout << op << ' ' << id << endl;
    cout << fixed << setprecision(3);
    // Imprime mensagem sobre o status da desativação
    cout << "Ponto de recarga " << id << (flag ? " desativado." : " já estava desativado.") << endl;
}

// Função principal
int main() {
    // Cria instâncias do QuadTree e da HashTable
    QuadTree quadtree(Box(Point(0.0, 0.0), Point(1e9, 1e9)));
    Hash hash;

    // Abre o arquivo base para leitura
    ifstream base("data/geracarga.base");

    // Processa o arquivo base
    process(base, quadtree, hash);

    // Abre o arquivo de eventos para leitura
    ifstream ev("data/geracarga.ev");

    int n;
    ev >> n; // Lê o número de operações

    // Executa cada operação
    for(int i = 0; i <= n; i++) {
        char op;
        ev >> op;

        if(op == 'A') {
            string id;
            ev >> id;
            ativar(id, op, quadtree, hash);

        } else if(op == 'D') {
            string id;
            ev >> id;
            desativar(id, op, quadtree, hash);

        } else if(op == 'C') {
            double x, y;
            int n;
            ev >> x >> y >> n;
            consultar(x, y, n, op, quadtree, hash);
        }
    }

    return 0;
}
