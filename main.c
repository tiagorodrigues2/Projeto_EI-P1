#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dados.h"
#include "ficheiros.h"


char menu( int, int, int, int );
t_membro* adicionar_membro( t_membro[], int*, int* );
void alterar_vacinacao( t_membro*, int, int* );
void alterar_confinamento( t_membro*, int );
void atualizar_estados( t_membro*, int, int* );
t_teste* agendar_teste( t_teste*, int*, int, t_membro*, int );
void menu_listar_testes( t_teste*, int, t_membro*, int );
void alterar_data( t_teste*, int );
void registar_resutlado_teste( t_teste*, int, t_membro*, int );
void mostrar_info( t_membro*, int, t_teste*, int, int );

int main( void )
{

    int qt_membros = 0;
    int qt_testes_agendados = 0;
    int qt_testes_realizados = 0;
    int qt_vacinados = 0;

    t_membro *membros = NULL;                   /* Membros da comunidade académica */
    t_teste *testes = NULL;                     /* Testes efetuados/agendados */

    membros = carregar_membros( &qt_membros, &qt_vacinados );
    testes = carregar_testes( &qt_testes_realizados, &qt_testes_agendados );

    char opcao = '\0';

    do
    {
        opcao = menu( qt_membros, qt_testes_agendados, qt_testes_realizados, qt_vacinados );

        switch ( opcao )
        {
            case 'M': /* Inserir membro académico */
                membros = adicionar_membro( membros, &qt_membros, &qt_vacinados );
                gravar_membros( membros, qt_membros );                                  /* Gravar automaticamente para poupar espaço no menu e evitar'esquecimentos' */
                break;
            case 'L': /* Listar membros académicos */
                listar_membros( membros, qt_membros, testes, qt_testes_agendados + qt_testes_realizados );
                break;
            case 'A': /* Atualizar membro */
                atualizar_estados( membros, qt_membros, &qt_vacinados );
                gravar_membros( membros, qt_membros );                                  /* Gravar automaticamente para poupar espaço no menu e evitar'esquecimentos' */
                break;
            case 'T': // Agendar teste
                testes = agendar_teste( testes, &qt_testes_agendados, qt_testes_realizados, membros, qt_membros );
                gravar_testes( testes, qt_testes_realizados, qt_testes_agendados );     /* Gravar automaticamente para poupar espaço no menu e evitar'esquecimentos' */
                break;
            case 'K': /* Listar testes */
                menu_listar_testes( testes, qt_testes_agendados + qt_testes_realizados, membros, qt_membros );
                break;
            case 'E': // Alterar data de um teste
                alterar_data( testes, qt_testes_agendados + qt_testes_realizados );
                gravar_testes( testes, qt_testes_realizados, qt_testes_agendados );     /* Gravar automaticamente para poupar espaço no menu e evitar'esquecimentos' */
                break;
            case 'R': // Registar resultado de um teste
                registar_resutlado_teste( testes, qt_testes_agendados + qt_testes_realizados, membros, qt_membros );
                qt_testes_agendados--;
                qt_testes_realizados++;
                gravar_testes( testes, qt_testes_realizados, qt_testes_agendados );     /* Gravar automaticamente para poupar espaço no menu e evitar'esquecimentos' */
                break;
            case 'I':
                mostrar_info( membros, qt_membros, testes, qt_testes_agendados, qt_testes_realizados );
                break;
            case 'F':
                printf( "FIM.\n" );
                break;
        }

    } while ( opcao != 'F' );

    /* Libertar espaço da memória */

    free( membros );
    free( testes );

    return 0;
}

