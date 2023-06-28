#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <regex>
#include <algorithm>
#include <fstream>
using namespace std;

const int TamAlfabeto = 26;


vector<string> separaFrase(string frase){
    vector<string> separado;

    stringstream ss(frase);
    string pal;
    while (ss >> pal) {
        separado.push_back(pal);
    }

    return separado;
}

string tratarTexto(const string& texto) {
    // Converter caracteres maiúsculos para minúsculos
    string textoTratado = texto;
    transform(textoTratado.begin(), textoTratado.end(), textoTratado.begin(), ::tolower);

    // Substituir caracteres especiais
    regex expressao("[áàãâäéèêëíìîïóòõôöúùûüç]", regex_constants::icase);
    textoTratado = regex_replace(textoTratado, expressao, "$1");

    // Remover caracteres não alfanuméricos e espaços em branco
    textoTratado = regex_replace(textoTratado, regex("[^a-zA-Z0-9\\s]"), "");

    return textoTratado;
}

class Ocorrencia {
    public:
        int posicao;
        Ocorrencia* proximo;
        Ocorrencia(int pos) : posicao(pos), proximo(nullptr) {
            //vazio
        }
        friend std::ostream& operator<<(std::ostream& os, const Ocorrencia& ocorrencia) {
            os << "Posicao: " << ocorrencia.posicao;
            return os;
        }
};


class Documento {
    public:
        string nome;
        list<Ocorrencia*> ocorrencias;
        Documento* proximo;
        Documento(const string& docNome) : nome(docNome), proximo(nullptr) {
            //construtor vazio com nenhuma entrada
        }
};

class NoTrie {
    public:
        string chave;
        list<Documento*> documentos;
        vector<NoTrie*> filhos;
        NoTrie(const string& chaveNo) : chave(chaveNo), filhos(TamAlfabeto, nullptr) {
            //contrutor NoTrie vazio
        }
};

class Trie {
private:
    NoTrie* raiz;

public:
    Trie() {
        //como nao teve nenhuma entrada, a raiz recebe um valor vazio
        raiz = new NoTrie("");
    }

    void inserir(const string& palavra, const string& nomeDocumento, int posicao) {
        NoTrie* atual = raiz;

        for (char x : palavra) {
            int indice = x - 'a';
            if (atual->filhos[indice] == nullptr) {
                atual->filhos[indice] = new NoTrie(atual->chave + x);
                cout << "string2 " << x << endl;
            }
            atual = atual->filhos[indice];
        }

        // Cria ou encontra o nó do documento
        Documento* documentoNo = nullptr;
        for (auto doc : atual->documentos) {
            if (doc->nome == nomeDocumento) {
                documentoNo = doc;
                break;
            }
        }
        if (documentoNo == nullptr) {
            documentoNo = new Documento(nomeDocumento);
            atual->documentos.push_back(documentoNo);
        }

        // Cria o nó de ocorrência e adiciona ao documento
        Ocorrencia* ocorrenciaNo = new Ocorrencia(posicao);
        documentoNo->ocorrencias.push_back(ocorrenciaNo);
    }

    map<string, list<Ocorrencia*>> buscaPalavra(const string& palavra) {
        map<string, list<Ocorrencia*>> ocorrencias;

        NoTrie* atual = raiz;

        for (char c : palavra) {
            int indice = c - 'a';
            if (atual->filhos[indice] == nullptr) {
                // Palavra não encontrada, retorna vazio
                return ocorrencias;
            }
            atual = atual->filhos[indice];
        }

        // Encontrou a palavra, comeca a incrementar essas ocorrencias numa lista
        for (Documento* doc : atual->documentos) {
            for (Ocorrencia* ocorrencia : doc->ocorrencias) {
                ocorrencias[doc->nome].push_back(ocorrencia);
            }
        }

        return ocorrencias;
    }

    map<string,list<list<Ocorrencia*>>> buscaFrase(const string& fraseInt){
        vector <string> frase = separaFrase(fraseInt);
        map<string, list<list<Ocorrencia*>>> ocorrencias;
        map<string, list<Ocorrencia*>> aux;

        for(auto x: frase){
            aux = buscaPalavra(x);
            for(auto y: aux){
                ocorrencias[y.first].push_back(y.second);
            }
        }

        for (const auto& ocorrencia : ocorrencias) {
            cout << "Documento: " << ocorrencia.first << endl;
            for (const auto& listaOcorrencias : ocorrencia.second) {
                for (const auto& ocorrenciaPtr : listaOcorrencias) {
                    cout << "Palavra: " << x << endl;
                    cout << "Posicao: " << ocorrenciaPtr->posicao << endl;
                }
            }
        }

        return ocorrencias;
    }
};

int main() {
    Trie trie;
    trie.inserir("exemplo", "documento1.txt", 10);
    trie.inserir("exemplo", "documento2.txt", 4);
    trie.inserir("seu", "documento5.txt", 50);
    trie.inserir("pai", "documento2.txt", 5);
    trie.inserir("exem", "documento2.txt", 5);


    string palavraBusca = "exemplo";
    string fraseBusca = "seu pai eh corno";
    map<string, list<Ocorrencia*>> ocorrenciasPalavra = trie.buscaPalavra(palavraBusca);
    map<string, list<list<Ocorrencia*>>> ocorrenciaFrase = trie.buscaFrase(fraseBusca);
    





    if (ocorrenciasPalavra.empty()) {
        cout << "Palavra '" << palavraBusca << "' nao encontrada." << endl;
    } 
    else {
        cout << "Ocorrencias da palavra '" << palavraBusca << "':" << endl;
        for (auto ocorrencia : ocorrenciasPalavra) {
            cout << ocorrencia.first << endl;
            for(auto x: ocorrencia.second){
                cout << *x <<endl;
            }
        }
    }



    if (ocorrenciaFrase.empty()) {
        cout << "Palavra '" << fraseBusca << "' nao encontrada." << endl;
    } 
    else {
        cout << "Ocorrencias da palavra '" << fraseBusca << "':" << endl;
        for (auto ocorrencia : ocorrenciaFrase) {
            cout << ocorrencia.first << endl;
            for(auto x: ocorrencia.second){
                for(auto y: x){
                    cout << *y <<endl;
                }
            }
        }
    }



    return 0;
}