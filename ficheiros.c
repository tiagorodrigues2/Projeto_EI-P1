#include <stdio.h>

#include "ficheiros.h"

/*
    falta te colocar os argumentos no fprintf...
    Sugeria refazeres a string para ser mais facil de ler
    atenção a duração, hora:minuto

*/
void log_vacina(char nome[MAX_NOME], int estado_vacinacao, t_membro v[]) {

    FILE *log_vacinas;
    log_vacinas = fopen("log_vacinas.txt", "a");
    if (log_vacinas != NULL) {
        fprintf(log_vacinas, "Nome: %s | Numero de Utente: %d | Estado de Vacinacao: %s. "
                "Dados de Teste - ID: %d | Tipo: %s | Data: %d/%d/%d | Resultado: %i | Hora: %d:%d | Duracao: %d \n");
    }
    fclose(log_vacinas);
}


void gravar_lembros( t_membro *m, int qt_membros )
{
    FILE *p_file = fopen( "membros.dat", "wb" );
    size_t controlo = 0;

    if ( p_file != NULL )
    {
        controlo = fwrite( qt_membros, sizeof(int), 1, p_file );
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
