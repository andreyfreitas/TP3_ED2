#include "Huffman.h"

Huffman::Huffman()
{
	decodedFileLenght = 0;
	alphabetSize = 0;
	nbits = 0;
	text = "";
}

Huffman::~Huffman()
{
	
}

void Huffman::EncodeFile()
{
	cout << "Lista de arquivos a serem Codificados\n";
	system("dir /b *.txt"); //Lista os arquivos a serem codificados
	cout << "\n";

	char nomeArquivo[60] = { '\0' }; // Nome do arquivo limitado a 60 caracteres no m�ximo.
	cout << "Nome do arquivo que deseja codificar: ";
	cin >> nomeArquivo;

	ifstream fin;
	text.clear();
	char b;

	fin.open(nomeArquivo); // Abre arquivo a ser compactado.

	nomeArquivo[strlen(nomeArquivo) - 3] = 'h';//apos abertura a variavel nomeArquivo passa representar
	nomeArquivo[strlen(nomeArquivo) - 2] = 'u';//o nome do arquivo de saida
	nomeArquivo[strlen(nomeArquivo) - 1] = 'f';

	if (!fin.is_open())
	{
		cout << "A abertura do arquivo falhou!" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	fin.get(b); // L� primeiro caractere

	while (!fin.eof()) // Enquanto o ifstream n�o estiver no final do arquivo
	{
		if (b > 0)
			text.operator+=(b); // L� pr�ximo valor

		fin.get(b);
	}


	fin.close();

	for (int i = 0; i < text.length(); i++)
		CreateAlphabet(text[i]); // Determina frequencias de cada caractere do texto

	header = new Header[alphabetSize];

	// Ordena headers de acordo com o simbolo
	list<Header> lHeader;
	for (int i = 0; i < alphabetSize; i++)
	{
		lHeader.push_back(*totalHeader[i]);
	}
	lHeader.sort([](const Header& first, const Header& second) { return first.simbolo < second.simbolo; });

	// Primeiro header da lista ordenada
	auto it = lHeader.begin();

	// Inicializa o vetor de headers e tamb�m instancia copias compativeis com a cria��o da arvore.
	for (int i = 0; i < alphabetSize; i++)
	{
		// Copia header da lista ordenada para vetor
		header[i] = *it;

		// Itera��o da lista ordenada de headers.
		it++;

		// Header compativel com cria��o da arvore.
		HeaderNo * no = new HeaderNo();
		no->a.simbolo = header[i].simbolo;
		no->a.freq = header[i].freq;
		no->dir = nullptr;
		no->esq = nullptr;

		// Adiciona HeaderNo � fila de prioridade.
		simpleFrontier.put(no, header[i].freq);
	}

	// Gera arvore a partir da fila de prioridade.
	HeaderNo * raiz = CreateTree();

	// Gera dicionario a partir da raiz da arvore.
	Dictionary.clear();
	GenerateDictionary(raiz, "");

	int contador = 0;
	char binaryChar = 0;
	string compactedText;

	// Cria arquivo que contem a informa��o compactada.
	ofstream fout;
	fout.open(nomeArquivo, ios_base::out | ios_base::binary);


	// Leitura de todos os caracteres da string.
	for (int i = 0; i < text.length(); i++)
	{
		// Pega o codigo correspondente da letra.
		string code = GetCode(text[i]);

		// Le todos os caracteres (numeros) do codigo da letra.
		for (int j = 0; j < code.length(); j++)
		{
			// Converte o numero (0 e 1) de sua representa��o na tabela ascii para inteiro.
			int result = (code[j] - 48);

			binaryChar = binaryChar << 1;		// Shift para esquerda
			binaryChar = binaryChar | result; // Situa��es: binaryChar | 1 , binaryChar | 0

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

	// Caso o ultimo caractere n�o tenha sido totalmente preenchido
	if (contador < 8)
	{
		// Move a informa��o binaria adicionada para o inicio do char
		for (int i = contador; i < 8; i++)
			binaryChar = binaryChar << 1;

		// Numero de bits do texto compactado.
		nbits = compactedText.length() * 8 + contador;

		//Adiciona ultimo caractere � string.
		compactedText.operator+=(binaryChar);
	}

	int vetsize = ((nbits % 8) == 0) ? (nbits / 8) : ((nbits / 8) + 1);

	fout.write((char*)&alphabetSize, sizeof(unsigned short));
	fout.write((char*)header, sizeof(Header) * alphabetSize);
	fout.write((char*)&nbits, sizeof(unsigned short));
	fout.write(compactedText.data(), sizeof(char)* vetsize);

	cout << "\n\nArquivo compactado. Saida: " << nomeArquivo << "\n\n";

	delete[] header;
	DeleteTree(raiz);

	for (auto it : Dictionary)
		delete it;

	for (int i = 0; i < alphabetSize; i++)
		delete totalHeader[i];

	fout.close();
	system("pause");
}


void Huffman::DecodeFile()
{
	cout << "Lista de arquivos a serem Decodificados\n";
	system("dir /b *.huf"); //Lista os arquivos a serem codificados
	cout << "\n";

	char nomeArquivo[60] = { '\0' }; // Nome do arquivo limitado a 60 caracteres no m�ximo.

	cout << "Nome do arquivo que deseja decodificar: ";
	cin >> nomeArquivo;

	ifstream fin;
	fin.open(nomeArquivo, ios_base::in | ios_base::binary);

	nomeArquivo[strlen(nomeArquivo) - 3] = 't';
	nomeArquivo[strlen(nomeArquivo) - 2] = 'x';
	nomeArquivo[strlen(nomeArquivo) - 1] = 't';

	// O n�mero de s�mbolos diferentes que aparecem no texto
	alphabetSize = 0;
	fin.read((char*)&alphabetSize, sizeof(unsigned short));

	// Aloca espa�o para a informa��o do cabe�alho
	header = new Header[alphabetSize];
	fin.read((char*)header, sizeof(Header)* alphabetSize);

	// L� o n�mero de bits armazenados no arquivo e calcula tamanho do vetor
	nbits = 0;
	fin.read((char*)&nbits, sizeof(unsigned short));
	int vetsize = ((nbits % 8) == 0) ? (nbits / 8) : ((nbits / 8) + 1);

	// Aloca espa�o para os dados
	char * data = new char[vetsize];
	fin.read((char*)data, sizeof(char)* vetsize);
	fin.close();

	// Copia a informa��o dos headers comuns em headers compativeis com a cria��o da arvore
	for (int i = 0; i < alphabetSize; i++)
	{
		HeaderNo * hNo = new HeaderNo;
		hNo->esq = nullptr;
		hNo->dir = nullptr;
		hNo->a.simbolo = header[i].simbolo;
		hNo->a.freq = header[i].freq;

		// Adiciona header � fila de prioridade.
		simpleFrontier.put(hNo, header[i].freq);
	}

	cout << "\n\nArquivo decodificado: \n\n";
	char result;
	ofstream fout;
	fout.open(nomeArquivo);//Caso o arquivo txt exista essa abertura o sobreescreve com o texto decodificado
	HeaderNo * raiz = CreateTree();

	for (int i = 0; i < alphabetSize; i++)
	{
		decodedFileLenght += header[i].freq; // Determina o tamanho descompactado do arquivo
	}

	// Descompacta��o iniciada
	int i = 0;
	while (i < vetsize) // Itera��o no vetor de caracteres (data)
	{
		for (int k = 0; // O k representa o bit lido no caractere.
			k != 8 && i < vetsize; // Se o k n�o for 8 (Leitura binaria final) e n�o for a itera��o final do data
			k++)
		{
			list<char> binaryChar;

			// Converte um caractere em uma string contendo 0 e 1 correspondente.
			string text = CharToBin(data[i]);

			for (int j = 0; j < text.length(); j++)
			{
				binaryChar.push_back(text[j]); // Adiciona 0/1 em uma lista
			}

			i++; // Itera��o

				 // Caso especial: Caso especial � acionado caso um caractere que estava sendo lido n�o deu informa��o binaria suficiente.
				 // Retorna leitura a partir de uma raiz temporaria determinada quando detectada a falta de informa��o.
			if (tempRaiz != nullptr)
			{
				// Tenta determinar qual o simbolo correspondente � informa��o binaria gravada.
				result = GetSymbol(tempRaiz, &binaryChar);

				// Leitura bem sucedida.
				if (result != -1)
				{
					cout << result;
					fout << result;
					tempRaiz = nullptr; // Raiz temporaria n�o � mais necessaria.
				}
			}
			else // Caso normal: Caso normal � iniciado caso esteja na leitura inicial do arquivo ou a leitura da informa��o binaria
				 // tenha sido suficiente para o caractere anterior. 
			{
				// Tenta determinar qual o simbolo correspondente � informa��o binaria gravada.
				result = GetSymbol(raiz, &binaryChar);

				// Leitura bem sucedida.
				if (result != -1)
				{
					cout << result;
					fout << result;
				}
			}



			// Caso ainda haja informa��o binaria, continue a gerar resultados.
			while (!binaryChar.empty())
			{
				// Tenta determinar qual o simbolo correspondente � informa��o binaria gravada.
				result = GetSymbol(raiz, &binaryChar);

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
	delete[] header;
	delete[] data;

	DeleteTree(raiz);

	cout << "\n\n";
	system("pause");
}

string Huffman::CharToBin(unsigned char input) // Converte caractere em binario.
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

// Retorna a informa��o binaria correspondente do caractere c
string Huffman::GetCode(char c)
{
	for (auto it = Dictionary.begin(); it != Dictionary.end(); it++)
	{
		if ((*it)->symb == c)
			return (*it)->form;
	}
	return "";
}

// Retorna simbolo correspondente � informa��o binaria
char Huffman::GetSymbol(HeaderNo * pt, list<char> * path)
{
	// Assegura que a leitura das informa��es binarias n�o ultrapasse para o lixo do ultimo caractere.
	if (decodedFileLenght == 0)
	{
		path->clear();
		return -1;
	}

	// A informa��o binaria gravada no path n�o foi suficiente para determinar o caractere correspondente.
	if (path->empty() && pt->a.simbolo == '\0')
	{
		tempRaiz = pt;
		return -1;
	}

	char c = pt->a.simbolo;

	// Caractere correspondente encontrado.
	if (c != '\0')
	{
		decodedFileLenght--; // Um caracter foi lido.
		return c;
	}

	// Se houver informa��o binaria a ser lida, leia.
	// 0 para esquerda e 1 para direita
	if (!path->empty() && path->front() == '0')
	{
		path->pop_front();
		c = GetSymbol(pt->esq, path);
	}

	// Uma verifica��o extra (c == '\0') para caso um caractere tenha sido encontrado na chamada da esquerda.
	// Necessaria para o retorno das chamadas recursivas.
	if (!path->empty() && path->front() == '1' && c == '\0')
	{
		path->pop_front();
		c = GetSymbol(pt->dir, path);
	}

	// Retorna caractere encontrado. Esse retorno � utilizado durante as chamadas recursivas.
	return c;
}

// Gera dicionario para a codifica��o.
// Utilizado navega��o pos-ordem.
void Huffman::GenerateDictionary(HeaderNo * pt, string path)
{
	if (pt->esq != nullptr)
	{
		path.operator+=('0');
		GenerateDictionary(pt->esq, path);
		path.pop_back();
	}

	if (pt->dir != nullptr)
	{
		path.operator+=('1');
		GenerateDictionary(pt->dir, path);
		path.pop_back();
	}

	if (pt->a.simbolo != '\0')
	{
		Definition * def = new Definition; // Cria defini��o do dicionario
		def->symb = pt->a.simbolo;
		def->form = path;
		Dictionary.push_back(def); // Grava informa��o no dicionario.
	}
}

// Cria arvore.
HeaderNo * Huffman::CreateTree()
{
	HeaderNo * n1;
	HeaderNo * n2;

	// Enquanto houver mais de um elemento na fila de prioridade significa que ainda existe n�s para ser ligados.
	while (simpleFrontier.elements.size() > 1)
	{
		n1 = simpleFrontier.get(); // Pega o no com menor frequencia.
		n2 = simpleFrontier.get(); // Pega o no com menor frequencia.

		HeaderNo * no = new HeaderNo(); // Gera novo n�
		no->esq = n1;
		no->dir = n2;
		no->a.freq = n1->a.freq + n2->a.freq;
		no->a.simbolo = '\0';

		simpleFrontier.put(no, n1->a.freq + n2->a.freq); // Adiciona n� � fila de prioridade
	}

	HeaderNo * raiz = simpleFrontier.get();
	return raiz;
}


void Huffman::DeleteTree(HeaderNo * pt)  // Deleta arvore.
{
	if (pt->esq != nullptr)
		DeleteTree(pt->esq);

	if (pt->dir != nullptr)
		DeleteTree(pt->dir);

	delete pt;
}


int Huffman::CreateAlphabet(char simbolo)  // frequencias de cada s�mbolo no texto
{
	if (simbolo == '\0' || simbolo < 0)
		return -2;

	for (int i = 0; i < alphabetSize; i++)
	{
		if (totalHeader[i]->simbolo == simbolo)
		{
			totalHeader[i]->freq++;
			return -1;
		}
	}

	// Adiciona caractere c como novo simbolo.
	totalHeader[alphabetSize] = new Header();
	totalHeader[alphabetSize]->simbolo = simbolo;
	totalHeader[alphabetSize]->freq++;
	alphabetSize++;
	return 0;
}


