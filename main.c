#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "dados.h"
#include "ficheiros.h"


char menu( int, int, int, int );
t_membro* adicionar_membro( t_membro[], int* );

int main()
{

    setlocale( LC_ALL, "" );

    int qt_membros = 0;
    int qt_testes_agendados = 0;
    int qt_testes_realizados = 0;
    int qt_vacinados = 0;

    char sub_opt = '\0';        // Auxiliar para guardar as opções dos sub-menus

    t_membro *membros = NULL;

    carregar_membros( membros, &qt_membros );

    char opcao = '\0';

    do
    {
        opcao = menu( qt_membros, qt_testes_agendados, qt_testes_realizados, qt_vacinados );

        switch ( opcao )
        {
            case 'I': /* Inserir membro académico */
                membros = adicionar_membro( membros, &qt_membros );       //dados.h
                gravar_membros( membros, qt_membros );                    //ficheiros.h
                break;
            case 'L': /* Listar membros académicos */
                listar_membros( membros, qt_membros );                    //dados.h
                break;
            case 'A': /* Atualizar membro */
                printf( "\n[V] - Alterar estado de Vacinação\n[C] - Alterar estado de Confinamento\n" );
                sub_opt = ler_char( "\t\tOpção --> ");
            break;

            default: printf( "Insira uma opção valida.\n" );
        }

    } while ( opcao != 'F' );

    return 0;
}

char menu( int qt_membros, int qt_testes_agendados, int qt_testes, int qt_vacinados )
{
    char opt = '\0';
    printf( "\n---------------------------------------MENU---------------------------------------\n" );
    printf( "Numero de membros: %13d\t\tNumero de testes: %13d\n", qt_membros, qt_testes );
    printf( "Numero de membros vacinados: %3d\t\tNumero de testes agendados: %3d\n\n", qt_vacinados, qt_testes_agendados );
    printf( "[I] - Inserir membro académico\n" );
    printf( "[L] - Listar membros académicos\n" );
    printf( "[A] - Atualizar estado de vacinação/confinamento\n" );
    opt = ler_char( "\n\t\tOpcão" );
    return toupper( opt ); // turnar o caracter maiosculo
}

t_membro* adicionar_membro( t_membro *m, int *qt_membros )
{

    if ( *qt_membros >= MAX_MEMBROS )
    {
        printf( "Excedido o numero máximo de membros académicos. (%d)\n", MAX_MEMBROS );
    }

    if ( *qt_membros == 0 ) // Se o pointeiro ainda nao foi inicializado...
    {
        m = malloc( 1 * sizeof( t_membro ));

        if ( m == NULL )
        {
            printf( "Erro ao alocar memória.\n" );
            return NULL;
        }
    }
    else // Se ja foi inicizliado, aloca mais um espaço
    {
        m = realloc( m, *qt_membros * sizeof( t_membro ) );

        if ( m == NULL )
        {
            printf( "Erro ao alocar memória.\n" );
            return NULL;
        }
    }

    m[*qt_membros] = ler_membro( m, *qt_membros ); // Le os dados do membro

    (*qt_membros)++; // Incrementa a quantidade de membros
    return m; // Retorna o endereço do ponteiro (Ver relatório)
}
