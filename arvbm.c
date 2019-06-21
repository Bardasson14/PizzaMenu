#include "arvbm.h"
#include <stdio.h>
#include <stdlib.h>

void salva_pagina(FILE* indices, TABM *pagina){

  fwrite(pagina, sizeof(TABM), 1, indices);

}

TABM *le_pagina(FILE *indices, int t){

  TABM *pagina = cria(t);
  fread(pagina, sizeof(TABM), 1, indices);

}



TABM *cria(int t){
  TABM* novo = (TABM*)malloc(sizeof(TABM));
  novo->nchaves = 0;
  novo->chave = (int*) malloc(sizeof(int)*((t*2)-1));
  novo->folha = 1;
  novo->ind = (long*) malloc(sizeof(long)*((t*2)-1));
  novo->filho = (long*)malloc(sizeof(long)*t*2);
  novo->prox = NULL;
  int i;

  for(i=0; i<(t*2); i++) novo->filho[i] = NULL;

  for(i=0; i<(2*t - 1); i++){
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

      for(i = 0; i < a->nchaves; i++){

        libera(a->chave[i]);
        libera(a->ind[i]);

      }
    }
    free(a);
  }
}

TPizza *busca_pizza(FILE *indices, FILE* catalogo, int cod){

  if(!indices || !catalogo) return NULL;

  TABM *pagina;

  //Pegando a pagina no arquivo
  fread(&pagina, sizeof(TABM), 1, indices);

  int i = 0;
  while((i < pagina->nchaves) && (cod > pagina->chave[i])) i++;

  if((i < pagina->nchaves) && (pagina->folha) && (cod == pagina->chave[i])){

    TPizza *pizza = NULL;
    fseek(catalogo, pagina->ind[i], SEEK_SET);
    pizza = le_pizza(catalogo);
    return pizza;

  }

  if(pagina->folha) return NULL;

  if(pagina->chave[i] == cod) i++;
  fseek(indices, pagina->filho[i], SEEK_SET);
  return busca_pizza(indices, catalogo, cod);

}

void imprime(TABM *a, int andar){
}

void divisao(FILE *indices, long pai, int i, long filho, int t){

  TABM *z = cria(t);

  fseek(indices, pai, SEEK_SET);
  TABM *x = le_pagina(indices, t);

  fseek(indices, filho, SEEK_SET);
  TABM *y = le_pagina(indices, t);

  z->folha = y->folha;
  if(!y->folha){

    int j;
    z->nchaves = t-1;
    for(j = 0; j < t; j++)  z->chave[j] = y->chave[j+t];
    for(j = 0; j < t; j++){

      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;

    }
  }
  else{

    int j;
    z->nchaves = t; //z possuirá uma chave a mais que y se for folha
    for(j = 0; j < t; j++)  z->chave[j] = y->chave[j+t-1];  //Caso em que y é folha, temos que passar a info do nó para a direita
    y->prox = z;

  }

  y->nchaves = t-1;
  int k;
  for(k = x->nchaves; k >= i; k--)  x->filho[k+1] = x->filho[k];
  x->filho[i] = z;
  for(k = x->nchaves; k >= i; k--)  x->chave[k] = x->chave[k+1];
  x->chave[i-1] = y->chave[t-1];
  x->nchaves++;

  //Nesse ponto ja fiz a divisão, agora basta sobreescrever no arquivo;

  fseek(indices, pai, SEEK_SET);  //Sobreescreve o pai
  salva_pagina(indices, x);

  fseek(indices, filho, SEEK_SET);  //Sobreescreve o filho
  salva_pagina(indices, y);

  fseek(indices, 0L, SEEK_END);  //Escreve a nova página no final;
  salva_pagina(indices, z);

  libera(x);
  libera(y);
  libera(z);

}


void *insere_nao_completo(FILE *indices, long *T, int mat, int t){
  fseek(indices, T, SEEK_SET);
  TPizza *p = le_pizza(indices);
  TABM *x = le_pagina(indices, t);
  int i = x->nchaves-1;
  if (x->folha){
    while ((i>=0)&&(mat<x->chave[i])){
      x->chave[i+1] = x->chave[i];
      i--;
    }
    x->chave[i+1] = mat;
    x->nchaves++;
    salva_pagina(indices, x);
    return;
  }
  while ((i>=0)&&(mat < x->chave[i]))
    i--;
  i++;
  fseek(indices, &x->filho[i], SEEK_SET);
  TABM *filho = le_pagina(indices, t);
  if ((filho->nchaves) == (2*t-1)){
    divisao(indices, x, i+1, x->filho[i], t);
    if (mat > x->chave[i])
      i++;
  }
  fseek(indices, &x->filho[i], SEEK_SET);
  salva_pagina(indices, x->filho[i]);
  return;
}


void *insere(long *T, int cod, int t, FILE *catalogo, FILE *indices){
  if (busca_pizza(indices, catalogo, cod))
    return;
  int aux = fseek(indices, T, SEEK_SET);
  if (aux!=0){
    TABM *x = cria(t);
    fseek(indices, 0L, SEEK_END);
    //long pos = ftell(indices);;
    x->chave[0] = cod;
    x->nchaves = 1;
    salva_pagina(indices, x);
    return;
  }

  if (!aux){
    fseek(indices, T, SEEK_SET);
    TABM *x = le_pagina(indices, t);
    if (x->nchaves == ((2*t)-1)){
      TABM *s = cria(t);
      s->nchaves = 0;
      s->folha = 0;
      s->filho[0] = cod;
      divisao(indices, s, 1, T, t);
      return;
    }
  }

  fseek(indices, T, SEEK_SET);
  TPizza *p = le_pizza(indices);
  insere_nao_completo(indices, T, p->cod, t);
  return;
}


int main(void){
  TABM * arvore = inicializa();
  int num = 0;
  FILE *dados = fopen("dados_iniciais.dat", "rb");
  FILE *indices = fopen("indices.dat", "rb+");
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
        TPizza *p = le_pizza(dados);
        printf("NOME DA PIZZA: %s\n", p->nome);
        printf("PREÇO DA PIZZA: %f\n", p->preco);
        printf("DESCRIÇÃO DA PIZZA: %s\n", p->descricao);
        printf("CÓDIGO DA PIZZA: %d\n", p->cod);
        fseek(indices, 0L, SEEK_END);
        insere(ftell(indices), p->cod, t, dados, indices);
         
    }
    printf("\n\n");
  }
}
