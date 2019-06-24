#include <stdio.h>
#include <stdlib.h>
#include "pizza.h"


typedef struct arvbm{
  int nchaves, folha, *chave;
  long *ind, *filho, prox;
}TABM;


TABM *le_pagina(FILE *indices, int t); //Lê uma página na posição atual do cursor

void salva_pagina(FILE *indices, TABM *pagina);  //Escreve página na posição atual do cursor

TABM *cria(int t);    //Cria árvore em MP

//void libera(TABM *a); //libera árvore na MP

void imprime(TABM *a, int andar, FILE *indices); //Imprime árvore do arquivo

void divisao(FILE *indices, long pai, int i, long filho, int t);  //Divide no arquivo

void insere_nao_completo(FILE *indices, long x, int mat, int t); //Insere em nó não completo

void insere(long T, int cod, int t, FILE *catalogo, FILE *indices);

void remove_pizza(); //Remove(a melhor opção é mudar os indices para nunca parar aqui)

void remove_categoria(); //Remove todas as pizzas de uma mesma categoria

TPizza *busca_pizza(FILE *indices, FILE* catalogo, int cod);  //Busca a pizza pela chave primária

TPizza **busca_categoria(FILE *indices, FILE* catalogo, char * categoria); //Busca todas as pizzas de uma categoria

void altera_Pizza(FILE *indices, FILE* catalogo, int cod);  //Pede inputs para alterar as informações de uma pizza de chave primária cod;
