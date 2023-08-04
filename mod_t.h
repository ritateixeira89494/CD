/**
 * @file mod_t.h
 * Definição da camada modulo T.
 */
#ifndef MOD_T_H_INCLUDED
#define MOD_T_H_INCLUDED

#include <stdio.h>
#define BLOCK_SIZE 256

/**
 * \brief Struct onde será armazenado o código SF.
 */
typedef struct {
    char codigo[10];
} CodigoSF;

/**
 * \brief Struct auxiliar para armazenar as frequencias correspondentes a cada simbolo.
 */
typedef struct {
    int ind;
    int freq;
} Caracter;

/**
 * \brief Função que indica toda a informação necessária sobre o módulo.
 * @param blocos Quantidade de blocos existentes no ficheiro de input.
 * @param tamanho_blocos Tamanhos dos blocos do ficheiro de input.
 * @param T_exec Tempo de execução
 * @param nome_gerado Nome do ficheiro de output.
 * \author João Silva/Carlos Santos
 */
void modinfo(int blocos, int *tamanho_blocos, double T_exec, char* nome_gerado);

/**
 * \brief Função que cria um novo nodo para uma árvore binária.
 * @param simb Estrutura onde está contida a informação sobre as frequências de cada símbolo.
 * \author Carlos Santos
 */
struct nodo* novoNodo(Caracter simb[BLOCK_SIZE]);

/**
 * \brief Função que calcula quantos simbolos foram usados.
 * @param lista Um array com elementos de estrutura Caracter.
 * \author Carlos Santos
 */
int checksize(Caracter lista[]);

/**
 * \brief Função que soma todos os elementos de uma lista.
 * @param lista Um array com elementos de estrutura Caracter.
 * @param n Número de elementos na lista.
 * \author Carlos Santos
 */
int soma_lista(Caracter lista[], int n);

/**
 * \brief Encontra a posição para a melhor divisão.
 * @param simb Um array com elementos de estrutura Caracter.
 * @param size Numero de elementos da lista.
 * \author Recebido do projeto
 */
int calcular_melhor_divisao(Caracter simb[],int size);

/**
 * \brief Função que distribui elementos da lista inicial em duas diferentes listas com objetivo de atingir frequencia minimamente apróximada.
 * @param lista Um array com elementos de estrutura Caracter.
 * @param n Número de elementos na lista.
 * @param simb1 Primeira divisão da lista.
 * @param simb2 Segunda divisão da lista.
 * \author Carlos Santos
 */
void distrib (Caracter simb[], int n,Caracter simb1[], Caracter simb2[]);

/**
 * \brief Função que recebe a função e divide um duas listas.
 * @param simb A lista que pretendemos dividir em dois.
 * @param parte Qual das duas partes pretendemos receber.
 * \author Carlos Santos
 */
Caracter *divs(Caracter simb[], int parte);

/**
 * \brief Função que percorre uma árvore binária com a lista de forma a encontrar o valor binário de cada Caracter.
 * @param simb Estrutura onde está contida a informação sobre as frequências de cada símbolo.
 * @param cod Estrutura onde está contida o valor binário de cada símbolo.
 * @param bloco Qual o bloco do ficheiro que estamos a trabalhar.
 * @param raiz Nodo atual da árvore binária.
 * @param bin String do valor binário do símbolo encontrado eventualmente em simb.
 * \author Carlos Santos
 */
void distbinary(CodigoSF cod[][BLOCK_SIZE], int bloco, struct nodo *raiz, char* bin);

/**
 * \brief Retorna '1' no caso de o ficheiro ter compressão RLE, e '0' no caso de nao ter.
 * @param nfile Nome do ficheiro original.
 * \author João Silva
 */
int check_rle (char *nfile);

/**
 * \brief Gera novo nome do ficheiro onde vão ser guardados os codigos SF.
 * @param nfile Nome do ficheiro original.
 * @param novo Nome do novo ficheiro a ser gerado pelo módulo.
 * \author João Silva
 */
void gera_novo_nome_t (char *nfile, char novo[]);

/**
 * \brief A partir do ficheiro, determina quantos blocos existem.
 * @param nome Nome original do ficheiro.
 * \author João Silva
 */
int obter_blocos (char *nome);

/**
 * \brief Obtem o tamanho do primeiro bloco
 * @param nome Nome do ficheiro original.
 * \author João Silva
 */
int obter_tamanho_blocos (char *nome);

/**
 * \brief Guarda o tamanho de cada bloco no seu respetivo (índice - 1) -> bloco 1 = indice 0.
 * @param nfile Nome do ficheiro original.
 * @param tamanhos Array onde vão ser guardados os ficheiros.
 * \author João Silva
 */
void guarda_tamanhos (char *nfile, int *tamanhos);

/**
 * \brief Retira as ocorrencias de arrobas no ficheiro inicial ("@R@2@991@74;0;;;;;;" -> "74;0;;;;;;;").
 * @param ficheiro Ficheiro de tipo .freq lido pelo programa.
 * \author João Silva
 */
char *retirar_arrobas_init(char *ficheiro);

/**
 * \brief Retira as ocorrencias das arrobas no ficheiro inicial (Quando ja estamos em algum bloco a seguir ao primeiro,há menos ocorrencias de "@").
 * @param ficheiro Ficheiro de tipo .freq lido pelo programa.
 * \author João Silva
 */
char *retirar_arrobas_final (char *ficheiro);

/**
 * \brief Obtem as frequencias de cada símbolo e em cada bloco.
 * @param nfile Nome do ficheiro original.
 * @param simb Estrutura onde será coloca a informação sobre as frequências.
 * @param blocos Número de blocos existentes no ficheiro.
 * \author João Silva
 */
void lerfreq(char *nfile, Caracter simb[][256], int blocos);

/**
 * \brief Ordena por ordem decrescente as frequencias de cada símbolo.
 * @param simb Estrutura onde está contida a informação sobre as frequências de cada símbolo.
 * @param n_blocos Número de blocos existentes no ficheiro.
 * \author Carlos Santos
 */
void ordlist(Caracter simb[][BLOCK_SIZE], int n_blocos);

/**
 * \brief Criação do código Shannon-Fannon para cada símbolo.
 * @param simb Estrutura onde está contida a informação sobre as frequências de cada símbolo.
 * @param cod Estrutura onde serão armazenados os códigos SF para cada símbolo.
 * @param blocos Número de blocos existentes no ficheiro.
 * \author Carlos Santos
 */
void execsf(Caracter simb[][BLOCK_SIZE], CodigoSF cod[][BLOCK_SIZE], int blocos);

/**
 * \brief Guarda os códigos SF correspondentes a cada símbolo num ficheiro.
 * @param cod Estrutura onde estão contidos os códigos SF para cada símbolo.
 * @param nfile Nome do ficheiro original.
 * @param novo Nome do ficheiro a ser criado pela função.
 * \author João Silva
 */
void savedata(CodigoSF cod [][BLOCK_SIZE], char *nfile, char *novo);

/**
 * \brief Função correspondente ao módulo T, chama as funções necessárias para a sua execução na ordem correta.
 * @param nfile Nome do ficheiro original.
 * \author João Silva/Carlos Santos
 */
int modulot(char *nfile);


#endif // MOD_T_H_INCLUDED
