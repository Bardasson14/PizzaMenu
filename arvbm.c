#include "arvbm.h"
#define t 2

void salva_pagina(FILE* indices, long indice, TABM *pagina){

  fseek(indices, indice, SEEK_SET);
  fwrite(pagina, sizeof(TABM), 1, indices);

  // fwrite(&pagina->nchaves, sizeof(int), 1, indices);
  // printf("foi1");
  // fwrite(&pagina->folha, sizeof(int), 1, indices);
  // printf("foi2");
  // fwrite(pagina->chave, sizeof(int*), sizeof(pagina->chave), indices);
  // printf("foi3");
  // fwrite(pagina->ind, sizeof(long*), sizeof(pagina->ind), indices);
  // printf("foi4");
  // fwrite(pagina->filho, sizeof(long*), sizeof(pagina->filho), indices);
  // printf("foi5");
  // fwrite(&pagina->prox, sizeof(long), 1, indices);
  // printf("foi6");
 
  fseek(indices, 0L, SEEK_SET);  //Retornando o cursor para o início do arquivo

}

TABM *le_pagina(FILE *indices, long indice){

  if(feof(indices)) return NULL;

  TABM *pagina = cria();
  fseek(indices, indice, SEEK_SET);


  size_t ok = fread(pagina, sizeof(TABM), 1, indices);
  fseek(indices, 0L, SEEK_SET); //Retornando o cursor para o início do arquivo

  if(ok != 1){
    return NULL;
  }

  return pagina;

}


TABM *cria(){
  TABM* novo = (TABM*)malloc(sizeof(TABM));
  novo->nchaves= 0;
  novo->folha = 1;
  novo->prox = -1;
  int i;

  for(i=0; i<(2*t); i++) 
    novo->filho[i] = -1;

  for(i=0; i<((2*t)-1); i++){
    novo->chave[i] = -1;
    novo->ind[i] = -1;
  }
  
  return novo;
}


TABM *inicializa(void){
  return NULL;
}

/* void libera(TABM *a){
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
*/

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
  TPizza *pizza = busca_pizza(indices, catalogo, cod); 
  fseek(indices, 0L, SEEK_SET); //Retornando cursor para o inicio do arquivo
  return pizza;

}

void imprime(TABM *a, int andar, FILE *indices){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      
      imprime(le_pagina(indices, a->filho[i]), andar+1, indices);
      for(j=0; j<=andar; j++) printf("   ");
      printf("%d\n", a->chave[i]);
    }
    imprime(le_pagina(indices, a->filho[i]),andar+1, indices);
  }
}

void divisao(FILE *indices, long pai, int i, long filho){

  printf("veio dividir\n");
  TABM *z = cria(t);
  TABM *x = le_pagina(indices, pai);
  TABM *y = le_pagina(indices, filho);


  z->folha = y->folha;
  if(!y->folha){
    int j;
    z->nchaves = t-1;
    for(j = 0; j < t; j++)  
      z->chave[j] = y->chave[j+t];
    for(j = 0; j < t; j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = -1;
    }
  }
  else{

    int j;
    z->nchaves = t; //z possuirá uma chave a mais que y se for folha
    for(j = 0; j < t; j++)  
      z->chave[j] = y->chave[j+t-1];  //Caso em que y é folha, temos que passar a info do nó para a direita
    y->prox = -1;

  }

  y->nchaves = t-1;
  int k;
  for(k = x->nchaves; k >= i; k--)  
    x->filho[k+1] = x->filho[k];
  /* Z está no final do arquivo, logo, é necessário dar o endereço do final do arquivo de
  índices */
  fseek(indices, 0L, SEEK_END);
  x->filho[i] = ftell(indices);
  for(k = x->nchaves; k >= i; k--)  
    x->chave[k] = x->chave[k+1];
  x->chave[i-1] = y->chave[t-1];
  x->nchaves++;

  //Nesse ponto ja fiz a divisão, agora basta sobreescrever no arquivo;

  salva_pagina(indices, pai, x);  //Sobreescreve o pai
  salva_pagina(indices, filho, y);  //Sobreescreve o filho
  salva_pagina(indices, 0L, z); //Escreve a nova página no final;

  //libera(x);
  //libera(y);
  //libera(z);

}


