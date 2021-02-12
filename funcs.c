#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<windows.h>
#include<conio.h>
#include<time.h>

#include "funcs.h"

#define SIZE 30

#define ARQPACIENTES "c:/dbs/pacientes.txt"
#define ARQFUNCIONARIOS "c:/dbs/funcionarios.txt"
#define ARQMEDICOS "c:/dbs/medicos.txt"
#define ARQAGEND "c:/dbs/agendamentos.txt"

int logado = 0;

Data getDataAtual(){
    time_t data;
    time(&data);

    struct tm *dataAtual = localtime(&data);

    Data dataHoje;
    dataHoje.dd = dataAtual->tm_mday;
    dataHoje.mm = dataAtual->tm_mon-1;
    dataHoje.yy = dataAtual->tm_year+1900;

    return dataHoje;
}

int validaData(Data dt){
    int diasMax, bissexto = 0;

    if(dt.yy % 4 == 0){
        if(dt.yy % 100 != 0){
            bissexto = 1;
        }else if(dt.yy % 400 == 0){
            bissexto = 1;
        }else{
            bissexto = 0;
        }
    }

    if(dt.dd < 1 || dt.mm < 1 || dt.mm > 12 || dt.yy < 1900){
        return 0;
    }else if(dt.mm == 1 || dt.mm == 3 || dt.mm == 5 || dt.mm == 7 || dt.mm == 8 || dt.mm == 10 || dt.mm == 12){
        diasMax = 31;
        if(dt.dd > diasMax) return 0;
    }else if(dt.mm == 4 || dt.mm == 6 ||dt.mm == 9 || dt.mm == 11){
        diasMax = 30;
        if(dt.dd > diasMax) return 0;
    }else if(dt.mm == 2){
        if(bissexto == 1) diasMax = 29;
        else diasMax = 28;
        if(dt.dd > diasMax) return 0;
    }

    return 1;
}

int validaDataNascimento(Data dt){
    int diasMax, bissexto = 0;
    //week days: segunda 1, terça 2, quarta 3, quinta 4, sexta 5

    time_t data_ano;
    time(&data_ano);

    struct tm *dataAtual = localtime(&data_ano);

    if(dt.yy % 4 == 0){
        if(dt.yy % 100 != 0){
            bissexto = 1;
        }else if(dt.yy % 400 == 0){
            bissexto = 1;
        }else{
            bissexto = 0;
        }
    }

    if(dt.yy > dataAtual->tm_year + 1900){
        return 0;
    }else if(dt.yy == dataAtual->tm_year+1900){
        if(dt.mm > dataAtual->tm_mon+1){
            return 0;
        }else if(dt.mm == dataAtual->tm_mon+1){
            if(dt.dd > dataAtual->tm_mday){
                return 0;
            }
        }
    }

    if(dt.dd < 1 || dt.mm < 1 || dt.mm > 12 || dt.yy < 1900){
        return 0;
    }else if(dt.mm == 1 || dt.mm == 3 || dt.mm == 5 || dt.mm == 7 || dt.mm == 8 || dt.mm == 10 || dt.mm == 12){
        diasMax = 31;
        if(dt.dd > diasMax) return 0;
    }else if(dt.mm == 4 || dt.mm == 6 ||dt.mm == 9 || dt.mm == 11){
        diasMax = 30;
        if(dt.dd > diasMax) return 0;
    }else if(dt.mm == 2){
        if(bissexto == 1) diasMax = 29;
        else diasMax = 28;
        if(dt.dd > diasMax) return 0;
    }

    return 1;
}

int validaDataAgendamento(Data dt, Hora hr){
    int diasMax, bissexto = 0;

    time_t data_ano;
    time(&data_ano);

    struct tm *dataAtual = localtime(&data_ano);

    if(dt.yy % 4 == 0){
        if(dt.yy % 100 != 0){
            bissexto = 1;
        }else if(dt.yy % 400 == 0){
            bissexto = 1;
        }else{
            bissexto = 0;
        }
    }

    if(dt.yy < dataAtual->tm_year + 1900){
        return 0;
    }else if(dt.yy == dataAtual->tm_year+1900){
        if(dt.mm < dataAtual->tm_mon+1){
            return 0;
        }else if(dt.mm == dataAtual->tm_mon+1){
            if(dt.dd < dataAtual->tm_mday){
                return 0;
            }else if(dt.dd == dataAtual->tm_mday){
                if((hr.hh - 3) < dataAtual->tm_hour){
                    return 0;
                }
            }
        }
    }

    if(dt.dd < 1 || dt.mm < 1 || dt.mm > 12 || dt.yy < 1900){
        return 0;
    }else if(dt.mm == 1 || dt.mm == 3 || dt.mm == 5 || dt.mm == 7 || dt.mm == 8 || dt.mm == 10 || dt.mm == 12){
        diasMax = 31;
        if(dt.dd > diasMax) return 0;
    }else if(dt.mm == 4 || dt.mm == 6 ||dt.mm == 9 || dt.mm == 11){
        diasMax = 30;
        if(dt.dd > diasMax) return 0;
    }else if(dt.mm == 2){
        if(bissexto == 1) diasMax = 29;
        else diasMax = 28;
        if(dt.dd > diasMax) return 0;
    }

    if(hr.hh < 8 || hr.hh == 12 || hr.hh == 13 || hr.hh > 17){
        return 0;
    }else if(hr.mm != 0 && hr.mm != 30){
        return 0;
    }

    return 1;
}

int validaCPF(char cpf[13]){
    int priVerificador, segVerificador, priVerifDigitado, segVerifDigitado;
    int soma1 = 0, soma2 = 0, mod1, mod2, j;
    int valores[11];

    for(int i = 0; i < 11; i++){
        valores[i] = cpf[i] - '0';
    }

    priVerificador = valores[9];
    segVerificador = valores[10];

    j = 10;
    for(int i = 0; i < 9; i++){
        soma1 = soma1 + (valores[i] * j);
        j--;
    }
    j = 11;
    for(int i = 0; i < 10; i++){
        soma2 = soma2 + (valores[i] * j);
        j--;
    }

    mod1 = soma1 % 11;
    mod2 = soma2 % 11;

    mod1 = 11 - mod1;
    mod2 = 11 - mod2;

    if(mod1 > 10) priVerifDigitado = 0;
    else priVerifDigitado = mod1;

    if(mod2 > 10) segVerifDigitado = 0;
    else segVerifDigitado = mod2;

    if(priVerifDigitado == priVerificador && segVerifDigitado == segVerificador){
        return 1;
    }else return 0;
}

