#ifndef DADOS_H_INCLUDED
#define DADOS_H_INCLUDED

#define MAX_NOME 50         // Caracteres do nome
#define MAX_MEMBROS 200     // Numero maximo de membros
#define GRUPO_TODOS 0
#define GRUPO_AGENDADOS 1
#define GRUPO_REALIZADOS 2

typedef struct
{
    int dia;
    int mes;
    int ano;
} t_data;

typedef struct
{
    int hora;
    int minuto;
} t_hora;

typedef struct
{
    int num_utente;
    char nome[MAX_NOME];
    char tipo;                  // Docente/tecnico/estudante
    int ano_nascimento;
    char estado_confinamento;   // Quarentena/isolamento profilatico/Nao confiado
    int vacinacao;              // Numero de vacinas (0-3)
    t_data ultima_vacina;       // Data da ultima vacina

    t_data data_confinamento;   // Data em que o membro entrou em confinamento
    int duracao;                // Duracao do confinamento em dias
} t_membro;

typedef struct
{
    int id;
    char tipo;
    t_data data;
    t_hora hora;
    int num_utente;

    /* Estes campos são reservados para depois do teste ser efetuado...*/
    int resultado;              // Positivo (1) ou negativo (0) ou inconclusivo ( -1 )
    t_hora duracao;             // Duração do teste... Se for maior que zero, o teste foi feito.

} t_teste;

extern void limpa_stdin( void );
extern int ler_inteiro( const char*, int, int ); // (Mensagem, minimo, maximo)
extern void ler_string( const char*, char[], int , int  ); // (Mensagem, string de retorno, maximo de caracteres, apagar new line [0/1])
extern char ler_char( const char* ); // (Mensagem)
extern t_data ler_data( const char*, int  ); // (Mensagem, ano minimo)
extern int procurar_membro( t_membro*, int, int ); // (Vetor membros, numero de utente, quantidade de membros)
extern t_membro ler_membro( t_membro*, int ); // (Vetor membros, quantidade membros)
extern void listar_membros( t_membro*, int, t_teste*, int ); // (Vetor membros, quantidade de membros, vetor testes, quantidade de total de testes)
extern void listar_testes( t_teste*, int, t_membro*, int, int ); // (Vetor testes, quantidade de testes, vetor membros, quantidade de membros, grupo dos testes [0-3]
extern int procurar_teste( t_teste*, int, int ); // (Vetor testes, ID do teste, quantidade total dos testes)
extern t_teste ler_teste( t_teste*, int, int, t_membro*, int ); // (Vetor testes, quantidade de testes realizados, quantidade de testes agendados, vetor membros, quantidade de membros)
extern t_hora ler_hora( const char* ); // (Mensagem)
extern void info_teste( t_teste*, int, t_membro*, int ); // (Vetor testes, quantidade total de testes, vetor membros, quantidade de membros)
extern void mostrar_dados_estatiticos( t_membro*, int, t_teste*, int, int ); // (Vetor membros, quantidade de membros, vetor testes, quantidade de testes realizados, quantidade de testes agendados)
extern void mostrar_casos_confinamento( t_membro*, int ); // (Vetor membros, quantidade de membros)

#endif // DADOS_H_INCLUDED
