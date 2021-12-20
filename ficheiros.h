#ifndef FICHEIROS_H_INCLUDED
#define FICHEIROS_H_INCLUDED

#include "dados.h"

extern void gravar_membros( t_membro*, int );
extern t_membro* carregar_membros( int* );
extern void gravar_testes( t_testes*, int, int );
extern t_testes* carregar_testes( int*, int * );

#endif // FICHEIROS_H_INCLUDED