void insere_nao_completo(FILE *indices, long T, int mat){

  printf("INSERE NÃO COMPLETO:\n");

  printf("%ld", T);

  TABM *x = le_pagina(indices, T); //Lê a página no indice T

  int i = x->nchaves - 1;
  printf("teste\n");
  if (x->folha){
    printf("nó folha\n");
    while ((i >= 0)&&(mat < x->chave[i])){
      x->chave[i+1] = x->chave[i];
      x->ind[i+1] = x->ind[i];
      i--;
    }
    x->chave[i+1] = mat;
    x->nchaves++;
    //return x;
    salva_pagina(indices, T, x);
    return;
  }
  while ((i>=0)&&(mat<x->chave[i]))
    i--;
  i++;

  long endfilho = x->filho[i];
  TABM *filhoI = le_pagina(indices, endfilho);
  if (filhoI->nchaves == (2*t-1)){
    printf("nó cheio. irá se dividir\n");
    divisao(indices, T, i, endfilho);
    if (mat > x->chave[i])
      i++;
  }
  insere_nao_completo(indices, endfilho, filhoI->chave[i]);
  
}


void insere(long T, int cod, FILE *catalogo, FILE *indices){
  
  //if(busca_pizza(indices, catalogo, cod)) return;
  TABM *x = le_pagina(indices, 0L);

  //passou aqui
  if(!x){
    printf("nó vazio\n");
    x=cria(t);
    x->chave[0] = cod;
    x->nchaves = 1;
    fseek(indices, 0L, SEEK_END);
    salva_pagina(indices, ftell(indices), x);
    return;
  }
  else if(x->nchaves == (2*t)-1){
    printf("nó cheio");
    TABM *S = cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;
    fseek(indices, 0L, SEEK_END);
    divisao(indices, ftell(indices),1 , T);
    fseek(indices, 0L, SEEK_END);
    insere_nao_completo(indices, ftell(indices), cod);
    return;
  }
  insere_nao_completo(indices, T, cod);
  return;
}


int main(void){
  TABM * arvore = inicializa();
  int num = 0;
  FILE *dados = fopen("dados_iniciais.dat", "rb+");
  FILE *indices = fopen("indices.bin", "a+b");
  TABM *T = cria();
  while(num != -1){
    printf("Digite um numero para adicionar. 0 para imprimir e -1 para sair\n");
    scanf("%d", &num);
    /* if(num == -1){
      printf("\n");
      imprime(arvore,0);
      //libera(arvore);
      return 0;
    }
    */
    /*
    else if(num == -9){
      scanf("%d", &from);
      arvore = retira(arvore, from, t);
      Imprime(arvore,0);
    }
    */
    if(!num){
      printf("\n");
      TABM *x = le_pagina(indices, 0L);
      imprime(x, 0, indices);
    }
    else if (num==1){

      int n;
      scanf("%d", &n);
      if(n == 0){
        TPizza *p = le_pizza(dados);
        printf("NOME DA PIZZA: %s\n", p->nome);
        printf("PREÇO DA PIZZA: %f\n", p->preco);
        printf("DESCRIÇÃO DA PIZZA: %s\n", p->descricao);
        printf("CÓDIGO DA PIZZA: %d\n", p->cod);
        fseek(indices, 0L, SEEK_SET);
        insere(/* o que botar aqui? */, p->cod, dados, indices);
      }
      
      if(n == 1){

        TABM * no = le_pagina(indices, 0L);

        
          printf("%d %d\n", no->nchaves, no->folha);
          printf("%d\n", no->chave[0]);
          printf("%ld\n", no->prox);



      }

    }
    printf("\n\n");
  }
}