void menuLogadoOff(){
    int arqSizeFun;
    FILE* arq;

    arq = fopen(ARQFUNCIONARIOS, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeFun = ftell(arq);
    fclose(arq);

    system("cls");
	printf("=================================YAMOTO============================\n");
	printf("===============================BEM-VINDO============================\n");
	printf("\nEscolha a opção desejada:\n\n");
	if(arqSizeFun > 0) printf("1 - Login\n");
    printf("2 - Cadastrar\n");
	printf("\n9 - Sair\n");

	char opcao;
	opcao = getch();
	fflush(stdin);

	switch(opcao) {
        case '1':
            login();
            break;
		case '2':
			cadastroLogadoOff();
			break;
		case '9':
            system("cls");
			printf("==============YAMOTO===========\n");
			printf("\nVolte sempre\n");
			Sleep(3);
			exit(0);
		default:
			printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
			getch();
			fflush(stdin);
			menuLogadoOff();
			break;

	}
}

void menuLogado(){
	int arqSizePac, arqSizeMed, arqSizeFun, arqSizeAge;
    FILE* arq;

    arq = fopen(ARQPACIENTES, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizePac = ftell(arq);
    fclose(arq);

    arq = fopen(ARQMEDICOS, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeMed = ftell(arq);
    fclose(arq);

    arq = fopen(ARQFUNCIONARIOS, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeFun = ftell(arq);
    fclose(arq);

    arq = fopen(ARQAGEND, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeAge = ftell(arq);
    fclose(arq);

	system("cls");

	printf("=================================YAMOTO============================\n");
	printf("=============================BEM - VINDO============================\n");
	printf("\nSelecione a opção desejada\n\n");
    printf("1 - Cadastrar\n");
	if(arqSizeAge > 0 || arqSizeFun > 0 || arqSizeMed > 0 || arqSizePac > 0) printf("2 - Consulta\n");
	if(arqSizeAge > 0 || arqSizeFun > 0 || arqSizeMed > 0 || arqSizePac > 0) printf("3 - Alterar Informações\n");
	if(arqSizeAge > 0 || arqSizeFun > 0 || arqSizeMed > 0 || arqSizePac > 0) printf("4 - Excluir Cadastros\n");
	if(arqSizeAge > 0) printf("5 - Relatórios\n");
	printf("\n9 - Sair\n");

	char opcao;
	opcao = getch();
	fflush(stdin);

	switch(opcao) {
		case '1':
			cadastroLogado();
			break;
		case '2':
			menuConsultas();
			break;
        case '3':
            menuAlterarInfos();
		case '4':
			menuExcluir();
			break;
        case '5':
            menuRelatorios();
            break;
		case '9':
			system("cls");
			printf("==============YAMOTO===========\n");
			printf("\nVolte sempre\n");
			Sleep(2);
			exit(0);
		default:
			printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
			getch();
			fflush(stdin);
			menuLogado();
			break;

	}
}

void login(){
    FILE* arq;
	Funcionario *acc_list, funConsulta;
	int acc_total = 0, arq_size = 0, i;

	memset(&funConsulta, 0, sizeof(funConsulta));

	arq = fopen(ARQFUNCIONARIOS, "rb");

    system("cls");
	printf("\t-----------YAMOTO-------------\n");
	printf("\nCPF: ");
	fgets(funConsulta.cpf, 13, stdin);
	funConsulta.cpf[strcspn(funConsulta.cpf, "\n")] = 0;
	fflush(stdin);

	printf("Senha: ");
	fgets(funConsulta.senha, SIZE, stdin);
	funConsulta.senha[strcspn(funConsulta.senha, "\n")] = 0;
	fflush(stdin);

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arq, 0, SEEK_END);
		arq_size = ftell(arq);
		acc_total = arq_size / sizeof(funConsulta);
		rewind(arq);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(funConsulta), acc_total, arq);
		fclose(arq);

		for(i = 0; i < acc_total; i++){
            if(strcmp(funConsulta.cpf, acc_list[i].cpf) == 0){
                if(strcmp(funConsulta.senha, acc_list[i].senha) == 0){
                    strcpy(funConsulta.nome, acc_list[i].nome);
                    logado = 1;
                    break;
                }
            }
		}
        free(acc_list);
	}

	if(logado == 0){
        printf("\nUsuário ou senha incorreto(s).");
        getch();
        fflush(stdin);
        menuLogadoOff();
	}
    else if (logado == 1){
        printf("\nOlá, %s! Pressione qualquer tecla para continuar...", funConsulta.nome);
    }

    getch();
    fflush(stdin);
    menuLogado();
}

void cadastroLogado(){
	system("cls");
	printf("\n\t\tYAMOTO - Cadastros\n\n");
	printf("\t==========================\n");

	printf("\nDigite a opção desejada:\n\n");
	printf("1 - Pacientes\n");
    printf("2 - Médicos\n");
	printf("3 - Funcionários\n");
	printf("4 - Agendamentos\n");
	printf("\n9 - Voltar ao menu\n");

	char opcao;
	opcao = getch();
	fflush(stdin);;

	switch(opcao) {
		case '1':
			cadastrarPaciente();
			break;
		case '2':
			cadastrarMedico();
			break;
		case '3':
			cadastrarFuncionarios();
			break;
		case '4':
			cadastrarAgendamento();
			break;
		case '9':
			logado != 0 ? menuLogado():menuLogadoOff();
			break;
	  	default:
	   		printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
			getch();
			fflush(stdin);
			cadastroLogado();
			break;
    }
}

void cadastroLogadoOff(){
	system("cls");
	printf("\n\t\tYAMOTO - Cadastros\n\n");
	printf("\t==========================\n");

	printf("\nDigite a opção desejada:\n\n");
	printf("1 - Funcionários\n");
	printf("\n9 - Voltar ao menu\n");

	char opcao;
	opcao = getch();
	fflush(stdin);;

	switch(opcao) {
		case '1':
			cadastrarFuncionarios();
			break;
		case '9':
			logado != 0 ? menuLogado():menuLogadoOff();
			break;
	  	default:
	   		printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
			getch();
			fflush(stdin);
			cadastroLogadoOff();
			break;
    }
}

void cadastrarPaciente(){
	FILE* arq;
	Paciente user_cad;
	int validacaoData = 0, validacaoCPF = 0;

	arq = fopen(ARQPACIENTES, "ab");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		memset(&user_cad, 0, sizeof(user_cad));

		do{
            system("cls");
            printf("\n\t\tYAMOTO Cadastro de pacientes\n\n");
            printf("\t===========================================\n\n");

            printf("CPF: ");
            fgets(user_cad.cpf, 13, stdin);
            user_cad.cpf[strcspn(user_cad.cpf, "\n")] = 0;
            fflush(stdin);

            validacaoCPF = validaCPF(user_cad.cpf);

            if(!validacaoCPF){
                printf("CPF inválido. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                char a;
                do{
                    a = getch();
                    fflush(stdin);
                    if(a == 32){
                        fclose(arq);
                        menuLogado();
                    }
                }while(a != 32 && a != 13);
            }

		}while(validacaoCPF == 0);

		printf("Nome Completo: ");
		fgets(user_cad.nome, SIZE, stdin);
		user_cad.nome[strcspn(user_cad.nome, "\n")] = 0;
		fflush(stdin);

		do{
            system("cls");
            printf("\n\t\tYAMOTO Cadastro de pacientes\n\n");
            printf("\t===========================================\n");

            printf("CPF: %s\n", user_cad.cpf);
            printf("Nome Completo: %s\n", user_cad.nome);

            printf("Data de Nascimento (DD/MM/AAAA): ");
            scanf("%d/%d/%d", &user_cad.dataNascimento.dd, &user_cad.dataNascimento.mm, &user_cad.dataNascimento.yy);
            fflush(stdin);

            validacaoData = validaDataNascimento(user_cad.dataNascimento);
            if(validacaoData == 0){
                printf("\nData inválida! Pressione qualquer tecla para tentar novamente...");
                getch();
                fflush(stdin);
            }
		}while(validacaoData == 0);

		printf("E-mail: ");
		fgets(user_cad.email, SIZE, stdin);
		user_cad.email[strcspn(user_cad.email, "\n")] = 0;
		fflush(stdin);

		printf("Endereço: ");
		fgets(user_cad.endereco, SIZE, stdin);
		user_cad.endereco[strcspn(user_cad.endereco, "\n")] = 0;
		fflush(stdin);

		do{
            printf("Unidade: ");
            scanf("%d", &user_cad.unidade);
            fflush(stdin);

            if(user_cad.unidade < 1 || user_cad.unidade > 3){
                printf("\nUnidade inválida, escolha apenas uma entre as unidades 1, 2 e 3. Pressione qualquer tecla para tentar novamente...\n\n");
                getch();
                fflush(stdin);
            }
		}while(user_cad.unidade < 1 || user_cad.unidade > 3);

		user_cad.freqUnd1 = 0;
		user_cad.freqUnd2 = 0;
		user_cad.freqUnd3 = 0;
		user_cad.valorTotalGasto = 0;

		fwrite(&user_cad, sizeof user_cad, 1, arq);
		fclose(arq);

		printf("\nUsuário cadastrado com sucesso. Pressione qualquer tecla para continuar...");
	}
	getch();
	fflush(stdin);
	logado != 0 ? menuLogado():menuLogadoOff();
}

void cadastrarMedico(){
    int validacaoData = 0, validacaoCPF = 0;

	FILE* arq;
	Medico user_cad;

    arq = fopen(ARQMEDICOS, "ab");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		memset(&user_cad, 0, sizeof(user_cad));

        do{
            system("cls");
            printf("\n\t\tYAMOTO Cadastro de Médicos\n\n");
            printf("\t=========================================\n");

            printf("\nCPF: ");
            fgets(user_cad.cpf, 13, stdin);
            user_cad.cpf[strcspn(user_cad.cpf, "\n")] = 0;
            fflush(stdin);

            validacaoCPF = validaCPF(user_cad.cpf);

            if(!validacaoCPF){
                printf("CPF inválido. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                char a;
                do{
                    a = getch();
                    fflush(stdin);
                    if(a == 32){
                        fclose(arq);
                        menuLogado();
                    }
                }while(a != 32 && a != 13);
            }
        }while(validacaoCPF == 0);

		printf("Senha: ");
		fgets(user_cad.senha, SIZE, stdin);
		user_cad.senha[strcspn(user_cad.senha, "\n")] = 0;
		fflush(stdin);

		printf("Nome Completo: ");
		fgets(user_cad.nome, SIZE, stdin);
		user_cad.nome[strcspn(user_cad.nome, "\n")] = 0;
		fflush(stdin);

		do{
            system("cls");
            printf("\n\t\tYAMOTO Cadastro de Médicos\n\n");
            printf("\t=========================================\n");

            printf("CPF: %s\n", user_cad.cpf);
            printf("Senha: %s\n", user_cad.senha);
            printf("Nome Completo: %s\n", user_cad.nome);

            printf("Data de Nascimento (DD/MM/AAAA): ");
            scanf("%d/%d/%d", &user_cad.dataNascimento.dd, &user_cad.dataNascimento.mm, &user_cad.dataNascimento.yy);
            fflush(stdin);

            validacaoData = validaDataNascimento(user_cad.dataNascimento);
            if(validacaoData == 0){
                printf("\nData inválida! Pressione qualquer tecla para tentar novamente...");
                getch();
                fflush(stdin);
            }
		}while(validacaoData == 0);

		printf("E-mail: ");
		fgets(user_cad.email, SIZE, stdin);
		user_cad.email[strcspn(user_cad.email, "\n")] = 0;
		fflush(stdin);

		do{
            printf("Unidade: ");
            scanf("%d", &user_cad.unidade);
            fflush(stdin);

            if(user_cad.unidade < 1 || user_cad.unidade > 3){
                printf("\nUnidade inválida, escolha apenas uma entre as unidades 1, 2 e 3. Pressione qualquer tecla para tentar novamente...\n\n");
                getch();
                fflush(stdin);
            }
		}while(user_cad.unidade < 1 || user_cad.unidade > 3);

		printf("CRM: ");
		fgets(user_cad.crm, SIZE, stdin);
		user_cad.crm[strcspn(user_cad.crm, "\n")] = 0;
		fflush(stdin);

		printf("Especialidade: ");
		fgets(user_cad.especialidade, SIZE, stdin);
		user_cad.especialidade[strcspn(user_cad.especialidade, "\n")] = 0;
		fflush(stdin);

		fwrite(&user_cad, sizeof user_cad, 1, arq);
		fclose(arq);

		printf("\nMédico cadastrado com sucesso. Pressione qualquer tecla para continuar...");
	}
    getch();
    fflush(stdin);
	logado != 0 ? menuLogado():menuLogadoOff;
}

void cadastrarFuncionarios(){
	FILE* arq;
	Funcionario user_cad;
	int validacaoData = 0, validacaoCPF = 0;

    arq = fopen(ARQFUNCIONARIOS, "ab");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		memset(&user_cad, 0, sizeof(user_cad));

		do{
            system("cls");
            printf("\n\t\tYAMOTO Cadastro de Funcionários\n\n");
            printf("\t==============================================\n");

            printf("\nCPF: ");
            fgets(user_cad.cpf, 13, stdin);
            user_cad.cpf[strcspn(user_cad.cpf, "\n")] = 0;
            fflush(stdin);

            validacaoCPF = validaCPF(user_cad.cpf);

            if(!validacaoCPF){
                printf("CPF inválido. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                char a;
                do{
                    a = getch();
                    fflush(stdin);
                    if(a == 32){
                        fclose(arq);
                        menuLogado();
                    }
                }while(a != 32 && a != 13);
            }
        }while(validacaoCPF == 0);

		printf("Senha: ");
		fgets(user_cad.senha, SIZE, stdin);
		user_cad.senha[strcspn(user_cad.senha, "\n")] = 0;
		fflush(stdin);

		printf("Nome Completo: ");
		fgets(user_cad.nome, SIZE, stdin);
		user_cad.nome[strcspn(user_cad.nome, "\n")] = 0;
		fflush(stdin);

		do{
            system("cls");
            printf("\n\t\tYAMOTO Cadastro de Funcionários\n\n");
            printf("\t=========================================\n");

            printf("CPF: %s\n", user_cad.cpf);
            printf("Senha: %s\n", user_cad.senha);
            printf("Nome Completo: %s\n", user_cad.nome);

            printf("Data de Nascimento (DD/MM/AAAA): ");
            scanf("%d/%d/%d", &user_cad.dataNascimento.dd, &user_cad.dataNascimento.mm, &user_cad.dataNascimento.yy);
            fflush(stdin);

            validacaoData = validaDataNascimento(user_cad.dataNascimento);
            if(validacaoData == 0){
                printf("\nData inválida! Pressione qualquer tecla para tentar novamente...");
                getch();
                fflush(stdin);
            }
		}while(validacaoData == 0);

		printf("E-mail: ");
		fgets(user_cad.email, SIZE, stdin);
		user_cad.email[strcspn(user_cad.email, "\n")] = 0;
		fflush(stdin);

		do{
            printf("Unidade: ");
            scanf("%d", &user_cad.unidade);
            fflush(stdin);

            if(user_cad.unidade < 1 || user_cad.unidade > 3){
                printf("\nUnidade inválida, escolha apenas uma entre as unidades 1, 2 e 3. Pressione qualquer tecla para tentar novamente...\n\n");
                getch();
                fflush(stdin);
            }
		}while(user_cad.unidade < 1 || user_cad.unidade > 3);

		printf("Departamento: ");
		fgets(user_cad.departamento, SIZE, stdin);
		user_cad.departamento[strcspn(user_cad.departamento, "\n")] = 0;
		fflush(stdin);

		fwrite(&user_cad, sizeof user_cad, 1, arq);
		fclose(arq);

		printf("\nFuncionário cadastrado com sucesso. Pressione qualquer tecla para continuar...");
	}
	getch();
	fflush(stdin);
	logado != 0 ? menuLogado():menuLogadoOff();
}

void cadastrarAgendamento(){
	FILE* arq;
	Agendamento *age_list, ageCad;
	Paciente *pac_list, paciConsulta;
	Medico *med_list, medConsulta;
	int acc_total = 0, arq_size = 0, i, paciExists = 0, medExists = 0, ageExists = 0, validacaoData = 0;

	memset(&ageCad, 0, sizeof(ageCad));

    arq = fopen(ARQAGEND, "ab");
    fclose(arq);
    //início busca paciente
    arq = fopen(ARQPACIENTES, "rb");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
        memset(&paciConsulta, 0, sizeof(paciConsulta));
        fseek(arq, 0, SEEK_END);
        arq_size = ftell(arq);
        acc_total = arq_size / sizeof(paciConsulta);
        rewind(arq);
        pac_list = malloc(arq_size);
        fread(pac_list, sizeof(paciConsulta), acc_total, arq);
        do{
            system("cls");
            printf("\n\t\tYAMOTO - Agendamento de Consulta\n\n");
            printf("\t====================================================\n");
            printf("\nCPF do Paciente: ");
            fgets(paciConsulta.cpf, 13, stdin);
            paciConsulta.cpf[strcspn(paciConsulta.cpf, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(paciConsulta.cpf, pac_list[i].cpf) == 0){
                    memcpy(&paciConsulta, &pac_list[i], sizeof(paciConsulta));
                    paciExists = 1;
                    strcpy(ageCad.pacienteCPF, paciConsulta.cpf);
                    strcpy(ageCad.pacienteNome, paciConsulta.nome);
                    break;
                }
            }
            if(paciExists == 0){
                printf("Paciente não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                    char a;
                    do{
                        a = getch();
                        fflush(stdin);
                        if(a == 32){
                            free(pac_list);
                            fclose(arq);
                            menuLogado();
                        }
                    }while(a != 32 && a != 13);
            }else{
                printf("\nPaciente encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nEndereco: %s\n\n",
                       ageCad.pacienteNome, ageCad.pacienteCPF, paciConsulta.dataNascimento.dd,
                       paciConsulta.dataNascimento.mm, paciConsulta.dataNascimento.yy ,paciConsulta.email,
                       paciConsulta.endereco
                       );
                printf("Confirma paciente? (s/n)");
                char a = getch();
                fflush(stdin);
                if(a != 's' && a != 'S'){
                        free(pac_list);
                        fclose(arq);
                        menuLogado();
                }
            }
        }while(paciExists == 0);
        free(pac_list);
        fclose(arq);
	}//fim busca paciente

    //início busca médico
    arq = fopen(ARQMEDICOS, "rb");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
        memset(&medConsulta, 0, sizeof(medConsulta));
        fseek(arq, 0, SEEK_END);
        arq_size = ftell(arq);
        acc_total = arq_size / sizeof(medConsulta);
        rewind(arq);
        med_list = malloc(arq_size);
        fread(med_list, sizeof(medConsulta), acc_total, arq);
        do{
            system("cls");
            printf("\n\t\tYAMOTO - Agendamento de Consulta\n\n");
            printf("\t====================================================\n");
            printf("\nCPF do Médico: ");
            fgets(medConsulta.cpf, 13, stdin);
            medConsulta.cpf[strcspn(medConsulta.cpf, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(medConsulta.cpf, med_list[i].cpf) == 0){
                    memcpy(&medConsulta, &med_list[i], sizeof(medConsulta));
                    medExists = 1;
                    strcpy(ageCad.medicoCPF, medConsulta.cpf);
                    strcpy(ageCad.medicoNome, medConsulta.nome);
                    ageCad.unidade = medConsulta.unidade;
                    strcpy(ageCad.especializacao, medConsulta.especialidade);
                    break;
                }
            }
            if(medExists == 0){
                printf("Médico não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                    char a;
                    do{
                        a = getch();
                        fflush(stdin);
                        if(a == 32){
                            free(med_list);
                            fclose(arq);
                            menuLogado();
                        }
                    }while(a != 32 && a != 13);
            }else{
                printf("\nMédico encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nUnidade: %d\nCRM: %s\nEspecialidade: %s\n\n",
                       ageCad.medicoNome, ageCad.medicoCPF, medConsulta.dataNascimento.dd,
                       medConsulta.dataNascimento.mm, medConsulta.dataNascimento.yy, medConsulta.email,
                       ageCad.unidade, medConsulta.crm, ageCad.especializacao
                       );
                printf("Confirma médico? (s/n)");
                char a = getch();
                fflush(stdin);
                if(a != 's' && a != 'S'){
                        free(med_list);
                        fclose(arq);
                        menuLogado();
                }
            }
        }while(medExists == 0);
        free(med_list);
        fclose(arq);
	}//fim busca médico

	do{
        system("cls");
        printf("\n\t\tYAMOTO - Agendamento de Consulta\n\n");
        printf("\t====================================================\n");

        printf("\nPaciente: %s\nMédico: %s\nUnidade: %d\nEspecialidade: %s\n", ageCad.pacienteNome, ageCad.medicoNome, ageCad.unidade, ageCad.especializacao);

        printf("\nData da Consulta (DD/MM/AAAA): ");
        scanf("%d/%d/%d", &ageCad.data.dd, &ageCad.data.mm, &ageCad.data.yy);
        fflush(stdin);

        printf("\nHora da consulta (HH:MM): ");
        scanf("%d:%d", &ageCad.hora.hh, &ageCad.hora.mm);
        fflush(stdin);

        validacaoData = validaDataAgendamento(ageCad.data, ageCad.hora);
        if(validacaoData == 0){
            printf("\nData/Hora inválida. Apenas é possível realizar consultas em horários redondos a cada 30 minutos, entre 8h e 12h e entre 13h e 18h.\n\nPressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
            char b;
            do{
                b = getch();
                fflush(stdin);
                if(b == 32){
                    menuLogado();
                }
            }while(b != 32 && b != 13);
        }
	}while(validacaoData == 0);

    printf("\nValor da consulta: R$ ");
    scanf("%f", &ageCad.valor);
    fflush(stdin);

    //verifica se já existe
    arq = fopen(ARQAGEND, "rb");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arq, 0, SEEK_END);
		arq_size = ftell(arq);
		acc_total = arq_size / sizeof(ageCad);
		rewind(arq);
		age_list = malloc(arq_size);
		fread(age_list, sizeof(ageCad), acc_total, arq);
		fclose(arq);

        for(i = 0; i < acc_total; i++){
            if(strcmp(ageCad.pacienteCPF, age_list[i].pacienteCPF) == 0 && strcmp(ageCad.medicoCPF, age_list[i].medicoCPF) == 0
               && memcmp(&ageCad.data, &age_list[i].data, sizeof(ageCad.data)) == 0 && memcmp(&ageCad.hora, &age_list[i].hora, sizeof(ageCad.hora)) == 0){
                ageExists = 1;
                break;
            }
        }

        if(ageExists == 0){
            arq = fopen(ARQAGEND, "ab");
            if (arq == NULL) printf("Erro ao abrir o arquivo.");
            else fwrite(&ageCad, sizeof ageCad, 1, arq);

            free(age_list);
            fclose(arq);

            printf("\nAgendamento cadastrado com sucesso. Pressione qualquer tecla para continuar...");
        }else{
            printf("\nAgendamendo já existe. Pressione qualquer tecla para voltar ao menu...");
            getch();
            fflush(stdin);
            free(age_list);
            fclose(arq);
            menuLogado();
        }
	}
    //fim verifica

	getch();
	fflush(stdin);
	logado != 0 ? menuLogado():menuLogadoOff();
}

void menuConsultas(){
    int arqSizePac, arqSizeMed, arqSizeFun, arqSizeAge;
    FILE* arq;

    arq = fopen(ARQPACIENTES, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizePac = ftell(arq);
    fclose(arq);

    arq = fopen(ARQMEDICOS, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeMed = ftell(arq);
    fclose(arq);

    arq = fopen(ARQFUNCIONARIOS, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeFun = ftell(arq);
    fclose(arq);

    arq = fopen(ARQAGEND, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeAge = ftell(arq);
    fclose(arq);

    system("cls");
	printf("\n\t\tYAMOTO - Consultas\n\n");
	printf("\t==========================\n");

	printf("\nDigite a opção desejada:\n\n");
	if(arqSizePac > 0) printf("1 - Pacientes\n");
    if(arqSizeMed > 0) printf("2 - Médicos\n");
	if(arqSizeFun > 0) printf("3 - Funcionários\n");
	if(arqSizeAge > 0) printf("4 - Agendamentos por Paciente\n");
	if(arqSizeAge > 0) printf("5 - Agendamentos por Médico\n");
	printf("\n9 - Voltar ao menu\n");

	char opcao;
	opcao = getch();
	fflush(stdin);

	switch(opcao){
		case '1':
			consultaPaciente();
			break;
		case '2':
			consultaMedico();
			break;
		case '3':
			consultaFuncionario();
			break;
		case '4':
			consultaAgendamentoPaciente();
			break;
        case '5':
            consultaAgendamentoMedico();
            break;
		case '9':
            menuLogado();
			break;
	  	default:
	   		printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
			getch();
			fflush(stdin);
			cadastroLogado();
			break;
	 }
}

void menuAlterarInfos(){
    int arqSizePac, arqSizeMed, arqSizeFun, arqSizeAge;
    FILE* arq;

    arq = fopen(ARQPACIENTES, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizePac = ftell(arq);
    fclose(arq);

    arq = fopen(ARQMEDICOS, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeMed = ftell(arq);
    fclose(arq);

    arq = fopen(ARQFUNCIONARIOS, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeFun = ftell(arq);
    fclose(arq);

    arq = fopen(ARQAGEND, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeAge = ftell(arq);
    fclose(arq);

    system("cls");
	printf("\n\t\tYAMOTO - Alterações\n\n");
	printf("\t==========================\n");

	printf("Digite a opção desejada:\n\n");
	if(arqSizePac > 0) printf("1 - Pacientes\n");
    if(arqSizeMed > 0) printf("2 - Médicos\n");
	if(arqSizeFun > 0) printf("3 - Funcionarios\n");
	if(arqSizeAge > 0) printf("4 - Agendamentos\n");
	printf("\n9 - Voltar ao Menu\n");

	char opcao;
	opcao = getch();
	fflush(stdin);

	switch(opcao) {
		case '1':
			alteraPaciente();
			break;
		case '2':
			alteraMedico();
			break;
		case '3':
			alteraFuncionario();
			break;
		case '4':
			alteraAgendamento();
			break;
		case '9':
			menuLogado();
			break;
	  	default:
	   		printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
			getch();
			fflush(stdin);
			menuLogado();
			break;
	 }
}

void menuExcluir(){
    int arqSizePac, arqSizeMed, arqSizeFun, arqSizeAge;
    FILE* arq;

    arq = fopen(ARQPACIENTES, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizePac = ftell(arq);
    fclose(arq);

    arq = fopen(ARQMEDICOS, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeMed = ftell(arq);
    fclose(arq);

    arq = fopen(ARQFUNCIONARIOS, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeFun = ftell(arq);
    fclose(arq);

    arq = fopen(ARQAGEND, "ab");
    fseek(arq, 0, SEEK_END);
    arqSizeAge = ftell(arq);
    fclose(arq);

    system("cls");
	printf("\n\t\tYAMOTO - Exclusões\n\n");
	printf("\t==========================\n\n");

	printf("Digite a opção desejada:\n\n");
	if(arqSizePac > 0) printf("1 - Pacientes\n");
    if(arqSizeMed > 0) printf("2 - Médicos\n");
	if(arqSizeFun > 0) printf("3 - Funcionários\n");
	if(arqSizeAge > 0) printf("4 - Agendamentos\n");
	printf("\n9 - Voltar ao Menu\n");

	char opcao;
	opcao = getch();
	fflush(stdin);

	switch(opcao) {
		case '1':
			excluiPaciente();
			break;
		case '2':
			excluiMedico();
			break;
		case '3':
			excluiFuncionario();
			break;
		case '4':
			excluiAgendamento();
			break;
		case '9':
			menuLogado();
			break;
	  	default:
	   		printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
			getch();
			fflush(stdin);
			menuExcluir();
			break;
	 }
}

void consultaPaciente(){
	FILE* arq;
	Paciente *acc_list, paciConsulta;
	int acc_total = 0, arq_size = 0, i, paciExists = 0;

	memset(&paciConsulta, 0, sizeof(paciConsulta));

	arq = fopen(ARQPACIENTES, "rb");

    do{
        system("cls");
        printf("\t-----------YAMOTO-------------\n");
        printf("\n\nDigite o CPF do paciente que deseja consultar (somente numeros): ");
        fgets(paciConsulta.cpf, 13, stdin);
        paciConsulta.cpf[strcspn(paciConsulta.cpf, "\n")] = 0;
        fflush(stdin);

        if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
        else{
            fseek(arq, 0, SEEK_END);
            arq_size = ftell(arq);
            acc_total = arq_size / sizeof(paciConsulta);
            rewind(arq);
            acc_list = malloc(arq_size);
            fread(acc_list, sizeof(paciConsulta), acc_total, arq);

            for(i = 0; i < acc_total; i++){
                if(strcmp(paciConsulta.cpf, acc_list[i].cpf) == 0){
                    memcpy(&paciConsulta, &acc_list[i], sizeof(paciConsulta));
                    paciExists = 1;
                    break;
                }
            }

            if(paciExists == 0){
                printf("Paciente não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                    char a;
                    do{
                        a = getch();
                        fflush(stdin);
                        if(a == 32){
                            menuLogado();
                        }
                    }while(a != 32 && a != 13);
            }
            else{
                printf("\n\n\nPaciente encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nEndereco: %s\nUnidade: %d\n\n",
                       paciConsulta.nome, paciConsulta.cpf, paciConsulta.dataNascimento.dd, paciConsulta.dataNascimento.mm,
                       paciConsulta.dataNascimento.yy, paciConsulta.email, paciConsulta.endereco, paciConsulta.unidade
                       );
            }
            free(acc_list);
        }
    }while(paciExists == 0);
	fclose(arq);
	getch();
    fflush(stdin);
	menuLogado();
}

void consultaMedico(){
    FILE* arq;
	Medico *acc_list, medConsulta;
	int acc_total = 0, arq_size = 0, i, medExists = 0;

	memset(&medConsulta, 0, sizeof(medConsulta));

	arq = fopen(ARQMEDICOS, "rb");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arq, 0, SEEK_END);
		arq_size = ftell(arq);
		acc_total = arq_size / sizeof(medConsulta);
		rewind(arq);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(medConsulta), acc_total, arq);
		fclose(arq);

		do{
            system("cls");
            printf("\t-----------YAMOTO-------------\n");
            printf("\n\nDigite o CPF do médico que deseja consultar (somente números): ");
            fgets(medConsulta.cpf, 13, stdin);
            medConsulta.cpf[strcspn(medConsulta.cpf, "\n")] = 0;

            for(i = 0; i < acc_total; i++){
                if(strcmp(medConsulta.cpf, acc_list[i].cpf) == 0){
                    memcpy(&medConsulta, &acc_list[i], sizeof(medConsulta));
                    medExists = 1;
                    break;
                }
            }

            if(medExists == 0){
                printf("Médico não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                    char a;
                    do{
                        a = getch();
                        fflush(stdin);
                        if(a == 32){
                            menuLogado();
                        }
                    }while(a != 32 && a != 13);
            }else{
                printf("\n\n\nMédico encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nUnidade: %d\nCRM: %s\nEspecialidade: %s\n\n",
                       medConsulta.nome, medConsulta.cpf, medConsulta.dataNascimento.dd,
                       medConsulta.dataNascimento.mm, medConsulta.dataNascimento.yy, medConsulta.email,
                       medConsulta.unidade, medConsulta.crm, medConsulta.especialidade
                );
            }
		}while(medExists == 0);
		free(acc_list);
	}
	getch();
    fflush(stdin);
	menuLogado();
}

void consultaFuncionario(){
    FILE* arq;
	Funcionario *acc_list, funConsulta;
	int acc_total = 0, arq_size = 0, i, funExists = 0;

	memset(&funConsulta, 0, sizeof(funConsulta));

	arq = fopen(ARQFUNCIONARIOS, "rb");

    if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
    else{
        fseek(arq, 0, SEEK_END);
        arq_size = ftell(arq);
        acc_total = arq_size / sizeof(funConsulta);
        rewind(arq);
        acc_list = malloc(arq_size);
        fread(acc_list, sizeof(funConsulta), acc_total, arq);

        do{
            system("cls");
            printf("\t-----------YAMOTO-------------\n");
            printf("\n\nDigite o CPF do funcionário que deseja consultar (somente números): ");
            fgets(funConsulta.cpf, 13, stdin);
            funConsulta.cpf[strcspn(funConsulta.cpf, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(funConsulta.cpf, acc_list[i].cpf) == 0){
                    memcpy(&funConsulta, &acc_list[i], sizeof(funConsulta));
                    funExists = 1;
                    break;
                }
            }

            if(funExists == 0){
                printf("Funcionário não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                    char a;
                    do{
                        a = getch();
                        fflush(stdin);
                        if(a == 32){
                            menuLogado();
                        }
                    }while(a != 32 && a != 13);
            }
            else{
                printf("\n\n\nFuncionário encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nUnidade: %d\nDepartamento: %s\n\n",
                       funConsulta.nome, funConsulta.cpf, funConsulta.dataNascimento.dd, funConsulta.dataNascimento.mm,
                       funConsulta.dataNascimento.yy, funConsulta.email, funConsulta.unidade, funConsulta.departamento
                );
            }
        }while(funExists == 0);
        free(acc_list);
    }
	fclose(arq);
	getch();
    fflush(stdin);
	menuLogado();
}

void consultaAgendamentoPaciente(){
    FILE* arq;
	Agendamento *age_list, ageConsulta;
	int acc_total = 0, arq_size = 0, i, ageExists = 0;

	memset(&ageConsulta, 0, sizeof(ageConsulta));

	arq = fopen(ARQAGEND, "rb");

    if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
    else{
        fseek(arq, 0, SEEK_END);
        arq_size = ftell(arq);
        acc_total = arq_size / sizeof(ageConsulta);
        rewind(arq);
        age_list = malloc(arq_size);
        fread(age_list, sizeof(ageConsulta), acc_total, arq);

        do{
            system("cls");
            printf("\t-----------YAMOTO-------------\n");
            printf("\n\nDigite o CPF do paciente que deseja consultar agendamentos (somente números): ");
            fgets(ageConsulta.pacienteCPF, 13, stdin);
            ageConsulta.pacienteCPF[strcspn(ageConsulta.pacienteCPF, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(ageConsulta.pacienteCPF, age_list[i].pacienteCPF) == 0){
                    memcpy(&ageConsulta, &age_list[i], sizeof(ageConsulta));
                    ageExists = 1;
                    printf("\n\nPaciente: %s\nCPF: %s\nMédico: %s\nCPF: %s\nEspecialidade: %s\nUnidade: %d\nData: %d/%d/%d\nHora: %d:%d\nValor: %.2f\n\n",
                           ageConsulta.pacienteNome, ageConsulta.pacienteCPF, ageConsulta.medicoNome, ageConsulta.medicoCPF,
                           ageConsulta.especializacao, ageConsulta.unidade, ageConsulta.data.dd, ageConsulta.data.mm, ageConsulta.data.yy,
                           ageConsulta.hora.hh, ageConsulta.hora.mm, ageConsulta.valor
                    );
                }
            }

            if(ageExists == 0){
                printf("Agendamentos não encontrados. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                    char a;
                    do{
                        a = getch();
                        fflush(stdin);
                        if(a == 32){
                            menuLogado();
                        }
                    }while(a != 32 && a != 13);
            }
        }while(ageExists == 0);
        free(age_list);
    }
	fclose(arq);
	getch();
    fflush(stdin);
	menuLogado();
}

void consultaAgendamentoMedico(){
    FILE* arq;
	Agendamento *age_list, ageConsulta;
	int acc_total = 0, arq_size = 0, i, ageExists = 0;

	memset(&ageConsulta, 0, sizeof(ageConsulta));

	arq = fopen(ARQAGEND, "rb");

    if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
    else{
        fseek(arq, 0, SEEK_END);
        arq_size = ftell(arq);
        acc_total = arq_size / sizeof(ageConsulta);
        rewind(arq);
        age_list = malloc(arq_size);
        fread(age_list, sizeof(ageConsulta), acc_total, arq);

        do{
            system("cls");
            printf("\t-----------YAMOTO-------------\n");
            printf("\n\nDigite o CPF do médico que deseja consultar agendamentos (somente números): ");
            fgets(ageConsulta.medicoCPF, 13, stdin);
            ageConsulta.medicoCPF[strcspn(ageConsulta.medicoCPF, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(ageConsulta.medicoCPF, age_list[i].medicoCPF) == 0){
                    memcpy(&ageConsulta, &age_list[i], sizeof(ageConsulta));
                    ageExists = 1;
                    printf("\n\nPaciente: %s\nCPF: %s\nMédico: %s\nCPF: %s\nEspecialidade: %s\nUnidade: %d\nData: %d/%d/%d\nHora: %d:%d\nValor: %.2f\n\n",
                           ageConsulta.pacienteNome, ageConsulta.pacienteCPF, ageConsulta.medicoNome, ageConsulta.medicoCPF,
                           ageConsulta.especializacao, ageConsulta.unidade, ageConsulta.data.dd, ageConsulta.data.mm, ageConsulta.data.yy,
                           ageConsulta.hora.hh, ageConsulta.hora.mm, ageConsulta.valor
                    );
                }
            }

            if(ageExists == 0){
                printf("Agendamentos não encontrados. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                    char a;
                    do{
                        a = getch();
                        fflush(stdin);
                        if(a == 32){
                            menuLogado();
                        }
                    }while(a != 32 && a != 13);
            }
        }while(ageExists == 0);
        free(age_list);
    }
	fclose(arq);
	getch();
    fflush(stdin);
	menuLogado();
}

void excluiPaciente(){
	FILE* arq;
	Paciente *acc_list, paciConsulta;
	int acc_total = 0, arq_size = 0, i, paciExists = 0, tempPosicao;

	memset(&paciConsulta, 0, sizeof(paciConsulta));

	arq = fopen(ARQPACIENTES, "rb");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arq, 0, SEEK_END);
		arq_size = ftell(arq);
		acc_total = arq_size / sizeof(paciConsulta);
		rewind(arq);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(paciConsulta), acc_total, arq);
		fclose(arq);

		do{
            system("cls");
            printf("\t-----------YAMOTO-------------\n");
            printf("\n\nDigite o CPF do paciente que deseja excluir (somente numeros): ");
            fgets(paciConsulta.cpf, 13, stdin);
            paciConsulta.cpf[strcspn(paciConsulta.cpf, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(paciConsulta.cpf, acc_list[i].cpf) == 0){
                    memcpy(&paciConsulta, &acc_list[i], sizeof(paciConsulta));
                    paciExists = 1;
                    tempPosicao = i;
                    break;
                }
            }

            if(paciExists == 0){
                printf("Paciente não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                char a;
                fflush(stdin);
                do{
                    a = getch();
                    if(a == 32){
                        menuLogado();
                    }
                }while(a != 32 && a != 13);
            }
            else{
                printf("\n\n\nPaciente encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nEndereco: %s\n\n",
                       paciConsulta.nome, paciConsulta.cpf, paciConsulta.dataNascimento.dd, paciConsulta.dataNascimento.mm,
                       paciConsulta.dataNascimento.yy ,paciConsulta.email, paciConsulta.endereco
                );

                printf("\n\nConfirma e exclusão? (s/n)\n");
                char opt = getch();
                fflush(stdin);

                if(opt == 's' || opt == 'S'){
                    for(i = tempPosicao; i < acc_total; i++){
                        acc_list[i] = acc_list[i+1];
                    }
                    remove(ARQPACIENTES);
                    arq = fopen(ARQPACIENTES, "wb");
                    fwrite(acc_list, sizeof(*acc_list), acc_total, arq);
                    fclose(arq);
                    printf("\nPaciente excluído com sucesso. Pressione qualquer tecla para continuar...");
                }else{
                    printf("\nPaciente não foi excluído. Pressione qualquer tecla para continuar...");
                }
            }
		}while(paciExists == 0);

		free(acc_list);
	}
	getch();
    fflush(stdin);
	menuLogado();
}

void excluiFuncionario(){
	FILE* arq;
	Funcionario *acc_list, funConsulta;
	int acc_total = 0, arq_size = 0, i, funExists = 0, tempPosicao;

	memset(&funConsulta, 0, sizeof(funConsulta));

	arq = fopen(ARQFUNCIONARIOS, "rb");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arq, 0, SEEK_END);
		arq_size = ftell(arq);
		acc_total = arq_size / sizeof(funConsulta);
		rewind(arq);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(funConsulta), acc_total, arq);
		fclose(arq);

		do{
            system("cls");
            printf("\t-----------YAMOTO-------------\n");
            printf("\n\nDigite o CPF do funcionário que deseja excluir (somente numeros): ");
            fgets(funConsulta.cpf, 13, stdin);
            funConsulta.cpf[strcspn(funConsulta.cpf, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(funConsulta.cpf, acc_list[i].cpf) == 0){
                    memcpy(&funConsulta, &acc_list[i], sizeof(funConsulta));
                    funExists = 1;
                    tempPosicao = i;
                    break;
                }
            }

            if(funExists == 0){
                printf("Funcionário não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                char a;
                do{
                    a = getch();
                    fflush(stdin);
                    if(a == 32){
                        menuLogado();
                    }
                }while(a != 32 && a != 13);
            }
            else{
                printf("\n\n\nFuncionário encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nUnidade: %d\nDepartamento: %s\n\n",
                       funConsulta.nome, funConsulta.cpf, funConsulta.dataNascimento.dd, funConsulta.dataNascimento.mm,
                       funConsulta.dataNascimento.yy, funConsulta.email, funConsulta.unidade, funConsulta.departamento
                );

                printf("Confirma e exclusão? (s/n)\n");
                char opt = getch();
                fflush(stdin);

                if(opt == 's' || opt == 'S'){
                    for(i = tempPosicao; i < acc_total; i++){
                        acc_list[i] = acc_list[i+1];
                    }
                    remove(ARQFUNCIONARIOS);
                    arq = fopen(ARQFUNCIONARIOS, "wb");
                    fwrite(acc_list, sizeof(*acc_list), acc_total, arq);
                    fclose(arq);
                    printf("\nFuncionário excluído com sucesso. Pressione qualquer tecla para continuar...");
                }else{
                    printf("\nFuncionário não foi excluído. Pressione qualquer tecla para continuar...");
                }
            }
		}while(funExists == 0);

		free(acc_list);
	}
	getch();
    fflush(stdin);
	menuLogado();
}

void excluiMedico(){
	FILE* arq;
	Medico *acc_list, medConsulta;
	int acc_total = 0, arq_size = 0, i, medExists = 0, tempPosicao;

	memset(&medConsulta, 0, sizeof(medConsulta));

	arq = fopen(ARQMEDICOS, "rb");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arq, 0, SEEK_END);
		arq_size = ftell(arq);
		acc_total = arq_size / sizeof(medConsulta);
		rewind(arq);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(medConsulta), acc_total, arq);
		fclose(arq);

		do{
            system("cls");
            printf("\t-----------YAMOTO-------------\n");
            printf("\n\nDigite o CPF do médico que deseja excluir (somente numeros): ");
            fgets(medConsulta.cpf, 13, stdin);
            medConsulta.cpf[strcspn(medConsulta.cpf, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(medConsulta.cpf, acc_list[i].cpf) == 0){
                    memcpy(&medConsulta, &acc_list[i], sizeof(medConsulta));
                    medExists = 1;
                    tempPosicao = i;
                    break;
                }
            }

            if(medExists == 0){
                printf("Médico não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                char a;
                do{
                    a = getch();
                    fflush(stdin);
                    if(a == 32){
                        menuLogado();
                    }
                }while(a != 32 && a != 13);
            }
            else{
                printf("\n\n\nMédico encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nUnidade: %d\nCRM: %s\nEspecialidade: %s\n\n",
                       medConsulta.nome, medConsulta.cpf, medConsulta.dataNascimento.dd, medConsulta.dataNascimento.mm, medConsulta.dataNascimento.yy,
                       medConsulta.email, medConsulta.unidade, medConsulta.crm, medConsulta.especialidade
                );

                printf("\n\nConfirma e exclusão? (s/n)\n");
                char opt = getch();
                fflush(stdin);

                if(opt == 's' || opt == 'S'){
                    for(i = tempPosicao; i < acc_total; i++){
                        acc_list[i] = acc_list[i+1];
                    }
                    remove(ARQMEDICOS);
                    arq = fopen(ARQMEDICOS, "wb");
                    fwrite(acc_list, sizeof(*acc_list), acc_total, arq);
                    fclose(arq);
                    printf("\nMédico excluído com sucesso. Pressione qualquer tecla para continuar...");
                }else{
                    printf("\nMédico não foi excluído. Pressione qualquer tecla para continuar...");
                }
            }
		}while(medExists == 0);

		free(acc_list);
	}
	getch();
    fflush(stdin);
	menuLogado();
}

void excluiAgendamento(){
    FILE* arq;
	Agendamento *age_list, ageConsulta;
	int acc_total = 0, arq_size = 0, i, ageExists = 0, tempPosicao;

	memset(&ageConsulta, 0, sizeof(ageConsulta));

	arq = fopen(ARQAGEND, "rb");

    if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
    else{
        fseek(arq, 0, SEEK_END);
        arq_size = ftell(arq);
        acc_total = arq_size / sizeof(ageConsulta);
        rewind(arq);
        age_list = malloc(arq_size);
        fread(age_list, sizeof(ageConsulta), acc_total, arq);

        do{
            system("cls");
            printf("\t-----------YAMOTO-------------\n");

            printf("\n\nDigite o CPF do paciente que deseja excluir o agendamento (somente números): ");
            fgets(ageConsulta.pacienteCPF, 13, stdin);
            ageConsulta.pacienteCPF[strcspn(ageConsulta.pacienteCPF, "\n")] = 0;
            fflush(stdin);

            printf("\n\nDigite o CPF do médico que deseja excluir o agendamento (somente números): ");
            fgets(ageConsulta.medicoCPF, 13, stdin);
            ageConsulta.medicoCPF[strcspn(ageConsulta.medicoCPF, "\n")] = 0;
            fflush(stdin);

            printf("\n\nDigite a data do agendamento (DD/MM/AAAA): ");
            scanf("%d/%d/%d", &ageConsulta.data.dd, &ageConsulta.data.mm, &ageConsulta.data.yy);
            fflush(stdin);

            printf("\n\nDigite a hora do agendamento (HH:MM): ");
            scanf("%d:%d", &ageConsulta.hora.hh, &ageConsulta.hora.mm);
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(ageConsulta.pacienteCPF, age_list[i].pacienteCPF) == 0 && strcmp(ageConsulta.medicoCPF, age_list[i].medicoCPF) == 0
                   && memcmp(&ageConsulta.data, &age_list[i].data, sizeof(ageConsulta.data)) == 0 && memcmp(&ageConsulta.hora, &age_list[i].hora, sizeof(ageConsulta.data)) == 0){
                    memcpy(&ageConsulta, &age_list[i], sizeof(ageConsulta));
                    ageExists = 1;
                    tempPosicao = i;
                    break;
                }
            }

            if(ageExists == 0){
                printf("Agendamento não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                    char a;
                    do{
                        a = getch();
                        fflush(stdin);
                        if(a == 32){
                            menuLogado();
                        }
                    }while(a != 32 && a != 13);
            }else{
                printf("\n\n\nAgendamento encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nMédico: %s\nCPF: %s\nUnidade: %d\nEspecialidade: %s\nData: %d/%d/%d\nHora: %d:%d\nValor: R$ %.2f",
                       ageConsulta.pacienteNome, ageConsulta.pacienteCPF, ageConsulta.medicoNome, ageConsulta.medicoCPF,
                       ageConsulta.unidade, ageConsulta.especializacao, ageConsulta.data.dd, ageConsulta.data.mm, ageConsulta.data.yy,
                       ageConsulta.hora.hh, ageConsulta.hora.mm, ageConsulta.valor
                );

                printf("\n\nConfirma e exclusão? (s/n)\n");
                char opt = getch();
                fflush(stdin);

                if(opt == 's' || opt == 'S'){
                    for(i = tempPosicao; i < acc_total; i++){
                        age_list[i] = age_list[i+1];
                    }
                    remove(ARQAGEND);
                    arq = fopen(ARQAGEND, "wb");
                    fwrite(age_list, sizeof(*age_list), acc_total, arq);
                    fclose(arq);
                    printf("\nAgendamento excluído com sucesso. Pressione qualquer tecla para continuar...");
                }else{
                    printf("\nAgendamento não foi excluído. Pressione qualquer tecla para continuar...");
                }
            }
        }while(ageExists == 0);
        free(age_list);
    }
	fclose(arq);
	getch();
    fflush(stdin);
	menuLogado();
}

void alteraPaciente(){
	FILE* arq;
	Paciente *acc_list, paciConsulta, user_cad;
	int acc_total = 0, arq_size = 0, i, paciExists = 0, tempPosicao, validacaoData = 0;;

	memset(&paciConsulta, 0, sizeof(paciConsulta));

	arq = fopen(ARQPACIENTES, "rb");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arq, 0, SEEK_END);
		arq_size = ftell(arq);
		acc_total = arq_size / sizeof(paciConsulta);
		rewind(arq);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(paciConsulta), acc_total, arq);
		fclose(arq);

		do{
            system("cls");
            printf("\t-----------YAMOTO-------------\n");
            printf("\n\nDigite o CPF do paciente que deseja alterar (somente numeros): ");
            fgets(paciConsulta.cpf, 13, stdin);
            paciConsulta.cpf[strcspn(paciConsulta.cpf, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(paciConsulta.cpf, acc_list[i].cpf) == 0){
                    memcpy(&paciConsulta, &acc_list[i], sizeof(paciConsulta));
                    paciExists = 1;
                    tempPosicao = i;
                    break;
                }
            }

            if(paciExists == 0){
                printf("Paciente não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                char a;
                do{
                    a = getch();
                    fflush(stdin);
                    if(a == 32){
                        menuLogado();
                    }
                }while(a != 32 && a != 13);
            }
            else{
                printf("\n\n\nPaciente encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nEndereco: %s\n\n",
                       paciConsulta.nome, paciConsulta.cpf, paciConsulta.dataNascimento.dd, paciConsulta.dataNascimento.mm,
                       paciConsulta.dataNascimento.yy,paciConsulta.email, paciConsulta.endereco
                );

                printf("\n\nRealize as alterações necessárias.\n");

                memset(&user_cad, 0, sizeof(user_cad));

                char escolha;

                strcpy(user_cad.cpf, paciConsulta.cpf);

                printf("\nDeseja editar Nome? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nNome Completo: ");
                    fgets(user_cad.nome, SIZE, stdin);
                    user_cad.nome[strcspn(user_cad.nome, "\n")] = 0;
                    fflush(stdin);
                }else strcpy(user_cad.nome, paciConsulta.nome);

                do{
                    system("cls");
                    printf("\t-----------YAMOTO-------------\n");
                    printf("\n\nDigite o CPF do paciente que deseja alterar (somente numeros): ");

                    printf("\n\n\nPaciente encontrado!");
                    printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nEndereco: %s\n\n",
                           paciConsulta.nome, paciConsulta.cpf, paciConsulta.dataNascimento.dd, paciConsulta.dataNascimento.mm,
                           paciConsulta.dataNascimento.yy,paciConsulta.email, paciConsulta.endereco
                    );

                    printf("\n\nRealize as alterações necessárias.\n");

                    printf("\nDeseja editar Nome? (s/n)");
                    printf("\nNome Completo: ");
                    printf("%s\n", paciConsulta.nome);

                    printf("\nDeseja editar Data de Nascimento? (s/n)");
                    escolha = getch();
                    fflush(stdin);
                    if(escolha == 's' || escolha == 'S'){
                        printf("\nData de Nascimento (DD/MM/AAAA): ");
                        scanf("%d/%d/%d", &user_cad.dataNascimento.dd, &user_cad.dataNascimento.mm, &user_cad.dataNascimento.yy);
                        fflush(stdin);

                        validacaoData = validaDataNascimento(user_cad.dataNascimento);
                        if(validacaoData == 0){
                            printf("Data inválida. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                            char b;
                            do{
                                b = getch();
                                fflush(stdin);
                                if(b == 32){
                                    menuLogado();
                                }
                            }while(b != 32 && b != 13);
                        }
                    }else{
                        memcpy(&user_cad.dataNascimento, &paciConsulta.dataNascimento, sizeof(user_cad.dataNascimento));
                        break;
                    }
                }while(validacaoData == 0);

                printf("\nDeseja editar E-mail? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nE-mail: ");
                    fgets(user_cad.email, SIZE, stdin);
                    user_cad.email[strcspn(user_cad.email, "\n")] = 0;
                    fflush(stdin);
                }else strcpy(user_cad.email, paciConsulta.email);

                printf("\nDeseja editar Endereço? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nEndereco: ");
                    fgets(user_cad.endereco, SIZE, stdin);
                    user_cad.endereco[strcspn(user_cad.endereco, "\n")] = 0;
                    fflush(stdin);
                }else strcpy(user_cad.endereco, paciConsulta.endereco);


                printf("\nDeseja editar Unidade? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    do{
                        printf("Unidade: ");
                        scanf("%d", &user_cad.unidade);
                        fflush(stdin);

                        if(user_cad.unidade < 1 || user_cad.unidade > 3){
                            printf("\nUnidade inválida, escolha apenas uma entre as unidades 1, 2 e 3. Pressione qualquer tecla para tentar novamente...\n\n");
                            getch();
                            fflush(stdin);
                        }
                    }while(user_cad.unidade < 1 || user_cad.unidade > 3);
                }else user_cad.unidade= paciConsulta.unidade;

                acc_list[tempPosicao] = user_cad;
                FILE* arqDest = fopen(ARQPACIENTES, "wb");
                fwrite(acc_list, sizeof(*acc_list), acc_total, arqDest);
                fclose(arqDest);

                printf("\nPaciente alterado com sucesso. Pressione qualquer tecla para continuar...");
            }
		}while(paciExists == 0);

		free(acc_list);
	}
	getch();
    fflush(stdin);
	menuLogado();
}

void alteraAgendamento(){
    FILE* arq;
	Agendamento *age_list, ageCad;
	Paciente *pac_list, paciConsulta;
	Medico *med_list, medConsulta;
	int acc_total = 0, arq_size = 0, i, paciExists = 0, medExists = 0, ageExists = 0, tempPosicao, validacaoData = 0;

	memset(&ageCad, 0, sizeof(ageCad));

    //início busca paciente
    arq = fopen(ARQPACIENTES, "rb");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
        memset(&paciConsulta, 0, sizeof(paciConsulta));
        fseek(arq, 0, SEEK_END);
        arq_size = ftell(arq);
        acc_total = arq_size / sizeof(paciConsulta);
        rewind(arq);
        pac_list = malloc(arq_size);
        fread(pac_list, sizeof(paciConsulta), acc_total, arq);
        do{
            system("cls");
            printf("\n\t\tYAMOTO - Alteração de Agendamento\n\n");
            printf("\t====================================================\n");
            printf("\nCPF do Paciente: ");
            fgets(paciConsulta.cpf, 13, stdin);
            paciConsulta.cpf[strcspn(paciConsulta.cpf, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(paciConsulta.cpf, pac_list[i].cpf) == 0){
                    memcpy(&paciConsulta, &pac_list[i], sizeof(paciConsulta));
                    paciExists = 1;
                    strcpy(ageCad.pacienteCPF, paciConsulta.cpf);
                    strcpy(ageCad.pacienteNome, paciConsulta.nome);
                    break;
                }
            }
            if(paciExists == 0){
                printf("Paciente não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                    char a;
                    do{
                        a = getch();
                        fflush(stdin);
                        if(a == 32){
                            free(pac_list);
                            fclose(arq);
                            menuLogado();
                        }
                    }while(a != 32 && a != 13);
            }else{
                printf("\nPaciente encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nEndereco: %s\n\n",
                       ageCad.pacienteNome, ageCad.pacienteCPF, paciConsulta.dataNascimento.dd, paciConsulta.dataNascimento.mm,
                       paciConsulta.dataNascimento.yy, paciConsulta.email, paciConsulta.endereco
                       );
                printf("Confirma paciente? (s/n)");
                char a = getch();
                fflush(stdin);
                if(a != 's' && a != 'S'){
                        free(pac_list);
                        fclose(arq);
                        menuLogado();
                }
            }
        }while(paciExists == 0);
        free(pac_list);
        fclose(arq);
	}//fim busca paciente

    //início busca médico
    arq = fopen(ARQMEDICOS, "rb");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
        memset(&medConsulta, 0, sizeof(medConsulta));
        fseek(arq, 0, SEEK_END);
        arq_size = ftell(arq);
        acc_total = arq_size / sizeof(medConsulta);
        rewind(arq);
        med_list = malloc(arq_size);
        fread(med_list, sizeof(medConsulta), acc_total, arq);
        do{
            system("cls");
            printf("\n\t\tYAMOTO - Alteração de Agendamento\n\n");
            printf("\t====================================================\n");
            printf("\nCPF do Médico: ");
            fgets(medConsulta.cpf, 13, stdin);
            medConsulta.cpf[strcspn(medConsulta.cpf, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(medConsulta.cpf, med_list[i].cpf) == 0){
                    memcpy(&medConsulta, &med_list[i], sizeof(medConsulta));
                    medExists = 1;
                    strcpy(ageCad.medicoCPF, medConsulta.cpf);
                    strcpy(ageCad.medicoNome, medConsulta.nome);
                    ageCad.unidade = medConsulta.unidade;
                    strcpy(ageCad.especializacao, medConsulta.especialidade);
                    break;
                }
            }
            if(medExists == 0){
                printf("Médico não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                    char a;
                    do{
                        a = getch();
                        fflush(stdin);
                        if(a == 32){
                            free(med_list);
                            fclose(arq);
                            menuLogado();
                        }
                    }while(a != 32 && a != 13);
            }else{
                printf("\nMédico encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nUnidade: %d\nCRM: %s\nEspecialidade: %s\n\n",
                       ageCad.medicoNome, ageCad.medicoCPF, medConsulta.dataNascimento.dd, medConsulta.dataNascimento.mm,
                       medConsulta.dataNascimento.yy, medConsulta.email, ageCad.unidade, medConsulta.crm, ageCad.especializacao
                       );
                printf("Confirma médico? (s/n)");
                char a = getch();
                fflush(stdin);
                if(a != 's' && a != 'S'){
                        free(med_list);
                        fclose(arq);
                        menuLogado();
                }
            }
        }while(medExists == 0);
        free(med_list);
        fclose(arq);
	}//fim busca médico

	do{
        system("cls");
        printf("\n\t\tYAMOTO - Alteração de Agendamento\n\n");
        printf("\t====================================================\n");

        printf("\nData da Consulta (DD/MM/AAAA): ");
        scanf("%d/%d/%d", &ageCad.data.dd, &ageCad.data.mm, &ageCad.data.yy);
        fflush(stdin);

        printf("\nHora da consulta (HH:MM): ");
        scanf("%d:%d", &ageCad.hora.hh, &ageCad.hora.mm);
        fflush(stdin);

        validacaoData = validaDataAgendamento(ageCad.data, ageCad.hora);
        if(validacaoData == 0){
            printf("\nData/Hora inválida. Apenas é possível realizar consultas em horários redondos a cada 30 minutos, entre 8h e 12h e entre 13h e 18h.\n\nPressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
            char b;
            do{
                b = getch();
                fflush(stdin);
                if(b == 32){
                    menuLogado();
                }
            }while(b != 32 && b != 13);
        }
	}while(validacaoData == 0);

	arq = fopen(ARQAGEND, "rb");

	fseek(arq, 0, SEEK_END);
    arq_size = ftell(arq);
    acc_total = arq_size / sizeof(ageCad);
    rewind(arq);
    age_list = malloc(arq_size);
    fread(age_list, sizeof(ageCad), acc_total, arq);

	do{
        for(i = 0; i < acc_total; i++){
            if(strcmp(ageCad.pacienteCPF, age_list[i].pacienteCPF) == 0 && strcmp(ageCad.medicoCPF, age_list[i].medicoCPF) == 0
               && memcmp(&ageCad.data, &age_list[i].data, sizeof(ageCad.data)) == 0 && memcmp(&ageCad.hora, &age_list[i].hora, sizeof(ageCad.hora)) == 0){
                memcpy(&ageCad, &age_list[i], sizeof(ageCad));
                ageExists = 1;
                tempPosicao = i;
                break;
            }
        }

        if(ageExists == 0){
            printf("Agendamento não encontrado. Pressione qualquer tecla para voltar ao menu.");
            getch();
            fflush(stdin);
            free(age_list);
            fclose(arq);
            menuLogado();
        }
    }while(ageExists == 0);
    fclose(arq);

	//system("cls");
    printf("\n\t\tYAMOTO - Alteração de Agendamento\n\n");
    printf("\t====================================================\n");

    printf("\nPaciente: %s\nCPF: %s\nMédico: %s\nCPF: %s\nEspecialidade: %s\nUnidade: %d\nData: %d/%d/%d\nHora: %d:%d\nValor: R$ %.2f",
           ageCad.pacienteNome, ageCad.pacienteCPF, ageCad.medicoNome, ageCad.medicoCPF, ageCad.especializacao, ageCad.unidade,
           ageCad.data.dd, ageCad.data.mm, ageCad.data.yy, ageCad.hora.hh, ageCad.hora.mm, ageCad.valor
           );
    printf("\n\nConfirma agendamento a ser alterado?(s/n)\n");
    char a = getch();
    fflush(stdin);
    if(a != 's' && a != 'S'){
            free(age_list);
            menuLogado();
    }

//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    printf("\nDeseja alterar paciente?(s/n)\n");
    a = getch();
    fflush(stdin);
    if(a == 's' || a == 'S'){
        arq = fopen(ARQPACIENTES, "rb");

        if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
        else{
            fseek(arq, 0, SEEK_END);
            arq_size = ftell(arq);
            acc_total = arq_size / sizeof(paciConsulta);
            rewind(arq);
            pac_list = malloc(arq_size);
            fread(pac_list, sizeof(paciConsulta), acc_total, arq);
            do{
                system("cls");
                printf("\n\t\tYAMOTO - Alteração de Agendamento\n\n");
                printf("\t====================================================\n");
                printf("\nNovo CPF do Paciente: ");
                fgets(paciConsulta.cpf, 13, stdin);
                paciConsulta.cpf[strcspn(paciConsulta.cpf, "\n")] = 0;
                fflush(stdin);

                for(i = 0; i < acc_total; i++){
                    if(strcmp(paciConsulta.cpf, pac_list[i].cpf) == 0){
                        memcpy(&paciConsulta, &pac_list[i], sizeof(paciConsulta));
                        paciExists = 1;
                        strcpy(ageCad.pacienteCPF, paciConsulta.cpf);
                        strcpy(ageCad.pacienteNome, paciConsulta.nome);
                        break;
                    }
                }
                if(paciExists == 0){
                    printf("Paciente não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                        char a;
                        do{
                            a = getch();
                            if(a == 32){
                                free(pac_list);
                                fclose(arq);
                                menuLogado();
                            }
                        }while(a != 32 && a != 13);
                }else{
                    printf("\nPaciente encontrado!");
                    printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nEndereco: %s\n\n",
                           paciConsulta.nome, paciConsulta.cpf, paciConsulta.dataNascimento.dd, paciConsulta.dataNascimento.mm,
                           paciConsulta.dataNascimento.yy, paciConsulta.email, paciConsulta.endereco
                           );
                    printf("Confirma paciente? (s/n)");
                    char a = getch();
                    fflush(stdin);
                    if(a != 's' && a != 'S'){
                            free(pac_list);
                            fclose(arq);
                            menuLogado();
                    }
                }
            }while(paciExists == 0);
            free(pac_list);
            fclose(arq);
        }//fim busca paciente
    }

    printf("\nDeseja alterar médico? (s/n)\n");
    a = getch();
    fflush(stdin);
    if(a == 's' || a == 'S'){
        //início busca médico
        arq = fopen(ARQMEDICOS, "rb");

        if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
        else{
            fseek(arq, 0, SEEK_END);
            arq_size = ftell(arq);
            acc_total = arq_size / sizeof(medConsulta);
            rewind(arq);
            med_list = malloc(arq_size);
            fread(med_list, sizeof(medConsulta), acc_total, arq);
            do{
                system("cls");
                printf("\n\t\tYAMOTO - Alteração de Agendamento\n\n");
                printf("\t====================================================\n");
                printf("\nNovo CPF do Médico: ");
                fgets(medConsulta.cpf, 13, stdin);
                medConsulta.cpf[strcspn(medConsulta.cpf, "\n")] = 0;
                fflush(stdin);

                for(i = 0; i < acc_total; i++){
                    if(strcmp(medConsulta.cpf, med_list[i].cpf) == 0){
                        memcpy(&medConsulta, &med_list[i], sizeof(medConsulta));
                        medExists = 1;
                        strcpy(ageCad.medicoCPF, medConsulta.cpf);
                        strcpy(ageCad.medicoNome, medConsulta.nome);
                        ageCad.unidade = medConsulta.unidade;
                        strcpy(ageCad.especializacao, medConsulta.especialidade);
                        break;
                    }
                }
                if(medExists == 0){
                    printf("Médico não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                        char a;
                        fflush(stdin);
                        do{
                            a = getch();
                            if(a == 32){
                                free(med_list);
                                fclose(arq);
                                menuLogado();
                            }
                        }while(a != 32 && a != 13);
                }else{
                    printf("\nMédico encontrado!");
                    printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nUnidade: %d\nCRM: %s\nEspecialidade: %s\n\n",
                           ageCad.medicoNome, ageCad.medicoCPF, medConsulta.dataNascimento.dd, medConsulta.dataNascimento.mm, medConsulta.dataNascimento.yy,
                           medConsulta.email, ageCad.unidade, medConsulta.crm, ageCad.especializacao
                           );
                    printf("Confirma médico? (s/n)");
                    char a = getch();
                    fflush(stdin);
                    if(a != 's' && a != 'S'){
                            free(med_list);
                            fclose(arq);
                            menuLogado();
                    }
                }
            }while(medExists == 0);
            free(med_list);
            fclose(arq);
        }//fim busca médico
    }

	system("cls");
    printf("\n\t\tYAMOTO - Alteração de Agendamento\n\n");
    printf("\t====================================================\n");

    printf("\nPaciente: %s\nMédico: %s\nUnidade: %d\nEspecialidade: %s\n", ageCad.pacienteNome, ageCad.medicoNome, ageCad.unidade, ageCad.especializacao);

    printf("\nDeseja alterar data da consulta? (s/n)\n");
    a = getch();
    fflush(stdin);
    if(a == 's' || a == 'S'){
        printf("\nNova data da Consulta (DD/MM/AAAA): ");
        scanf("%d/%d/%d", &ageCad.data.dd, &ageCad.data.mm, &ageCad.data.yy);
        fflush(stdin);
    }

    printf("\nDeseja alterar hora da consulta? (s/n)\n");
    a = getch();
    fflush(stdin);
    if(a == 's' || a == 'S'){
        printf("\nNova hora da consulta (HH:MM): ");
        scanf("%d:%d", &ageCad.hora.hh, &ageCad.hora.mm);
        fflush(stdin);
    }

    printf("\nDeseja alterar valor da consulta? (s/n)\n");
    a = getch();
    fflush(stdin);
    if(a == 's' || a == 'S'){
        printf("\nNovo valor da consulta: R$ ");
        scanf("%f", &ageCad.valor);
        fflush(stdin);

    }

    arq = fopen(ARQAGEND, "wb");
	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
        age_list[tempPosicao] = ageCad;
        fwrite(age_list, sizeof(*age_list), acc_total, arq);
        fclose(arq);
	}
    //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    free(age_list);
	fflush(stdin);
	logado != 0 ? menuLogado():menuLogadoOff();
}

void alteraFuncionario(){
    FILE* arq;
	Funcionario *acc_list, funConsulta, user_cad;
	int acc_total = 0, arq_size = 0, i, funExists = 0, tempPosicao;

	memset(&funConsulta, 0, sizeof(funConsulta));

	arq = fopen(ARQFUNCIONARIOS, "rb");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arq, 0, SEEK_END);
		arq_size = ftell(arq);
		acc_total = arq_size / sizeof(funConsulta);
		rewind(arq);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(funConsulta), acc_total, arq);
		fclose(arq);

		do{
            system("cls");
            printf("\t-----------YAMOTO-------------\n");
            printf("\n\nDigite o CPF do funcionário que deseja alterar (somente numeros): ");
            fgets(funConsulta.cpf, 13, stdin);
            funConsulta.cpf[strcspn(funConsulta.cpf, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(funConsulta.cpf, acc_list[i].cpf) == 0){
                    memcpy(&funConsulta, &acc_list[i], sizeof(funConsulta));
                    funExists = 1;
                    tempPosicao = i;
                    break;
                }
            }

            if(funExists == 0){
                printf("Funcionário não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                char a;
                do{
                    a = getch();
                    fflush(stdin);
                    if(a == 32){
                        menuLogado();
                    }
                }while(a != 32 && a != 13);
            }else{
                printf("\n\n\nFuncionário encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nUnidade: %d\nDepartamento: %s\n\n",
                       funConsulta.nome, funConsulta.cpf, funConsulta.dataNascimento.dd, funConsulta.dataNascimento.mm, funConsulta.dataNascimento.yy,
                       funConsulta.email, funConsulta.unidade, funConsulta.departamento
                );

                printf("\n\nRealize as alterações necessárias.\n");

                memset(&user_cad, 0, sizeof(user_cad));

                char escolha;

                strcpy(user_cad.cpf, funConsulta.cpf);

                printf("\nDeseja editar Nome? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nNome Completo: ");
                    fgets(user_cad.nome, SIZE, stdin);
                    user_cad.nome[strcspn(user_cad.nome, "\n")] = 0;
                    fflush(stdin);
                }else strcpy(user_cad.nome, funConsulta.nome);

                printf("\nDeseja editar Data de Nascimento? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nData de Nascimento (DD/MM/AAAA): ");
                    scanf("%d/%d/%d", &user_cad.dataNascimento.dd, &user_cad.dataNascimento.mm, &user_cad.dataNascimento.yy);
                    fflush(stdin);
                }else memcpy(&user_cad.dataNascimento, &funConsulta.dataNascimento, sizeof(user_cad.dataNascimento));

                printf("\nDeseja editar E-mail? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nE-mail: ");
                    fgets(user_cad.email, SIZE, stdin);
                    user_cad.email[strcspn(user_cad.email, "\n")] = 0;
                    fflush(stdin);
                }else strcpy(user_cad.email, funConsulta.email);

                printf("\nDeseja editar Unidade? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    do{
                        printf("Unidade: ");
                        scanf("%d", &user_cad.unidade);
                        fflush(stdin);

                        if(user_cad.unidade < 1 || user_cad.unidade > 3){
                            printf("\nUnidade inválida, escolha apenas uma entre as unidades 1, 2 e 3. Pressione qualquer tecla para tentar novamente...\n\n");
                            getch();
                            fflush(stdin);
                        }
                    }while(user_cad.unidade < 1 || user_cad.unidade > 3);
                }else user_cad.unidade = funConsulta.unidade;

                printf("\nDeseja editar Departamento? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nDepartamento: ");
                    fgets(user_cad.departamento, SIZE, stdin);
                    user_cad.departamento[strcspn(user_cad.departamento, "\n")] = 0;
                    fflush(stdin);
                }else strcpy(user_cad.departamento, funConsulta.departamento);

                printf("\nDeseja editar Senha? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nSenha: ");
                    fgets(user_cad.senha, SIZE, stdin);
                    user_cad.senha[strcspn(user_cad.senha, "\n")] = 0;
                    fflush(stdin);
                }else strcpy(user_cad.senha, funConsulta.senha);

                acc_list[tempPosicao] = user_cad;
                FILE* arqDest = fopen(ARQFUNCIONARIOS, "wb");
                fwrite(acc_list, sizeof(*acc_list), acc_total, arqDest);
                fclose(arqDest);

                printf("\nFuncionário alterado com sucesso. Pressione qualquer tecla para continuar...");
            }
		}while(funExists == 0);

		free(acc_list);
	}
	getch();
    fflush(stdin);
	menuLogado();
}

