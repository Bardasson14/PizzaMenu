#include "arvbm.h"

void salva_pagina(FILE* indices, long indice, TABM *pagina){

  fseek(indices, indice, SEEK_SET);
  fwrite(pagina, sizeof(TABM), 1, indices);

}

TABM *le_pagina(FILE *indices, long indice){

  if(feof(indices)) return NULL;

  TABM *pagina = (TABM*) malloc(sizeof(TABM));
  fseek(indices, indice, SEEK_SET);


  size_t ok = fread(pagina, sizeof(TABM), 1, indices);
  
  if(ok != 1){
    printf("Não tem árvore no arquivo\n");
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

TPizza *busca_pizza(FILE *indices, FILE* dados, int cod, long indAtual){

  //printf("entrou na busca;\n");
  if(!indices || !dados) return NULL;

  //Pegando a pagina no arquivo
  TABM *pagina = le_pagina(indices, indAtual);
  if(!pagina) return NULL;

  int i = 0;
  while((i < pagina->nchaves) && (cod > pagina->chave[i])) i++;
  if((i < pagina->nchaves) && (pagina->folha) && (cod == pagina->chave[i])){

    long dadosAtual = ftell(dados);
    //printf("chave[i] = %d", pagina->chave[i]);
    
    fseek(dados, pagina->ind[i], SEEK_SET);  //Bota o cursor na posição da pizza no arquivo de dados
    TPizza *pizza = le_pizza(dados);

    fseek(dados, dadosAtual, SEEK_SET);
    //printf("cod pizza -> %d", pizza->cod);
    return pizza;

  }

  if(pagina->folha) return NULL;
  if(pagina->chave[i] == cod) i++;
  
  TPizza *pizza = NULL;
  if(pagina->filho[i] != -1)  
    pizza = busca_pizza(indices, dados, cod, pagina->filho[i]); 
  
  //printf("saiu da busca\n");
  return pizza;

}

long busca_end_pizza(FILE *indices, FILE* dados, int cod, long indAtual){

  //printf("entrou na busca;\n");
  if(!indices || !dados) return -1;

  //Pegando a pagina no arquivo
  TABM *pagina = le_pagina(indices, indAtual);
  if(!pagina) return -1;

  int i = 0;
  while((i < pagina->nchaves) && (cod > pagina->chave[i])) i++;
  if((i < pagina->nchaves) && (pagina->folha) && (cod == pagina->chave[i])){

    return pagina->ind[i];

  }

  if(pagina->folha) return -1;
  if(pagina->chave[i] == cod) i++;
  
  // TPizza *pizza = NULL;
  if(pagina->filho[i] != -1)  
    return busca_end_pizza(indices, dados, cod, pagina->filho[i]); 
  
  //printf("saiu da busca\n");
  return pagina->ind[i];  //Se chegou aqui, pagina->ind[i] == -1

}

 
void imprime(long indAtual, int andar, FILE *indices){

  TABM *a = le_pagina(indices, indAtual);
  if(a){
    int i,j;
    for(i = 0; i <= a->nchaves-1; i++){
      if(a->filho[i] != -1) imprime(a->filho[i], andar+1, indices);
      for(j = 0; j <= andar; j++) printf("   ");
      printf("%d\n", a->chave[i]);
    }
    if(a->filho[i] != -1) imprime(a->filho[i],andar+1, indices);
  }
}

void busca_categoria(FILE *indices, FILE* catalogo, char * categoria, long indAtual){
  
  TABM *a = le_pagina(indices, indAtual);

  if (!a) return;

  if (!a->folha){

    return busca_categoria(indices, catalogo, categoria, a->filho[0]);
  }

  for (int i = 0; i < a->nchaves;i++){
    fseek(catalogo, a->ind[i], SEEK_SET);
    TPizza *p = le_pizza(catalogo);
    if (!(strcmp(categoria, p->descricao)))
      imprime_pizza(p);
  }

  if (a->prox != -1) 
    busca_categoria(indices, catalogo, categoria, a->prox);

}

void imprime_linear(FILE *indices, FILE *catalogo, long indAtual){

  TABM *pag = le_pagina(indices, indAtual);

  for (int i = 0; i < pag->nchaves;i++){
  fseek(catalogo, pag->ind[i], SEEK_SET);
  TPizza *p = le_pizza(catalogo);
  imprime_pizza(p);

  }

  imprime_linear(indices, catalogo, pag->prox);

}

void divisao(FILE *indices, long pai, int i, long filho){

  TABM *z = cria();
  TABM *x = le_pagina(indices, pai);
  TABM *y = le_pagina(indices, filho);

  long indZ;

  z->folha = y->folha;
  int j;
  if(!y->folha){

    z->nchaves = t-1;
    for(j = 0; j < t-1; j++){
      z->chave[j] = y->chave[j+t];
      z->ind[j] = y->ind[j+t];
    }

    for(j = 0; j < t; j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = -1;
    }

  }
  else{
    
    z->nchaves = t; //z possuirá uma chave a mais que y se for folha
    for(j = 0; j < t; j++){ 
     
      z->chave[j] = y->chave[j+t-1];  //Caso em que y é folha, temos que passar a info do nó para a direita
      z->ind[j] = y->ind[j+t-1];   

    }


    fseek(indices, 0L, SEEK_END);
    indZ = ftell(indices);
    z->prox  = y->prox;
    y->prox = indZ;
    

  }

  y->nchaves = t-1;
  for(j = x->nchaves; j >= i; j--)  
    x->filho[j+1] = x->filho[j];
  /* Z está no final do arquivo, logo, é necessário dar o endereço do final do arquivo de
  índices */
  fseek(indices, 0L, SEEK_END);
  indZ = ftell(indices);
  x->filho[i] = indZ;
  for(j = x->nchaves; j >= i; j--){

    x->chave[j] = x->chave[j-1];
    x->ind[j] = x->ind[j-1];
  
  }
  x->chave[i-1] = y->chave[t-1];
  x->ind[i-1] = y->ind[t-1];
  x->nchaves++;
  
  //Nesse ponto ja fiz a divisão, agora basta sobreescrever no arquivo;

  salva_pagina(indices, pai, x);  //Sobreescreve o pai
  salva_pagina(indices, filho, y);  //Sobreescreve o filho
  salva_pagina(indices, indZ, z); //Escreve a nova página no final;

  //libera(x);
  //libera(y);
  //libera(z);

}


void insere_nao_completo(FILE *indices, long indAtual, int mat, long indPizza){

  printf("VEIO INSERIR:\n");

  TABM *pag = le_pagina(indices, indAtual); //Lê a página no indice atual

  int i = pag->nchaves - 1;

  //Se a página atual for folha, já estamos na página certa para inserir
  //Então é só achar a posição correta e inserir a chave primária e a Pizza 
  if (pag->folha){
    printf("É FOLHA\n");
    while ((i >= 0)&&(mat < pag->chave[i])){
      pag->chave[i+1] = pag->chave[i];
      pag->ind[i+1] = pag->ind[i];
      i--;
    }
    printf("SAIU DO WHILE\n");
    pag->chave[i+1] = mat;
    pag->ind[i+1] = indPizza;
    pag->nchaves++;
    salva_pagina(indices, indAtual, pag); //AO SOBREESCREVER AQUI, NÃO ESTÁ MUDANDO O nchaves
    return;
  }
  
  //Caso não seja folha, procura qual filho tem que chamar recursivamente
  while ((i>=0)&&(mat < pag->chave[i])) i--;
  i++;
  printf("entrou \n");
  TABM *filhoI = le_pagina(indices, pag->filho[i]);
  printf("saiu\n");
  //Se o filho estiver cheio, divide
  if (filhoI->nchaves == (2*t-1)){
    divisao(indices, indAtual, (i+1), pag->filho[i]); //Divide o filho em 2 páginas e sobe a informação intermediária
    pag = le_pagina(indices, indAtual);
    if (mat > pag->chave[i])  i++;
  }

  //Chama recursivamente para o filho[i]
  insere_nao_completo(indices, pag->filho[i], mat, indPizza);
  
}


long insere(long indRaiz, int cod, FILE *dados, FILE *indices, long indPizza){
  
  if(busca_pizza(indices, dados, cod, indRaiz)){

  printf("Já existe uma pizza de mesmo código no arquivo.\n");
  return indRaiz;
  
  }

  TABM *pag = le_pagina(indices, indRaiz);

  //Se a raiz não existir, nada foi escrito no arquivo ainda, então cria uma primeira página
  if(!pag){

    //printf("Cria página\n");
    pag = cria();
    pag->chave[0] = cod;
    pag->ind[0] = indPizza;
    pag->nchaves = 1;
    salva_pagina(indices, indRaiz, pag);

    return indRaiz;
  }

  //Se a raiz estiver cheia, divide
  if(pag->nchaves == (2*t)-1){  
    //Caso a raiz esteja cheia, cria um novo nó e divide a raiz.
    TABM *S = cria();
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
  
  insere_nao_completo(indices, indRaiz, cod, indPizza);
  return  indRaiz;  //Não mudou o "ponteiro" para a raiz
}

void altera_dados(int cod, FILE *indices, FILE *catalogo, long indRaiz){
  
  long endP = busca_end_pizza(indices, catalogo, cod, indRaiz);
  
  if (endP == -1){
    printf("A pizza não foi encontrada\n");
    return;
  }

  long indAtual = ftell(catalogo);  //Guarda o índice da atual do catálogo

  fseek(catalogo, endP, SEEK_SET);  //Bota o cursor na posição da pizza 
  TPizza *p = le_pizza(catalogo);   //Lê a pizza
  printf("\nPizza antes das alterações: \n");
  imprime_pizza(p);

  printf("INSIRA AS NOVAS INFORMAÇÕES: \n");
  char nome[50], descricao[20];
  float preco;

  printf("NOME DA PIZZA: ");
  scanf("%s", nome);

  printf("CATEGORIA DA PIZZA: ");
  scanf("%s", descricao);

  printf("PREÇO DA PIZZA: R$ ");
  scanf("%f", &preco);

  // long indAtual = ftell(catalogo);  //Guarda o índice da atual do catálogo

  // fseek(catalogo, endP, SEEK_SET);  //Bota o cursor na posição da pizza 
  // TPizza *p = le_pizza(catalogo);   //Lê a pizza
  strcpy(p->nome, nome);
  strcpy(p->descricao, descricao);
  p->preco = preco;

  printf("\nPizza depois das alterações: \n");
  imprime_pizza(p);

  fseek(catalogo, endP, SEEK_SET);  //Bota o cursor na posição da pizza
  salva_pizza(p, catalogo);         //Sobreescreve a pizza

  fseek(catalogo, indAtual, SEEK_SET);  //Volta para o índice do catálogo
  
}

int main(void){
  TABM * arvore = inicializa();
  int num = 0;
  FILE *dados = fopen("dados_iniciais.dat", "rb+");
  FILE *indices = fopen("indices.bin", "r+b");


  //Para manter a posição da raiz mesmo depois de fechar o arquivo, reservei o indice 0L para guardar esta informação
  //Desta maneira toda vez que iniciar o programa, lê o 0L para checar se já tem um indice de raiz guardado 
  long indRaiz;
  rewind(indices);  //Just in case
  size_t ok = fread(&indRaiz, sizeof(long), 1, indices);  //Lê a posição 0L em busca de um indice
  if(!ok)  indRaiz = 10L; //Se não tem nada 
  printf("indRaiz: %ld", indRaiz);

  TABM *T;
  while(num!=-1){

    printf("indRaiz: %ld\n", indRaiz);

    printf("Comandos:\n");
    printf(" 0 - insere próxima pizza\n");
    printf(" 1 - imprime árvore \n");
    printf(" 2 - busca pizza pelo código \n");
    printf(" 3 - altera pizza\n");
    printf(" 4 - busca todas as pizzas de uma categoria \n");
    printf("10 - insere todas as pizzas restantes do catálogo \n");
    printf("-1 - sair\n");
    printf("-9 - limpa árvore\n");
    
    scanf("%d", &num);


    if(num == 0){

      long indPizza = ftell(dados);
      TPizza *p = le_pizza(dados);

      if(p){
        imprime_pizza(p);

        indRaiz = insere(indRaiz, p->cod, dados, indices, indPizza);
      }
      else printf("!!!! ACABOU O ARQUIVO DE DADOS !!!!\n");
    }

    else if (num == 1){

      imprime(indRaiz, 0, indices);

    }

    else if (num == 2){

      int cod;
      printf("\nDigite um código para buscar a pizza: \n");
      scanf("%d", &cod);

      TPizza *pizza = busca_pizza(indices, dados, cod, indRaiz);
      
      if(!pizza) printf("Não existe uma pizza de código %d no arquivo.\n", cod);
      else  imprime_pizza(pizza);

    }

    else if (num == 3){
      int cod;
      printf("entre com o codigo \n");
      scanf("%d", &cod);
      altera_dados(cod, indices, dados, indRaiz);
    }

    else if (num == 4){

      printf("Digite uma categoria: \n");
      char categoria[20];
      scanf(" %20[^\n]", categoria);
      // setbuf(stdin, NULL);
      busca_categoria(indices, dados, categoria, indRaiz);

    }

    else if(num == 5){

      TABM *a = le_pagina(indices, indRaiz);
      printf("indfilho: %ld\n", a->filho[0]);

      TABM *b = le_pagina(indices, a->filho[0]);
      printf("indfilhodofilho: %ld\n", b->filho[0]);

    }

    else if(num == 10){

      int tudo_certo = 1;
      while(tudo_certo){

        long indPizza = ftell(dados);
        TPizza *p = le_pizza(dados);

        if(p){
          indRaiz = insere(indRaiz, p->cod, dados, indices, indPizza);
        }
        else{
          printf("!!!! ACABOU O ARQUVO DE DADOS !!!!\n");
          tudo_certo = 0;
        }      
      }
    }

    else if(num == -9){

      fclose(indices);
      indices = fopen("indices.bin", "w+b");
      fclose(indices);
      indices = fopen("indices.bin", "r+b");

    }

    // TPizza* h = le_pizza(dados);
    // imprime_pizza(h);

    // fseek(indices, 0L, SEEK_SET);
    // salva_pizza(h, indices);

    // fseek(indices, 0L, SEEK_SET);
    // TPizza *g = le_pizza(indices);
    // imprime_pizza(g);

    // h->cod = 1000;
    // fseek(indices, 0L, SEEK_SET);
    // salva_pizza(h, indices);

    // fseek(indices, 0L, SEEK_SET);
    // TPizza *f = le_pizza(indices);
    // printf("%d", f->cod);
    // imprime_pizza(f);



    printf("\n\n");
  }

  rewind(indices);
  fwrite(&indRaiz, sizeof(long), 1, indices);

  fclose(indices);
  fclose(dados);

}
