/*******************************************************************************/
//						Andrey Alves de Freitas									/
//						Marcio Bruno Ferreira de Sá								/
//									TP3											/
/*******************************************************************************/
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
		NoRaiz * node = new NoRaiz();
		node->a.simbolo = no[i].simbolo;
		node->a.freq = no[i].freq;
		node->dir = nullptr;
		node->esq = nullptr;
		fp.put(node, no[i].freq);
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

	
	alphabetSize = 0;
	fin.read((char*)&alphabetSize, sizeof(unsigned short));
	no = new No[alphabetSize];
	fin.read((char*)no, sizeof(No)* alphabetSize);
	nbits = 0;
	fin.read((char*)&nbits, sizeof(unsigned short));
	int vetsize = ((nbits % 8) == 0) ? (nbits / 8) : ((nbits / 8) + 1);
	char * data = new char[vetsize];
	fin.read((char*)data, sizeof(char)* vetsize);
	fin.close();

	for (int i = 0; i < alphabetSize; i++)
	{
		NoRaiz * node = new NoRaiz;
		node->esq = nullptr;
		node->dir = nullptr;
		node->a.simbolo = no[i].simbolo;
		node->a.freq = no[i].freq;
		fp.put(node, no[i].freq);
	}
	wait();
	cout << "\n\nTexto contido no arquivo: \n\n";
	char result;
	ofstream fout;
	fout.open(nomeArquivo);
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
	cout << "\n\nNovo arquivo decodificado: " << nomeArquivo << " !\n";
	fout.close();
	delete[] no;
	delete[] data;

	DeleteArvore(raiz);

	cout << "\n\n";
	system("echo Pressione qualquer tecla para retornar ao menu");
	system("pause>null");
	system("cls");
}

// Converte caractere em binario.
string Huffman::ConversorBinario(unsigned char input) 
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


string Huffman::GetPrefixo(char c)
{
	for (auto it = Dicionario.begin(); it != Dicionario.end(); it++)
	{
		if ((*it)->simbolo == c)
			return (*it)->codigo;
	}
	return "";
}

char Huffman::GetSimbolo(NoRaiz * pt, list<char> * caminho)
{
	if (TamArquivoDecodificado == 0)
	{
		caminho->clear();
		return -1;
	}

	if (caminho->empty() && pt->a.simbolo == '\0')
	{
		tempRaiz = pt;
		return -1;
	}

	char c = pt->a.simbolo;

	if (c != '\0')
	{
		TamArquivoDecodificado--; 
		return c;
	}

	if (!caminho->empty() && caminho->front() == '0')
	{
		caminho->pop_front();
		c = GetSimbolo(pt->esq, caminho);
	}

	if (!caminho->empty() && caminho->front() == '1' && c == '\0')
	{
		caminho->pop_front();
		c = GetSimbolo(pt->dir, caminho);
	}

	return c;
}


void Huffman::CriaDicionario(NoRaiz * pt, string caminho)
{
	if (pt->esq != nullptr)
	{
		caminho.operator+=('0');
		CriaDicionario(pt->esq, caminho);
		caminho.pop_back();
	}

	if (pt->dir != nullptr)
	{
		caminho.operator+=('1');
		CriaDicionario(pt->dir, caminho);
		caminho.pop_back();
	}

	if (pt->a.simbolo != '\0')
	{
		Chave * definicao = new Chave;
		definicao->simbolo = pt->a.simbolo;
		definicao->codigo = caminho;
		Dicionario.push_back(definicao);
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

	totalDeNos[alphabetSize] = new No();
	totalDeNos[alphabetSize]->simbolo = simbolo;
	totalDeNos[alphabetSize]->freq++;
	alphabetSize++;
	return 0;
}


void Huffman::wait() {
	cout << "Convertendo";
	for (size_t i = 0; i < 10; i++)
	{
		Sleep(100);
		cout << ".";
	}
	cout << endl;
}