void alteraMedico(){
    FILE* arq;
	Medico *acc_list, medConsulta, user_cad;
	int acc_total = 0, arq_size = 0, i, medExists = 0, tempPosicao;

	memset(&medConsulta, 0, sizeof(medConsulta));

	arq = fopen(ARQMEDICOS, "rb");

	if(arq == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arq, 0, SEEK_END);
		arq_size = ftell(arq);
		acc_total = arq_size / sizeof(medConsulta);
		rewind(arq);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(medConsulta), acc_total, arq);
		fclose(arq);

		do{
            system("cls");
            printf("\t-----------YAMOTO-------------\n");
            printf("\n\nDigite o CPF do médico que deseja alterar (somente numeros): ");
            fgets(medConsulta.cpf, 13, stdin);
            medConsulta.cpf[strcspn(medConsulta.cpf, "\n")] = 0;
            fflush(stdin);

            for(i = 0; i < acc_total; i++){
                if(strcmp(medConsulta.cpf, acc_list[i].cpf) == 0){
                    memcpy(&medConsulta, &acc_list[i], sizeof(medConsulta));
                    medExists = 1;
                    tempPosicao = i;
                    break;
                }
            }

            if(medExists == 0){
                printf("Médico não encontrado. Pressione ESPAÇO para voltar ao menu anterior ou ENTER para tentar novamente.");
                char a;
                do{
                    a = getch();
                    fflush(stdin);
                    if(a == 32){
                        menuLogado();
                    }
                }while(a != 32 && a != 13);
            }
            else{
                printf("\n\n\nMédico encontrado!");
                printf("\n\nNome: %s\nCPF: %s\nData de Nascimento: %d/%d/%d\nE-mail: %s\nUnidade: %d\nCRM: %s\nEspecialidade: %s\n\n",
                       medConsulta.nome, medConsulta.cpf, medConsulta.dataNascimento.dd, medConsulta.dataNascimento.mm, medConsulta.dataNascimento.yy,
                       medConsulta.email, medConsulta.unidade, medConsulta.crm, medConsulta.especialidade
                );

                printf("\n\nRealize as alterações necessárias.\n");

                memset(&user_cad, 0, sizeof(user_cad));

                char escolha;

                strcpy(user_cad.cpf, medConsulta.cpf);

                printf("\nDeseja editar Nome? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nNome Completo: ");
                    fgets(user_cad.nome, SIZE, stdin);
                    user_cad.nome[strcspn(user_cad.nome, "\n")] = 0;
                    fflush(stdin);
                }else strcpy(user_cad.nome, medConsulta.nome);

                printf("\nDeseja editar Data de Nascimento? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nData de Nascimento (DD/MM/AAAA): ");
                    scanf("%d/%d/%d", &user_cad.dataNascimento.dd, &user_cad.dataNascimento.mm, &user_cad.dataNascimento.yy);
                    fflush(stdin);
                }else memcpy(&user_cad.dataNascimento, &medConsulta.dataNascimento, sizeof(user_cad.dataNascimento));

                printf("\nDeseja editar E-mail? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nE-mail: ");
                    fgets(user_cad.email, SIZE, stdin);
                    user_cad.email[strcspn(user_cad.email, "\n")] = 0;
                    fflush(stdin);
                }else strcpy(user_cad.email, medConsulta.email);

                printf("\nDeseja editar Unidade? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    do{
                        printf("Unidade: ");
                        scanf("%d", &user_cad.unidade);
                        fflush(stdin);

                        if(user_cad.unidade < 1 || user_cad.unidade > 3){
                            printf("\nUnidade inválida, escolha apenas uma entre as unidades 1, 2 e 3. Pressione qualquer tecla para tentar novamente...\n\n");
                            getch();
                            fflush(stdin);
                        }
                    }while(user_cad.unidade < 1 || user_cad.unidade > 3);
                }else user_cad.unidade = medConsulta.unidade;

                printf("\nDeseja editar CRM? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nDepartamento: ");
                    fgets(user_cad.crm, SIZE, stdin);
                    user_cad.crm[strcspn(user_cad.crm, "\n")] = 0;
                    fflush(stdin);
                }else strcpy(user_cad.crm, medConsulta.crm);

                printf("\nDeseja editar Especialidade? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nEspecialidade: ");
                    fgets(user_cad.especialidade, SIZE, stdin);
                    user_cad.especialidade[strcspn(user_cad.especialidade, "\n")] = 0;
                    fflush(stdin);
                }else strcpy(user_cad.especialidade, medConsulta.especialidade);

                printf("\nDeseja editar Senha? (s/n)");
                escolha = getch();
                fflush(stdin);
                if(escolha == 's' || escolha == 'S'){
                    printf("\nSenha: ");
                    fgets(user_cad.senha, SIZE, stdin);
                    user_cad.senha[strcspn(user_cad.senha, "\n")] = 0;
                    fflush(stdin);
                }else strcpy(user_cad.senha, medConsulta.senha);

                acc_list[tempPosicao] = user_cad;
                FILE* arqDest = fopen(ARQMEDICOS, "wb");
                fwrite(acc_list, sizeof(*acc_list), acc_total, arqDest);
                fclose(arqDest);

                printf("\nMédico alterado com sucesso. Pressione qualquer tecla para continuar...");
            }
		}while(medExists == 0);

		free(acc_list);
	}
	getch();
    fflush(stdin);
	menuLogado();
}

