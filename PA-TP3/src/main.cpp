#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
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

        getline(sstream, temp.idend, ';');
        getline(sstream, campo, ';');
        temp.id_logrado = stoll(campo);
        getline(sstream, temp.sigla_tipo, ';');
        getline(sstream, temp.nome_logra, ';');
        getline(sstream, campo, ';');
        temp.numero_imo = stoi(campo);
        getline(sstream, temp.nome_bairr, ';');
        getline(sstream, temp.nome_regio, ';');
        getline(sstream, campo, ';');
        temp.cep = stoi(campo);
        getline(sstream, campo, ';');
        x = stod(campo);
        getline(sstream, campo, ';');
        y = stod(campo);

        temp.coordenadas = Point(x, y);

        quadtree.insert(temp);
        hash.insert(temp);
    }
}

void consulta_proximos_pontos(QuadTree& quadtree, const string& t, double x, double y, int n) {
    cout << fixed << setprecision(6);
    cout << t << ' ' << x << ' ' << y << ' ' << n << endl;
    cout << fixed << setprecision(3);

    Pair* st = quadtree.KNN(Point(x, y), n);
    for (int i = 0; i < n; i++) {
        cout << st[i].second << " (" << st[i].first << ")" << endl;
    }

    delete[] st;
}

void ativar_pontos(const string& t, const string& id, QuadTree& quadtree, const Hash& hash) {
    bool flag = quadtree.activate(hash.get(id));
    cout << fixed << setprecision(6);
    cout << t << ' ' << id << endl;
    cout << fixed << setprecision(3);
    cout << "Ponto de recarga " << id << (flag ? " já estava ativo." : " ativado.") << endl;
}

void desativar_pontos(const string& t, const string& id, QuadTree& quadtree, const Hash& hash) {
    bool flag = quadtree.inactivate(hash.get(id));
    cout << fixed << setprecision(6);
    cout << t << ' ' << id << endl;
    cout << fixed << setprecision(3);
    cout << "Ponto de recarga " << id << (flag ? " já estava desativado." : " desativado.") << endl;
}

int main() {
    QuadTree quadtree(Box(Point(0.0, 0.0), Point(1e6, 1e6)));
    Hash hash;

    ifstream base("geracarga.base");
    if (!base) {
        cerr << "Erro ao abrir o arquivo geracarga.base" << endl;
        return 1;
    }
    process(base, quadtree, hash);

    ifstream ev("geracarga.ev");
    if (!ev) {
        cerr << "Erro ao abrir o arquivo geracarga.ev" << endl;
        return 1;
    }

    int n;
    ev >> n;

    string comando;
    while (ev >> comando) {
        cout << comando << endl;
        if (comando == "C") {
            double x, y;
            int tam;
            ev >> x >> y >> tam;
            consulta_proximos_pontos(quadtree, comando, x, y, tam);
        } else if (comando == "A") {
            string id;
            ev >> id;
            ativar_pontos(comando, id, quadtree, hash);
        } else if (comando == "D") {
            string id;
            ev >> id;
            desativar_pontos(comando, id, quadtree, hash);
        } else {
            cerr << "Comando desconhecido: " << comando << endl;
        }
    }

    return 0;
}
