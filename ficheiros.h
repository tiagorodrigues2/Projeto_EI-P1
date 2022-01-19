#ifndef FICHEIROS_H_INCLUDED
#define FICHEIROS_H_INCLUDED

#include "dados.h"

extern void gravar_membros( t_membro*, int ); // (Vetor dos membros, quantidade de membros)
extern t_membro* carregar_membros( int*, int* ); // (Quantidade de membros, membros vacinados)
extern void gravar_testes( t_teste*, int, int ); // (Vetor dos testes, quantidade de testes realizados, quantidade de testes agendados)
extern t_teste* carregar_testes( int*, int * ); // (Quantidade de testes agendados, quantidade de testes realizados)
extern void log_resultado( t_teste, t_membro*, int ); // (Struct teste, vetor membros, quantidade de membros)

#endif // FICHEIROS_H_INCLUDED
