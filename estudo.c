#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include<conio.h>
#define SIZE 30


typedef struct{
	int codigo;
	char nome[SIZE];
	char cpf [SIZE];
	char dataNascimento [SIZE];
	char email [SIZE];
	char endereco [SIZE];
	char convenio[SIZE];
	} Paciente;

typedef struct{
	int codigomed;
	char nome[SIZE];
	char cpf [SIZE];
	char dataNascimento [SIZE];
	char email [SIZE];
	char unidade [SIZE];
	char crm [SIZE];
	char especialidade[SIZE];
} Medico ;

typedef struct{
	int codigofunci;
	char nome[SIZE];
	char cpf [SIZE];
	char dataNascimento [SIZE];
	char email [SIZE];
	char unidade [SIZE];
	char matricula [SIZE];
	char departamento [SIZE];
} Funcionario ;

typedef struct{
	int codigoagend;
	char nome [SIZE];
	char codigopaciente[SIZE];
	char especializacao[SIZE];
	char dataAgendamento [SIZE];
	char unidade [SIZE];
	char email [SIZE];
	char cpf [SIZE];
} Agenda ;


void listaPaciente();
void listaMedico();
void listaFuncionario();
void listaAgendamento();
void infoPaciente(Paciente paci);
void infoMedico(Medico medi);
void infoFuncionario(Funcionario funci);
void infoAgenda(Agenda agenda);
void login();
void menu();
void cadastro();
void cadastrarPaciente();
void cadastrarMedico();
void relatorios();
void cadastrarAgendamento();
void reclamacoesElogios();
void consultas();
Medico consultarMedico(int codigomed);
Paciente consultarPaciente(int codigo);
int * temCadastro(int codigo);
void informacoes();
void cadastrarFuncionarios();

static int contador_medico=0;
static int contador_paciente=0;
static int contador_funcionario=0;
static int contador_agenda=0;

static Medico medico[50];
static Paciente paciente [50];
static Funcionario funcionario [50];
static Agenda agenda [50];


 void infoPaciente(Paciente paci){
	printf("Codigo: %d \n Nome: %s \n CPF: %s \n Data de nascimento: %s \n Email: %s \n Endereco: %s \n Convenio: %s \n ", paci.codigo, paci.nome, paci.cpf, paci.dataNascimento, paci.email, paci.endereco, paci.convenio);
}void infoMedico(Medico medi){
	printf("Codigo: %d \n Nome: %s \n CPF: %s \n Data de nascimento: %s \n Email: %s \n Unidade: %s \n CRM: %s \n Especialidade: %s \n ", medi.codigomed,medi.nome, medi.cpf, medi.dataNascimento, medi.email, medi.unidade, medi.crm, medi.especialidade);
}void infoFuncionario(Funcionario funci){
	printf("Codigo: %d \n Nome: %s \n CPF: %s \n Data de nascimento: %s \n Email: %s \n Unidade: %s \n Matricula: %s Departamento %s \n ", funci.codigofunci, funci.nome, funci.cpf, funci.dataNascimento, funci.email, funci.unidade, funci.matricula, funci.departamento);
}void infoAgenda(Agenda agend){
	printf("Codigo: %d \n Nome: %s \n CPF: %s \n Data de agendamento: %f \n Email: %s \n Unidade: %s \n Especializacao: %s Codigo do paciente %s \n ", agend.codigoagend, agend.nome, agend.cpf, agend.dataAgendamento, agend.email, agend.unidade, agend.especializacao, agend.codigopaciente);
}


int main(){
	menu();
	void login();
	
		
	return 0;
}

