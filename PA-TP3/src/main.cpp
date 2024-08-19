#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstring>
#include <chrono> // Para medição de tempo
#include "QuadTree.h"
#include "Hash.h"

using namespace std;
using namespace std::chrono;


// Função para processar o arquivo de entrada e inserir dados no QuadTree e na HashTable
void process(istream& input, QuadTree& quadtree, Hash& hash, int tam) {
    auto start = high_resolution_clock::now(); // Início da medição de tempo
    string linha;
    
    // Lê o arquivo linha por linha
    for (int i = 0; i < tam; i++) {
        getline(input, linha);
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
    auto end = high_resolution_clock::now(); // Fim da medição de tempo
    auto duration = duration_cast<microseconds>(end - start); // Usar microsegundos para maior precisão
    cout << fixed << setprecision(6); // Definir precisão para 6 casas decimais
    cout << "\nTempo de processamento: " << duration.count() / 1e6 << " s" << endl; // Converter microsegundos para segundos
}

// Função para consultar os pontos mais próximos de uma coordenada
void consultar(double x, double y, int n, char op, QuadTree& quadtree, Hash& hash) {
    auto start = high_resolution_clock::now(); // Início da medição de tempo

    // cout << fixed << setprecision(6);
    // cout << op << ' ' << x << ' ' << y << ' ' << n << endl;
    // cout << fixed << setprecision(3);

    // Obtém os n pontos mais próximos da coordenada (x, y) do QuadTree
    Pair<double, Addr>* stations = quadtree.KNN(Point(x, y), n);
    // Imprime cada ponto encontrado com a distância
    // for(int i = 0; i < n; i++) {
    //     cout << stations[i].second << " (" << stations[i].first << ")" << endl;
    // }

    delete[] stations; 
    // Libera a memória alocada para os pontos
    auto end = high_resolution_clock::now(); // Fim da medição de tempo
    auto duration = duration_cast<microseconds>(end - start); // Usar microsegundos para maior precisão
    cout << fixed << setprecision(6); // Definir precisão para 6 casas decimais
    cout << "\nTempo de consulta: " << duration.count() / 1e6 << " s" << endl; // Converter microsegundos para segundos

}

// Função para ativar um ponto de recarga
void ativar(string id, char op, QuadTree& quadtree, Hash& hash) {
    auto start = high_resolution_clock::now(); // Início da medição de tempo

    bool flag = quadtree.activate(hash.get(id)); // Ativa o ponto de recarga

    // cout << fixed << setprecision(6);
    // cout << op << ' ' << id << endl;
    // cout << fixed << setprecision(3);
    // Imprime mensagem sobre o status da ativação
    // cout << "Ponto de recarga " << id << (flag ? " já estava ativo." : " ativado.") << endl;

    auto end = high_resolution_clock::now(); // Fim da medição de tempo
    auto duration = duration_cast<microseconds>(end - start); // Usar microsegundos para maior precisão
    cout << fixed << setprecision(6); // Definir precisão para 6 casas decimais
    cout << "\nTempo de ativação: " << duration.count() / 1e6 << " s" << endl; // Converter microsegundos para segundos
}

// Função para desativar um ponto de recarga
void desativar(string id, char op, QuadTree& quadtree, Hash& hash) {
    auto start = high_resolution_clock::now(); // Início da medição de tempo
    bool flag = quadtree.inactivate(hash.get(id)); // Desativa o ponto de recarga

    // cout << fixed << setprecision(6);
    // cout << op << ' ' << id << endl;
    // cout << fixed << setprecision(3);
    // Imprime mensagem sobre o status da desativação
    // cout << "Ponto de recarga " << id << (flag ? " desativado." : " já estava desativado.") << endl;
    auto end = high_resolution_clock::now(); // Fim da medição de tempo
    auto duration = duration_cast<microseconds>(end - start); // Usar microsegundos para maior precisão
    cout << fixed << setprecision(6); // Definir precisão para 6 casas decimais
    cout << "\nTempo de desativação: " << duration.count() / 1e6 << " s" << endl; // Converter microsegundos para segundos
}


// Função para processar as consultas do arquivo .ev
void processQueries(istream& input, QuadTree& quadtree, Hash& hash, int tam) {
    string linha;
    
    // Lê o arquivo linha por linha
    for (int i = 0; i < tam; i++) {
        getline(input, linha);
        istringstream sstream(linha);
        char op;
        sstream >> op;
        
        if (op == 'C') {
            double x, y;
            int n;
            sstream >> x >> y >> n;
            consultar(x, y, n, op, quadtree, hash);
        } else if (op == 'A') {
            string id;
            sstream >> id;
            ativar(id, op, quadtree, hash);
        } else if (op == 'D') {
            string id;
            sstream >> id;
            desativar(id, op, quadtree, hash);
        } else if(""){
            break;
        } else {
            cerr << "Operação inválida: " << op << endl;
        }
    }
}

// Função principal
int main(int argc, char *argv[]) {
    // ler da entrada padrao
    QuadTree quadtree(Box(Point(0.0, 0.0), Point(1e9, 1e9)));
    Hash hash;


    // Processa o arquivo base
    int tam;
    cin >> tam;
    cin.ignore();
    process(cin, quadtree, hash, tam);

    int t;
    cin >> t;
    cin.ignore();
    processQueries(cin, quadtree, hash, t);

    return 0;
}
