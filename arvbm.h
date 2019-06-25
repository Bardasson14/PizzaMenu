#include "pizza.h"

#define t 2

typedef struct arvbm{
  int nchaves, folha, chave[(2*t)-1];
  long ind[(2*t)-1], filho[2*t], prox;
}TABM;


TABM *le_pagina(FILE *indices, long indice); //Lê uma página na posição atual do cursor

void salva_pagina(FILE *indices, long indice, TABM *pagina);  //Escreve página na posição atual do cursor

TABM *cria();    //Cria árvore em MP

//void libera(TABM *a); //libera árvore na MP

void imprime(TABM *a, int andar, FILE *indices); //Imprime árvore do arquivo

void divisao(FILE *indices, long pai, int i, long filho);  //Divide no arquivo

void insere_nao_completo(FILE *indices, long x, int mat); //Insere em nó não completo

void insere(long T, int cod, FILE *catalogo, FILE *indices);

void remove_pizza(); //Remove(a melhor opção é mudar os indices para nunca parar aqui)

void remove_categoria(); //Remove todas as pizzas de uma mesma categoria

TPizza *busca_pizza(FILE *indices, FILE* catalogo, int cod);  //Busca a pizza pela chave primária

TPizza **busca_categoria(FILE *indices, FILE* catalogo, char * categoria); //Busca todas as pizzas de uma categoria

void altera_Pizza(FILE *indices, FILE* catalogo, int cod);  //Pede inputs para alterar as informações de uma pizza de chave primária cod;
