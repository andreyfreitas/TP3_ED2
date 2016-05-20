#include <iostream>
#include <cstring>
#include <queue>
#include <functional>
#include <list>
#include <fstream>
using namespace std;

struct Header
{
	char simbolo;
	unsigned short freq;
};

struct HeaderNo //Nó Raiz
{
	HeaderNo * esq;
	HeaderNo * dir;
	Header a;
};

struct Definition //chave do dicionario
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

class Huffman
{
public:
	Huffman();
	~Huffman();
	void EncodeFile();
	void DecodeFile();
	
private:

	int decodedFileLenght;
	unsigned short alphabetSize;
	unsigned short nbits;
	string text;

	HeaderNo * tempRaiz;
	Header * totalHeader[256];
	Header * header;
	PriorityQueue simpleFrontier;
	list<Definition *> Dictionary;

	int CreateAlphabet(char simbolo);
	char GetSymbol(HeaderNo * pt, list<char> * path);
	void GenerateDictionary(HeaderNo * pt, string path);

	string GetCode(char c);
	string CharToBin(unsigned char input);
	HeaderNo * CreateTree();
	void DeleteTree(HeaderNo * pt);

	
};