char menu( int qt_membros, int qt_testes_agendados, int qt_testes, int qt_vacinados )
{
    char opt = '\0';
    printf( "\n---------------------------------------MENU---------------------------------------\n" );
    printf( "Numero de membros: %13d\t\tNumero de testes realizados: %2d\n", qt_membros, qt_testes );
    printf( "Numero de membros vacinados: %3d\t\tNumero de testes agendados: %3d\n\n", qt_vacinados, qt_testes_agendados );
    printf( "[M] - Inserir membro academico\n" );
    printf( "[L] - Listar membros academicos\n" );
    printf( "[A] - Atualizar estado de vacinacao/confinamento\n\n" );
    printf( "[T] - Agendar um teste\n" );
    printf( "[K] - Listar testes\n" );
    printf( "[E] - Alterar data de um teste\n" );
    printf( "[R] - Registar resultado de um teste\n\n" );
    printf( "[I] - Informacoes\n" );                            /* Aqui devem ser feitos os ultimos 3 pontos do enumciado num sub-menu */
    printf( "\n[F] - Sair\n" );
    opt = ler_char( "\n\t\tOpcao" );
    return toupper( opt ); // turnar o caracter maiosculo
}

void mostrar_info( t_membro* p_membro, int qt_membros, t_teste* p_testes, int qt_testes_agendados, int qt_testes_realizados )
{
    char sub_opt = '\0';

    /* Sub menu das informaoes */
    do
    {
        sub_opt = toupper( ler_char( "[T] - Informacoes de um teste\n[E] - Dados estatisticos\n[C] - Casos de confinamento\n\t\tOpcao --> " ) );

        if ( sub_opt != 'T' && sub_opt != 'E' && sub_opt != 'C' )
            printf( "Introduza uma opcao valida" );

    } while ( sub_opt != 'T' && sub_opt != 'E' && sub_opt != 'C' );

    switch ( sub_opt )
    {
        case 'T':
            info_teste( p_testes, qt_testes_agendados + qt_testes_realizados, p_membro, qt_membros );
            break;
        case 'E':
            mostrar_dados_estatiticos( p_membro, qt_membros, p_testes, qt_testes_realizados, qt_testes_agendados );
            break;
        case 'C':
            mostrar_casos_confinamento( p_membro, qt_membros );
            break;
    }
}

void registar_resutlado_teste( t_teste *p_teste, int qt_testes, t_membro* p_membro, int qt_membros )
{
    int pos = -1;
    int id = -1;

    do
    {
        id = ler_inteiro( "Introduza o ID do teste a registar resultado", 1, 9999 );

        pos = procurar_teste( p_teste, id, qt_testes );

        if ( pos != -1 && (p_teste[pos].duracao.hora + p_teste[pos].duracao.minuto) > 0 ) /* Se o teste ja tiver sido realizado, mostrar o erro... */
            pos = -1;

        if ( pos == -1 )
            printf( "Nao existe nenhum teste agendados com o id fornecido.\n" );

    } while ( pos == -1 );

    char res = '\0';

    do /* Pedir resultado do teste */
    {
        res = toupper( ler_char( "Resultado do teste:\n[P] - Positivo\n[N] - Negativo\n[I] - Inconclusivo\n" ) );

        if ( res != 'P' && res != 'N' && res != 'I' )
            printf( "Insira uma opcao valida\n" );

    } while ( res != 'P' && res != 'N' && res != 'I' );

    switch ( res ) /* Tranformar o input (char) em int */
    {
        case 'N': p_teste[pos].resultado = 0;
            break;
        case 'P': p_teste[pos].resultado = 1;
            break;
        case 'I': p_teste[pos].resultado = -1;
            break;
    }

    p_teste[pos].duracao = ler_hora( "Introduza a duracao que o teste teve" );

    /* Guardar resultado em log ... */
    log_resultado( p_teste[pos], p_membro, qt_membros );
}

void alterar_data( t_teste* p_teste, int qt_testes ) // qt_testes = testes agendados + testes realizados
{
    int pos = -1;
    int id = -1;

    do
    {
        id = ler_inteiro( "Introduza o ID do teste a alterar a data", 1, 9999 );

        pos = procurar_teste( p_teste, id, qt_testes );

        if ( pos != -1 && (p_teste[pos].duracao.hora + p_teste[pos].duracao.minuto) > 0 ) /* Se o teste ja tiver sido realizado, mostrar o erro... */
            pos = -1;

        if ( pos == -1 )
            printf( "Nao existe nenhum teste agendados com o id fornecido.\n" );

    } while ( pos == -1 );

    t_data nova_data = ler_data( "Introduza a nova data do teste", 2021 );
    t_hora nova_hora = ler_hora( "Introduza a nova hora do teste" );

    p_teste[pos].data = nova_data;
    p_teste[pos].hora = nova_hora;

    printf( "Data alterada com sucesso.\n" );
}