void menuRelatorios(){
    system("cls");
	printf("\n\t\tYAMOTO - Relatórios\n\n");
	printf("\t==========================\n");

	printf("\nDigite a opção desejada:\n\n");
	printf("1 - Pacientes\n");
    printf("2 - Atendimentos\n");
	printf("3 - Faturamento\n");
	printf("\n9 - Voltar ao menu\n");

	char opcao;
	opcao = getch();
	fflush(stdin);

	switch(opcao){
		case '1':
			system("cls");
            printf("\n\t\tYAMOTO - Relatórios\n\n");
            printf("\t==========================\n");

            printf("\nDigite a opção desejada:\n\n");
            printf("1 - Lista de Pacientes\n");
            printf("2 - Paciente que mais gastou\n");
            printf("3 - Paciente que mais consultou\n");
            printf("\n9 - Voltar ao menu\n");

            opcao = getch();
            fflush(stdin);

            switch(opcao){
                case '1':
                    system("cls");
                    printf("\n\t\tYAMOTO - Relatórios\n\n");
                    printf("\t==========================\n");

                    printf("\nDigite a opção desejada:\n\n");
                    printf("1 - Unidade 1\n");
                    printf("2 - Unidade 2\n");
                    printf("3 - Unidade 3\n");
                    printf("4 - Todas as Unidades\n");
                    printf("\n9 - Voltar ao menu\n");

                    opcao = getch();
                    fflush(stdin);

                    switch(opcao){
                        case '1':
                            relatPaciUnd(1);
                            break;
                        case '2':
                            relatPaciUnd(2);
                            break;
                        case '3':
                            relatPaciUnd(3);
                            break;
                        case '4':
                            relatPaciRede();
                            break;
                        case '9':
                            menuLogado();
                            break;
                        default:
                            printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
                            getch();
                            fflush(stdin);
                            menuRelatorios();
                            break;
                    }
                    break;
                case '2':
                    system("cls");
                    printf("\n\t\tYAMOTO - Relatórios\n\n");
                    printf("\t==========================\n");

                    printf("\nDigite a opção desejada:\n\n");
                    printf("1 - Unidade 1\n");
                    printf("2 - Unidade 2\n");
                    printf("3 - Unidade 3\n");
                    printf("4 - Todas as Unidades\n");
                    printf("\n9 - Voltar ao menu\n");

                    opcao = getch();
                    fflush(stdin);

                    switch(opcao){
                        case '1':
                            relatPaciGastouMaisUnd(1);
                            break;
                        case '2':
                            relatPaciGastouMaisUnd(2);
                            break;
                        case '3':
                            relatPaciGastouMaisUnd(3);
                            break;
                        case '4':
                            relatPaciGastouMaisRede();
                            break;
                        case '9':
                            menuLogado();
                            break;
                        default:
                            printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
                            getch();
                            fflush(stdin);
                            menuRelatorios();
                            break;
                    }
                    break;
                case '3':
                    system("cls");
                    printf("\n\t\tYAMOTO - Relatórios\n\n");
                    printf("\t==========================\n");

                    printf("\nDigite a opção desejada:\n\n");
                    printf("1 - Unidade 1\n");
                    printf("2 - Unidade 2\n");
                    printf("3 - Unidade 3\n");
                    printf("4 - Todas as Unidades\n");
                    printf("\n9 - Voltar ao menu\n");

                    opcao = getch();
                    fflush(stdin);

                    switch(opcao){
                        case '1':
                            relatPaciMaisConsultaUnd(1);
                            break;
                        case '2':
                            relatPaciMaisConsultaUnd(2);
                            break;
                        case '3':
                            relatPaciMaisConsultaUnd(3);
                            break;
                        case '4':
                            relatPaciMaisConsultaRede();
                            break;
                        case '9':
                            menuLogado();
                            break;
                        default:
                            printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
                            getch();
                            fflush(stdin);
                            menuRelatorios();
                            break;
                    }
                    break;
                case '9':
                    menuLogado();
                    break;
                default:
                    printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
                    getch();
                    fflush(stdin);
                    menuRelatorios();
                    break;
            }

			break;
		case '2':
            relatAtendUnd();
            break;
		case '3':
            system("cls");
            printf("\n\t\tYAMOTO - Relatórios\n\n");
            printf("\t==========================\n");

            printf("\nDigite a opção desejada:\n\n");
            printf("1 - Diário\n");
            printf("2 - Mensal\n");
            printf("3 - Total\n");
            printf("\n9 - Voltar ao menu\n");

            opcao = getch();
            fflush(stdin);

            switch(opcao){
                case '1':
                    relatFaturDiarioRede();
                    break;
                case '2':
                    relatFaturMensalRede();
                    break;
                case '3':
                    relatFaturTotalRede();
                    break;
                case '9':
                    menuLogado();
                    break;
                default:
                    printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
                    getch();
                    fflush(stdin);
                    menuRelatorios();
                    break;
            }
		case '9':
            menuLogado();
			break;
	  	default:
	   		printf("Opção inválida. Pressione qualquer tecla para continuar...\n");
			getch();
			fflush(stdin);
			menuRelatorios();
			break;
	 }
}

