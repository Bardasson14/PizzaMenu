#include "pizza.h"

#define t 2

typedef struct arvbm{
  int nchaves, folha, chave[(2*t)-1];
  long ind[(2*t)-1], filho[2*t], prox;
}TABM;


TABM *le_pagina(FILE *indices, long indice); //Lê uma página na posição indice

void salva_pagina(FILE *indices, long indice, TABM *pagina);  //Escreve página na posição indice

TABM *cria();    //Cria árvore em MP

//void libera(TABM *a); //libera árvore na MP

void imprime(long indAtual, int andar, FILE *indices); //Imprime árvore do arquivo

/*  
  A função divisão divide uma página cheia e leva o valor intermediario para o pai
  
  Parametros:
    indices -> arquivo de indices
    pai -> posição do pai no arquivo
    i -> indica a posição que no pai que entrará o valor intermediário
    filho -> posição do filho(nó cheio) no arquivo
*/
void divisao(FILE *indices, long pai, int i, long filho);  //Divide no arquivo

/*
  A função insere_nao_completo vai percorrer recursivamente a arvore procurando a posição corre de inserir a Pizza.
  Caso a página da iteração atual estiver cheia, vai chamar a divisão.

  Parametros:
    indices -> arquivo de indices
    indAtual -> posição da página a ser lida nesta iteração
    mat -> chave primária da Pizza
    indPizza -> posição da Pizza no arquivo de dados

 */

void insere_nao_completo(FILE *indices, long indAtual, int mat, long indPizza); //Insere em nó não completo

/*
  A função insere serve para verificar se a raiz está vazia ou cheia, e então chama a insere_nao_completo para fazer a inserção
  Caso a raiz esteja vazia, vai criar uma página e inserir o valor lá.
  Caso a raiz esteja cheia, vai criar um nó vazio "acima" da raiz, e chamar a divisão.

  Parametros:
    indRaiz -> posição da raiz no arquivo de indices;
    cod -> chave primária da pizza
    catalogo -> arquivo de dados
    indices -> arquivo de indices
    indPizza -> posição da Pizza no arquivo de dados;

*/

long insere(long indRaiz, int cod, FILE *catalogo, FILE *indices, long indPizza);

void remove_pizza(); //Remove(a melhor opção é mudar os indices para nunca parar aqui)

void remove_categoria(); //Remove todas as pizzas de uma mesma categoria

TPizza *busca_pizza(FILE *indices, FILE* catalogo, int cod, long indAtual);  //Busca a pizza pela chave primária

void busca_categoria(FILE *indices, FILE* catalogo, char * categoria); //Busca todas as pizzas de uma categoria

void altera_Pizza(FILE *indices, FILE* catalogo, int cod);  //Pede inputs para alterar as informações de uma pizza de chave primária cod;
