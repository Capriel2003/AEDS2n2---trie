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
        Ocorrencia(int pos) : posicao(pos), proximo(nullptr){}
        /*
        friend ostream& operator<<(ostream& os, const Ocorrencia& ocorrencia) {
            cout << "Posicao: " << ocorrencia.posicao << endl;
            os << "Posicao: " << ocorrencia.posicao;
            return os;
        }*/
};


class Documento {
    public:
        string nome;
        list<int> ocorrencias;
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
        NoTrie(const string& chaveNo) : chave(chaveNo), filhos(TamAlfabeto + 1, nullptr) {
        }
};

class Trie {
public:
    NoTrie* raiz;

    Trie() {
        //como nao teve nenhuma entrada, a raiz recebe um valor vazio
        raiz = new NoTrie("");
    }

    void imprime(NoTrie* atual){
        NoTrie* aux = atual;
        for(auto x: atual->filhos)
            cout << x << " ";
        cout << endl << endl;
        for(int i = 0; i<26; i++){
            if(atual->filhos[i] != nullptr){
                cout << "No do " << (char)(i+97) << ": " << endl;
                aux = atual->filhos[i];
                imprime(aux);
            }
        }
    }

    void realoca(NoTrie* no){
        NoTrie* atual = raiz;
        string palavra = no->chave;
        int indice;
        for (char x: palavra){
            indice = x - 'a';
            if (atual->filhos[indice] == nullptr){
                string s(1, x);
                atual->filhos[indice] = new NoTrie(s);
                atual = atual->filhos[indice];
                atual->filhos[26] = no;
                return;
            }
            atual = atual->filhos[indice];
        }
        atual->filhos[26] = no;
    }

    void inserir(const string& nomeDocumento, const string& palavra, int posicao) {
        NoTrie* atual = raiz;
        int indice;
        for (char x: palavra) {
            indice = x - 'a';
            if (atual->filhos[26] == nullptr){//verifica se não é o final de uma ramificação
                if (atual->filhos[indice] == nullptr) {//se o caractere estiver livre, adiciona a palavra e o ponto para representar o final de uma ramificação
                    string s(1, x);
                    atual->filhos[indice] = new NoTrie(s);
                    atual = atual->filhos[indice];
                    break;
                }
            }
            else{ //se for o fim de uma ramificação
                string s(1, x);
                atual->filhos[indice] = new NoTrie(s);
                NoTrie* aux = atual->filhos[26];
                atual->filhos[26] = nullptr;
                realoca(aux);
            }
            atual = atual->filhos[indice];
        }

        if(atual->filhos[26] == nullptr)
            atual->filhos[26] = new NoTrie(palavra);

        atual = atual->filhos[26];

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
        documentoNo->ocorrencias.push_back(posicao);

        //atual->documentos.push_back(documentoNo);
    }

    map<string, pair<string, list<int>>> buscaPalavra(string palavra) {
        map<string, pair<string, list<int>>> ocorrencias;
        NoTrie* atual = raiz;
        for (char c: palavra) {
            int indice = c - 'a';
            if (atual->filhos[indice] == nullptr){
                if(atual->filhos[26] == nullptr)
                    return ocorrencias;
                break;
            }
            atual = atual->filhos[indice];
        }
        // Encontrou a palavra, comeca a incrementar essas ocorrencias numa lista
        for (Documento* doc : atual->filhos[26]->documentos) {
            ocorrencias[doc->nome].first = palavra;
            for (auto x: doc->ocorrencias) {
                ocorrencias[doc->nome].second.push_back(x);
            }
        }
        return ocorrencias;
    }

    map<string,list<pair<string, list<int>>>> buscaFrase(const string& fraseInt){
        vector <string> frase = separaFrase(fraseInt);
        map<string,list<pair<string, list<int>>>> ocorrencias;
        map<string, pair<string, list<int>>> aux;

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

    trie.inserir("documento2.txt", "casa", 10);
    trie.inserir("documento2.txt", "casamento", 12);
    NoTrie* raiz = trie.raiz;
    trie.imprime(raiz);
    //trie.inserir("documento1.txt", "este", 20);

    string palavraBusca = "casamento";
    string fraseBusca = "seu pai eh corno";
    map<string, pair<string, list<int>>> ocorrenciasPalavra = trie.buscaPalavra(palavraBusca);
    //map<string, list<pair<string, list<int>>>> ocorrenciaFrase = trie.buscaFrase(fraseBusca);

    if (ocorrenciasPalavra.empty()) {
        cout << "Palavra '" << palavraBusca << "' nao encontrada." << endl;
    }
    else {
        cout << "Ocorrencias da palavra '" << palavraBusca << "':" << endl;
        for (auto ocorrencia : ocorrenciasPalavra) {
            cout << "    " << ocorrencia.first << endl;
            for(auto x: ocorrencia.second.second){
                cout << "        " << x <<endl;
            }
        }
    }
    palavraBusca = "casa";
    ocorrenciasPalavra = trie.buscaPalavra(palavraBusca);
    if (ocorrenciasPalavra.empty()) {
        cout << "Palavra '" << palavraBusca << "' nao encontrada." << endl;
    }
    else {
        cout << "Ocorrencias da palavra '" << palavraBusca << "':" << endl;
        for (auto ocorrencia : ocorrenciasPalavra) {
            cout << "    " << ocorrencia.first << endl;
            for(auto x: ocorrencia.second.second){
                cout << "        " << x <<endl;
            }
        }
    }

/*
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
*/
    return 0;
}
