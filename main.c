#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<windows.h>
#include<conio.h>
#include<locale.h>

#include "funcs.h"

#define ARQPASTADBS "C:/dbs/temp.txt"

int main(){
	setlocale(LC_ALL, "Portuguese");

    //inicio cria pasta dos arquivos txt
    FILE* arq = fopen(ARQPASTADBS, "ab");
    if(arq == NULL) mkdir("c:/dbs");
    fclose(arq);
    remove(ARQPASTADBS);
    //fim cria pasta

	menuLogadoOff();

	return 0;
}
