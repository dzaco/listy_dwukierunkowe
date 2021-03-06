// lab07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <stdlib.h>

struct pomiar {
	unsigned int nr_pomiaru;
	unsigned int nr_czujnika;
	char data_i_czas[20];
	float temp;
	struct pomiar *nast;
	struct pomiar *poprz;
};

struct podzial
{
	struct pomiar * glowa1;
	struct pomiar * glowa2;
	struct pomiar * glowa3;
	struct pomiar * glowa4;
};

void new_list(pomiar*& head, pomiar*& wsk, pomiar*& n)
{
	if (n == NULL) {
		n = head;
		wsk = n;
		//maxTemp = wsk->temp;
		wsk->poprz = NULL;
	}
	else {
		wsk->poprz = wsk->nast;
		wsk->nast = head;
		wsk = wsk->nast;
		/*if (wsk->temp > maxTemp) {
			maxTemp = head->temp;
		}*/
	}
	head = head->nast;
	//pom++;
}

pomiar * open_and_save( char nazwa[20] )
{
	FILE * stream;
	errno_t err;

	err = fopen_s(&stream, nazwa , "r");

	if (err != 0)
	{
		printf("Nie mozna otworzyc pliku %s\n", nazwa);
		exit( 0 );
	}

	//zapisujemy do listy dynamicznej dwukierunkowej
	
	pomiar * glowa = NULL;
	pomiar * wsk = NULL;
	int ile_rekordow = 0;

	unsigned int pom_nr_pomiaru;
	unsigned int pom_nr_czujnika;
	char pom_data_i_czas[20];
	float pom_temp;

	fscanf_s(stream, "%d", &pom_nr_pomiaru);
	fscanf_s(stream, "%d", &pom_nr_czujnika);
	fscanf_s(stream, "%s", pom_data_i_czas, 20);
	fscanf_s(stream, "%f", &pom_temp);

	while ( !feof(stream) )
	{
		ile_rekordow++;
		if ( glowa == NULL )
		{
			glowa = wsk = new pomiar;
			glowa->poprz = NULL;
		}
		else
		{
			wsk->nast = new pomiar;
			wsk->nast->poprz = wsk;
			wsk = wsk->nast;
			wsk->nast = NULL;
		}
		wsk->nr_pomiaru = pom_nr_pomiaru;
		wsk->nr_czujnika = pom_nr_czujnika;
		strcpy_s( wsk->data_i_czas , pom_data_i_czas);
		wsk->temp = pom_temp;

		fscanf_s(stream, "%d", &pom_nr_pomiaru);
		fscanf_s(stream, "%d", &pom_nr_czujnika);
		fscanf_s(stream, "%s", pom_data_i_czas, 20);
		fscanf_s(stream, "%f", &pom_temp);		
	}

	fclose( stream );

	return glowa;
}

podzial separate_list( pomiar * glowa )
{
	pomiar * wsk = NULL;
	pomiar * usuw = NULL;
	pomiar * glowa1 = NULL;
	pomiar * glowa2 = NULL;
	pomiar * glowa3 = NULL;
	pomiar * glowa4 = NULL;
	pomiar * wsk1 = NULL;
	pomiar * wsk2 = NULL;
	pomiar * wsk3 = NULL;
	pomiar * wsk4 = NULL;

	wsk = glowa;

	while (glowa->nast != NULL) {
		switch (glowa->nr_czujnika) {
		case 1:
			new_list(glowa, wsk1, glowa1);
			break;
		case 2:
			new_list(glowa, wsk2, glowa2);
			break;
		case 3:
			new_list(glowa, wsk3, glowa3);
			break;
		case 4:
			new_list(glowa, wsk4, glowa4);
			break;
		default:
			break;
		}
	}
	wsk1->nast = NULL;
	wsk2->nast = NULL;
	wsk3->nast = NULL;
	wsk4->nast = NULL;

	
	/*wsk = glowa1;
	printf("%d\t%d\t%s\t%.2f\n", wsk->nr_pomiaru, wsk->nr_czujnika, wsk->data_i_czas, wsk->temp);
	while (wsk->nast != NULL)
	{
		wsk = wsk->nast;
		printf("%d\t%d\t%s\t%.2f\n", wsk->nr_pomiaru, wsk->nr_czujnika, wsk->data_i_czas, wsk->temp);
	}*/


	podzial p;
	p.glowa1 = glowa1;
	p.glowa2 = glowa2;
	p.glowa3 = glowa3; 
	p.glowa4 = glowa4;

	return p;
}

