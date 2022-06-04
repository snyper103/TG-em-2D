#ifndef STDLD_H_INCLUDED
#define STDLD_H_INCLUDED
#define MAX 5

typedef struct informacao
{
    double ctrlpoints[6][3];
    double vetColorEdge[3], vetColorPoint[3];
    short ID, numCtrlPoints;
}info;

typedef struct elementos elem;
typedef struct noDiscritor lista;

bool criaLista( lista** );
bool criaNo( elem**, info );
bool liberaLista( lista* );

bool listaVazia( lista* );
bool listaCheia( lista* );
short listaTamanho( lista* );

bool insereListaInicio( lista*, info );
bool insereListaMeio( lista*, info );
bool insereListaFinal( lista*, info );

bool removeInicio( lista* );
bool removeMeio( lista*, short );
bool removeFinal( lista* );

bool consultaListaPos( lista*, unsigned short, info* );

#endif
