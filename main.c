#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "dados.h"
#include "ficheiros.h"


char menu( int, int, int, int );
t_membro* adicionar_membro( t_membro[], int* );
void alterar_vacinacao( t_membro*, int );
void alterar_confinamento( t_membro*, int );
void atualizar_estados( t_membro*, int );

int main()
{

    setlocale( LC_ALL, "" );

    int qt_membros = 0;
    int qt_testes_agendados = 0;
    int qt_testes_realizados = 0;
    int qt_vacinados = 0;

    t_membro *membros = NULL;                   /* Membros da comunidade acad�mica */
    t_teste *testes = NULL;                     /* Testes efetuados */
    t_teste_agendado *agendados = NULL;         /* Testes agendados */

    membros = carregar_membros( &qt_membros );

    char opcao = '\0';

    do
    {
        opcao = menu( qt_membros, qt_testes_agendados, qt_testes_realizados, qt_vacinados );

        switch ( opcao )
        {
            case 'I': /* Inserir membro acad�mico */
                membros = adicionar_membro( membros, &qt_membros );
                gravar_membros( membros, qt_membros );                  /* Gravar automaticamente para poupar espa�o no menu e evitar'esquecimentos' */
                break;
            case 'L': /* Listar membros acad�micos */
                listar_membros( membros, qt_membros );
                break;
            case 'A': /* Atualizar membro */
                atualizar_estados( membros, qt_membros );
                break;

            default: printf( "Insira uma op��o valida.\n" );
        }

    } while ( opcao != 'F' );

    /* Libertar espa�o da mem�ria */

    free( membros );
    free( testes );
    free( agendados );

    return 0;
}

char menu( int qt_membros, int qt_testes_agendados, int qt_testes, int qt_vacinados )
{
    char opt = '\0';
    printf( "\n---------------------------------------MENU---------------------------------------\n" );
    printf( "N�mero de membros: %13d\t\tNumero de testes: %13d\n", qt_membros, qt_testes );
    printf( "N�mero de membros vacinados: %3d\t\tNumero de testes agendados: %3d\n\n", qt_vacinados, qt_testes_agendados );
    printf( "[I] - Inserir membro acad�mico\n" );
    printf( "[L] - Listar membros acad�micos\n" );
    printf( "[A] - Atualizar estado de vacina��o/confinamento\n" );
    opt = ler_char( "\n\t\tOpc�o" );
    return toupper( opt ); // turnar o caracter maiosculo
}

t_membro* adicionar_membro( t_membro *m, int *qt_membros )
{

    if ( *qt_membros >= MAX_MEMBROS )
    {
        printf( "Excedido o numero m�ximo de membros acad�micos. (%d)\n", MAX_MEMBROS );
    }

    if ( *qt_membros == 0 ) // Se o pointeiro ainda nao foi inicializado...
    {
        m = malloc( 1 * sizeof( t_membro ));

        if ( m == NULL )
        {
            printf( "Erro ao alocar mem�ria.\n" );
            return NULL;
        }
    }
    else // Se ja foi inicizliado, aloca mais um espa�o
    {
        m = realloc( m, *qt_membros * sizeof( t_membro ) );

        if ( m == NULL )
        {
            printf( "Erro ao alocar mem�ria.\n" );
            return NULL;
        }
    }

    m[*qt_membros] = ler_membro( m, *qt_membros ); // Le os dados do membro

    (*qt_membros)++; // Incrementa a quantidade de membros
    return m; // Retorna o endere�o do ponteiro (Ver relat�rio)
}

void alterar_confinamento( t_membro* m, int qt_membros )
{
    int pos = 0;
    int num_utente = -1;

    do
    {
        num_utente = ler_inteiro( "Introduza o n�mero de utente do membro acad�mico a atualizar", 1, 9999 ); /* Pedir numero de utente do utilizador a atualizar */

        pos = procurar_membro( m, num_utente, qt_membros );

        if ( pos == -1 )
            printf( "N�o foi possivel encontrar membro acad�mico.\n" );
    } while ( pos == -1 );

    printf( "Estado de confinamento atual: " ); /* Mostrar estado de confinamenteo atual para referencia  */

    switch ( m[pos].estado_confinamento )
    {
        case 'N': printf( "N�o confinado\n" );
            break;
        case 'Q': printf( "Quarentena\n" );
            break;
        case 'I': printf( "Isolamento profil�tico\n" );
            break;
    }

    int check = 1;

    do
    {
        m[pos].estado_confinamento = toupper( ler_char( "Introduza o novo estado de confinamento:\n[N] - N�o Confinado\n[Q] - Quarentena\n[I] - Isolamento Profil�tico\n" ) );

        check = ( m[pos].estado_confinamento != 'N' && m[pos].estado_confinamento != 'Q' && m[pos].estado_confinamento !='I' ); /* Verifica��o se o caracter � algum em (N, I, Q) */

        if ( check )
            printf( "Insira uma op��o v�lida.\n" );
    } while ( check );
}

void alterar_vacinacao( t_membro *m, int qt_membros )
{
    int pos = 0;
    int num_utente = -1;

    do
    {
        num_utente = ler_inteiro( "Introduza o n�mero de utente do membro acad�mico a atualizar", 1, 9999 ); /* Pedir numero de utente de membro a ser atualizado */

        pos = procurar_membro( m, num_utente, qt_membros );

        if ( pos == -1 )
            printf( "N�o foi possivel encontrar membro acad�mico.\n" );
    } while ( pos == -1 );

    if ( m[pos].vacinacao != 0 )
        printf( "Vacina��o atual: %d Dose\n", m[pos].vacinacao ); /* Mostrar numero de vacinas para referencia */
    else
        printf( "Vacina��o atual: Sem vacina\n" );

    m[pos].vacinacao = ler_inteiro( "Introduza o novo n�mero de vacinas", 0, 3 ); /* Pedir novo numero de vacinas */

    if ( m[pos].vacinacao == 0 )    /* Se o novo numero de vacina��o for zero, limpar a data da ultima vacina��o */
    {
        m[pos].ultima_vacina.ano = 0;
        m[pos].ultima_vacina.dia = 0;
        m[pos].ultima_vacina.mes = 0;
    }
    else                            /* Se n�o, perguntar a nova data... */
    {
        m[pos].ultima_vacina = ler_data( "Insira a data da ultima vacina��o", m[pos].ano_nascimento );
    }
}

void atualizar_estados( t_membro *m, int qt_membros ) /* Sub Menu para atualizar membro */
{
    char sub_opt = '\0';

    do
    {
        printf( "\n[V] - Alterar estado de Vacina��o\n[C] - Alterar estado de Confinamento\n" );
        sub_opt = toupper( ler_char( "\t\tOp��o --> ") );

        if ( sub_opt != 'V' && sub_opt != 'C' ) printf( "Insira uma op��o v�lida." );

    } while ( sub_opt != 'V' && sub_opt != 'C' );

    switch ( sub_opt )
    {
        case 'V':
            alterar_vacinacao( m, qt_membros );
            break;
        case 'C':
            alterar_confinamento( m, qt_membros );
            break;
    }
}

