#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "QuadTree.h"
#include "Hash.h"

using namespace std;

void process(ifstream& file, QuadTree& quadtree, Hash& hash) {
    string linha;
    while (getline(file, linha)) {
        istringstream sstream(linha);
        Addr temp;
        double x, y;
        string campo;
 
        getline(sstream, campo, ';');
        temp.idend = campo;
        getline(sstream, campo, ';');
        try {
            temp.id_logrado = stoll(campo);
        } catch (const exception& e) {
            cerr << "Erro na conversão de id_logrado: " << e.what() << " | valor: " << campo << endl;
            continue;
        }

        getline(sstream, temp.sigla_tipo, ';');
        getline(sstream, temp.nome_logra, ';');
        getline(sstream, campo, ';');
        try {
            temp.numero_imo = stoi(campo);
        } catch (const exception& e) {
            cerr << "Erro na conversão de numero_imo: " << e.what() << " | valor: " << campo << endl;
            continue;
        }

        getline(sstream, temp.nome_bairr, ';');
        getline(sstream, temp.nome_regio, ';');
        getline(sstream, campo, ';');
        try {
            temp.cep = stoi(campo);
        } catch (const exception& e) {
            cerr << "Erro na conversão de CEP: " << e.what() << " | valor: " << campo << endl;
            continue;
        }

        getline(sstream, campo, ';');
        try {
            x = stod(campo);
        } catch (const exception& e) {
            cerr << "Erro na conversão de coordenada X: " << e.what() << " | valor: " << campo << endl;
            continue;
        }

        getline(sstream, campo, ';');
        try {
            y = stod(campo);
        } catch (const exception& e) {
            cerr << "Erro na conversão de coordenada Y: " << e.what() << " | valor: " << campo << endl;
            continue;
        }

        temp.coordenadas = Point(x, y);
        temp.ativo = true;

        try {
            quadtree.insert(temp);
        } catch (const exception& e) {
            cerr << "Erro ao inserir no QuadTree: " << e.what() << endl;
            continue;
        }

        try {
            hash.insert(temp);
        } catch (const exception& e) {
            cerr << "Erro ao inserir na HashTable: " << e.what() << endl;
            continue;
        }
    }
}

void consultar(double x, double y, int n, char op, QuadTree& quadtree, Hash& hash) {
    cout << fixed << setprecision(6);
    cout << op << ' ' << x << ' ' << y << ' ' << n << endl;
    cout << fixed << setprecision(3);

    Pair<double, Addr>* stations = quadtree.KNN(Point(x, y), n);
    for(int i = 0; i < n; i++) {
        cout << stations[i].second << " (" << stations[i].first << ")" << endl;
    }

    delete[] stations;
}

void ativar(string id, char op, QuadTree& quadtree, Hash& hash) {
    bool flag = quadtree.activate(hash.get(id));

    cout << fixed << setprecision(6);
    cout << op << ' ' << id << endl;
    cout << fixed << setprecision(3);
    cout << "Ponto de recarga " << id << (flag ? " já estava ativo." : " ativado.") << endl;
}

void desativar(string id, char op, QuadTree& quadtree, Hash& hash) {
    bool flag = quadtree.inactivate(hash.get(id));

    cout << fixed << setprecision(6);
    cout << op << ' ' << id << endl;
    cout << fixed << setprecision(3);
    cout << "Ponto de recarga " << id << (flag ? " já estava desativado." : " desativado.") << endl;
}

int main() {
    QuadTree quadtree(Box(Point(0.0, 0.0), Point(1e9, 1e9)));
    Hash hash;

    ifstream base("data/geracarga.base");

    process(base, quadtree, hash);

    ifstream ev("data/geracarga.ev");

    int n;
    ev >> n;

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
