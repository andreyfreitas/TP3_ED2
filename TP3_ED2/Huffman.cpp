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
	//Lista os arquivos a serem codificados
	cout << "Lista de arquivos a serem Codificados\n";
	system("dir /b *.txt"); 
	cout << "\n";
	char nomeArquivo[60] = { '\0' }; 
	cout << "Nome do arquivo a codificar: ";
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
	
	
	for (int i = 0; i < texto.length(); i++)
		Alfabeto(texto[i]); 

	no = new No[alphabetSize];

	// Ordena nos de acordo com o simbolo
	list<No> listaDeNos;
	for (int i = 0; i < alphabetSize; i++)
	{
		listaDeNos.push_back(*totalDeNos[i]);
	}
	listaDeNos.sort([](const No& first, const No& second) { return first.simbolo < second.simbolo; });
	
	auto it = listaDeNos.begin();

	for (int i = 0; i < alphabetSize; i++)
	{
		no[i] = *it;
		it++;
		NoRaiz * no = new NoRaiz();
		no->a.simbolo = no[i].a.simbolo;
		no->a.freq = no[i].a.freq;
		no->dir = nullptr;
		no->esq = nullptr;
		fp.put(no, no[i].a.freq);
	}

	// Cria arvore 
	NoRaiz * raiz = CriaArvore();

	// Cria dicionario 
	Dicionario.clear();
	CriaDicionario(raiz, "");
	int contador = 0;
	char valorBinairo = 0;
	string textoCompactado;
	wait();
	
	// Cria arquivo .huf
	ofstream fout;
	fout.open(nomeArquivo, ios_base::out | ios_base::binary);



	for (int i = 0; i < texto.length(); i++)
	{
	
		string codigoPrefixo = GetPrefixo(texto[i]);

		
		for (int j = 0; j < codigoPrefixo.length(); j++)
		{
			int valorASCII = (codigoPrefixo[j] - 48);
			valorBinairo = valorBinairo << 1;			//Shift
			valorBinairo = valorBinairo | valorASCII;	//Ou bit a bit
			contador++;

			if (contador == 8) 
			{
				textoCompactado.operator+=(valorBinairo);
				valorBinairo = 0;
				contador = 0;
			}
		}
	}

	//Complementação do ultimo caracter se necessario
	if (contador < 8)
	{
		
		for (int i = contador; i < 8; i++)
			valorBinairo = valorBinairo << 1;

		nbits = textoCompactado.length() * 8 + contador;
		textoCompactado.operator+=(valorBinairo);
	}

	int vetsize = ((nbits % 8) == 0) ? (nbits / 8) : ((nbits / 8) + 1);

	fout.write((char*)&alphabetSize, sizeof(unsigned short));
	fout.write((char*)no, sizeof(No) * alphabetSize);
	fout.write((char*)&nbits, sizeof(unsigned short));
	fout.write(textoCompactado.data(), sizeof(char)* vetsize);

	cout << "\n\nArquivo compactado. Saida: " << nomeArquivo << "\n\n";

	delete[] no;
	DeleteArvore(raiz);

	for (auto it : Dicionario)
		delete it;

	for (int i = 0; i < alphabetSize; i++)
		delete totalDeNos[i];

	fout.close();
	system("echo Pressione qualquer tecla para retornar ao menu");
	system("pause>null");
	system("cls");
}


void Huffman::Decodificador()
{
	
	//Lista os arquivos a serem codificados
	cout << "Lista de arquivos a serem Decodificados\n";
	system("dir /b *.huf"); 
	cout << "\n";
	char nomeArquivo[60] = { '\0' }; 
	cout << "Nome do arquivo a decodificar: ";
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
	wait();
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
	system("echo Pressione qualquer tecla para retornar ao menu");
	system("pause>null");
	system("cls");
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

// Cria arvore de Huffman.
NoRaiz * Huffman::CriaArvore()
{
	NoRaiz * n1;
	NoRaiz * n2;

	
	while (fp.elementos.size() > 1)
	{
		// Pega dois nos com menor frequencia para ligar.
		n1 = fp.get(); 
		n2 = fp.get(); 

		// Cria novo nó
		NoRaiz * no = new NoRaiz(); 
		no->esq = n1;
		no->dir = n2;
		no->a.freq = n1->a.freq + n2->a.freq;
		no->a.simbolo = '\0';

		// Adiciona nó à fila de prioridade
		fp.put(no, n1->a.freq + n2->a.freq); 
	}

	NoRaiz * raiz = fp.get();
	return raiz;
}


void Huffman::DeleteArvore(NoRaiz * pt)  
{
	if (pt->esq != nullptr)
		DeleteArvore(pt->esq);

	if (pt->dir != nullptr)
		DeleteArvore(pt->dir);

	delete pt;
}

// Determina frequencias de cada caractere do texto
int Huffman::Alfabeto(char simbolo)  
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

void Huffman::Wait(int waitTime)
{
	for (int i = 0; i < waitTime; i++)
	{
		continue;
	}
}

void Huffman::wait() {
	cout << "Convertendo";
	for (size_t i = 0; i < 10; i++)
	{
		Wait(100000000);
		cout << ".";
	}
	cout << endl;
}



