#include "arvbm.h"
#include <stdio.h>
#include <stdlib.h>

TABM* cria(int t){
  TABM* novo = (TABM*)malloc(sizeof(TABM));
  novo->nchaves = 0;
  novo->chave = (int*) malloc(sizeof(int)*((t*2)-1));
  novo->folha = 1;
  novo->ind =  (long*)malloc(sizeof(long)*(2*t-1));
  novo->filho = (long*)malloc(sizeof(long)*t*2);
  novo->prox = NULL;
  int i;
  for(i=0; i<(t*2); i++) 
    novo->filho[i] = NULL;
  for (i=0; i<(t*2-1); i++){
    novo->chave[i] = NULL;
    novo->ind[i] = NULL;
  }


  return novo;
}


TABM *inicializa(void){
  return NULL;
}

void libera(TABM *a){
  if(a){
    if(!a->folha){
      int i;
      for(i = 0; i <= a->nchaves; i++) libera(a->filho[i]);
    }
    int i;
    for(i = 0; i < a->nchaves;i++) free(a->chave[i]);
    free(a);
  }
}

TABM *busca(TABM *a, int mat){
}

void imprime(TABM *a, int andar){
}

TABM *divisao(TABM *x, int i, TABM* y, int t){
}


TABM *insere_nao_completo(TABM *x, int mat, int t){
}

TABM *insere(TABM *T, int mat, int t){
}


int main(void){
  TABM * arvore = inicializa();
  int num = 0;
  FILE *f = fopen("dados_iniciais.dat", "rb");
  int t = 2;
  while(num != -1){
    printf("Digite um numero para adicionar. 0 para imprimir e -1 para sair\n");
    scanf("%i", &num);
    if(num == -1){
      printf("\n");
      imprime(arvore,0);
      libera(arvore);
      return 0;
    }
    /*
    else if(num == -9){
      scanf("%d", &from);
      arvore = retira(arvore, from, t);
      Imprime(arvore,0);
    }
    */
    else if(!num){
      printf("\n");
      imprime(arvore,0);
    }
    else if (num==1){
        TPizza *p = le_pizza(f);
        printf("NOME DA PIZZA: %s\n", p->nome);
        printf("PREÇO DA PIZZA: %f\n", p->preco);
        printf("DESCRIÇÃO DA PIZZA: %s\n", p->descricao);
        printf("CÓDIGO DA PIZZA: %d\n", p->cod);
        arvore = insere(arvore, p, t);
    }
    printf("\n\n");
  }
}