int ile_rekordow( pomiar * glowa )
{
	int ile = 1;
	pomiar * wsk = NULL;
	wsk = glowa;

	while ( wsk->nast != NULL )
	{
		wsk = wsk->nast;
		ile++;
	}

	return ile;
}

pomiar * max_temp( pomiar * glowa )
{
	pomiar * wsk = NULL;
	wsk = glowa;
	float max;
	max = glowa->temp;
	int pozycja = glowa->nr_pomiaru;

	while (wsk->nast != NULL)
	{
		if (wsk->temp > max)
		{
			pozycja = wsk->nr_pomiaru;
			max = wsk->temp;
		}
		wsk = wsk->nast;
	}

	wsk = glowa;
	while (wsk->nast != NULL)
	{
		if (wsk->nr_pomiaru == pozycja)
			break;
		
		wsk = wsk->nast;
	}

	return wsk;

}

pomiar * min_temp( pomiar * glowa)
{
	pomiar * wsk = NULL;
	wsk = glowa;
	float max;
	max = glowa->temp;
	int pozycja = glowa->nr_pomiaru;

	while (wsk->nast != NULL)
	{
		if (wsk->temp < max)
		{
			pozycja = wsk->nr_pomiaru;
			max = wsk->temp;
		}
		wsk = wsk->nast;
	}

	wsk = glowa;
	while (wsk->nast != NULL)
	{
		if (wsk->nr_pomiaru == pozycja)
			break;

		wsk = wsk->nast;
	}

	return wsk;
}

void dealloc( pomiar * wsk )
{
	pomiar * usuw;
	while( wsk != NULL )
	{
		usuw = wsk;
		wsk = usuw->nast;
		delete(usuw);
	}
}

void line()
{
	for (int i = 0; i < 40; i++)
	{
		printf("-+-");
	}
	printf("\n");
}

//void creat_name( char nazwa[20] , int ilosc_plikow)
//{
//	int length = strlen( nazwa );
//
//	if ( strspn( nazwa , ".txt") )
//	{
//		for (int i = 0; i < ilosc_plikow; i++)
//		{
//			char nr = (char)i;
//			strcat_s( *nazwa[length - 3] , 20, &nr );
//			printf("%s\n" , nazwa );
//		}
//	}
//}

int main()
{
	char nazwa[20];

	printf("Podaj nazwe pliku do odczytu:\n");
	strcpy_s( nazwa , "tmax.txt" ); //scanf_s("%s",nazwa, 20);

	//1
	pomiar * glowa = NULL;
	glowa = open_and_save( nazwa );

	
	//2
	podzial p;
	p = separate_list( glowa );

	//3
	printf("Czujnik 1\n");
	printf("Ilosc rekordow : %d\n", ile_rekordow( p.glowa1 ) );
	printf("Najwieksza temperatura : %.2f\n", max_temp(p.glowa1)->temp );
	printf("Najnizsza temperatura : %.2f\n" , min_temp(p.glowa1)->temp );
	line();

	printf("Czujnik 2\n");
	printf("Ilosc rekordow : %d\n", ile_rekordow(p.glowa2));
	printf("Najwieksza temperatura : %.2f\n", max_temp(p.glowa2)->temp);
	printf("Najnizsza temperatura : %.2f\n", min_temp(p.glowa2)->temp);
	line();

	printf("Czujnik 3\n");
	printf("Ilosc rekordow : %d\n", ile_rekordow(p.glowa3));
	printf("Najwieksza temperatura : %.2f\n", max_temp(p.glowa3)->temp);
	printf("Najnizsza temperatura : %.2f\n", min_temp(p.glowa3)->temp);
	line();

	printf("Czujnik 4\n");
	printf("Ilosc rekordow : %d\n", ile_rekordow(p.glowa4));
	printf("Najwieksza temperatura : %.2f\n", max_temp(p.glowa4)->temp);
	printf("Najnizsza temperatura : %.2f\n", min_temp(p.glowa4)->temp);
	line();

	//4
	dealloc(p.glowa1);
	dealloc(p.glowa2);
	dealloc(p.glowa3);
	dealloc(p.glowa4);

    return 0;
}