void menu(){
	
		printf("=================================MEDLIFE============================\n");
		printf("=================================BEM - VINDO============================\n");
		printf("Selecione as opcao no menu \n");
		printf("1- Login\n");
        printf("2- Cadastrar\n");
		printf("3- Consulta\n");
		printf("4- Informacoes\n");
		printf("5- Relatorios\n");
		printf("6- Sair\n");
		
		int opcao;
		printf("Digite aqui: ");
		scanf("%d", &opcao);
		getchar();
		system("cls");
		
		switch(opcao) {
			case 1:
				login();
				break;	
			case 2:
				cadastro();
				break;
			case 3:	
				consultas();
				break;
			case 4:
				informacoes();
				break;
			case 5:
				relatorios();
				break;
			case 6:
				printf("==============MedLife===========\n");
				printf("Volte sempre \n");
				Sleep(2);
				exit(0);
			default:
				printf("Opcao invalida. \n");
				Sleep(2);
				menu();
				break;
												
		}
  
}


	void login(){
	char c;
	
       char login[20];
       char senha[20];
       int a=10;
       int i=0;
       int verifica_senha=0;
       while(verifica_senha==0){
       printf("\n\t\tMedLife Login\n\n Usuario: ");
       fflush(stdin);  //Limpando o buffer do teclado
       gets(login);
       printf("\n Senha: ");
       while((c=getch())!=13){ //13 é o valor da tabela ascii
       senha[i]=c;
       i++;
       printf("*");      //vai imprimir o asterisco
       }
	       senha[i]='\0';
	       i=0;
	       system("cls");
	       a= strcmp(senha,""); //
	       if(a==0){printf("SENHA CORRETA");system("color 0a");verifica_senha=1;Beep(1500,2000);}
	       else{printf("SENHA DIGITADA ERRADA\n",senha);system("color 0c");Beep(1200,200);}
	       printf("\n");
	       system("pause");
			menu();
       }
   }	

		void cadastro(){
			printf("\n\t\tMedLife Cadastro\n\n");
			printf("\t==========================\n");
			
			printf("Digite a opcao que deseja cadastrar: \n");	
			printf("1- Cadastro de pacientes\n");
		    printf("2- Cadastro de medicos\n");
			printf("3- Cadastro de funcionarios\n");	
			printf("4- Cadastro de agendamentos\n");
			printf("5- Voltar ao menu\n");
			
				int escolha;
				printf("Digite aqui: ");
				scanf("%d", &escolha);
				getchar();
				system("cls");
				
				switch(escolha) {
					case 1:
						cadastrarPaciente();
						break;
					case 2:
						cadastrarMedico();
						break;
					case 3:	
						cadastrarFuncionarios();
						break;
					case 4:	
						cadastrarAgendamento();
						break;
					case 5:			
						Sleep(2);
						menu();
				  	default:
				   		printf("Opcao invalida. \n");
						Sleep(2);
						menu();
						break;
				   		system("pause");
				   		
		}
	}
	
void cadastrarPaciente(){
    printf("\n\t\tMedLife Cadastro de pacientes\n\n");
	printf("\t==========================\n");
	
	printf("\nDigite o seu nome completo: \n");
	fgets(paciente[contador_paciente].nome, 30, stdin);
	
	printf("\nDigite o seu CPF: \n");
	fgets(paciente[contador_paciente].cpf, 30, stdin);
	
	printf("\nDigite sua data de nascimento: \n");
	fgets(paciente[contador_paciente].dataNascimento, 30, stdin);
	
	printf("\nDigite seu email: \n");
	fgets(paciente[contador_paciente].email, 30, stdin);
	
	printf("\nDigite seu Endereco: \n");
	fgets(paciente[contador_paciente].endereco, 30, stdin);
	
	printf("\nCaso tenha convenio digite-o \n");
	fgets(paciente[contador_paciente].convenio, 30, stdin);
	
	printf("\n%s Obrigado por se cadastrar! Seu cadastro foi feito com sucesso. \n", (paciente[contador_paciente].nome, "\n"));
	
	paciente[contador_paciente].codigo= (contador_paciente +1);
	contador_paciente++;
	
	menu();
		
}

void cadastrarMedico(){

	printf("\n\t\tMedLife Cadastro de Medicos\n\n");
	printf("\t==========================\n");
	
	printf("\nDigite o nome do medico: \n");
	fgets(medico[contador_medico].nome, 30, stdin);
	
	printf("\nDigite o CPF: \n");
	fgets(medico[contador_medico].cpf, 30, stdin);	
	
	printf("\nDigite a data de nascimento: \n");
	fgets(medico[contador_medico].dataNascimento, 30, stdin);
	
	printf("\nDigite o email: \n");
	fgets(medico[contador_medico].email, 30, stdin);
	
	printf("\nDigite a unidade do medico: \n");
	fgets(medico[contador_medico].unidade, 30, stdin);
	
	printf("\nDigite o CRM do medico: \n");
	fgets(medico[contador_medico].crm, 30, stdin);
	
	printf("\nDigite a especialidade do medico: \n");
	fgets(medico[contador_medico].crm, 30, stdin);
	
	printf("\nDr. %s Obrigado por se cadastrar! Seu cadastro foi feito com sucesso. \n", (medico[contador_medico].nome));
	
	medico[contador_medico].codigomed= (contador_medico +1);
	contador_medico++;
	
	menu();
}