void relatPaciUnd(int unidade){
    system("cls");
    printf("\n\tYAMOTO - Lista de Pacientes\n\n");
    printf("\t==========================\n");
    printf("\n\tUNIDADE %d\n\n", unidade);

    FILE *arqPaci, *arqAge;
	Paciente *acc_list, accConsulta;
	Agendamento *age_list, ageConsulta;
	int acc_total = 0, arq_size = 0, accExists = 0, age_total = 0, ageExists = 0, totalPacientes = 0;

	arqAge = fopen(ARQAGEND, "rb");

	if(arqAge == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqAge, 0, SEEK_END);
		arq_size = ftell(arqAge);
		age_total = arq_size / sizeof(ageConsulta);
		rewind(arqAge);
		age_list = malloc(arq_size);
		fread(age_list, sizeof(ageConsulta), age_total, arqAge);
		fclose(arqAge);
	}

    arqPaci = fopen(ARQPACIENTES, "rb");

	if(arqPaci == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqPaci, 0, SEEK_END);
		arq_size = ftell(arqPaci);
		acc_total = arq_size / sizeof(accConsulta);
		rewind(arqPaci);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(accConsulta), acc_total, arqPaci);
		fclose(arqPaci);
	}

    for(int i = 0; i < acc_total; i++){
        int validacaoCPF = 0;
        validacaoCPF = validaCPF(acc_list[i].cpf);

        if(validacaoCPF){
            if(acc_list[i].unidade == unidade){
                int totalConsultas = 0;

                for(int j = 0; j < age_total; j++){
                    if(strcmp(acc_list[i].cpf, age_list[j].pacienteCPF) == 0){
                        acc_list[i].valorTotalGasto += age_list[j].valor;

                        if(age_list[j].unidade == 1) acc_list[i].freqUnd1++;
                        else if(age_list[j].unidade == 2) acc_list[i].freqUnd2++;
                        else if(age_list[j].unidade == 3) acc_list[i].freqUnd3++;

                        totalConsultas++;
                        totalPacientes++;
                    }//end if
                }//end for

                printf("Nome: %s", acc_list[i].nome);
                printf("\nCPF: %s", acc_list[i].cpf);
                printf("\nData de Nascimento: %d/%d/%d", acc_list[i].dataNascimento.dd, acc_list[i].dataNascimento.mm, acc_list[i].dataNascimento.yy);
                printf("\nQuantidade total de consultas realizadas: %d", totalConsultas);

                //verifica qual unidade mais frequentada
                if(!(acc_list[i].freqUnd1 == 0 && acc_list[i].freqUnd2 == 0 && acc_list[i].freqUnd3 == 0)){
                    if(acc_list[i].freqUnd1 > acc_list[i].freqUnd2){
                        if(acc_list[i].freqUnd1 > acc_list[i].freqUnd3) printf("\nUnidade mais frequentada: 1");
                        else printf("\nUnidade mais frequentada: 3");
                    }else if(acc_list[i].freqUnd2 > acc_list[i].freqUnd3) printf("\nUnidade mais frequentada: 2");
                    else printf("\nUnidade mais frequentada: 3");
                }
                //fim verifica
                printf("\nValor total pago: R$ %.2f\n\n", acc_list[i].valorTotalGasto);

                acc_list[i].freqUnd1 = 0;
                acc_list[i].freqUnd2 = 0;
                acc_list[i].freqUnd3 = 0;
                acc_list[i].valorTotalGasto = 0;
            }//end if
        }//end if
    }//end for
