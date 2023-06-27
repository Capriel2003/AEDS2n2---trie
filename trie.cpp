#include <iostream>
#include <vector>
#include <list>
#include <string>

const int ALPHABET_SIZE = 26;

struct Ocorrencia {
    int posicao;
    Ocorrencia* proximo;

    Ocorrencia(int pos) : posicao(pos), proximo(nullptr) {}
};

struct Documento {
    std::string nome;
    std::list<Ocorrencia*> ocorrencias;
    Documento* proximo;

    Documento(const std::string& docNome) : nome(docNome), proximo(nullptr) {}
};

struct No {
    std::string chave;
    std::list<Documento*> documentos;
    std::vector<No*> filhos;

    No(const std::string& chaveNo) : chave(chaveNo), filhos(ALPHABET_SIZE + 1, nullptr) {}
};

class Trie {
private:
    No* raiz;

public:
    Trie() {
        raiz = new No("");
    }

    void inserir(const std::string& palavra, const std::string& nomeDocumento, int posicao) {
        No* atual = raiz;

        for (char c : palavra) {
            int indice = c - 'a';
            if (atual->filhos[indice] == nullptr) {
                atual->filhos[indice] = new No(atual->chave + c);
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

    std::list<Ocorrencia*> buscar(const std::string& palavra) {
        std::list<Ocorrencia*> ocorrencias;

        No* atual = raiz;

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
    trie.inserir("exem", "documento2.txt", 5);


    std::string palavraBusca = "exe";
    std::list<Ocorrencia*> ocorrencias = trie.buscar(palavraBusca);

    if (ocorrencias.empty()) {
        std::cout << "Palavra '" << palavraBusca << "' nao encontrada." << std::endl;
    } else {
        std::cout << "Ocorrencias da palavra '" << palavraBusca << "':" << std::endl;
        for (Ocorrencia* ocorrencia : ocorrencias) {
            std::cout << ", Posicao: " << ocorrencia->posicao << std::endl;
        }
    }

    return 0;
}