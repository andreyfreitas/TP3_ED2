/*******************************************************************************/
//						Andrey Alves de Freitas									/
//						Marcio Bruno Ferreira de Sá								/
//									TP3											/
/*******************************************************************************/
#include <iostream>
#include <cstring>
#include <queue>
#include <functional>
#include <list>
#include <fstream>
#include<Windows.h>
using namespace std;

struct No
{
	char simbolo;
	unsigned short freq;
};

struct NoRaiz 
{
	NoRaiz * esq;
	NoRaiz * dir;
	No a;
};

//codigo prefixo
struct Chave 
{
	char simbolo;
	string codigo;
};

// Par de valores para a manipulação da fila de prioridade
typedef pair<int, NoRaiz *> ElementoDaFila; 

// Compara o no escolhendo o simbolo com menor frequencia
struct Comparador 
{
	bool operator()(const ElementoDaFila& e1, const ElementoDaFila& e2) const
	{
		return (e1.first > e2.first ? true : false);
	}
};

// Cria a fila de prioridade
struct FilaPrioridade 
{
	priority_queue<ElementoDaFila, vector<ElementoDaFila>,
		Comparador> elementos;

	// Adiciona um NoRaiz à fila de prioridade
	inline void put(NoRaiz * item, int prioridade) { 
		elementos.emplace(prioridade, item);
	}

	// Retorna o No com maior prioridade removendo o elemento da fila.
	inline NoRaiz * get() { 
		NoRaiz * item = elementos.top().second;
		elementos.pop();
		return item;
	}
};

class Huffman
{
public:
	Huffman();
	~Huffman();
	void Codificador();
	void Decodificador();
	
	
	
private:

	int TamArquivoDecodificado;
	unsigned short alphabetSize;
	unsigned short nbits;
	string texto;

	NoRaiz * tempRaiz;
	No * totalDeNos[256];
	No * no;
	FilaPrioridade fp;
	list<Chave *> Dicionario;

	int Alfabeto(char simbolo);
	char GetSimbolo(NoRaiz * pt, list<char> * caminho);
	void CriaDicionario(NoRaiz * pt, string caminho);

	string GetPrefixo(char c);
	string ConversorBinario(unsigned char entrada);
	NoRaiz * CriaArvore();
	void DeleteArvore(NoRaiz * pt);
	void wait();
	
};


