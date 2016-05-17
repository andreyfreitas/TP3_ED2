#include "Functions.h"
#include <iostream>
using namespace std;

int main()
{
	char opt = 0;
	while (opt != 's' && opt != 'S')
	{
		system("cls");
		cout << "______________________________________\n";
		cout << "Codificador/Decodificador de texto\n";
		cout << "______________________________________\n";

		cout << "(C)odificar\n" <<
			"(D)ecodificar\n" <<
			"(S)air\n";
		cout << "______________________________________\n";
		cout << "Escolha: ";
		cin >> opt;
		system("cls");

		switch (opt)
		{
		case 'c':
		case 'C':
			EncodeFile();
			break;
		case 'd':
		case 'D':
			DecodeFile();
			break;
		}
	}

	system("pause");
	return 0;
}



/*
#include "Functions.h"
#include <windows.h> /* Para usar as funções de busca de arquivos */
/*#include <vector>    /* Para usar */
/*#include <sstream>   /* Para usar o Stringstream */
/*#include <iostream>  /* Para manipular a entrada/saída de streams */
/*#include <fstream>   /* Para usar o ifstream */
/*
using namespace std;
vector<string> ProcurarArquivosTXT();
vector<string> ProcurarArquivosHUF();
int main()
{
	vector<string> arquivos = ProcurarArquivosTXT();
	vector<string> arquivos2 = ProcurarArquivosHUF();
	cout << arquivos[1] << endl;
	cout << arquivos2[1] << endl;

	system("pause");
	char opt = 0;
	while (opt != 's' && opt != 'S')
	{
		system("cls");
		cout << "______________________________________\n";
		cout << "Codificador/Decodificador de texto\n";
		cout << "______________________________________\n";

		cout << "(C)odificar\n" <<
			"(D)ecodificar\n" <<
			"(S)air\n";
		cout << "______________________________________\n";
		cout << "Escolha: ";
		cin >> opt;
		system("cls");

		switch (opt)
		{
		case 'c':
		case 'C':
			EncodeFile();
			break;
		case 'd':
		case 'D':
			DecodeFile();
			break;
		}
	}

	system("pause");
	return 0;
}
*/