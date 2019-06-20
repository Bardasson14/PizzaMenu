#include "arvbm.h"


TABM *cria(int t){
  TABM* novo = (TABM*)malloc(sizeof(TABM));
  novo->nchaves = 0;
  novo->chave =(int*)malloc(sizeof(int)*((t*2)-1));
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

TABM *divisao(TABM *x, int i, TABM* y, int t){
}


TABM *insere_nao_completo(TABM *x, int mat, int t){
}

TABM *insere(TABM *T, int mat, int t){
}


/*int main(void){
  TABM * arvore = inicializa();
  int num = 0, from, to;
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
    
    else if(!num){
      printf("\n");
      imprime(arvore,0);
    }
    else arvore = insere(arvore, num, t);
    printf("\n\n");
  }

 */
}

