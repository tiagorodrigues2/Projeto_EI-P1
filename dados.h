#ifndef DADOS_H_INCLUDED
#define DADOS_H_INCLUDED

#define MAX_NOME 50         // Caracteres do nome
#define MAX_MEMBROS 200     // Numero maximo de membros

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
} t_membro;

typedef struct
{
    int id;
    char tipo;
    t_data data;
    int num_utente;
    int resultado;              // Positivo (1) ou negativo (0) ou inconclusivo ( -1 )
    t_hora hora;
    t_hora duracao;
} t_teste;

typedef struct
{
    t_data data;
    t_hora hora;
    char tipo;                  //PCR / antigénio
    int num_utente;
} t_teste_agendado;

extern void limpa_stdin( void );
extern int ler_inteiro( const char*, int, int );
extern int ler_float( const char*, float, float );
extern void ler_string( const char*, char[], int , int  );
extern char ler_char( const char* );
extern t_data ler_data( const char*, int  );
extern void data_string( char[], t_data );
extern void hora_string ( char[], t_hora );
extern int procurar_membro( t_membro[], int, int );
extern t_membro ler_membro( t_membro[], int );
extern void listar_membros( t_membro*, int );

#endif // DADOS_H_INCLUDED
