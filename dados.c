#include <stdio.h>

#include "dados.h"


void limpa_stdin( void )
{
   /* char c = '\0';

    do
    {
        c = getchar();
    } while ( c != '\n' && c != EOF ); */
    fflush( stdin ); // Limpa stdin
}

// Le um caracter
char ler_char( const char* msg )
{
    char car = '\0';
    int controlo = 0;

    do
    {
        printf( "%s: ", msg );
        controlo = scanf( "%c", &car );
        limpa_stdin();

        if ( controlo != 1 ) printf( "N�o foi possivel ler um caracter.\n" );

    } while ( controlo != 1 );

    return car;
}

// Le um numero inteiro entre min e max
int ler_inteiro( const char* msg, int min, int max )
{
    int num = min - 1;
    int controlo = -1;

    do
    {
        printf( "%s (%d - %d): ", msg, min, max );
        controlo = scanf( "%d", &num );
        limpa_stdin();

        if ( controlo == 0 ) printf( "N�o foi possivel ler nenhum numero inteiro.\n" );
        else if ( num < min ) printf( "O n�mero n�o pode ser menor que %d\n", min );
        else if ( num > max ) printf( "O n�mero n�o pode ser maior que %d\n", max );

    } while ( controlo == 0 || num < min || num > max );

    return num;
}

// Le um float entre min e max
int ler_float( const char* msg, float min, float max )
{
    float num = min - 1.f;
    int controlo = -1;

    do
    {
        printf( "%s (%.2f - %.2f): ", msg, min, max );
        controlo = scanf( "%f", &num );
        limpa_stdin();

        if ( controlo == 0 ) printf( "N�o foi possivel ler nenhum n�mero real.\n" );
        else if ( num < min ) printf( "O n�mero n�o pode ser menor que %.2f\n", min );
        else if ( num > max ) printf( "O n�mero n�o pode ser maior que %.2f\n", max );

    } while ( controlo == 0 || num < min || num > max );

}

//Le uma string
void ler_string( const char* msg, char out[], int tamanho_max, int apagar_newline )
{
    int tamanho = 0;

    do
    {
        printf( "%s (Max: %d caracteres): ", msg, tamanho_max );
        fgets( out, tamanho_max, stdin ); // Caso a menssagens tenha mais caracteres do que 'tamanho_max', a messagem fica cortada sem avisar o utilizador
        limpa_stdin();
        tamanho = strlen( out );

        if ( tamanho == 1 ) printf( "Dever� inserir pelo menos um caracter.\n" );

    } while ( tamanho == 1 );

    if ( apagar_newline ) // Apagar o \n do final, caso "apagaar_newline" for != 0
        out[tamanho - 1] = '\0';
}

//Le uma data com um ano minimo (min_ano)
//C�digo fornecido pelo poressor e adaptado ao projeto
t_data ler_data( const char* msg, int min_ano )
{
    t_data data;
    int max_mes = 31;

    printf( "%s:\n" , msg );

    data.ano = ler_inteiro( "Ano", min_ano, 2100 );
    data.mes = ler_inteiro( "M�s", 1, 12 );

    switch ( data.mes )
    {
        case 2:
            if ( data.ano % 400 == 0 || data.ano % 4 == 0 || data.ano % 100 != 0 ) // Verificar se � um ano bisexto
                max_mes = 29;
            else
                max_mes = 28;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            max_mes = 30;
            break;
    }

    data.dia = ler_inteiro( "Dia", 1, max_mes ); // pedir o dia em ultimo para calcular o dia maximo, dependendo do mes e do ano


    return data;
}

void data_string( char out[], t_data data ) // Transforma uma data em string
{
    sprintf( out, "%2d/%2d/%4d", data.dia, data.mes, data.ano );
}

void hora_string ( char out[], t_hora hora) // Transforma uma hora em string
{
    sprintf( out, "%2d:%2d", hora.hora, hora.minuto );
}

