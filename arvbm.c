#include "arvbm.h"

TABM *cria(int t){
  TABM* novo = (TABM*)malloc(sizeof(TABM));
  novo->nchaves = 0;
  novo->chave = (TPizza**) malloc(sizeof(TPizza*)*((t*2)-1));
  novo->folha = 1;
  novo->filho = (TABM**)malloc(sizeof(TABM*)*t*2);
  novo->prox = NULL;
  int i;
  for(i=0; i<(t*2); i++) novo->filho[i] = NULL;
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

TABM *busca_prim(TABM *a, int cod){
  if (!a) return NULL;
  int i = 0;
  while ((i < a->nchaves) && (cod > a->chave[i]->cod)) i++;
  if ((i < a->nchaves) && (a->folha) && (cod == a->chave[i]->cod)) return a;
  if (a-> folha) return NULL;
  if (a->chave[i]->cod == cod) i++;
  return busca_prim(a->filho[i], cod);
}

void imprime(TABM *a, int andar){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      imprime(a->filho[i],andar+1);
      for(j=0; j<=andar; j++) printf("   ");
      printf("%d\n", a->chave[i]->cod);
    }
    imprime(a->filho[i],andar+1);
  }
}

TABM *divisao(TABM *x, int i, TABM* y, int t){
  TABM *z = cria(t);
  z->folha = y->folha;
  int j;
  if(!y->folha){
    z->nchaves = t-1;
    for(j=0;j<t-1;j++) z->chave[j]->cod = y->chave[j+t]->cod;
    for(j=0;j<t;j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;
    }
  }
  else {
    z->nchaves = t; //z possuir� uma chave a mais que y se for folha
    for(j=0;j < t;j++) z->chave[j]->cod = y->chave[j+t-1]->cod;//Caso em que y � folha, temos q passar a info para o n� da direita
    y->prox = z;
  }
  y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
  x->filho[i] = z;
  for(j=x->nchaves; j>=i; j--) x->chave[j]->cod = x->chave[j-1]->cod;
  x->chave[i-1]->cod = y->chave[t-1]->cod;
  x->nchaves++;
  return x;
}


TABM *insere_nao_completo(TABM *x, TPizza *pizza, int t){
  int i = x->nchaves-1;
  if(x->folha){
    while((i>=0) && (pizza->cod < x->chave[i]->cod)){
      x->chave[i+1]->cod = x->chave[i]->cod;
      i--;
    }
    x->chave[i+1] = pizza;
    x->nchaves++;
    return x;
  }
  while((i>=0) && (pizza->cod < x->chave[i]->cod)) i--;
  i++;
  if(x->filho[i]->nchaves == ((2*t)-1)){
    x = divisao(x, (i+1), x->filho[i], t);
    if(pizza->cod > x->chave[i]->cod) i++;
  }
  x->filho[i] = insere_nao_completo(x->filho[i], pizza, t);
  return x;
}

TABM *insere(TABM *T, TPizza *pizza, int t){
  if(busca_prim(T, pizza->cod)) return T;
  if(!T){
    T=cria(t);
    T->chave[0] = pizza;
    T->nchaves=1;
    return T;
  }
  if(T->nchaves == (2*t)-1){
    TABM *S = cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;
    S = divisao(S,1,T,t);
    S = insere_nao_completo(S, pizza, t);
    return S;
  }
  T = insere_nao_completo(T, pizza, t);
  return T;
}


int main(void){
  TABM * arvore = inicializa();
  int num = 0;
  FILE *f = fopen("dados_iniciais.dat", "rb");
  while(num != -1){
    int t = 2;
    printf("Digite um numero para adicionar. 0 para imprimir e -1 para sair\n");
    scanf("%d", &num);
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
    else{

      TPizza *p = le_pizza(f);
      
      arvore = insere(arvore, p, t);

    }
    printf("\n\n");
  }
}

