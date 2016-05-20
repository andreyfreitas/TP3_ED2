#include "Huffman.h"

int main()
{
	char x; //Variavel de opção
	do
	{
		cout << "------------------------------------\n";
		cout << "Codificador e Decodificador Huffman\n";
		cout << "------------------------------------\n";

		cout << "1 - Codificar\n" <<
				"2 - Decodificar\n" <<
				"3 - Sair\n";
		cout << "------------------------------------\n";
		cout << "Escolha: ";
		cin >> x;
		system("cls");

		Huffman huf;

		if (x == '1' || x == '2' || x == '3')
		{
			switch (x)
			{
			case '1':
				huf.EncodeFile();
				break;
			case '2':
				huf.DecodeFile();
				break;
			}
		}
		else
		{
			cout << "Por favor digite um valor da lista abaixo. " << endl;
		}
		

	} while (x != '3');
	
	system("pause");
	return 0;
}

