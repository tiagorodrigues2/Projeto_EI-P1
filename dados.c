#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

        if ( controlo != 1 ) printf( "Nao foi possivel ler um caracter.\n" );

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

        if ( controlo == 0 ) printf( "Nao foi possivel ler nenhum numero inteiro.\n" );
        else if ( num < min ) printf( "O numero não pode ser menor que %d\n", min );
        else if ( num > max ) printf( "O numero não pode ser maior que %d\n", max );

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

        if ( controlo == 0 ) printf( "Nao foi possivel ler nenhum numero real.\n" );
        else if ( num < min ) printf( "O numero nao pode ser menor que %.2f\n", min );
        else if ( num > max ) printf( "O numero nao pode ser maior que %.2f\n", max );

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

        if ( tamanho == 1 ) printf( "Devera inserir pelo menos um caracter.\n" );

    } while ( tamanho == 1 );

    if ( apagar_newline ) // Apagar o \n do final, caso "apagaar_newline" for != 0
        out[tamanho - 1] = '\0';
}

int procurar_teste( t_teste* p_teste, int id, int qt_testes )
{
    for ( int i = 0; i < qt_testes; i++ )
    {
        if ( id == p_teste[i].id )
            return i;
    }

    return -1;
}

int qt_testes_data( t_teste *p_teste, int qt_testes, t_data data )
{
    int contador = 0;

    for ( int i = 0; i < qt_testes; i++ )
    {
        if ( p_teste[i].data.ano == data.ano && p_teste[i].data.dia == data.dia && p_teste[i].data.mes == data.mes )
            contador++;
    }

    return contador;
}

t_teste ler_teste( t_teste *p_teste, int qt_testes_agendados, int qt_testes_realizados, t_membro *p_membro, int qt_membros )
{
    t_teste teste = { 0 };
    int pos = -1;

    do
    {
        teste.id = ler_inteiro( "Insira o id do teste", 1, 9999 );

        pos = procurar_teste( p_teste, teste.id, qt_testes_realizados + qt_testes_agendados );

        if ( pos != -1 ) printf( "Ja existe um teste com esse ID.\n" );
    } while ( pos != -1 );                                                                                     /* Verificacao se já existe teste com o id fornecido*/

    do
    {
        teste.data = ler_data( "Data do teste", 2021 );

        do
        {
            teste.tipo = toupper( ler_char( "Introduza o tipo de teste:\n[P] - PCR\n[A] - Antigenio\n" ) );

            if ( teste.tipo != 'A' && teste.tipo != 'P' ) printf( "Introduza uma opçao valida" );

        } while ( teste.tipo != 'A' && teste.tipo != 'P' );                                                                     /* Verificacao se o char lido é uma opcao valida */

        if ( teste.tipo == 'P' && qt_testes_data( p_teste, qt_testes_realizados + qt_testes_agendados, teste.data ) >= 15 )
            printf( "Nao se pode efetuar mais do que 15 testes PCR por dia.\n" );
    } while ( teste.tipo == 'P' && qt_testes_data( p_teste, qt_testes_realizados + qt_testes_agendados, teste.data ) >= 15 );    /* Verificao se já ha 15 testes pcr agendados na data fornecida */


    teste.hora = ler_hora( "Introduza a hora do teste" );

    do
    {
        teste.num_utente = ler_inteiro( "Introduza o numero de utente do membro a ser testado", 1, 9999 );

        pos = procurar_membro( p_membro, teste.num_utente, qt_membros );

        if ( pos == -1 ) printf( "Nao existe nenhum membro com esse numero de utente.\n" );         /* Verificar se o numero de utente existe */

    } while ( pos == -1 );

    teste.duracao.hora = 0;         /* Dados só fornecidos quando o teste for realizado... */
    teste.duracao.minuto = 0;
    teste.resultado = -1;

    return teste;
}

t_hora ler_hora( const char* msg )
{
    t_hora hora;

    printf( "%s:\n", msg );

    hora.hora = ler_inteiro( "Hora: ", 0, 24 );
    hora.minuto = ler_inteiro( "Minuto: ", 0, 59 );

    return hora;
}

//Le uma data com um ano minimo (min_ano)
//Código fornecido pelo poressor e adaptado ao projeto
t_data ler_data( const char* msg, int min_ano )
{
    t_data data;
    int max_mes = 31;

    printf( "%s:\n" , msg );

    data.ano = ler_inteiro( "Ano", min_ano, 2100 );
    data.mes = ler_inteiro( "Mes", 1, 12 );

    switch ( data.mes )
    {
        case 2:
            if ( data.ano % 400 == 0 || data.ano % 4 == 0 || data.ano % 100 != 0 ) // Verificar se é um ano bisexto
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

int procurar_membro( t_membro *membros, int num_utente, int qt_membros ) // Retorna a posição do membro encontrado
{
    // num utente = numero de utente do membro a encontrar
    int pos = -1;

    for ( int i = 0; i < qt_membros; i++ ) // correr todas as posições do vetor
    {
        if ( membros[i].num_utente == num_utente ) // Se o numero de utente da posição 'i' do vetor for igual ao numero de utente a procurar
        {
            // guardar a posição e sair do loop
            pos = i;
            break;
        }
    }

    return pos;
}

t_membro ler_membro( t_membro *m, int num_membros )
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
        membro.tipo = toupper( ler_char( "[E] - Estudante\n[D] - Docente\n[T] - Tecnico\nTipo de membro" )); // o caracter inserido é transformado em upper
    } while ( membro.tipo != 'E' && membro.tipo != 'D' && membro.tipo != 'T' ); // Verifica se é um caracter valido para o tipo de membro

    do
    {
        membro.estado_confinamento = toupper( ler_char( "[N] - Nao confinado\n[Q] - Quarentena\n[I] - Isolamento profilatico\nEstado do confinamento" )); // o caracter inserido é transformado em upper
    } while ( membro.estado_confinamento != 'N' && membro.estado_confinamento != 'I' && membro.estado_confinamento != 'Q' ); // Verifica se é um caracter valido para o estado de confinamento

    membro.vacinacao = ler_inteiro( "Numero de vacinas", 0, 3 );

    if ( membro.vacinacao > 0 ) // Caso o membro já tenha sido vacinado, pedir a data da ultima vacina
    {
        membro.ultima_vacina = ler_data( "Introduza a data da ultima vacinacao", membro.ano_nascimento );
    }


    return membro;
}

