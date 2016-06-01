#include "Huffman.h"

Huffman::Huffman()
{
	TamArquivoDecodificado = 0;
	alphabetSize = 0;
	nbits = 0;
	texto = "";
}

Huffman::~Huffman()
{
	
}

void Huffman::Codificador()
{
	cout << "Lista de arquivos a serem Codificados\n";
	//Lista os arquivos a serem codificados
	system("dir /b *.txt"); 
	cout << "\n";
	// Nome do arquivo limitado a 60 caracteres
	char nomeArquivo[60] = { '\0' }; 
	cout << "Nome do arquivo que deseja codificar: ";
	cin >> nomeArquivo;

	ifstream fin;
	texto.clear();
	char b;
	
	// Abre arquivo a ser compactado.
	fin.open(nomeArquivo); 

	nomeArquivo[strlen(nomeArquivo) - 3] = 'h';
	nomeArquivo[strlen(nomeArquivo) - 2] = 'u';
	nomeArquivo[strlen(nomeArquivo) - 1] = 'f';

	if (!fin.is_open())
	{
		cout << "A abertura do arquivo falhou!" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	
	fin.get(b); 
	while (!fin.eof()) 
	{
		if (b > 0)
			texto.operator+=(b);
		fin.get(b);
	}
	fin.close();
	
	// Determina frequencias de cada caractere do texto
	for (int i = 0; i < texto.length(); i++)
		Alfabeto(texto[i]); 

	no = new No[alphabetSize];

	// Ordena headers de acordo com o simbolo
	list<No> lHeader;
	for (int i = 0; i < alphabetSize; i++)
	{
		lHeader.push_back(*totalDeNos[i]);
	}
	lHeader.sort([](const No& first, const No& second) { return first.simbolo < second.simbolo; });

	// Primeiro header da lista ordenada
	auto it = lHeader.begin();

	// Inicializa o vetor de headers e também instancia copias compativeis com a criação da arvore.
	for (int i = 0; i < alphabetSize; i++)
	{
		// Copia header da lista ordenada para vetor
		no[i] = *it;

		// Iteração da lista ordenada de headers.
		it++;

		// No compativel com criação da arvore.
		NoRaiz * no = new NoRaiz();
		no->a.simbolo = no[i].a.simbolo;
		no->a.freq = no[i].a.freq;
		no->dir = nullptr;
		no->esq = nullptr;

		// Adiciona No à fila de prioridade.
		fp.put(no, no[i].a.freq);
	}

	// Gera arvore a partir da fila de prioridade.
	NoRaiz * raiz = CriaArvore();

	// Gera dicionario a partir da raiz da arvore.
	Dicionario.clear();
	CriaDicionario(raiz, "");

	int contador = 0;
	char binaryChar = 0;
	string compactedText;

	// Cria arquivo que contem a informação compactada.
	ofstream fout;
	fout.open(nomeArquivo, ios_base::out | ios_base::binary);


	// Leitura de todos os caracteres da string.
	for (int i = 0; i < texto.length(); i++)
	{
		// Pega o codigo correspondente da letra.
		string code = GetPrefixo(texto[i]);

		// Le todos os caracteres (numeros) do codigo da letra.
		for (int j = 0; j < code.length(); j++)
		{
			// Converte o numero (0 e 1) de sua representação na tabela ascii para inteiro.
			int result = (code[j] - 48);

			binaryChar = binaryChar << 1;		// Shift para esquerda
			binaryChar = binaryChar | result; // Situações: binaryChar | 1 , binaryChar | 0

			contador++;

			if (contador == 8) // Limite binario do tipo char
			{
				// Adiciona caractere gerado e reseta variaveis auxiliares.
				compactedText.operator+=(binaryChar);
				binaryChar = 0;
				contador = 0;
			}
		}
	}

	// Caso o ultimo caractere não tenha sido totalmente preenchido
	if (contador < 8)
	{
		// Move a informação binaria adicionada para o inicio do char
		for (int i = contador; i < 8; i++)
			binaryChar = binaryChar << 1;

		// Numero de bits do texto compactado.
		nbits = compactedText.length() * 8 + contador;

		//Adiciona ultimo caractere à string.
		compactedText.operator+=(binaryChar);
	}

	int vetsize = ((nbits % 8) == 0) ? (nbits / 8) : ((nbits / 8) + 1);

	fout.write((char*)&alphabetSize, sizeof(unsigned short));
	fout.write((char*)no, sizeof(No) * alphabetSize);
	fout.write((char*)&nbits, sizeof(unsigned short));
	fout.write(compactedText.data(), sizeof(char)* vetsize);

	cout << "\n\nArquivo compactado. Saida: " << nomeArquivo << "\n\n";

	delete[] no;
	DeleteArvore(raiz);

	for (auto it : Dicionario)
		delete it;

	for (int i = 0; i < alphabetSize; i++)
		delete totalDeNos[i];

	fout.close();
	system("pause");
}


void Huffman::Decodificador()
{
	cout << "Lista de arquivos a serem Decodificados\n";
	system("dir /b *.huf"); //Lista os arquivos a serem codificados
	cout << "\n";

	char nomeArquivo[60] = { '\0' }; // Nome do arquivo limitado a 60 caracteres no máximo.

	cout << "Nome do arquivo que deseja decodificar: ";
	cin >> nomeArquivo;

	ifstream fin;
	fin.open(nomeArquivo, ios_base::in | ios_base::binary);

	nomeArquivo[strlen(nomeArquivo) - 3] = 't';
	nomeArquivo[strlen(nomeArquivo) - 2] = 'x';
	nomeArquivo[strlen(nomeArquivo) - 1] = 't';

	// O número de símbolos diferentes que aparecem no texto
	alphabetSize = 0;
	fin.read((char*)&alphabetSize, sizeof(unsigned short));

	// Aloca espaço para a informação do cabeçalho
	no = new No[alphabetSize];
	fin.read((char*)no, sizeof(No)* alphabetSize);

	// Lê o número de bits armazenados no arquivo e calcula tamanho do vetor
	nbits = 0;
	fin.read((char*)&nbits, sizeof(unsigned short));
	int vetsize = ((nbits % 8) == 0) ? (nbits / 8) : ((nbits / 8) + 1);

	// Aloca espaço para os dados
	char * data = new char[vetsize];
	fin.read((char*)data, sizeof(char)* vetsize);
	fin.close();

	// Copia a informação dos headers comuns em headers compativeis com a criação da arvore
	for (int i = 0; i < alphabetSize; i++)
	{
		NoRaiz * hNo = new NoRaiz;
		hNo->esq = nullptr;
		hNo->dir = nullptr;
		hNo->a.simbolo = no[i].simbolo;
		hNo->a.freq = no[i].freq;

		// Adiciona header à fila de prioridade.
		fp.put(hNo, no[i].freq);
	}

	cout << "\n\nArquivo decodificado: \n\n";
	char result;
	ofstream fout;
	fout.open(nomeArquivo);//Caso o arquivo txt exista essa abertura o sobreescreve com o texto decodificado
	NoRaiz * raiz = CriaArvore();

	for (int i = 0; i < alphabetSize; i++)
	{
		TamArquivoDecodificado += no[i].freq; // Determina o tamanho descompactado do arquivo
	}

	// Descompactação iniciada
	int i = 0;
	while (i < vetsize) // Iteração no vetor de caracteres (data)
	{
		for (int k = 0; // O k representa o bit lido no caractere.
			k != 8 && i < vetsize; // Se o k não for 8 (Leitura binaria final) e não for a iteração final do data
			k++)
		{
			list<char> binaryChar;

			// Converte um caractere em uma string contendo 0 e 1 correspondente.
			string text = ConversorBinario(data[i]);

			for (int j = 0; j < text.length(); j++)
			{
				binaryChar.push_back(text[j]); // Adiciona 0/1 em uma lista
			}

			i++; // Iteração

				 // Caso especial: Caso especial é acionado caso um caractere que estava sendo lido não deu informação binaria suficiente.
				 // Retorna leitura a partir de uma raiz temporaria determinada quando detectada a falta de informação.
			if (tempRaiz != nullptr)
			{
				// Tenta determinar qual o simbolo correspondente à informação binaria gravada.
				result = GetSimbolo(tempRaiz, &binaryChar);

				// Leitura bem sucedida.
				if (result != -1)
				{
					cout << result;
					fout << result;
					tempRaiz = nullptr; // Raiz temporaria não é mais necessaria.
				}
			}
			else // Caso normal: Caso normal é iniciado caso esteja na leitura inicial do arquivo ou a leitura da informação binaria
				 // tenha sido suficiente para o caractere anterior. 
			{
				// Tenta determinar qual o simbolo correspondente à informação binaria gravada.
				result = GetSimbolo(raiz, &binaryChar);

				// Leitura bem sucedida.
				if (result != -1)
				{
					cout << result;
					fout << result;
				}
			}



			// Caso ainda haja informação binaria, continue a gerar resultados.
			while (!binaryChar.empty())
			{
				// Tenta determinar qual o simbolo correspondente à informação binaria gravada.
				result = GetSimbolo(raiz, &binaryChar);

				// Leitura bem sucedida.
				if (result != -1)
				{
					cout << result;
					fout << result;

				}
			}

		}
	}
	cout << "\n\nArquivo decodificado em " << nomeArquivo << " !\n";
	fout.close();
	delete[] no;
	delete[] data;

	DeleteArvore(raiz);

	cout << "\n\n";
	system("pause");
}

string Huffman::ConversorBinario(unsigned char input) // Converte caractere em binario.
{
	unsigned temp = (unsigned)input;
	string tmp = "", tm = "";
	tmp.reserve(8);
	tm.reserve(8);
	while (temp)
	{
		tmp += '0' + temp % 2;
		temp /= 2;
	}
	reverse(tmp.begin(), tmp.end());
	while (tmp.size()<8)
	{
		tm = "0";
		tm += tmp;
		tmp = tm;
	}
	return tmp;
}

// Retorna a informação binaria correspondente do caractere c
string Huffman::GetPrefixo(char c)
{
	for (auto it = Dicionario.begin(); it != Dicionario.end(); it++)
	{
		if ((*it)->simbolo == c)
			return (*it)->codigo;
	}
	return "";
}

// Retorna simbolo correspondente à informação binaria
char Huffman::GetSimbolo(NoRaiz * pt, list<char> * path)
{
	// Assegura que a leitura das informações binarias não ultrapasse para o lixo do ultimo caractere.
	if (TamArquivoDecodificado == 0)
	{
		path->clear();
		return -1;
	}

	// A informação binaria gravada no path não foi suficiente para determinar o caractere correspondente.
	if (path->empty() && pt->a.simbolo == '\0')
	{
		tempRaiz = pt;
		return -1;
	}

	char c = pt->a.simbolo;

	// Caractere correspondente encontrado.
	if (c != '\0')
	{
		TamArquivoDecodificado--; // Um caracter foi lido.
		return c;
	}

	// Se houver informação binaria a ser lida, leia.
	// 0 para esquerda e 1 para direita
	if (!path->empty() && path->front() == '0')
	{
		path->pop_front();
		c = GetSimbolo(pt->esq, path);
	}

	// Uma verificação extra (c == '\0') para caso um caractere tenha sido encontrado na chamada da esquerda.
	// Necessaria para o retorno das chamadas recursivas.
	if (!path->empty() && path->front() == '1' && c == '\0')
	{
		path->pop_front();
		c = GetSimbolo(pt->dir, path);
	}

	// Retorna caractere encontrado. Esse retorno é utilizado durante as chamadas recursivas.
	return c;
}

// Gera dicionario para a codificação.
// Utilizado navegação pos-ordem.
void Huffman::CriaDicionario(NoRaiz * pt, string path)
{
	if (pt->esq != nullptr)
	{
		path.operator+=('0');
		CriaDicionario(pt->esq, path);
		path.pop_back();
	}

	if (pt->dir != nullptr)
	{
		path.operator+=('1');
		CriaDicionario(pt->dir, path);
		path.pop_back();
	}

	if (pt->a.simbolo != '\0')
	{
		Chave * def = new Chave; // Cria definição do dicionario
		def->simbolo = pt->a.simbolo;
		def->codigo = path;
		Dicionario.push_back(def); // Grava informação no dicionario.
	}
}

// Cria arvore.
NoRaiz * Huffman::CriaArvore()
{
	NoRaiz * n1;
	NoRaiz * n2;

	// Enquanto houver mais de um elemento na fila de prioridade significa que ainda existe nós para ser ligados.
	while (fp.elementos.size() > 1)
	{
		n1 = fp.get(); // Pega o no com menor frequencia.
		n2 = fp.get(); // Pega o no com menor frequencia.

		NoRaiz * no = new NoRaiz(); // Gera novo nó
		no->esq = n1;
		no->dir = n2;
		no->a.freq = n1->a.freq + n2->a.freq;
		no->a.simbolo = '\0';

		fp.put(no, n1->a.freq + n2->a.freq); // Adiciona nó à fila de prioridade
	}

	NoRaiz * raiz = fp.get();
	return raiz;
}


void Huffman::DeleteArvore(NoRaiz * pt)  // Deleta arvore.
{
	if (pt->esq != nullptr)
		DeleteArvore(pt->esq);

	if (pt->dir != nullptr)
		DeleteArvore(pt->dir);

	delete pt;
}


int Huffman::Alfabeto(char simbolo)  // frequencias de cada símbolo no texto
{
	if (simbolo == '\0' || simbolo < 0)
		return -2;

	for (int i = 0; i < alphabetSize; i++)
	{
		if (totalDeNos[i]->simbolo == simbolo)
		{
			totalDeNos[i]->freq++;
			return -1;
		}
	}

	// Adiciona caractere c como novo simbolo.
	totalDeNos[alphabetSize] = new No();
	totalDeNos[alphabetSize]->simbolo = simbolo;
	totalDeNos[alphabetSize]->freq++;
	alphabetSize++;
	return 0;
}


