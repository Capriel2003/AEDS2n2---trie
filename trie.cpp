#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <regex>
#include <algorithm>
using namespace std;

const int TamAlfabeto = 26;

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

struct Ocorrencia {
    int posicao;
    Ocorrencia* proximo;
    Ocorrencia(int pos) : posicao(pos), proximo(nullptr) {
        //vazio
    }
};

struct Documento {
    string nome;
    list<Ocorrencia*> ocorrencias;
    Documento* proximo;
    Documento(const string& docNome) : nome(docNome), proximo(nullptr) {
        //construtor vazio com nenhuma entrada
    }
};

struct NoTrie {
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

    list<Ocorrencia*> buscar(const string& palavra) {
        list<Ocorrencia*> ocorrencias;

        NoTrie* atual = raiz;

        for (char c : palavra) {
            int indice = c - 'a';
            if (atual->filhos[indice] == nullptr) {
                // Palavra não encontrada, retorna lista vazia
                return ocorrencias;
            }
            atual = atual->filhos[indice];
        }

        // Encontrou a palavra, adiciona as ocorrências de cada documento à lista
        for (Documento* doc : atual->documentos) {
            for (Ocorrencia* ocorrencia : doc->ocorrencias) {
                ocorrencias.push_back(ocorrencia);
            }
        }

        return ocorrencias;
    }
};

int main() {
    Trie trie;
    trie.inserir("exemplo", "documento1.txt", 10);
    trie.inserir("exemplo", "documento1.txt", 4);
    trie.inserir("examplo", "documento5.txt", 50);
    trie.inserir("exam", "documento2.txt", 5);
    trie.inserir("exem", "documento2.txt", 5);


    string palavraBusca = "exemplo";
    list<Ocorrencia*> ocorrencias = trie.buscar(palavraBusca);

    if (ocorrencias.empty()) {
        cout << "Palavra '" << palavraBusca << "' nao encontrada." << endl;
    } 
    else {
        cout << "Ocorrencias da palavra '" << palavraBusca << "':" << endl;
        for (Ocorrencia* ocorrencia : ocorrencias) {
            cout << "Posicao: " << ocorrencia->posicao << endl;
        }
    }

    return 0;
}