printf("Pressione qualquer tecla para voltar ao menu");
    getch();
    fflush(stdin);
    menuLogado();
}

void relatPaciRede(){
    system("cls");
    printf("\n\tYAMOTO - Lista de Pacientes\n\n");
    printf("\t==========================\n");
    printf("\n\tTODAS AS UNIDADES\n\n");

    FILE *arqPaci, *arqAge;
	Paciente *acc_list, accConsulta;
	Agendamento *age_list, ageConsulta;
	int acc_total = 0, arq_size = 0, accExists = 0, age_total = 0, ageExists = 0, totalPacientes = 0;

	arqAge = fopen(ARQAGEND, "rb");

	if(arqAge == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqAge, 0, SEEK_END);
		arq_size = ftell(arqAge);
		age_total = arq_size / sizeof(ageConsulta);
		rewind(arqAge);
		age_list = malloc(arq_size);
		fread(age_list, sizeof(ageConsulta), age_total, arqAge);
		fclose(arqAge);
	}

    arqPaci = fopen(ARQPACIENTES, "rb");

	if(arqPaci == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqPaci, 0, SEEK_END);
		arq_size = ftell(arqPaci);
		acc_total = arq_size / sizeof(accConsulta);
		rewind(arqPaci);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(accConsulta), acc_total, arqPaci);
		fclose(arqPaci);
	}

    for(int i = 0; i < acc_total; i++){
        int validacaoCPF = 0;
        validacaoCPF = validaCPF(acc_list[i].cpf);

        if(validacaoCPF){
            int totalConsultas = 0;

            for(int j = 0; j < age_total; j++){
                if(strcmp(acc_list[i].cpf, age_list[j].pacienteCPF) == 0){
                    acc_list[i].valorTotalGasto += age_list[j].valor;

                    if(age_list[j].unidade == 1) acc_list[i].freqUnd1++;
                    else if(age_list[j].unidade == 2) acc_list[i].freqUnd2++;
                    else if(age_list[j].unidade == 3) acc_list[i].freqUnd3++;

                    totalConsultas++;
                    totalPacientes++;
                }//end if
            }//end for

            printf("Nome: %s", acc_list[i].nome);
            printf("\nCPF: %s", acc_list[i].cpf);
            printf("\nData de Nascimento: %d/%d/%d", acc_list[i].dataNascimento.dd, acc_list[i].dataNascimento.mm, acc_list[i].dataNascimento.yy);
            printf("\nQuantidade total de consultas realizadas: %d", totalConsultas);

            //verifica qual unidade mais frequentada
            if(!(acc_list[i].freqUnd1 == 0 && acc_list[i].freqUnd2 == 0 && acc_list[i].freqUnd3 == 0)){
                if(acc_list[i].freqUnd1 > acc_list[i].freqUnd2){
                    if(acc_list[i].freqUnd1 > acc_list[i].freqUnd3) printf("\nUnidade mais frequentada: 1");
                    else printf("\nUnidade mais frequentada: 3");
                }else if(acc_list[i].freqUnd2 > acc_list[i].freqUnd3) printf("\nUnidade mais frequentada: 2");
                else printf("\nUnidade mais frequentada: 3");
            }
            //fim verifica
            printf("\nValor total pago: R$ %.2f\n\n", acc_list[i].valorTotalGasto);

            acc_list[i].freqUnd1 = 0;
            acc_list[i].freqUnd2 = 0;
            acc_list[i].freqUnd3 = 0;
            acc_list[i].valorTotalGasto = 0;
        }//end if
    }//end for
    printf("Presione qualquer tecla para voltar ao menu");
    getch();
    fflush(stdin);
    menuLogado();
}

