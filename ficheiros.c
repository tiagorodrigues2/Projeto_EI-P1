#include <stdio.h>

#include "ficheiros.h"

void gravar_membros( t_membro *m, int qt_membros )
{
    FILE *p_file = fopen( "membros.dat", "wb" );
    size_t controlo = 0;

    if ( p_file != NULL )
    {
        controlo = fwrite( &qt_membros, sizeof(int), 1, p_file );
        if ( controlo != 1 )
        {
            printf( "***ERRO AO GUARDAR CONTAGEM DE DADOS***\n" );
            fclose( p_file );
            return;
        }

        controlo = fwrite( m, sizeof(t_membro), qt_membros, p_file );
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

void carregar_membros( t_membro* m, int *qt_membros )
{
    FILE* p_file = fopen( "membros.dat", "rb" );
    size_t controlo = 0;

    if ( p_file != NULL )
    {
        controlo = fread( qt_membros, sizeof(int), 1, p_file );
        if ( controlo != (size_t)1 )
        {
            printf( "**ERRO AO LER QUANTIDADE DE DADOS**\n" );
            fclose( p_file );
            return;
        }

        if ( *qt_membros > 0 )
        {
            controlo = fread( m, sizeof(t_membro), *qt_membros, p_file );
            if ( controlo != *qt_membros )
            {
                printf( "**ERRO AO LER DADOS**\n" );
                fclose( p_file );
                return;
            }
        }

    }
    else
    {
        printf( "**ERRO AO ABRIR FICHEIRO PARA LER BINARIO**\n" );
    }
}