void menu_listar_testes( t_teste *p_teste, int qt_testes, t_membro *p_membro, int qt_membros )
{
    char sub_opt = '\0';
    int grupo = GRUPO_TODOS;

    if ( qt_testes == 0 )
    {
        printf( "Nao existem testes a mostrar.\n" );
        return;
    }

    do
    {
        sub_opt = toupper( ler_char( "[A] - Testes agendados\n[R] - Testes realizados\n[T] - Todos\n\tOpcao --> " ) );

        if ( sub_opt != 'A' && sub_opt != 'R' && sub_opt != 'T' )
            printf( "Introduza uma opcao valida.\n" );
    } while ( sub_opt != 'A' && sub_opt != 'R' && sub_opt != 'T' );

    if ( sub_opt == 'A' ) grupo = GRUPO_AGENDADOS;
    else if ( sub_opt == 'R' ) grupo = GRUPO_REALIZADOS;
    else if ( sub_opt == 'T' ) grupo = GRUPO_TODOS;

    listar_testes( p_teste, qt_testes, p_membro, qt_membros, grupo ); // Função definida em dados.c
}

t_membro* adicionar_membro( t_membro *m, int *qt_membros, int* qt_vacinados )
{

    if ( *qt_membros >= MAX_MEMBROS )
    {
        printf( "Excedido o numero maximo de membros academicos. (%d)\n", MAX_MEMBROS );
    }

    if ( *qt_membros == 0 ) // Se o pointeiro ainda nao foi inicializado...
    {
        m = malloc( 1 * sizeof( t_membro ));

        if ( m == NULL )
        {
            printf( "Erro ao alocar memoria.\n" );
            return NULL;
        }
    }
    else // Se ja foi inicizliado, aloca mais um espaço
    {
        m = realloc( m, (*qt_membros + 1) * sizeof( t_membro ) );

        if ( m == NULL )
        {
            printf( "Erro ao alocar memoria.\n" );
            return NULL;
        }
    }

    m[*qt_membros] = ler_membro( m, *qt_membros ); // Le os dados do membro

    if ( m[*qt_membros].vacinacao > 0 ) // Se o novo membro é vacinado
        (*qt_vacinados)++;

    *qt_membros = *qt_membros + 1;      // Incrementa a quantidade de membros
    return m; // Retorna o endereço do ponteiro (Ver relatório)
}

void alterar_confinamento( t_membro* m, int qt_membros )
{
    int pos = 0;
    int num_utente = -1;

    do
    {
        num_utente = ler_inteiro( "Introduza o numero de utente do membro academico a atualizar", 1, 9999 ); /* Pedir numero de utente do utilizador a atualizar */

        pos = procurar_membro( m, num_utente, qt_membros );

        if ( pos == -1 )
            printf( "Nao foi possivel encontrar membro academico.\n" );
    } while ( pos == -1 );

    printf( "Estado de confinamento atual: " ); /* Mostrar estado de confinamenteo atual para referencia  */

    switch ( m[pos].estado_confinamento )
    {
        case 'N': printf( "Nao confinado\n" );
            break;
        case 'Q': printf( "Quarentena\n" );
            break;
        case 'I': printf( "Isolamento profilatico\n" );
            break;
    }

    int check = 1;

    do
    {
        m[pos].estado_confinamento = toupper( ler_char( "Introduza o novo estado de confinamento:\n[N] - Nao Confinado\n[Q] - Quarentena\n[I] - Isolamento Profilatico\n" ) );

        check = ( m[pos].estado_confinamento != 'N' && m[pos].estado_confinamento != 'Q' && m[pos].estado_confinamento !='I' ); /* Verificação se o caracter é algum em (N, I, Q) */

        if ( check )
            printf( "Insira uma opçao valida.\n" );
    } while ( check );

    if ( m[pos].estado_confinamento == 'I' || m[pos].estado_confinamento == 'Q' ) /* Se o membro está em confinamento, indicar a data e duraçao */
    {
        m[pos].data_confinamento = ler_data( "Indique a data em que o membro entrou em confinamento", 2021 );
        m[pos].duracao = ler_inteiro( "Indique a duracao do confinamento em dias", 7, 30 );
    }
}