/* Grupo difine os tipos de teste a ser mostrados...
    grupo = 0: Todos os testes
    grupo = 1: Testes agendados
    grupo = 2: Testes realizados
*/
void listar_testes( t_teste* p_testes, int qt_testes, t_membro* p_membros, int qt_membros, int grupo )
{
    int m_pos = -1;
    int realizado = 0;

    for ( int i = 0; i < qt_testes; i++ )
    {
        t_teste t = p_testes[i];
        m_pos = procurar_membro( p_membros, t.num_utente, qt_membros );
        t_membro m = p_membros[m_pos];

        realizado = t.duracao.hora + t.duracao.minuto;

        switch ( grupo )
        {
            case 0:
                printf( "\nLISTAGEM DE TODOS OS TESTES:\n" );
                break;
            case 1:
                printf( "\nLISTAGEM DE TESTES AGENDADOS\n" );
                if ( realizado )
                    continue;
                break;
            case 2:
                printf( "\nLISTAGEM DE TESTES REALIZADOS\n" );
                if ( !realizado )
                    continue;
                break;
        }

        printf( "Teste %s (%04d)\n", realizado ? "realizado" : "agendado", t.id );
        printf( "Tipo: %s\n", t.tipo == 'P' ? "PCR" : "Antigenio" );
        printf( "Utente: (%04d) %s\n", m.num_utente, m.nome );
        printf( "Data: %02d/%02d/%02d\n", t.data.dia, t.data.mes, t.data.ano );
        printf( "Hora: %02d:%02d\n", t.hora.hora, t.hora.minuto );

        if ( !realizado )
            continue;

        printf( "Duracao: %02d:%02d\n", t.duracao.hora, t.duracao.minuto );
        printf( "Resultado: " );
        switch ( t.resultado )
        {
            case 0: printf( "Negativo\n" ); break;
            case 1: printf( "Positivo\n" ); break;
            case -1: printf( "Inconclusivo\n" ); break;
        }
        printf( "\n" );
    }
}

void listar_membros( t_membro *membros, int qt_membros, t_teste *p_testes, int qt_testes )              // qt_testes = testes agendados + testes realizados
{

    if ( membros == NULL ) // Prevenir que o programe crashe, não necessário mas evita perca de tempo enquanto se debuga/programa
        return;

    if ( qt_membros == 0 )
    {
        printf( "Nao existem membros para mostrar.\n" );
        return;
    }

    for ( int i = 0; i < qt_membros; i++ ) //Mostrar informação fácil de compreender
    {
        printf( "\nUtente (%04d): %s\n", membros[i].num_utente, membros[i].nome );

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

        switch ( membros[i].estado_confinamento ) //Mostrar informação fácil de compreender
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
            printf( "Data da ultima vacinacao: %02d/%02d/%02d\n\n", membros[i].ultima_vacina.dia, membros[i].ultima_vacina.mes, membros[i].ultima_vacina.ano );
        }

        /* Verificar se tem testes agendados/realizados */

        if ( p_testes == NULL ) /* Se p_testes for nulo, seguir para o proximo clico...*/
            continue;

        for ( int i2 = 0; i2 < qt_testes; i2++ )       /* Ciclo por todos os testes */
        {
            if ( membros[i].num_utente == p_testes[i2].num_utente )
            {
                if ( p_testes[i2].duracao.minuto + p_testes[i2].duracao.hora > 0 )
                    printf( "TESTE REALIZADO " );
                else
                    printf( "TESTE AGENDADO " );

                printf( "ID: %04d\n", p_testes[i2].id );
                printf( "Tipo: %s\n", p_testes[i2].tipo == 'P' ? "PCR" : "Antigenio" );
                printf( "Data: %02d/%02d/%02d\n", p_testes[i2].data.dia, p_testes[i2].data.mes, p_testes[i2].data.ano );
                printf( "Hora: %02d:%02d\n", p_testes[i2].hora.hora, p_testes[i2].hora.minuto );

                if ( p_testes[i2].duracao.minuto + p_testes[i2].duracao.hora > 0 )
                {
                    printf( "Duracao do teste: %02d:%02d\n", p_testes[i2].duracao.hora, p_testes[i2].duracao.minuto );
                    printf( "Resultado: " );
                    switch ( p_testes[i2].resultado )
                    {
                        case -1: printf( "Inconclusivo\n" ); break;
                        case 0: printf( "Negativo\n" ); break;
                        case 1: printf( "Positivo\n" ); break;
                    }
                }
                printf( "\n" );
            }
        }
    }
}

