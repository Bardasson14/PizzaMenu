#include "arvbm.h"

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

TABM *le_pagina(FILE *indices, long indice, int t){

  printf("entrou");

  if(feof(indices)) return NULL;

 

  TABM *pagina = (TABM*) malloc(sizeof(TPizza));
  fseek(indices, indice, SEEK_SET);


// 	if (0 >= fread(&pagina->nchaves, sizeof(int), 1, indices)) {
// 		printf("aaaaaaaaaaaaadfsdfds\n");
// 		return NULL;
// 	}


// ;
//   fread(&pagina->folha, sizeof(int), 1, indices);


//   fread(pagina->chave, sizeof(int*), sizeof(pagina->chave), indices);

//   fread(pagina->ind, sizeof(long*), sizeof(pagina->ind), indices);

//   fread(pagina->filho, sizeof(long*), sizeof(pagina->filho), indices);

//   fread(&pagina->prox, sizeof(long), 1, indices);

  printf("saiu");

  size_t ok = fread(pagina, sizeof(TABM), 1, indices);
  fseek(indices, 0L, SEEK_SET); //Retornando o cursor para o início do arquivo
  if(ok != 1){
    printf("tem que entrar aqui\n");
    return NULL;
  }
  printf("le_pagdfp,sd\n");
  return pagina;

}


TABM *cria(int t){
  TABM* novo = (TABM*)malloc(sizeof(TABM));
  novo->nchaves= 0;
  novo->chave = (int*)malloc(sizeof(int)*((t*2)-1));
  novo->folha = 1;
  novo->ind = (long*)malloc(sizeof(long)*((t*2)-1));
  novo->filho = (long*)malloc(sizeof(long)*t*2);
  novo->prox = -1;
  int i;

  for(i=0; i<(2*t); i++) 
    novo->filho[i] = -1;

  for(i=0; i<(2*t-1); i++){
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

void imprime(TABM *a, int andar, FILE *indices, int t){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      
      imprime(le_pagina(indices, a->filho[i], t), andar+1, indices, t);
      for(j=0; j<=andar; j++) printf("   ");
      printf("%d\n", a->chave[i]);
    }
    imprime(le_pagina(indices, a->filho[i], t),andar+1, indices, t);
  }
}

void divisao(FILE *indices, long pai, int i, long filho, int t){

  TABM *z = cria(t);
  TABM *x = le_pagina(indices, pai, t);
  TABM *y = le_pagina(indices, filho, t);

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


void insere_nao_completo(FILE *indices, long T, int mat, int t){

  printf("INSERE NÃO COMPLETO: ");

  TABM *x = le_pagina(indices, T, t); //Lê a página no indice T

  //Está lendo x corretamente, mas o x->nchaves ta dando segfault
  int i = x->nchaves - 1;
  if (x->folha){
    printf("nó folha\n");
    printf("%d", x->chave[0]);
    printf("aaaaaaaaaaaaaaaaaaa\n");
    while ((i >= 0)&&(mat < x->chave[i])){
      printf("martir\n");
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
  TABM *filhoI = le_pagina(indices, endfilho, t);
  if (filhoI->nchaves == (2*t-1)){
    printf("nó cheio. irá se dividir\n");
    divisao(indices, T, i, endfilho, t);
    if (mat > x->chave[i])
      i++;
  }
  insere_nao_completo(indices, endfilho, filhoI->chave[i], t);
  
}


void insere(long T, int cod, int t, FILE *catalogo, FILE *indices){
  
  //if(busca_pizza(indices, catalogo, cod)) return;
  TABM *x = le_pagina(indices, 0L, t);

  //passou aqui

  //printf("b\n");
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
    divisao(indices, ftell(indices),1 , T, t);
    fseek(indices, 0L, SEEK_END);
    insere_nao_completo(indices, ftell(indices), cod, t);
    return;
  }
  printf("aqui\n");
  insere_nao_completo(indices, T, cod, t);
  return;
}


int main(void){
  TABM * arvore = inicializa();
  int num = 0;
  FILE *dados = fopen("dados_iniciais.dat", "rb+");
  FILE *indices = fopen("indices.bin", "a+b");
  int t = 2;
  TABM *T = cria(t);
  while(num != -1){
    printf("Digite um numero para adicionar. 0 para imprimir e -1 para sair\n");
    scanf("%d", &num);
    printf("b");
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
      TABM *x = le_pagina(indices, 0L,t);
      imprime(x, 0, indices, t);
    }
    else if (num==1){

      int n = 1;
      if(n == 0){
        TPizza *p = le_pizza(dados);
        printf("NOME DA PIZZA: %s\n", p->nome);
        printf("PREÇO DA PIZZA: %f\n", p->preco);
        printf("DESCRIÇÃO DA PIZZA: %s\n", p->descricao);
        printf("CÓDIGO DA PIZZA: %d\n", p->cod);
        fseek(indices, 0L, SEEK_SET);
        insere(ftell(dados) - sizeof(TPizza), p->cod, t, dados, indices);
      }
      
      if(n == 1){

        TABM * no = le_pagina(indices, 0L, t);

        
        printf("%d\n", no->nchaves);
        printf("fdsmdj\n");
        printf("%d\n", no->folha);
        printf("%d", no->chave[0]);


      }

    }
    printf("\n\n");
  }
}
