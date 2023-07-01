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

string tratarTexto(string& texto) {
    // Converter caracteres maiúsculos para minúsculos
    transform(texto.begin(), texto.end(), texto.begin(), ::tolower);

    // Substituir caracteres acentuados por suas versões sem acento
    texto = regex_replace(texto, regex("[áàãâä]"), "a");
    texto = regex_replace(texto, regex("[éèêë]"), "e");
    texto = regex_replace(texto, regex("[íìîï]"), "i");
    texto = regex_replace(texto, regex("[óòõôö]"), "o");
    texto = regex_replace(texto, regex("[úùûü]"), "u");
    texto = regex_replace(texto, regex("[ç]"), "c");

    // Remover caracteres não alfanuméricos e espaços em branco
    texto = regex_replace(texto, regex("[^a-zA-Z\\s]"), "");

    return texto;
}

class Ocorrencia {
    public:
        int posicao;
        Ocorrencia* proximo;
        Ocorrencia(int pos) : posicao(pos), proximo(nullptr) {
            //vazio
        }
        friend ostream& operator<<(ostream& os, const Ocorrencia& ocorrencia) {
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

    map<string, pair<string, list<Ocorrencia*>>> buscaPalavra(const string& palavra) {
        map<string, pair<string, list<Ocorrencia*>>> ocorrencias;

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
                ocorrencias[doc->nome].first=palavra;
                ocorrencias[doc->nome].second.push_back(ocorrencia);
            }
        }

        return ocorrencias;
    }

    map<string,list<pair<string, list<Ocorrencia*>>>> buscaFrase(const string& fraseInt){
        vector <string> frase = separaFrase(fraseInt);
        map<string,list<pair<string, list<Ocorrencia*>>>> ocorrencias;
        map<string, pair<string, list<Ocorrencia*>>> aux;

        for(auto x: frase){
            aux = buscaPalavra(x);
            for(auto y: aux){
                ocorrencias[y.first].push_back(y.second);
            }
        }

        return ocorrencias;
    }
};

int main() {
    Trie trie;
    
    ifstream entrada;
    string documento = "documento1.txt";
    entrada.open(documento);
    
    if(entrada.is_open()){
        string linha;
        int pos = 1;
        vector<pair<string, int>> palavras;
        while(getline(entrada, linha)){
            int fim = 0;
            while ((fim = linha.find(" ")) != string::npos) {
                palavras.push_back(make_pair(linha.substr(0, fim), pos));
                pos = pos+fim+1;
                linha.erase(0, fim + 1);
            }
            palavras.push_back(make_pair(linha, pos));
            pos = pos+linha.length();
        }
        for(auto &str :palavras) {
            cout << "inserindo: " << str.first << " " << str.second << endl;
            trie.inserir(documento, tratarTexto(str.first), str.second);
        }
        entrada.close();
    
        for(auto &str :palavras) 
            cout << str.first << " " << str.second << endl;
    }

    
    string p = "Exemplõ";
    tratarTexto(p);
    trie.inserir("exemplo", "documento1.txt", 10);
    trie.inserir(p, "documento2.txt", 4);
    trie.inserir("exemplo", "documento7.txt", 4);
    trie.inserir("exemplo", "documento25.txt", 4);
    trie.inserir("seu", "documento5.txt", 50);
    trie.inserir("pai", "documento2.txt", 5);
    trie.inserir("exem", "documento2.txt", 5);


    string palavraBusca = "exemplo";
    string fraseBusca = "seu pai eh corno";
    map<string, pair<string, list<Ocorrencia*>>>  ocorrenciasPalavra = trie.buscaPalavra(palavraBusca);
    map<string,list<pair<string, list<Ocorrencia*>>>> ocorrenciaFrase = trie.buscaFrase(fraseBusca);
    





    if (ocorrenciasPalavra.empty()) {
        cout << "Palavra '" << palavraBusca << "' nao encontrada." << endl;
    } 
    else {
        cout << "Ocorrencias da palavra '" << palavraBusca << "':" << endl;
        for (auto ocorrencia : ocorrenciasPalavra) {
            cout << ocorrencia.first << endl;
            for(auto x: ocorrencia.second.second){
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
            cout << "Documento: " << ocorrencia.first << endl;
            for(auto x: ocorrencia.second){
                cout << "Palavra: " << x.first << endl;
                for (auto y: x.second){
                    cout << *y << endl;
                }
            }
        }
    }




    return 0;
}
