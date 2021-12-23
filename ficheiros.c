#include <stdio.h>
#include <stdlib.h>

#include "ficheiros.h"

void gravar_membros( t_membro *m, int qt_membros )
{
    FILE *p_file = fopen( "membros.dat", "wb" );
    size_t controlo = 0;

    if ( p_file != NULL )
    {
        controlo = fwrite( &qt_membros, sizeof(int), 1, p_file );       /* Escrever o numero de registos para depois saber quantos registos a ler... */
        if ( controlo != 1 )
        {
            printf( "***ERRO AO GUARDAR CONTAGEM DE DADOS***\n" );
            fclose( p_file );
            return;
        }

        controlo = fwrite( m, sizeof(t_membro), qt_membros, p_file );       /* Escrever dados de registo */
        if ( controlo != qt_membros )
        {
            printf( "***ERRO AO GUARDAR DADOS***\n" );
            fclose( p_file );
            return;
        }

        fclose( p_file );
    }
    else
    {
        printf( "***ERRO AO ABRIR ABRIR FICHEIRO membros.dat***\n" );
    }
}

t_membro* carregar_membros( int *qt_membros, int *qt_vacinados )
{
    FILE* p_file = fopen( "membros.dat", "rb" );
    size_t controlo = 0;
    t_membro* m = NULL;

    if ( p_file != NULL )
    {
        controlo = fread( qt_membros, sizeof(int), 1, p_file ); /* Ler numero de registos */
        if ( controlo != (size_t)1 )
        {
            printf( "**ERRO AO LER QUANTIDADE DE DADOS**\n" );
            fclose( p_file );
            return NULL;
        }

        if ( *qt_membros > 0 ) /* Se houver mais que um... */
        {
            m = malloc( *qt_membros * sizeof(t_membro) ); /* Alocar o espa�o na memoria necessitado */

            controlo = fread( m, sizeof(t_membro), *qt_membros, p_file ); /* Ler dados dos membros acad�micos */
            if ( controlo != *qt_membros )
            {
                printf( "**ERRO AO LER DADOS**\n" );
                fclose( p_file );
                return NULL;
            }
        }

    }

    for ( int i = 0; i < *qt_membros; i++ ) // Calcular numero de vacinados
        *qt_vacinados += m[i].vacinacao > 0;        // Se m[i].vacinacao > 0, soma 1, se nao, soma 0

    return m;
}

void gravar_testes( t_teste* p_teste, int qt_realizados, int qt_agendados )
{
    FILE *p_file = fopen( "testes.dat", "wb" );
    size_t controlo = 0;

    if ( p_file != NULL )
    {
        controlo += fwrite( &qt_realizados, sizeof(int), 1, p_file );
        controlo += fwrite( &qt_agendados, sizeof(int), 1, p_file );

        if ( controlo != 2 )
        {
            printf( "**ERRO AO GUARDAR QUANTIDADE DE DADOS**\n" );
            fclose( p_file );
            return;
        }

        controlo = fwrite( p_teste, sizeof(t_teste), qt_agendados + qt_realizados, p_file );

        if ( controlo != qt_agendados + qt_realizados )
        {
            printf( "**ERRO AO GUARDAR DADOS**\n" );
            fclose( p_file );
            return;
        }

        fclose( p_file );
    }
}

t_teste* carregar_testes( int *p_realizados, int *p_agendados )
{
    t_teste *p_teste = NULL;
    FILE *p_file = fopen( "testes.dat", "rb" );
    size_t controlo = 0;

    if ( p_file != NULL )
    {
        controlo += fread( p_realizados, sizeof(int), 1, p_file );
        controlo += fread( p_agendados, sizeof(int), 1, p_file );

        if ( controlo != 2 )
        {
            printf( "**ERRO AO LER QUANTIDADE DE REGISTOS**\n" );
            fclose( p_file );
            return NULL;
        }

        if ( *p_realizados + *p_agendados == 0 )
            return NULL;

        p_teste = malloc( sizeof(t_teste) * ((*p_realizados) + (*p_agendados)));
        controlo = fread( p_teste, sizeof(t_teste), ((*p_realizados) + (*p_agendados)), p_file );

        if ( controlo != ((*p_realizados) + (*p_agendados)) )
        {
            printf( "**ERRO AO LER REGISTOS**\n" );
            fclose( p_file );
            return NULL;
        }

        fclose( p_file );
    }

    return p_teste;
}

extern void log_resultado( t_teste t, t_membro *p_membros, int qt_membros )
{
    FILE *pFile = fopen( "resultados.txt", "a" );
    int pos = -1;

    pos = procurar_membro( p_membros, t.num_utente, qt_membros );
    t_membro m = p_membros[pos];

    if ( pFile )
    {

        fprintf( pFile, "Registado um resultado do teste %04d\n", t.id );
        fprintf( pFile, "Resultado: " );
        switch ( t.resultado )
        {
            case -1: fprintf( pFile, "INCONCLUSIVO\n" ); break;
            case 0: fprintf( pFile, "NEGATIVO\n" ); break;
            case 1: fprintf( pFile, "POSITIVO\n" ); break;
        }
        fprintf( pFile, "Tipo de teste: %s\n", t.tipo == 'P' ? "PCR" : "Antigenio" );
        fprintf( pFile, "Data e Hora: %02d/%02d/%02d %02d:%02d\n", t.data.dia, t.data.mes, t.data.ano, t.hora.hora, t.hora.minuto );
        fprintf( pFile, "Duracao: %02d:%02d\n", t.duracao.hora, t.duracao.minuto );
        fprintf( pFile, "Utente: %s (%d Dose)\n_______________________\n\n", m.nome, m.vacinacao );


        fclose( pFile );
    }
    else
    {
        printf( "**ERRO AO ABRIR FICHEIRO LOG**\n" );
    }
}