void alterar_vacinacao( t_membro *m, int qt_membros, int* qt_vacinados )
{
    int pos = 0;
    int num_utente = -1;
    int last_vacinacao = 0;

    do
    {
        num_utente = ler_inteiro( "Introduza o numero de utente do membro académico a atualizar", 1, 9999 ); /* Pedir numero de utente de membro a ser atualizado */

        pos = procurar_membro( m, num_utente, qt_membros );

        if ( pos == -1 )
            printf( "Nao foi possivel encontrar membro academico.\n" );
    } while ( pos == -1 );

    if ( m[pos].vacinacao != 0 )
        printf( "Vacinacao atual: %d Dose\n", m[pos].vacinacao ); /* Mostrar numero de vacinas para referencia */
    else
        printf( "Vacinacao atual: Sem vacina\n" );

    last_vacinacao = m[pos].vacinacao;
    m[pos].vacinacao = ler_inteiro( "Introduza o novo numero de vacinas", 0, 3 ); /* Pedir novo numero de vacinas */

    if ( m[pos].vacinacao == 0 )    /* Se o novo numero de vacinação for zero, limpar a data da ultima vacinação */
    {
        m[pos].ultima_vacina.ano = 0;
        m[pos].ultima_vacina.dia = 0;
        m[pos].ultima_vacina.mes = 0;

        if ( last_vacinacao > 0 ) // Se o utilizador estava vacinado e dexou de estar
            (*qt_vacinados)--;

    }
    else                            /* Se não, perguntar a nova data... */
    {
        m[pos].ultima_vacina = ler_data( "Insira a data da ultima vacinacao", m[pos].ano_nascimento );

        if ( last_vacinacao == 0 ) // Se o utilizador nao estava vacinado e comecou a estar
            (*qt_vacinados)++;
    }
}

t_teste* agendar_teste( t_teste* p_teste, int *p_qt_testes_agendados, int qt_testes_realizados, t_membro *p_membros, int qt_membros )
{

    if ( p_teste == NULL )
        p_teste = malloc( sizeof(t_teste) );

    else
        p_teste = realloc( p_teste, (*p_qt_testes_agendados + qt_testes_realizados + 1) * sizeof(t_teste) );

    if ( p_teste == NULL )
    {
        printf( "***ERRO AO ALOCAR ESPACO NA MEMORIA***\n" );
        return NULL;
    }

    p_teste[*p_qt_testes_agendados + qt_testes_realizados] = ler_teste( p_teste, qt_testes_realizados, *p_qt_testes_agendados, p_membros, qt_membros );

    (*p_qt_testes_agendados)++;
    return p_teste;
}

void atualizar_estados( t_membro *m, int qt_membros, int* qt_vacinados ) /* Sub Menu para atualizar membro */
{
    char sub_opt = '\0';

    do
    {
        printf( "\n[V] - Alterar estado de Vacinacao\n[C] - Alterar estado de Confinamento\n" );
        sub_opt = toupper( ler_char( "\t\tOpcao --> ") );

        if ( sub_opt != 'V' && sub_opt != 'C' ) printf( "Insira uma opcao valida." );

    } while ( sub_opt != 'V' && sub_opt != 'C' );

    switch ( sub_opt )
    {
        case 'V':
            alterar_vacinacao( m, qt_membros, qt_vacinados );
            break;
        case 'C':
            alterar_confinamento( m, qt_membros );
            break;
    }
}