void cadastrarFuncionarios(){
	FILE* arq;

	printf("\n\t\tMedLife Cadastro de Funcionarios\n\n");
	printf("\t==========================\n");
	
	arq = fopen("cadastrofunci.txt", "a");
	if (arq){
	printf("\nDigite o nome completo do funcionario: \n");
	fgets(funcionario[contador_funcionario].nome, 30, stdin);
	fputs(funcionario[contador_funcionario].nome, arq);
	
	printf("\nDigite o CPF do funcionario: \n");
	fgets(funcionario[contador_funcionario].cpf, 30, stdin);
	fputs(funcionario[contador_funcionario].cpf, arq);
	
	printf("\nDigite a data de nascimento: \n");
	fgets(funcionario[contador_funcionario].dataNascimento, 30, stdin);
	fputs(funcionario[contador_funcionario].dataNascimento, arq);
	
	printf("\nDigite o email: \n");
	fgets(funcionario[contador_funcionario].email, 30, stdin);
	fputs(funcionario[contador_funcionario].email, arq);
	
	printf("\nDigite a unidade: \n");
	fgets(funcionario[contador_funcionario].unidade, 30, stdin);
	fputs(funcionario[contador_funcionario].unidade, arq);
	
	printf("\nDigite a matricula \n");
	fgets(funcionario[contador_funcionario].matricula, 30, stdin);
	fputs(funcionario[contador_funcionario].matricula, arq);
	
	printf("\nDigite o departamento \n");
	fgets(funcionario[contador_funcionario].departamento, 30, stdin);
	fputs(funcionario[contador_funcionario].departamento, arq);
		
	printf("\n%s Obrigado por se cadastrar! Seu cadastro foi feito com sucesso. Bem vindo(a)! \n", (funcionario[contador_funcionario].nome, "\n"));
		
	}else{
		printf("Nao foi possivel criar o arquivo. \n");
	}
	
	
	
	funcionario[contador_funcionario].codigofunci= (contador_funcionario +1);
	contador_funcionario++;
	fclose(arq);
	menu();
}

void cadastrarAgendamento(){
	
	printf("\n\t\tMedLife Cadastro de Agendamento\n\n");
	printf("\t==========================\n");
	
	printf("\nDigite o nome completo do Paciente: \n");
	fgets(agenda[contador_agenda].nome, 30, stdin);
	
	printf("\nDigite o CPF do paciente: \n");
	fgets(agenda[contador_agenda].cpf, 30, stdin);
	
	printf("\nDigite a data de agendamento: \n");
	fgets(agenda[contador_agenda].dataAgendamento, 30, stdin);
	
	printf("\nDigite o email do paciente: \n");
	fgets(agenda[contador_agenda].email, 30, stdin);
	
	printf("\nDigite a unidade de preferencia do cliente: \n");
	fgets(agenda[contador_agenda].unidade, 30, stdin);
	
	printf("\nDigite a especializacao \n");
	fgets(agenda[contador_agenda].especializacao, 30, stdin);
	
	printf("\nDigite o codigo do paciente \n");
	fgets(agenda[contador_agenda].codigopaciente, 30, stdin);
		
	printf("\n%s Agenda reservada com sucesso! A consulta sera no dia %f . \n", (agenda[contador_agenda].nome, "\n"));
	
	agenda[contador_agenda].codigoagend= (contador_agenda +1);
	contador_agenda++;	
}

void consultas(){
	
	printf("\n\t\tMedLife Consultas\n\n");
	printf("\t==========================\n");
	
	printf("Digite o tipo de consulta que deseja fazer: \n");	
	printf("1- Consulta de pacientes\n");
    printf("2- Consulta de medicos\n");
	printf("3- Consulta de funcionarios\n");
	printf("4- Consulta de Agendamentos\n");	
	printf("5- Voltar ao menu\n");
	printf("Digite aqui: ");
	
		int escolha;
		scanf("%d", &escolha);
		getchar();
		system("cls");
		
		switch(escolha) {
			case 1:
				listaPaciente();
				break;
			case 2:
				listaMedico();
				break;
			case 3:	
				listaFuncionario();
				break;
			case 4:	
				listaAgendamento();
				break;
			case 5:
				Sleep(2);
				menu();
		  	default:
		   		printf("Opcao invalida. \n");
				Sleep(2);
				menu();
				break;
		   		
		 }
	
}

