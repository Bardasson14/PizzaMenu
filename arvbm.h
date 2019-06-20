#include <stdio.h>
#include <stdlib.h>
#include <pizza.h>

const int t = 2;

typedef struct arvbm{
  int nchaves, folha;
  long *ind, *filho, prox;
}TABM;

void cria(int t);    //Aloca espaço no para a estrutura no arquivo

void libera(TABM *a); //libera árvore na MP

void imprime(TABM *a, int andar); //Imprime árvore do arquivo

void divisao(TABM *x, int i, TABM* y, int t);  //Divide no arquivo

void insere_nao_completo(TABM *x, int mat, int t); //Insere em nó não completo

void insere(TABM *T, int mat, int t);  //Insere

void remove(); //Remove(a melhor opção é mudar os indices para nunca parar aqui)

void remove_categoria(); //Remove todas as pizzas de uma mesma categoria

TPizza *busca_pizza(FILE *indices, FILE* catalogo, int cod);  //Busca a pizza pela chave primária

TPizza **busca_categoria(FILE *indices, FILE* catalogo, char * categoria); //Busca todas as pizzas de uma categoria

void altera_Pizza(FILE *indices, FILE* catalogo, int cod);  //Pede inputs para alterar as informações de uma pizza de chave primária cod;

