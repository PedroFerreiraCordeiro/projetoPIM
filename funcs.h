
#define SIZE 30

typedef struct{
    int dd;
    int mm;
    int yy;
}Data;

typedef struct{
    int hh;
    int mm;
}Hora;

typedef struct{
	char cpf[13];
	char nome[SIZE];
	Data dataNascimento;
	char email[SIZE];
	char endereco[120];
	int unidade;
	int freqUnd1;
	int freqUnd2;
	int freqUnd3;
	float valorTotalGasto;
}Paciente;

typedef struct{
	char nome[SIZE];
	char cpf [13];
	char email[SIZE];
	char crm[SIZE];
	char especialidade[SIZE];
	char senha[SIZE];
	int unidade;
	Data dataNascimento;
}Medico;

typedef struct{
	char nome[SIZE];
	char cpf[13];
	char email[SIZE];
	char departamento[SIZE];
	char senha[SIZE];
	int unidade;
	Data dataNascimento;
}Funcionario;

typedef struct{
	char pacienteCPF[SIZE];
	char pacienteNome[SIZE];
	char medicoCPF[SIZE];
	char medicoNome[SIZE];
	char especializacao[SIZE];
	int unidade;
	float valor;
	Data data;
	Hora hora;
}Agendamento;

typedef struct{
    int id;
    int pacientes;
    int atendimentos;
    float faturamento;
}Unidade;

int validaDataAgendamento(Data dt, Hora hr);
int validaDataNascimento(Data dt);
void atualizaClientes();

Data getDataAtual();

void login();
void menuLogadoOff();
void menuLogado();
void cadastroLogado();
void cadastroLogadoOff();

void menuConsultas();
void menuAlterarInfos();
void menuExcluir();
void menuRelatorios();
void menuReclamacoesElogios();

void cadastrarFuncionarios();
void cadastrarPaciente();
void cadastrarMedico();
void cadastrarAgendamento();

void consultaPaciente();
void consultaMedico();
void consultaFuncionario();
void consultaAgendamentoPaciente();
void consultaAgendamentoMedico();

void alteraFuncionario();
void alteraPaciente();
void alteraMedico();
void alteraAgendamento();

void excluiFuncionario();
void excluiPaciente();
void excluiMedico();
void excluiAgendamento();

void relatPaciUnd(int unidade);
void relatPaciRede();
void relatPaciMaisConsultaUnd(int unidade);
void relatPaciMaisConsultaRede();
void relatPaciGastouMaisUnd(int unidade);
void relatPaciGastouMaisRede();

void relatAtendUnd();

void relatFaturDiarioRede();
void relatFaturMensalRede();
void relatFaturTotalRede();
