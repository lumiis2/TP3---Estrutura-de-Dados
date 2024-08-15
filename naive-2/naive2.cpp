#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

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
bool ler_csv(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Não foi possível abrir o arquivo: " << filename << endl;
        return false;
    }

    string linha;

    // Pula o cabeçalho
    getline(file, linha);


    while (getline(file, linha)) {
        istringstream sstream(linha);
        string campo;
        addr_t temp;


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
    file.close();
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
                cout << "Ponto de recarga " << id << " já estava ativo." << endl;
            } else {
                recarga.ativo = true;
                cout << "Ponto de recarga " << id << " ativado." << endl;
            }
            return;
        }
    }
    cout << "Ponto de recarga " << id << " não encontrado." << endl;
}

// Função para desativar um ponto de recarga
void desativar_pontos(string id) {
    for (auto& recarga : rechargevet) {
        if (recarga.idend == id) {
            if (!recarga.ativo) {
                cout << "Ponto de recarga " << id << " já estava desativado." << endl;
            } else {
                recarga.ativo = false;
                cout << "Ponto de recarga " << id << " desativado." << endl;
            }
            return;
        }
    }
    cout << "Ponto de recarga " << id << " não encontrado." << endl;
}

// Função para consultar os n pontos de recarga mais próximos
void consulta_proximos_pontos(double tx, double ty, int n) {
    knn_vector kvet;
    cout << rechargevet.size() << endl;

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
        printrecharge(kvet[i].id);
        printf(" (%f)\n", kvet[i].dist);
    }
}

int main() {
    ler_csv("endereco_recarga.csv");
    cout << "ok - TAM: " << rechargevet.size() << endl;
    // Processar entradas
    string comando;
    while (cin >> comando) {
        cout << comando << endl;
        if (comando == "C") {
            double x, y;
            int n;
            cin >> x >> y >> n;
            consulta_proximos_pontos(x, y, n);
        } else if (comando == "A") {
            string id;
            cin >> id;
            ativar_pontos(id);
        } else if (comando == "D") {
            string id;
            cin >> id;
            desativar_pontos(id);
        } else {
            cerr << "Comando desconhecido: " << comando << endl;
        }
    }

    return 0;
}
