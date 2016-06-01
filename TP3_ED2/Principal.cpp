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

		

		if (x == '1' || x == '2')
		{
			Huffman huf;
			switch (x)
			{
			case '1':
				huf.Codificador();
				
				break;
			case '2':
				huf.Decodificador();
				
				break;
			}
		}
		else if (x != '3')
		{
			cout << "Por favor digite um valor da lista abaixo. " << endl;
		}
		

	} while (x != '3');
	
	system("pause");
	return 0;
}

