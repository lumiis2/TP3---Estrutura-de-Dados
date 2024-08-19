#include <iostream>   // Para entrada e saída padrão
#include <fstream>    // Para operações de arquivo
#include <sstream>    // Para operações de string stream
#include <iomanip>    // Para manipulação de formato de entrada/saída
#include <cmath>      // Para funções matemáticas
#include <vector>     // Para o uso de vetores
#include <string>     // Para manipulação de strings
#include <algorithm>  // Para algoritmos padrão (como sort)
#include <cstdlib>    // Para funções utilitárias (como malloc e free)
#include <cstdio>     // Para funções C de entrada/saída
#include <map>        // Para uso de mapas (dicionários)
#include <cstring>    // Para manipulação de C-strings
#include <chrono>     // Para manipulação de tempo


using namespace std;
using namespace std::chrono;

// Struct que contém as informações sobre os locais de recarga
struct addr_t {
    string idend;
    long id_logrado;
    string sigla_tipo;
    string nome_logra;
    int numero_imo;
    string nome_bairr;
    string nome_regio;
    int cep;
    double x;
    double y;
    bool ativo;  // Nova variável para rastrear se o ponto de recarga está ativo ou não
};

struct knn_t {
    int id;
    double dist;
};

typedef vector<knn_t> knn_vector;
typedef vector<knn_t>::iterator ptr_knn_t;

vector<addr_t> rechargevet;

// Função para ler o CSV e armazenar os dados na variável global
bool process(int n) {
    auto start = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        addr_t temp;
        string linha;
        
        // Ler linha completa da entrada
        getline(cin, linha);
        istringstream sstream(linha);
        string campo;

        // Lê cada campo separado por ';'
        getline(sstream, temp.idend, ';');
        getline(sstream, campo, ';');
        temp.id_logrado = stol(campo);
        getline(sstream, temp.sigla_tipo, ';');
        getline(sstream, temp.nome_logra, ';');
        getline(sstream, campo, ';');
        temp.numero_imo = stoi(campo);
        getline(sstream, temp.nome_bairr, ';');
        getline(sstream, temp.nome_regio, ';');
        getline(sstream, campo, ';');
        temp.cep = stoi(campo);
        getline(sstream, campo, ';');
        temp.x = stod(campo);
        getline(sstream, campo, ';');
        temp.y = stod(campo);
        temp.ativo = true; // todos ativos inicialmente

        rechargevet.push_back(temp);  // Armazena na variável global
    }
    
    auto end = high_resolution_clock::now(); // Fim da medição de tempo
    auto duration = duration_cast<microseconds>(end - start); // Usar microsegundos para maior precisão
    cout << fixed << setprecision(6); // Definir precisão para 6 casas decimais
    cout << "\nTempo de processamento: " << duration.count() / 1e6 << " s" << endl; // Converter microsegundos para segundos
    return true;
}

// Função de comparação para ordenar pelo campo `dist` no vetor de knn
bool cmpknn(const knn_t& k1, const knn_t& k2) {
    return k1.dist < k2.dist;
}

// Calcula a distância Euclidiana entre (x1, y1) e (x2, y2)
double dist(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Função para imprimir as informações da estação de recarga
void printrecharge(int pos) {
    cout << rechargevet[pos].sigla_tipo << " "
         << rechargevet[pos].nome_logra << ", "
         << rechargevet[pos].numero_imo << ", "
         << rechargevet[pos].nome_bairr << ", "
         << rechargevet[pos].nome_regio << ", "
         << rechargevet[pos].cep;
}

// Função para ativar um ponto de recarga
void ativar_pontos(string id) {
    for (auto& recarga : rechargevet) {
        if (recarga.idend == id) {
            if (recarga.ativo) {
                // cout << "Ponto de recarga " << id << " já estava ativo." << endl;
            } else {
                recarga.ativo = true;
                // cout << "Ponto de recarga " << id << " ativado." << endl;
            }
            return;
        }
    }
    // cout << "Ponto de recarga " << id << " não encontrado." << endl;
}

// Função para desativar um ponto de recarga
void desativar_pontos(string id) {
    for (auto& recarga : rechargevet) {
        if (recarga.idend == id) {
            if (!recarga.ativo) {
                // cout << "Ponto de recarga " << id << " já estava desativado." << endl;
            } else {
                recarga.ativo = false;
                // cout << "Ponto de recarga " << id << " desativado." << endl;
            }
            return;
        }
    }
    // cout << "Ponto de recarga " << id << " não encontrado." << endl;
}

// Função para consultar os n pontos de recarga mais próximos
void consulta_proximos_pontos(double tx, double ty, int n) {
    knn_vector kvet;
    // cout << rechargevet.size() << endl;

    for (size_t i = 0; i < rechargevet.size(); i++) {
        if (rechargevet[i].ativo) {  // Apenas considere os pontos ativos
            knn_t knn;
            knn.id = i;
            knn.dist = dist(tx, ty, rechargevet[i].x, rechargevet[i].y);
            kvet.push_back(knn);
        }
    }

    // Ordenar o vetor de distâncias
    sort(kvet.begin(), kvet.end(), cmpknn);

    // Imprimir as n localizações de recarga mais próximas
    for (int i = 0; i < min(n, (int)kvet.size()); i++) {
        // printrecharge(kvet[i].id);
        // printf(" (%f)\n", kvet[i].dist);
    }
}

int main(int argc, char *argv[]) {
    int tam;
    cin >> tam;
    cin.ignore();
    process(tam);

    int t;
    cin >> t;
    cin.ignore();
    string comando;

    auto start1 = high_resolution_clock::now();
    while (cin >> comando && comando == "C") {
        double x, y;
        int n;
        cin >> x >> y >> n;
        consulta_proximos_pontos(x, y, n);
    }
    auto end1 = high_resolution_clock::now(); // Fim da medição de tempo
    auto duration1 = duration_cast<microseconds>(end1 - start1); // Usar microsegundos para maior precisão
    cout << fixed << setprecision(6); // Definir precisão para 6 casas decimais
    cout << "\nTempo de consulta: " << duration1.count() / 1e6 << " s" << endl; // Converter microsegundos para segundos

    auto start2= high_resolution_clock::now();
    while (cin >> comando && comando == "A") {
        string id;
        cin >> id;
        ativar_pontos(id);
    }
    auto end2 = high_resolution_clock::now(); // Fim da medição de tempo
    auto duration2 = duration_cast<microseconds>(end2 - start2); // Usar microsegundos para maior precisão
    cout << fixed << setprecision(6); // Definir precisão para 6 casas decimais
    cout << "\nTempo de ativação: " << duration2.count() / 1e6 << " s" << endl; // Converter microsegundos para segundos

    auto start3 = high_resolution_clock::now();
    while (cin >> comando && comando == "D") {
        string id;
        cin >> id;
        desativar_pontos(id);
    }
    auto end3 = high_resolution_clock::now(); // Fim da medição de tempo
    auto duration3 = duration_cast<microseconds>(end3 - start3); // Usar microsegundos para maior precisão
    cout << fixed << setprecision(6); // Definir precisão para 6 casas decimais
    cout << "\nTempo de desativação: " << duration3.count() / 1e6 << " s" << endl; // Converter microsegundos para segundos

    return 0;
}