void relatPaciMaisConsultaUnd(int unidade){
    system("cls");
    printf("\n\tYAMOTO - Paciente que Mais Consultou\n\n");
    printf("\t==========================\n");
    printf("\n\tUNIDADE %d\n\n", unidade);

    FILE *arqPaci, *arqAge;
	Paciente *acc_list, accConsulta;
	Agendamento *age_list, ageConsulta;
	int acc_total = 0, arq_size = 0, accExists = 0, age_total = 0, ageExists = 0, totalPacientes = 0;

	memset(&accConsulta, 0, sizeof(accConsulta));
	accConsulta.freqUnd1 = 0;
	accConsulta.freqUnd2 = 0;
	accConsulta.freqUnd3 = 0;

	arqAge = fopen(ARQAGEND, "rb");

	if(arqAge == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqAge, 0, SEEK_END);
		arq_size = ftell(arqAge);
		age_total = arq_size / sizeof(ageConsulta);
		rewind(arqAge);
		age_list = malloc(arq_size);
		fread(age_list, sizeof(ageConsulta), age_total, arqAge);
		fclose(arqAge);
	}

    arqPaci = fopen(ARQPACIENTES, "rb");

	if(arqPaci == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqPaci, 0, SEEK_END);
		arq_size = ftell(arqPaci);
		acc_total = arq_size / sizeof(accConsulta);
		rewind(arqPaci);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(accConsulta), acc_total, arqPaci);
		fclose(arqPaci);
	}

    for(int i = 0; i < acc_total; i++){
        int validacaoCPF = 0;
        validacaoCPF = validaCPF(acc_list[i].cpf);

        if(validacaoCPF){
            if(acc_list[i].unidade == unidade){
                 for(int j = 0; j < age_total; j++){
                    if(strcmp(acc_list[i].cpf, age_list[j].pacienteCPF) == 0){
                        acc_list[i].valorTotalGasto += age_list[j].valor;

                        if(age_list[j].unidade == 1) acc_list[i].freqUnd1++;
                        else if(age_list[j].unidade == 2) acc_list[i].freqUnd2++;
                        else if(age_list[j].unidade == 3) acc_list[i].freqUnd3++;
                    }//end if
                }//end for

                if((acc_list[i].freqUnd1 + acc_list[i].freqUnd2 + acc_list[i].freqUnd3) > (accConsulta.freqUnd1 + accConsulta.freqUnd2 + accConsulta.freqUnd3)){
                    memcpy(&accConsulta, &acc_list[i], sizeof(accConsulta));
                }

                acc_list[i].freqUnd1 = 0;
                acc_list[i].freqUnd2 = 0;
                acc_list[i].freqUnd3 = 0;
                acc_list[i].valorTotalGasto = 0;
            }//end if
        }//end if
    }//end for

    if(accConsulta.freqUnd1 + accConsulta.freqUnd2 + accConsulta.freqUnd3 < 1){
        printf("Os pacientes dessa unidade ainda não realizaram consultas.\n\n");
    }else{
        printf("Nome: %s\n", accConsulta.nome);
        printf("CPF: %s\n", accConsulta.cpf);
        printf("Data de Nascimento: %d/%d/%d\n", accConsulta.dataNascimento.dd, accConsulta.dataNascimento.mm, accConsulta.dataNascimento.yy);
        printf("E-mail: %s\n", accConsulta.email);
        printf("Endereço: %s\n", accConsulta.endereco);
        printf("Unidade em que foi cadastrado: %d\n", accConsulta.unidade);
        printf("Consultas na Unidade 1: %d\n", accConsulta.freqUnd1);
        printf("Consultas na Unidade 2: %d\n", accConsulta.freqUnd2);
        printf("Consultas na Unidade 3: %d\n", accConsulta.freqUnd3);
        printf("Total de Consultas: %d\n", accConsulta.freqUnd1 + accConsulta.freqUnd2 + accConsulta.freqUnd3);
        printf("Valor Total Pago: R$ %.2f\n\n", accConsulta.valorTotalGasto);
    }
    printf("Pressione qualquer tecla para voltar ao menu...");
    getch();
    fflush(stdin);
    menuLogado();
}

void relatPaciMaisConsultaRede(){
    system("cls");
    printf("\n\tYAMOTO - Paciente que Mais Consultou\n\n");
    printf("\t==========================\n");
    printf("\n\tTODAS AS UNIDADES\n\n");

    FILE *arqPaci, *arqAge;
	Paciente *acc_list, accConsulta;
	Agendamento *age_list, ageConsulta;
	int acc_total = 0, arq_size = 0, accExists = 0, age_total = 0, ageExists = 0, totalPacientes = 0;

	memset(&accConsulta, 0, sizeof(accConsulta));
	accConsulta.freqUnd1 = 0;
	accConsulta.freqUnd2 = 0;
	accConsulta.freqUnd3 = 0;

	arqAge = fopen(ARQAGEND, "rb");

	if(arqAge == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqAge, 0, SEEK_END);
		arq_size = ftell(arqAge);
		age_total = arq_size / sizeof(ageConsulta);
		rewind(arqAge);
		age_list = malloc(arq_size);
		fread(age_list, sizeof(ageConsulta), age_total, arqAge);
		fclose(arqAge);
	}

    arqPaci = fopen(ARQPACIENTES, "rb");

	if(arqPaci == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqPaci, 0, SEEK_END);
		arq_size = ftell(arqPaci);
		acc_total = arq_size / sizeof(accConsulta);
		rewind(arqPaci);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(accConsulta), acc_total, arqPaci);
		fclose(arqPaci);
	}

    for(int i = 0; i < acc_total; i++){
        int validacaoCPF = 0;
        validacaoCPF = validaCPF(acc_list[i].cpf);

        if(validacaoCPF){
             for(int j = 0; j < age_total; j++){
                if(strcmp(acc_list[i].cpf, age_list[j].pacienteCPF) == 0){
                    acc_list[i].valorTotalGasto += age_list[j].valor;

                    if(age_list[j].unidade == 1) acc_list[i].freqUnd1++;
                    else if(age_list[j].unidade == 2) acc_list[i].freqUnd2++;
                    else if(age_list[j].unidade == 3) acc_list[i].freqUnd3++;
                }//end if
            }//end for

            if((acc_list[i].freqUnd1 + acc_list[i].freqUnd2 + acc_list[i].freqUnd3) > (accConsulta.freqUnd1 + accConsulta.freqUnd2 + accConsulta.freqUnd3)){
                memcpy(&accConsulta, &acc_list[i], sizeof(accConsulta));
            }

            acc_list[i].freqUnd1 = 0;
            acc_list[i].freqUnd2 = 0;
            acc_list[i].freqUnd3 = 0;
            acc_list[i].valorTotalGasto = 0;
        }//end if
    }//end for

    if(accConsulta.freqUnd1 + accConsulta.freqUnd2 + accConsulta.freqUnd3 < 1){
        printf("Os pacientes da clínica ainda não realizaram consultas.\n\n");
    }else{
        printf("Nome: %s\n", accConsulta.nome);
        printf("CPF: %s\n", accConsulta.cpf);
        printf("Data de Nascimento: %d/%d/%d\n", accConsulta.dataNascimento.dd, accConsulta.dataNascimento.mm, accConsulta.dataNascimento.yy);
        printf("E-mail: %s\n", accConsulta.email);
        printf("Endereço: %s\n", accConsulta.endereco);
        printf("Unidade em que foi cadastrado: %d\n", accConsulta.unidade);
        printf("Consultas na Unidade 1: %d\n", accConsulta.freqUnd1);
        printf("Consultas na Unidade 2: %d\n", accConsulta.freqUnd2);
        printf("Consultas na Unidade 3: %d\n", accConsulta.freqUnd3);
        printf("Total de Consultas: %d\n", accConsulta.freqUnd1 + accConsulta.freqUnd2 + accConsulta.freqUnd3);
        printf("Valor Total Pago: R$ %.2f\n\n", accConsulta.valorTotalGasto);
    }
    printf("Pressione qualquer tecla para voltar ao menu...");
    getch();
    fflush(stdin);
    menuLogado();
}

void relatPaciGastouMaisUnd(int unidade){
    system("cls");
    printf("\n\tYAMOTO - Paciente que Mais Gastou\n\n");
    printf("\t==========================\n");
    printf("\n\tUNIDADE %d\n\n", unidade);

    FILE *arqPaci, *arqAge;
	Paciente *acc_list, accConsulta;
	Agendamento *age_list, ageConsulta;
	int acc_total = 0, arq_size = 0, accExists = 0, age_total = 0, ageExists = 0, totalPacientes = 0;

	memset(&accConsulta, 0, sizeof(accConsulta));
	accConsulta.valorTotalGasto = 0;

	arqAge = fopen(ARQAGEND, "rb");

	if(arqAge == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqAge, 0, SEEK_END);
		arq_size = ftell(arqAge);
		age_total = arq_size / sizeof(ageConsulta);
		rewind(arqAge);
		age_list = malloc(arq_size);
		fread(age_list, sizeof(ageConsulta), age_total, arqAge);
		fclose(arqAge);
	}

    arqPaci = fopen(ARQPACIENTES, "rb");

	if(arqPaci == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqPaci, 0, SEEK_END);
		arq_size = ftell(arqPaci);
		acc_total = arq_size / sizeof(accConsulta);
		rewind(arqPaci);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(accConsulta), acc_total, arqPaci);
		fclose(arqPaci);
	}

    for(int i = 0; i < acc_total; i++){
        int validacaoCPF = 0;
        validacaoCPF = validaCPF(acc_list[i].cpf);

        if(validacaoCPF){
            if(acc_list[i].unidade == unidade){
                 for(int j = 0; j < age_total; j++){
                    if(strcmp(acc_list[i].cpf, age_list[j].pacienteCPF) == 0){
                        acc_list[i].valorTotalGasto += age_list[j].valor;

                        if(age_list[j].unidade == 1) acc_list[i].freqUnd1++;
                        else if(age_list[j].unidade == 2) acc_list[i].freqUnd2++;
                        else if(age_list[j].unidade == 3) acc_list[i].freqUnd3++;
                    }//end if
                }//end for

                if(acc_list[i].valorTotalGasto > accConsulta.valorTotalGasto){
                    memcpy(&accConsulta, &acc_list[i], sizeof(accConsulta));
                }

                acc_list[i].freqUnd1 = 0;
                acc_list[i].freqUnd2 = 0;
                acc_list[i].freqUnd3 = 0;
                acc_list[i].valorTotalGasto = 0;
            }//end if
        }//end if
    }//end for

    if(accConsulta.freqUnd1 + accConsulta.freqUnd2 + accConsulta.freqUnd3 < 1){
        printf("Os pacientes dessa unidade ainda não realizaram consultas.\n\n");
    }else{
        printf("Nome: %s\n", accConsulta.nome);
        printf("CPF: %s\n", accConsulta.cpf);
        printf("Data de Nascimento: %d/%d/%d\n", accConsulta.dataNascimento.dd, accConsulta.dataNascimento.mm, accConsulta.dataNascimento.yy);
        printf("E-mail: %s\n", accConsulta.email);
        printf("Endereço: %s\n", accConsulta.endereco);
        printf("Unidade em que foi cadastrado: %d\n", accConsulta.unidade);
        printf("Consultas na Unidade 1: %d\n", accConsulta.freqUnd1);
        printf("Consultas na Unidade 2: %d\n", accConsulta.freqUnd2);
        printf("Consultas na Unidade 3: %d\n", accConsulta.freqUnd3);
        printf("Total de Consultas: %d\n", accConsulta.freqUnd1 + accConsulta.freqUnd2 + accConsulta.freqUnd3);
        printf("Valor Total Pago: R$ %.2f\n\n", accConsulta.valorTotalGasto);
    }
    printf("Pressione qualquer tecla para voltar ao menu...");
    getch();
    fflush(stdin);
    menuLogado();
}

