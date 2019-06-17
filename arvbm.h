#include <stdio.h>
#include <stdlib.h>
#include "pizza.h"

const int t = 2;

typedef struct arvbm{
  int nchaves, folha;
  TPizza *chave;
  struct arvbm **filho, *prox;
}TABM;

TABM *cria(int t);
TABM *inicializa(void);
void libera(TABM *a);
TABM *busca(TABM *a, int mat);
void imprime(TABM *a, int andar);
TABM *divisao(TABM *x, int i, TABM* y, int t);
TABM *insere_nao_completo(TABM *x, int mat, int t);
TABM *insere(TABM *T, int mat, int t);

