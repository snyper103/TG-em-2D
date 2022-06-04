#include "stdld.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <clocale>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#define SWAP(a,b)({a^=b;b^=a;a^=b;})
#define descobreSize(arr,n)({n=sizeof(arr)/sizeof(arr[0]);})

struct elementos
{
    struct informacao dados;
    struct elementos* prox,* ant;
};
struct noDiscritor
{
    struct elementos* pInicio,* pFinal;
    unsigned long qtd;
};

/**Funções proprias da .cpp*/
void fazTroca( struct informacao* dado1, struct informacao* dado2 )
{
    info temp = *dado1;
    *dado1 = *dado2;
    *dado2 = temp;
}
bool encontraNoPosicao( struct elementos** no, long pos )
{
    register long i;

    if ( !no )
        return false;

    for ( i = 0; (i < pos) && ((*no)->prox != NULL); ++i, (*no) = (*no)->prox );

    if ( (i != pos) && ((*no)->prox == NULL) )
        return false;

    return true;
}

/**Funções que estão na .h*/
bool criaLista( lista** li )
{
    *li = (lista*)malloc(sizeof(lista));

    if ( !li )
        return false;

    (*li)->pInicio = NULL;
    (*li)->pFinal = NULL;
    (*li)->qtd = 0;

    return true;
}
bool criaNo( elem** no, info dado )
{
    *no = (elem*)malloc(sizeof(elem));

    if ( !no )
        return false;

    (*no)->dados = dado;
    (*no)->prox = NULL;
    (*no)->ant = NULL;

    return true;
}
bool liberaLista( lista* li )
{
    elem* no;

    if ( !li )
        return false;

    while ( (li->pInicio) != NULL )
    {
        no = li->pInicio;
        li->pInicio = li->pInicio->prox;

        free(no);
    }

    free(li);

    return true;
}

bool listaVazia( lista* li )
{
    if ( !li )
        return false;

    return (li->qtd == 0) ? (true) : (false);
}
bool listaCheia( lista* li )
{
    if ( !li )
        return false;

    return (li->qtd == MAX) ? (true) : (false);
}
short listaTamanho( lista* li )
{
    if ( !li )
        return -1;

    return li->qtd;
}

bool insereListaInicio( lista* li, info dado )
{
    elem* no;
    short bol = criaNo(&no, dado);

    if ( (!li) || (!bol) )
        return false;

    if ( listaCheia(li) )
        return false;

    no->prox = li->pInicio;

    if ( listaVazia(li) )
        li->pFinal = no;

    else
        li->pInicio->ant = no;

    li->pInicio = no;
    ++li->qtd;

    return true;
}
bool insereListaMeio( lista* li, info dado )
{
    elem* no,* aux;
    bool bol;

    if ( !li )
        return false;

    if ( listaCheia(li) )
        return false;

    if ( listaVazia(li) )
    {
        bol = insereListaInicio(li, dado);
        return bol;
    }

    bol = criaNo(&no, dado);

    if ( !bol )
        return false;

    if ( li->pFinal->dados.ID <= dado.ID )
    {
        bol = insereListaFinal(li, dado);
        return bol;
    }

    if ( li->pInicio->dados.ID >= dado.ID )
    {
        bol = insereListaInicio(li, dado);
        return bol;
    }

    for ( aux = li->pInicio; (aux->prox != NULL) && (aux->dados.ID <= dado.ID); aux = aux->prox );

    no->prox = aux;
    no->ant = aux->ant;
    aux->ant->prox = no;
    aux->ant = no;

    ++li->qtd;

    return true;
}
bool insereListaFinal( lista* li, info dado )
{
    elem* no;
    short bol = criaNo(&no, dado);

    if ( (!li) || (!bol) )
        return false;

    if ( listaCheia(li) )
        return false;

    no->ant = li->pFinal;

    if ( listaVazia(li) )
        li->pInicio = no;

    else
        li->pFinal->prox = no;

    li->pFinal = no;
    ++li->qtd;

    return true;
}

bool removeInicio( lista* li )
{
    elem* no;

    if ( !li )
        return false;

    if ( listaVazia(li) )
        return false;

    no = li->pInicio;

    if ( li->qtd == 1 )
    {
        li->pFinal = NULL;
        li->pInicio = NULL;
    }

    else
    {
        li->pInicio = li->pInicio->prox;
        li->pInicio->ant = NULL;
    }

    --li->qtd;

    free(no);

    return true;
}
bool removeMeio( lista* li, short ID )
{
    elem* no;
    short bol;

    if ( !li )
        return false;

    if ( listaVazia(li) )
        return false;

    if ( li->pInicio->dados.ID == ID )
    {
        bol = removeInicio(li);
        return bol;
    }

    if ( li->pFinal->dados.ID == ID )
    {
        bol = removeFinal(li);
        return bol;
    }

    for ( no = li->pInicio->prox; (no->prox != NULL) && (no->dados.ID != ID); no = no->prox );

    if ( no->prox == NULL )
        return false;

    no->ant->prox = no->prox;
    no->prox->ant = no->ant;

    --li->qtd;

    free(no);

    return true;
}
bool removeFinal( lista* li )
{
    elem* no;

    if ( !li )
        return false;

    if ( listaVazia(li) )
        return false;

    no = li->pFinal;

    if ( li->qtd == 1 )
    {
        li->pFinal = NULL;
        li->pInicio = NULL;
    }

    else
    {
        li->pFinal = li->pFinal->ant;
        li->pFinal->prox = NULL;
    }

    --li->qtd;

    free(no);

    return true;
}

bool consultaListaPos( lista* li, unsigned short pos, info* dado )
{
    elem* no;
    register short i;

    if ( !li )
        return false;

    if ( (listaVazia(li)) || (pos <= 0) || (pos > li->qtd) )
        return false;

    for ( i = 1, no = li->pInicio; (i != pos) && (no != NULL); ++i, no = no->prox );

    if ( !no )
        return false;

    *dado = no->dados;

    return true;
}