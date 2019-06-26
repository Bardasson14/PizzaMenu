#include "arvbm.h"
#define t 2

void salva_pagina(FILE* indices, long indice, TABM *pagina){

  fseek(indices, indice, SEEK_SET);
  printf("SALVA:\n nchaves: %d\n", pagina->nchaves);
  fwrite(pagina, sizeof(TABM), 1, indices);
 
  fseek(indices, 0L, SEEK_SET);  //Retornando o cursor para o início do arquivo

}

TABM *le_pagina(FILE *indices, long indice){

  if(feof(indices)) return NULL;

  TABM *pagina = (TABM*) malloc(sizeof(TABM));
  fseek(indices, indice, SEEK_SET);


  size_t ok = fread(pagina, sizeof(TABM), 1, indices);
  fseek(indices, 0L, SEEK_SET); //Retornando o cursor para o início do arquivo
  
  if(ok != 1){
    printf("NÃO LEU CORRETAMENTE\n");
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
    for(i=0; i <= a->nchaves-1; i++){
      
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


void insere_nao_completo(FILE *indices, long indAtual, int mat, long indPizza){

  printf("INSERE NÃO COMPLETO:\n");

  TABM *pag = le_pagina(indices, indAtual); //Lê a página no indice atual
  int i = pag->nchaves - 1;

  //Se a página atual for folha, já estamos na página certa para inserir
  //Então é só achar a posição correta e inserir a chave primária e a Pizza 
  if (pag->folha){
    printf("nchaves antes: %d\n", pag->nchaves);
    while ((i >= 0)&&(mat < pag->chave[i])){
      pag->chave[i+1] = pag->chave[i];
      pag->ind[i+1] = pag->ind[i];
      i--;
    }
    pag->chave[i+1] = mat;
    pag->ind[i+1] = indPizza;
    pag->nchaves++;
    pag->chave[0] = 123;
    printf("nchaves: %d\n", pag->nchaves);
    printf("endereço: %ld\n", indAtual);
    salva_pagina(indices, indAtual, pag); //AO SOBREESCREVER AQUI, NÃO ESTÁ MUDANDO O nchaves
    printf("é folha\n");
    return;
  }
  
  //Caso não seja folha, procura qual filho tem que chamar recursivamente
  while ((i>=0)&&(mat < pag->chave[i])) i--;

  i++;
  TABM *filhoI = le_pagina(indices, pag->filho[i]);
  //Se o filho estiver cheio, divide
  if (filhoI->nchaves == (2*t-1)){
    printf("nó cheio. irá se dividir\n");
    divisao(indices, indAtual, (i+1), pag->filho[i]); //Divide o filho em 2 páginas e sobe a informação intermediária
    if (mat > pag->chave[i])  i++;
  }

  printf("Nó não cheio\n");
  //Chama recursivamente para o filho[i]
  insere_nao_completo(indices, pag->filho[i], mat, indPizza);
  
}


long insere(long indRaiz, int cod, FILE *catalogo, FILE *indices, long indPizza){
  
  printf("INSERE:\n");
  //if(busca_pizza(indices, catalogo, cod)) return;
  TABM *pag = le_pagina(indices, indRaiz);

  //Se a raiz não existir, nada foi escrito no arquivo ainda, então cria uma primeira página
  if(!pag){
    printf("nó vazio\n");
    pag = cria(t);
    pag->chave[0] = cod;
    pag->nchaves = 1;
    //fseek(indices, 0L, SEEK_END); Tirei essa linha, pois quero que sobreescreva o que já estiver no arquivo
    salva_pagina(indices, indRaiz, pag);

    printf("nchaves: %d\n", pag->nchaves);
    return indRaiz;
  }
  printf("nchaves: %d\n", pag->nchaves);

  //Se a raiz estiver cheia, divide
  if(pag->nchaves == (2*t)-1){  
    //Caso a raiz esteja cheia, cria um novo nó e divide a raiz.
    printf("nó cheioooooooooooooooooooooo\n");
    TABM *S = cria(t);
    S->nchaves = 0;
    S->folha = 0;
    S->filho[0] = indRaiz;

    
    fseek(indices, 0L, SEEK_END);   //Botei o cursor no final do arquivo
    long indS = ftell(indices);     //Salvei o índice que S será escrito
    salva_pagina(indices, indS, S); //Escrevi S no arquivo para fazer a divisão

    divisao(indices, indS, 1, indRaiz);  //Divide a raiz(indRaiz) em S.
    insere_nao_completo(indices, indS, cod, indPizza);  //Vai inserir recursivamente a partir de S


    return indS;  //S será a nova raiz
  }
  printf("Raiz não cheia\n");
  insere_nao_completo(indices, indRaiz, cod, indPizza);
  return  indRaiz;  //Não mudou o "ponteiro" para a raiz
}


int main(void){
  TABM * arvore = inicializa();
  int num = 0;
  FILE *dados = fopen("dados_iniciais.dat", "rb+");
  FILE *indices = fopen("indices.bin", "a+b");
  long indRaiz = 0L;
  TABM *T;
  while(num != -1){
    printf("Digite 0 para inserir a próxima pizza, e 1 para imprimir\n");
    scanf("%d", &num);


    if(num == 0){

      long indPizza = ftell(dados);
        TPizza *p = le_pizza(dados);
        printf("NOME DA PIZZA: %s\n", p->nome);
        printf("PREÇO DA PIZZA: %f\n", p->preco);
        printf("DESCRIÇÃO DA PIZZA: %s\n", p->descricao);
        printf("CÓDIGO DA PIZZA: %d\n", p->cod);
        fseek(indices, indRaiz, SEEK_SET);
        printf("Raiz atual: %ld\n", indRaiz);
        indRaiz = insere(indRaiz, p->cod, dados, indices, indPizza);


    }

    else if (num == 1){

      TABM *no = le_pagina(indices, indRaiz);
      imprime(no, 0, indices);

    }

    printf("\n\n");
  }
}
