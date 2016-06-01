/*******************************************************************************/
//						Andrey Alves de Freitas									/
//						Marcio Bruno Ferreira de Sá								/
//									TP3											/
/*******************************************************************************/
#include "Huffman.h"

void loading();
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
	cout << endl;
	loading();
	return 0;
}

//www.vivaolinux.com.br/script/Barra-de-Loading-no-console
void loading() {
	for (int i = 0; i <= 70; i++)
	{
		printf("  %d%%\r", i + 30);
		fflush(stdout);

		for (int j = 0; j < i; j++)
		{
			if (!j) 
				printf(" ");

			printf("%c", 219);
			Sleep(1);
		}
	}

	printf("\n\nFinalizando...");
	Sleep(1000);
	printf("\r \t\t\t\t  FINALIZADO\n\n\n");
	printf("\t\t  Autores: Andrey Freitas & Marcio Bruno \n\n\n\n");
	Sleep(2500);
	system(" ");
}