int procurar_membro( t_membro membros[], int num_utente, int qt_membros ) // Retorna a posi��o do membro encontrado
{
    // num utente = numero de utente do membro a encontrar
    int pos = -1;

    for ( int i = 0; i < qt_membros; i++ ) // correr todas as posi��es do vetor
    {
        if ( membros[i].num_utente == num_utente ) // Se o numero de utente da posi��o 'i' do vetor for igual ao numero de utente a procurar
        {
            // guardar a posi��o e sair do loop
            pos = i;
            break;
        }
    }

    return pos;
}

t_membro ler_membro( t_membro m[], int num_membros )
{
    t_membro membro;
    int pos = 0;

    do
    {
        membro.num_utente = ler_inteiro( "Introduza o numero de utente do membro academico", 1, 9999 );

        pos = procurar_membro( m, membro.num_utente, num_membros ); // procura um membro com o numero de utente inserido

        if ( pos != -1 )
            printf( "Ja existe um membro com esse numero de utente.\n" ); // Mostrar menssagem de erro casa exista

    } while ( pos != -1 ); // Pede novamente caso exista

    ler_string( "Introduza o nome do membro academico", membro.nome, MAX_NOME, 1 );
    membro.ano_nascimento = ler_inteiro( "Introduza o ano de nascimento", 1920, 2020 );

    do
    {
        membro.tipo = toupper( ler_char( "[E] - Estudante\n[D] - Docente\n[T] - Tecnico\nTipo de membro" )); // o caracter inserido � transformado em upper
    } while ( membro.tipo != 'E' && membro.tipo != 'D' && membro.tipo != 'T' ); // Verifica se � um caracter valido para o tipo de membro

    do
    {
        membro.estado_confinamento = toupper( ler_char( "[N] - Nao confinado\n[Q] - Quarentena\n[I] - Isolamento profilatico\nEstado do confinamento" )); // o caracter inserido � transformado em upper
    } while ( membro.estado_confinamento != 'N' && membro.estado_confinamento != 'I' && membro.estado_confinamento != 'Q' ); // Verifica se � um caracter valido para o estado de confinamento

    membro.vacinacao = ler_inteiro( "Numero de vacinas", 0, 3 );

    if ( membro.vacinacao > 0 ) // Caso o membro j� tenha sido vacinado, pedir a data da ultima vacina
    {
        membro.ultima_vacina = ler_data( "Introduza a data da ultima vacinacao", membro.ano_nascimento );
    }

    return membro;
}

void listar_membros( t_membro *membros, int qt_membros )
{

    if ( membros == NULL ) // Prevenir que o programe crashe, n�o necess�rio mas evita perca de tempo enquanto se debuga/programa
        return;

    for ( int i = 0; i < qt_membros; i++ ) //Mostrar informa��o f�cil de compreender
    {
        printf( "\nUtente (%d): %s\n", membros[i].num_utente, membros[i].nome );

        switch ( membros[i].tipo )
        {
            case 'D': printf( "Tipo: Docente\n" );
                break;
            case 'E': printf( "Tipo: Estudante\n" );
                break;
            case 'T': printf( "Tipo: Tecnico\n" );
                break;
        }

        printf( "Ano de nascimento: %d\n", membros[i].ano_nascimento );

        switch ( membros[i].estado_confinamento ) //Mostrar informa��o f�cil de compreender
        {
            case 'N': printf( "Estado de confinamento: Nao\n" );
                break;
            case 'Q': printf( "Estado de confinamento: Quarentena\n" );
                break;
            case 'I': printf( "Estado de confinamento: Isolamento profilatico\n" );
                break;
        }

        printf( "Numero de vacinas: %d\n", membros[i].vacinacao );

        if ( membros[i].vacinacao ) // Se o membro for vacinado, mostrar a data
        {
            char data_str[9];
            data_string( data_str, membros[i].ultima_vacina );
            printf( "Data da ultima vacinacao: %s\n", data_str );
        }
    }
}