void listaPaciente(){	

		
		int i=0;
		printf("\t-----------MedLife-------------\n");
		printf("Lista de Pacientes: \n");	
		if(contador_paciente > 0){
		for( i= 0; i < contador_paciente; i++){
	   	 infoPaciente(paciente[i]);
		 printf("----------------------");
		 Sleep(1);
	     }
			}else{
			printf("Nao temos ainda Pacientes cadastrados. \n");
			
			}
		
		printf("\nDigite a opcao desejada: Voltar ao menu [1] Voltar a consulta [2] Sair do programa [3] \n");
		printf("\nDigite aqui: ");
			
			int escolha;
			scanf("%d",&escolha);
			getchar();
			system("cls");
			
			switch(escolha) {
			case 1:
				menu();
				break;
			case 2:
				consultas();
				break;
			case 3:
				printf("Volte sempre! ");
				Sleep(2);
				exit(0);
			default:
				printf("Opcao invalida. \n");
				Sleep(2);
				listaPaciente();
				break;
		}
	}

void listaMedico(){
		int i=0;
		printf("\t-----------MedLife-------------\n");
		printf("Lista de Medicos: \n");	
		if(contador_medico > 0){
		 for( i= 0; i < contador_medico; i++){
		 infoMedico(medico[i]);
	 	 printf("----------------------");
		 Sleep(1);		
		 }
			}else{
			printf("Nao temos ainda Medicos cadastrados. \n");
	 }
	 
	 	printf("\nDigite a opcao desejada: Voltar ao menu [1] Voltar a consulta [2] Sair do programa [3] \n");
		printf("\nDigite aqui: ");
			
			int escolha;
			scanf("%d",&escolha);
			getchar();
			system("cls");
			
			switch(escolha) {
			case 1:
				menu();
				break;
			case 2:
				consultas();
				break;
			case 3:
				printf("Volte sempre! ");
				Sleep(2);
				exit(0);
			default:
				printf("Opcao invalida. \n");
				Sleep(2);
				listaMedico();
				break;
		}
}

void listaFuncionario(){
		
		FILE* leitura;
		
		
		int i=0;
		printf("\t-----------MedLife-------------\n");
		printf("Lista de Funcionarios: \n");	
		    
	   	leitura = fopen("cadastrofunci.txt", "rb");
		
		if(leitura == NULL){
			printf("Nao temos cadastro de funcionarios. \n");
			getchar;
			exit(0);
		}/*
		else{
			while(fread(&funci, sizeof(),1, leitura)==1 ){
			getchar;
			exit(0);
			}
			
		}	fclose(leitura);
		*/
		if(contador_funcionario> 0) {
		 for( i= 0; i < contador_funcionario; i++){
		 infoFuncionario(funcionario[i]);
		 printf("----------------------");
		 Sleep(1);		
					
				
			}
	 }  
	
		 

			

			
			
    printf("\nDigite a opcao desejada: Voltar ao menu [1] Voltar a consulta [2] Sair do programa [3] \n");
		printf("\nDigite aqui: ");
			
			int escolha;
			scanf("%d",&escolha);
			getchar();
			system("cls");
			
			switch(escolha) {
			case 1:
				menu();
				break;
			case 2:
				consultas();
				break;
			case 3:
				printf("Volte sempre! ");
				Sleep(2);
				exit(0);
			default:
				printf("Opcao invalida. \n");
				Sleep(2);
				listaFuncionario();
				break;
}

}
	
void listaAgendamento(){
		int i=0;
		printf("\t-----------MedLife-------------\n");
		printf("Lista de Agenda: \n");	
		if(contador_agenda > 0){
		 for( i= 0; i < contador_agenda; i++){
		 infoAgenda(agenda[i]);
		 printf("----------------------");
		 Sleep(1);		
		 }
		}else{
		printf("\nNao temos ainda agendas marcadas. \n");
	}	
	
	printf("\nDigite a opcao desejada: Voltar ao menu [1] Voltar a consulta [2] Sair do programa [3] \n");
		printf("\nDigite aqui: ");
			
			int escolha;
			scanf("%d",&escolha);
			getchar();
			system("cls");
			
			switch(escolha) {
			case 1:
				menu();
				break;
			case 2:
				consultas();
				break;
			case 3:
				printf("Volte sempre! ");
				Sleep(2);
				exit(0);
			default:
				printf("Opcao invalida. \n");
				Sleep(2);
				listaAgendamento();
				break;
	
}

}

void informacoes(){
}

void relatorios(){	
}

void reclamacoesElogios(){
}

Medico consultarMedico(int codigomed){	
}

Paciente consultarPaciente(int codigo){
	
}



