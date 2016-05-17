#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cstring>
#include <queue>
#include <functional>
#include <list>
using namespace std;

struct Header
{
	char symb;
	unsigned short freq;
};

struct HeaderNo // Registro utilizado na criação da arvore
{
	HeaderNo * esq;
	HeaderNo * dir;
	char symb;
	unsigned short freq;
};

struct Definition // Registro utilizado na criação do dicionario
{
	char symb;
	string form;
};

typedef pair<int, HeaderNo *> PQElement; // Par de valores para a manipulação da fila de prioridade

struct HeaderCompare // (Binary predicate) para a escolha do simbolo com menor frequencia
{
	bool operator()(const PQElement& lhs, const PQElement& rhs) const
	{
		return lhs.first > rhs.first;
	}
};

struct PriorityQueue // Fila de prioridade
{
	priority_queue<PQElement, vector<PQElement>,
		HeaderCompare> elements;

	inline void put(HeaderNo * item, int priority) { // Adiciona um HeaderNo à fila de prioridade
		elements.emplace(priority, item);
	}

	inline HeaderNo * get() { // Retorna o HeaderNo com maior prioridade (de acordo com o Binary predicate) e remove o elemento da fila.
		HeaderNo * best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};

int CreateAlphabet(char c);
char GetSymbol(HeaderNo * pt, list<char> * path);
void EncodeFile();
void DecodeFile();
void GenerateDictionary(HeaderNo * pt, string path);

string GetCode(char c);
string CharToBin(unsigned char input);
HeaderNo * CreateTree();
void DeleteTree(HeaderNo * pt);

#endif