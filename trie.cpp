#include <iostream>
#include <unordered_map>
#include <memory>
using namespace std;

class No {
private:
    unordered_map<char, shared_ptr<No>> filho;
    bool finalPalavra;

public:
    No() : finalPalavra(false) {}

    bool temFilho(char ch) const {
        return filho.find(ch) != filho.end();
    }

    shared_ptr<No> getFilho(char ch) const {
        return filho.at(ch);
    }

    void addFilho(char ch, shared_ptr<No> child) {
        filho[ch] = child;
    }

    bool finalP() const {
        return finalPalavra;
    }

    void setFinalP() {
        finalPalavra = true;
    }
};

class Trie {
private:
    shared_ptr<No> raiz;

public:
    Trie() : raiz(make_shared<No>()) {}

    void insere(const string& palavra) {
        shared_ptr<No> noAtual = raiz;

        for (char ch : palavra) {
            if (!noAtual->temFilho(ch)) {
                shared_ptr<No> newChild = make_shared<No>();
                noAtual->addFilho(ch, newChild);
            }

            noAtual = noAtual->getFilho(ch);
        }

        noAtual->setFinalP();
    }

    bool procurarFrase(const string& palavra) const {
        shared_ptr<No> noAtual = raiz;

        for (char ch : palavra) {
            if (!noAtual->temFilho(ch)) {
                return false;
            }

            noAtual = noAtual->getFilho(ch);
        }

        return noAtual->finalP();
    }

    bool procurarPalavra(const string& prefix) const {
        shared_ptr<No> noAtual = raiz;

        for (char ch : prefix) {
            if (!noAtual->temFilho(ch)) {
                return false;
            }
            noAtual = noAtual->getFilho(ch);
        }

        return true;
    }
};

int main() {
    Trie trie;

    trie.insere("apple");

    cout << "Search 'apple': " << (trie.procurarPalavra("apple") ? "Found" : "Not Found") << endl;

    return 0;
}
