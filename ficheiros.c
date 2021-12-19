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

        controlo = fwrite( m, sizeof(t_membro), qt_membros, p_file );       /* Esvrever dados de registo */
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

t_membro* carregar_membros( int *qt_membros )
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
    else
    {
        printf( "**ERRO AO ABRIR FICHEIRO PARA LER BINARIO**\n" );
    }

    return m;
}
