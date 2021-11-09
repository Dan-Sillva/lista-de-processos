/*
    Implementar um escalonador de processos por prioridades.

Requisitos

O processo deve conter um id do processo (PID) e o nome do processo.

O escalonador deve conter pelo menos duas filas de prioridade.

Devem constar as seguintes funções

Adicionar processo: adiciona um novo processo a uma fila. Caso não seja definida a fila o processo será adicionado à 
fila de menor prioridade

Executar processo: executa o primeiro processo da fila de maior prioridade que esteja com processos pendentes. O processo 
é movido para o final da fila de prioridade imediatamente inferior.

Mover processo entre filas: transfere um processo de uma fila para outra. O processo movido pode estar em qualquer posição 
da fila de origem e será colocado no final da fila de destino.

Finalizar processo: Encerra o primeiro processo da fila de maior prioridade, removendo-o completamente do sistema.

Finalizar processo específico: Encerra um processo a partir do ID escolhido, removendo-o completamente do sistema. 
O processo pode estar em qualquer fila no momento da remoção.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct processo Processo;
typedef struct lista Lista;

// ---> struct
struct lista
{
    Processo *inicio = NULL;
};

struct processo
{
    int PID;
    char nome[200];
    Processo *proximo;
};

// ---> funcoes
Processo *criarProcesso(char nome[200]);

void adicionarProcesso(Processo *processo, bool prioridade);
void adicionarProcesso(Processo *processo);

void executarProcesso();
void moverProcesso(Lista *listaA, Lista *listaB, int posicao);
void finalizarProcesso();
void compararProcesso(Processo *pAtual, Processo *pAnterior, int pid);
void finalizarProcessoEspecifico(int PID);

// ---> variaveis globais
Lista *listaPrioridade = (Lista *)malloc(sizeof(Lista));
Lista *listaComum = (Lista *)malloc(sizeof(Lista));

int id_list = 0;

int main() // MAIN-----------------------------------------------------------------
{

    listaPrioridade->inicio = NULL;
    listaComum->inicio = NULL;

    //printf("tudo ok\n");
    //Processo *p = criarProcesso("teste");
    //printf("--p nome: %s", p->nome);

    finalizarProcessoEspecifico(2);
}

Processo *criarProcesso(char nome[200])
{
    Processo *novoProcesso = (Processo *)malloc(sizeof(Processo));
    novoProcesso->PID = id_list;
    id_list++;
    strcpy(novoProcesso->nome, nome);
    novoProcesso->proximo = NULL;

    return novoProcesso;
}

void adicionarProcesso(Processo *processo, bool prioridade)
{
    Processo *novoProcesso = processo;

    if (prioridade)
    {
        if (listaPrioridade->inicio != NULL)
        {
            listaPrioridade->inicio = novoProcesso;
        }
        else
        {
            Processo *inicial = listaPrioridade->inicio;
            while (inicial->proximo != NULL)
            {
                inicial = inicial->proximo;
            }
            inicial->proximo = novoProcesso;
        }
    }
    else
    {
        if (listaComum->inicio != NULL)
        {
            listaComum->inicio = novoProcesso;
        }
        else
        {
            Processo *inicial = listaComum->inicio;
            while (inicial->proximo != NULL)
            {
                inicial = inicial->proximo;
            }
            inicial->proximo = novoProcesso;
        }
    }
}

void adicionarProcesso(Processo *processo)
{
    Processo *novoProcesso = processo;

    if (listaComum->inicio != NULL)
    {
        listaComum->inicio = novoProcesso;
    }
    else
    {
        Processo *inicial = listaComum->inicio;
        while (inicial->proximo != NULL)
        {
            inicial = inicial->proximo;
        }
        inicial->proximo = novoProcesso;
    }
}

void executarProcesso()
{
    if (listaPrioridade->inicio == NULL)
    {
        if (listaComum->inicio == NULL)
        {
            printf("\nNenhum processo escalado.");
        }
        else
        {
            //lista comum
            Processo *auxProcesso = listaComum->inicio;

            listaComum->inicio = auxProcesso->proximo;

            free(auxProcesso);
        }
    }
    else
    {
        //lista especial
        Processo *auxProcesso = listaComum->inicio;
        while (auxProcesso->proximo != NULL)
        {
            auxProcesso = auxProcesso->proximo;
        }

        auxProcesso->proximo = listaPrioridade->inicio;
        auxProcesso = auxProcesso->proximo;
        listaPrioridade->inicio = auxProcesso->proximo;
        auxProcesso->proximo = NULL;
    }
};

void moverProcesso(Lista *listaA, Lista *listaB, int posicao)
{
    //Move o processo da lista A para a lista B
    Processo *auxProcesso = listaA->inicio;
    Processo *anteriorAux = NULL;
    for (int n = 1; n < posicao; n++)
    {
        anteriorAux = auxProcesso;
        auxProcesso = auxProcesso->proximo;
    }
    // auxProcesso->anterior->proximo = auxProcesso->proximo;

    if (anteriorAux == NULL)
    {
        listaA->inicio = auxProcesso->proximo;
    }
    else
    {
        anteriorAux = auxProcesso->proximo;
    }

    auxProcesso->proximo = listaB->inicio;
    listaB->inicio = auxProcesso;
};

void finalizarProcesso()
{
    Processo *auxProcesso;
    if (listaPrioridade->inicio == NULL)
    {
        auxProcesso = listaComum->inicio;
        listaComum->inicio = auxProcesso->proximo;
        free(auxProcesso);
    }
    else
    {
        auxProcesso = listaPrioridade->inicio;
        listaPrioridade->inicio = auxProcesso->proximo;
        free(auxProcesso);
    }
};

void compararProcesso(Processo *pAtual, Processo *pAnterior, int pid)
{
    if (pAtual->PID == pid)
    {
        pAnterior->proximo = pAtual->proximo;
        free(pAtual);
    }
}

void finalizarProcessoEspecifico(int PID)
{
    if (listaPrioridade->inicio != NULL || listaComum->inicio != NULL) //verificar se lista de prioridade está vazia
    {
        Processo *auxProcesso;
        Processo *anteriorAux = NULL;
        auxProcesso = listaPrioridade->inicio; // verificar na lista de prioridade
        while (auxProcesso->proximo != NULL)
        {
            compararProcesso(auxProcesso, anteriorAux, PID);
            anteriorAux = auxProcesso;
            auxProcesso = auxProcesso->proximo;
            compararProcesso(auxProcesso, anteriorAux, PID);
        }

        auxProcesso = NULL; //limpar variável
        anteriorAux = NULL;

        auxProcesso = listaComum->inicio; // verificar na lista comum
        while (auxProcesso->proximo != NULL)
        {
            compararProcesso(auxProcesso, anteriorAux, PID);
            anteriorAux = auxProcesso;
            auxProcesso = auxProcesso->proximo;
            compararProcesso(auxProcesso, anteriorAux, PID);
        }
    }
    else
    {
        printf("\nERRO: nao foi possivel finalizar o processo(01)\n(01: sem processos existentes)"); //listas vazias
    }
};