void relatPaciGastouMaisRede(){
    system("cls");
    printf("\n\tYAMOTO - Paciente que Mais Gastou\n\n");
    printf("\t==========================\n");
    printf("\n\tTODAS AS UNIDADES\n\n");

    FILE *arqPaci, *arqAge;
	Paciente *acc_list, accConsulta;
	Agendamento *age_list, ageConsulta;
	int acc_total = 0, arq_size = 0, accExists = 0, age_total = 0, ageExists = 0, totalPacientes = 0;

	memset(&accConsulta, 0, sizeof(accConsulta));
	accConsulta.valorTotalGasto = 0;

	arqAge = fopen(ARQAGEND, "rb");

	if(arqAge == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqAge, 0, SEEK_END);
		arq_size = ftell(arqAge);
		age_total = arq_size / sizeof(ageConsulta);
		rewind(arqAge);
		age_list = malloc(arq_size);
		fread(age_list, sizeof(ageConsulta), age_total, arqAge);
		fclose(arqAge);
	}

    arqPaci = fopen(ARQPACIENTES, "rb");

	if(arqPaci == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqPaci, 0, SEEK_END);
		arq_size = ftell(arqPaci);
		acc_total = arq_size / sizeof(accConsulta);
		rewind(arqPaci);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(accConsulta), acc_total, arqPaci);
		fclose(arqPaci);
	}

    for(int i = 0; i < acc_total; i++){
        int validacaoCPF = 0;
        validacaoCPF = validaCPF(acc_list[i].cpf);

        if(validacaoCPF){
            for(int j = 0; j < age_total; j++){
                if(strcmp(acc_list[i].cpf, age_list[j].pacienteCPF) == 0){
                    acc_list[i].valorTotalGasto += age_list[j].valor;

                    if(age_list[j].unidade == 1) acc_list[i].freqUnd1++;
                    else if(age_list[j].unidade == 2) acc_list[i].freqUnd2++;
                    else if(age_list[j].unidade == 3) acc_list[i].freqUnd3++;
                }//end if
            }//end for

            if(acc_list[i].valorTotalGasto > accConsulta.valorTotalGasto){
                memcpy(&accConsulta, &acc_list[i], sizeof(accConsulta));
            }

            acc_list[i].freqUnd1 = 0;
            acc_list[i].freqUnd2 = 0;
            acc_list[i].freqUnd3 = 0;
            acc_list[i].valorTotalGasto = 0;
        }//end if
    }//end for

    if(accConsulta.freqUnd1 + accConsulta.freqUnd2 + accConsulta.freqUnd3 < 1){
        printf("Os pacientes da clínica ainda não realizaram consultas.\n\n");
    }else{
        printf("Nome: %s\n", accConsulta.nome);
        printf("CPF: %s\n", accConsulta.cpf);
        printf("Data de Nascimento: %d/%d/%d\n", accConsulta.dataNascimento.dd, accConsulta.dataNascimento.mm, accConsulta.dataNascimento.yy);
        printf("E-mail: %s\n", accConsulta.email);
        printf("Endereço: %s\n", accConsulta.endereco);
        printf("Unidade em que foi cadastrado: %d\n", accConsulta.unidade);
        printf("Consultas na Unidade 1: %d\n", accConsulta.freqUnd1);
        printf("Consultas na Unidade 2: %d\n", accConsulta.freqUnd2);
        printf("Consultas na Unidade 3: %d\n", accConsulta.freqUnd3);
        printf("Total de Consultas: %d\n", accConsulta.freqUnd1 + accConsulta.freqUnd2 + accConsulta.freqUnd3);
        printf("Valor Total Pago: R$ %.2f\n\n", accConsulta.valorTotalGasto);
    }
    printf("Pressione qualquer tecla para voltar ao menu...");
    getch();
    fflush(stdin);
    menuLogado();
}

void relatAtendUnd(){
    system("cls");
    printf("\n\tYAMOTO - Total de Atendimentos por Unidade\n\n");
    printf("\t==========================\n");

    FILE *arqPaci, *arqAge;
	Paciente *acc_list, accConsulta;
	Agendamento *age_list, ageConsulta;
	Unidade unidades[3];
	int acc_total = 0, arq_size = 0, accExists = 0, age_total = 0, ageExists = 0, totalPacientes = 0;

    for(int i = 0; i < 3; i++){
        unidades[i].id = i+1;
        unidades[i].atendimentos = 0;
        unidades[i].pacientes = 0;
        unidades[i].faturamento = 0;
    }

	arqAge = fopen(ARQAGEND, "rb");

	if(arqAge == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqAge, 0, SEEK_END);
		arq_size = ftell(arqAge);
		age_total = arq_size / sizeof(ageConsulta);
		rewind(arqAge);
		age_list = malloc(arq_size);
		fread(age_list, sizeof(ageConsulta), age_total, arqAge);
		fclose(arqAge);
	}

    arqPaci = fopen(ARQPACIENTES, "rb");

	if(arqPaci == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqPaci, 0, SEEK_END);
		arq_size = ftell(arqPaci);
		acc_total = arq_size / sizeof(accConsulta);
		rewind(arqPaci);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(accConsulta), acc_total, arqPaci);
		fclose(arqPaci);
	}

	//FOR UNIDADES
	for(int j = 0; j < 3; j++){

        //FOR PACIENTES
        for(int i = 0; i < acc_total; i++){
            int validacaoCPF = 0;
            validacaoCPF = validaCPF(acc_list[i].cpf);

            if(validacaoCPF){
                if(unidades[j].id == acc_list[i].unidade){
                    unidades[j].pacientes++;
                }
            }
        }//end for
        //END PACIENTES

        //FOR AGENDAMENTOS
        for(int i = 0; i < age_total; i++){
            int validacaoCPF = 0;

            validacaoCPF = validaCPF(age_list[i].pacienteCPF);

            if(validacaoCPF){
                if(unidades[j].id == age_list[i].unidade){
                    unidades[j].atendimentos++;
                    unidades[j].faturamento += age_list[i].valor;
                }
            }
        }//end for
        //END AGENDAMENTOS

        printf("\nUnidade: %d", unidades[j].id);
        printf("\nPacientes Cadastrados: %d", unidades[j].pacientes);
        printf("\nTotal de Atendimentos Realizados: %d", unidades[j].atendimentos);
        printf("\nFaturamento Total: R$ %.2f\n\n", unidades[j].faturamento);
    }//end for unidades

    printf("Pressione qualquer tecla para voltar ao menu...");
    getch();
    fflush(stdin);
    menuLogado();
}

void relatFaturDiarioRede(){
    FILE *arqPaci, *arqAge;
	Paciente *acc_list, accConsulta;
	Agendamento *age_list, ageConsulta;
	Unidade unidades[3];
	Data dt;
	int acc_total = 0, arq_size = 0, accExists = 0, age_total = 0, ageExists = 0, totalPacientes = 0, validacaoData = 0;

    for(int i = 0; i < 3; i++){
        unidades[i].id = i+1;
        unidades[i].atendimentos = 0;
        unidades[i].pacientes = 0;
        unidades[i].faturamento = 0;
    }

    arqAge = fopen(ARQAGEND, "rb");

    if(arqAge == NULL) printf("\nErro ao abrir o arquivo.\n\n");
    else{
        fseek(arqAge, 0, SEEK_END);
        arq_size = ftell(arqAge);
        age_total = arq_size / sizeof(ageConsulta);
        rewind(arqAge);
        age_list = malloc(arq_size);
        fread(age_list, sizeof(ageConsulta), age_total, arqAge);
        fclose(arqAge);
    }

    arqPaci = fopen(ARQPACIENTES, "rb");

	if(arqPaci == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqPaci, 0, SEEK_END);
		arq_size = ftell(arqPaci);
		acc_total = arq_size / sizeof(accConsulta);
		rewind(arqPaci);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(accConsulta), acc_total, arqPaci);
		fclose(arqPaci);
	}

    do{
        system("cls");
        printf("\n\tYAMOTO - Faturamento Diário\n\n");
        printf("\t==========================\n");

        printf("\nDigite a data desejada (DD/MM/AAAA): ");
        scanf("%d/%d/%d", &dt.dd, &dt.mm, &dt.yy);
        fflush(stdin);

        validacaoData = validaData(dt);
            if(validacaoData == 0){
                printf("\nData inválida! Pressione qualquer tecla para tentar novamente...");
                getch();
                fflush(stdin);
            }
    }while(validacaoData == 0);

    system("cls");
    printf("\n\tYAMOTO - Faturamento Diário\n\n");
    printf("\t==========================\n");
    printf("\t DIA %d/%d/%d\n", dt.dd, dt.mm, dt.yy);

    //FOR UNIDADES
    for(int j = 0; j < 3; j++){
        //FOR PACIENTES
        for(int i = 0; i < acc_total; i++){
            int validacaoCPF = 0;
            validacaoCPF = validaCPF(acc_list[i].cpf);

            if(validacaoCPF){
                if(unidades[j].id == acc_list[i].unidade){
                    unidades[j].pacientes++;
                }
            }
        }//end for
        //END PACIENTES

        //FOR AGENDAMENTOS
        for(int i = 0; i < age_total; i++){
            int validacaoCPF = 0;

            validacaoCPF = validaCPF(age_list[i].pacienteCPF);

            if(validacaoCPF){
                if(unidades[j].id == age_list[i].unidade){
                    if(dt.dd == age_list[i].data.dd && dt.mm == age_list[i].data.mm && dt.yy == age_list[i].data.yy){
                        unidades[j].atendimentos++;
                        unidades[j].faturamento += age_list[i].valor;
                    }
                }
            }
        }
        //FOR AGENDAMENTOS

        printf("\nUnidade: %d", unidades[j].id);
        printf("\nPacientes Cadastrados: %d", unidades[j].pacientes);
        printf("\nTotal de Atendimentos Realizados: %d", unidades[j].atendimentos);
        printf("\nFaturamento do Dia: R$ %.2f\n\n", unidades[j].faturamento);
    }//end for unidades

    printf("Pressione qualquer tecla para voltar ao menu...");
    getch();
    fflush(stdin);
    menuLogado();
}

void relatFaturMensalRede(){
    FILE *arqPaci, *arqAge;
	Paciente *acc_list, accConsulta;
	Agendamento *age_list, ageConsulta;
	Unidade unidades[3];
	Data dt;
	int acc_total = 0, arq_size = 0, accExists = 0, age_total = 0, ageExists = 0, totalPacientes = 0, validacaoData = 0;

    for(int i = 0; i < 3; i++){
        unidades[i].id = i+1;
        unidades[i].atendimentos = 0;
        unidades[i].pacientes = 0;
        unidades[i].faturamento = 0;
    }

    arqAge = fopen(ARQAGEND, "rb");

    if(arqAge == NULL) printf("\nErro ao abrir o arquivo.\n\n");
    else{
        fseek(arqAge, 0, SEEK_END);
        arq_size = ftell(arqAge);
        age_total = arq_size / sizeof(ageConsulta);
        rewind(arqAge);
        age_list = malloc(arq_size);
        fread(age_list, sizeof(ageConsulta), age_total, arqAge);
        fclose(arqAge);
    }

    arqPaci = fopen(ARQPACIENTES, "rb");

	if(arqPaci == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqPaci, 0, SEEK_END);
		arq_size = ftell(arqPaci);
		acc_total = arq_size / sizeof(accConsulta);
		rewind(arqPaci);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(accConsulta), acc_total, arqPaci);
		fclose(arqPaci);
	}

    do{
        system("cls");
        printf("\n\tYAMOTO - Faturamento Mensal\n\n");
        printf("\t==========================\n");

        printf("\nDigite a data desejada (MM/AAAA): ");
        scanf("%d/%d", &dt.mm, &dt.yy);
        fflush(stdin);

        dt.dd = 10;

        validacaoData = validaData(dt);
            if(validacaoData == 0){
                printf("\nData inválida! Pressione qualquer tecla para tentar novamente...");
                getch();
                fflush(stdin);
            }
    }while(validacaoData == 0);

    system("cls");
    printf("\n\tYAMOTO - Faturamento Diário\n\n");
    printf("\t==========================\n");
    printf("\t MÊS %d/%d\n", dt.mm, dt.yy);

    //FOR UNIDADES
    for(int j = 0; j < 3; j++){
        //FOR PACIENTES
        for(int i = 0; i < acc_total; i++){
            int validacaoCPF = 0;
            validacaoCPF = validaCPF(acc_list[i].cpf);

            if(validacaoCPF){
                if(unidades[j].id == acc_list[i].unidade){
                    unidades[j].pacientes++;
                }
            }
        }//end for
        //END PACIENTES

        //FOR AGENDAMENTOS
        for(int i = 0; i < age_total; i++){
            int validacaoCPF = 0;

            validacaoCPF = validaCPF(age_list[i].pacienteCPF);

            if(validacaoCPF){
                if(unidades[j].id == age_list[i].unidade){
                    if(dt.mm == age_list[i].data.mm && dt.yy == age_list[i].data.yy){
                        unidades[j].atendimentos++;
                        unidades[j].faturamento += age_list[i].valor;
                    }
                }
            }
        }
        //FOR AGENDAMENTOS

        printf("\nUnidade: %d", unidades[j].id);
        printf("\nPacientes Cadastrados: %d", unidades[j].pacientes);
        printf("\nTotal de Atendimentos Realizados no mês: %d", unidades[j].atendimentos);
        printf("\nFaturamento do mês: R$ %.2f\n\n", unidades[j].faturamento);
    }//end for unidades

    printf("Pressione qualquer tecla para voltar ao menu...");
    getch();
    fflush(stdin);
    menuLogado();
}

void relatFaturTotalRede(){
    system("cls");
    printf("\n\tYAMOTO - Faturamento Total\n\n");
    printf("\t==========================\n");

    FILE *arqPaci, *arqAge;
	Paciente *acc_list, accConsulta;
	Agendamento *age_list, ageConsulta;
	Unidade unidades[3];
	int acc_total = 0, arq_size = 0, accExists = 0, age_total = 0, ageExists = 0, totalPacientes = 0;

    for(int i = 0; i < 3; i++){
        unidades[i].id = i+1;
        unidades[i].atendimentos = 0;
        unidades[i].pacientes = 0;
        unidades[i].faturamento = 0;
    }

	arqAge = fopen(ARQAGEND, "rb");

	if(arqAge == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqAge, 0, SEEK_END);
		arq_size = ftell(arqAge);
		age_total = arq_size / sizeof(ageConsulta);
		rewind(arqAge);
		age_list = malloc(arq_size);
		fread(age_list, sizeof(ageConsulta), age_total, arqAge);
		fclose(arqAge);
	}

    arqPaci = fopen(ARQPACIENTES, "rb");

	if(arqPaci == NULL) printf("\nErro ao abrir o arquivo.\n\n");
	else{
		fseek(arqPaci, 0, SEEK_END);
		arq_size = ftell(arqPaci);
		acc_total = arq_size / sizeof(accConsulta);
		rewind(arqPaci);
		acc_list = malloc(arq_size);
		fread(acc_list, sizeof(accConsulta), acc_total, arqPaci);
		fclose(arqPaci);
	}

	//FOR UNIDADES
	for(int j = 0; j < 3; j++){

        //FOR PACIENTES
        for(int i = 0; i < acc_total; i++){
            int validacaoCPF = 0;
            validacaoCPF = validaCPF(acc_list[i].cpf);

            if(validacaoCPF){
                if(unidades[j].id == acc_list[i].unidade){
                    unidades[j].pacientes++;
                }
            }
        }//end for
        //END PACIENTES

        //FOR AGENDAMENTOS
        for(int i = 0; i < age_total; i++){
            int validacaoCPF = 0;

            validacaoCPF = validaCPF(age_list[i].pacienteCPF);

            if(validacaoCPF){
                if(unidades[j].id == age_list[i].unidade){
                    unidades[j].atendimentos++;
                    unidades[j].faturamento += age_list[i].valor;
                }
            }
        }//end for
        //END AGENDAMENTOS

        printf("\nUnidade: %d", unidades[j].id);
        printf("\nPacientes Cadastrados: %d", unidades[j].pacientes);
        printf("\nTotal de Atendimentos Realizados: %d", unidades[j].atendimentos);
        printf("\nFaturamento Total: R$ %.2f\n\n", unidades[j].faturamento);
    }//end for unidades

    printf("Pressione qualquer tecla para voltar ao menu...");
    getch();
    fflush(stdin);
    menuLogado();
}
