#include "mod_t.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024
#define BLOCK_SIZE 256

struct nodo {
    Caracter simb[BLOCK_SIZE];
    struct nodo *esq;
    struct nodo *dir;
};

void modinfo(int blocos, int *tamanho_blocos, double T_exec, char* nome_gerado){
    //João Silva/Carlos Santos
    printf("Carlos Santos, a85617, MIEI/CD, 1-jan-2021\n"
           "Joao Silva, a89293, MIEI/CD, 1-jan-2021\n"
           "Modulo: t (calculo dos codigos dos simbolos)\n"
           "Numero de blocos: %d\n"
           "Tamanho dos blocos analisados no ficheiro de simbolos: ", blocos);
           for (int i = 0; i < blocos; i++) {
               printf("%d", tamanho_blocos[i]);
               if (i < blocos-1)
                    printf("/");
           }
           printf(" bytes.\n");
           printf("Tempo de execucao do modulo (milissegundos): %.2fms\n"
                  "Ficheiro gerado: %s\n", T_exec*1000, nome_gerado);
}

int checksize(Caracter lista[]){
    //Carlos Santos
    int i=0;
    for(; lista[i].freq != 0; i++);
    return i;
}

struct nodo* novoNodo(Caracter simb[BLOCK_SIZE])
{
    //Carlos Santos
    struct nodo* nodo = (struct nodo*)calloc(1, sizeof(struct nodo));
    int size = checksize(simb);
    for (int i = 0; i < size; i++) {
        nodo->simb[i] = simb[i];
    }

    nodo->esq = NULL;
    nodo->dir = NULL;
    return(nodo);
}

int soma_lista(Caracter lista[], int n){
    //Carlos Santos
    // Soma dos elementos de uma lista
    int total = 0;
    for (int i=0; i<n; i++) total += lista[i].freq;
    return total;
}

/*
int calcular_melhor_divisao(Caracter simb[],int size){
    //Recebido do projeto
int div=0;
printf("%d\n",soma_lista(simb,size));
int total = soma_lista (simb,size);
int mindif = soma_lista (simb,size);
int dif = soma_lista (simb,size);
while (dif == mindif){
    int g1= soma_lista (simb,div);
    int g2=total-g1;
    dif=abs(g1-g2);
    if (dif<mindif)
    {
        div=div+1;
        mindif=dif;
    }
    else dif=mindif+1;
}
return div;
}
*/

int calcular_melhor_divisao(Caracter simb[],int size)
{
    //Recebido do projeto
int div=0;
int total = soma_lista(simb, size);
int mindif = soma_lista(simb, size);
int dif = soma_lista(simb, size);
int g1 = 0;

while (dif==mindif)
{ g1 = g1 + simb[div].freq;
dif=abs(2*g1-total);
if (dif<mindif)
        {
        div=div+1;
        mindif=dif;
        }
else dif=mindif+1;
}
return div;
}

void distrib (Caracter simb[], int n,Caracter simb1[], Caracter simb2[])
{
    //Carlos Santos
    int divpos = calcular_melhor_divisao(simb,n);
    for(int i = 0; i<divpos;i++) simb1[i] = simb[i];
    for(int i = divpos; i<n;i++) simb2[i-divpos] = simb[i];
}

Caracter *divs(Caracter simb[], int parte)
{
    //Carlos Santos
    //As duas partes
    Caracter *simb1 = (Caracter *) calloc(150, sizeof(Caracter));
    Caracter *simb2 = (Caracter *) calloc(150, sizeof(Caracter));

    //Numero de elementos do array
    int n = 0;
    while (simb[n].freq != 0) n++;

    //Distribuir os elementos de acordo com a frequencia
    distrib(simb, n,simb1,simb2);

    //Escolhe qual parte vai ser retornada
    if (parte == 0) return (Caracter *) simb1;
    else return (Caracter *) simb2;
}

void distbinary(CodigoSF cod[][BLOCK_SIZE], int bloco, struct nodo *raiz, char* bin){

    //Carlos Santos
    char bin1[50]; strcpy(bin1,bin);
    char bin2[50]; strcpy(bin2,bin);
    //Verifica se é o unico simbolo
    if (checksize(raiz->simb)> 1){
        //Divide em duas listas
        Caracter *metade1 = divs(raiz->simb,0);
        Caracter *metade2 = divs(raiz->simb,1);
        strcat(bin1,"0");
        strcat(bin2,"1");
        //Vai preenchendo a arvore até apenas haver um simbolo
        raiz->esq = novoNodo(metade1);
        distbinary(cod,bloco,raiz->esq,bin1); free(metade1);
        raiz->dir = novoNodo(metade2);
        distbinary(cod,bloco,raiz->dir,bin2); free(metade2);
    }
    else {
        //Atribui o valor binario para o seu index
        int index = raiz->simb[0].ind;
        strcpy(cod[bloco][index].codigo ,bin);
        free(raiz);
        return;
    }
}

//Retorna '1' no caso de o ficheiro ter compressão RLE, e '0' no caso de nao ter.
int check_rle (char *nfile) {
    //João Silva
    char teste[2];
    FILE *fp = fopen(nfile, "rb");
    //Le o ficheiro e verifica se tem 'R' logo após a 1ª '@'
    if (fp == NULL)
        printf("Erro a abrir o ficheiro.\n");
    else
        fread(teste, 2, 1, fp);
    fclose(fp);
    //No caso de ter a identificação 'R' (Representa compressão RLE), Retorna 1.
    if (teste[1] == 'R')
        return 1;
    else
        return 0;
}

//Gera novo nome do ficheiro onde vão ser guardados os codigos SF.
void gera_novo_nome_t (char *nfile, char novo[]) {
    //João Silva
    if (check_rle(nfile) == 1){
        strcpy(novo, nfile);
        strtok(novo, ".");
        strcat(novo, ".txt.rle.cod");
    }
    else {
        strcpy(novo, nfile);
        strtok(novo, ".");
        strcat(novo, ".txt.cod");
    }
}
//A partir do ficheiro, determina quantos blocos existem
int obter_blocos (char *nome) {
    //João Silva
    FILE *fp;
    char blocos[BLOCK_SIZE], *end;
    int i = 0;
    fp = fopen(nome, "rb");
    //Lê os primeiros 20 chars do documento (Não deverá ser necessário mais apenas para ler o numero de blocos).
    if (fp == NULL)
        printf("Erro a abrir o ficheiro.\n");
    else
        fread(blocos, BLOCK_SIZE, 1, fp);
    //Divide o conteúdo lido quando chega ao char "@".
    strtok(blocos, "@");
    //Volta a fazer essa divisão, ficando apenas o número que representa a quantidade de blocos.
    char *final = strtok(NULL, "@");
    fclose(fp);
    //Faz a conversão de char para int e dá return desse valor.
    return strtol(final, &end, 10);
}

//Obtem o tamanho do primeiro bloco
int obter_tamanho_blocos (char *nome) {
    //João Silva
     FILE *fp;
     char linha1[BUF_SIZE];
     char *tamanho, *end;
     //Abre o ficheiro
     fp = fopen(nome, "rb");
     //Coloca no array "linha1" os primeiros 50 chars (não deverá ser necessário mais do que 50 para analisar o tamanho dos blocos).
     fread(linha1, BUF_SIZE, 1, fp);
     //Faz a divisão no char "@"
     strtok(linha1, "@");
     //Faz a mesma divisão mais 2 vezes.
     for (int i = 0; i < 2; i++)
         tamanho = strtok(NULL, "@");
     fclose(fp);
     //Converte a String para Int e retorna esse valor.
     return strtol(tamanho, &end, 10);
 }

void guarda_tamanhos (char *nfile, int *tamanhos) {
    //João Silva
    FILE *fp;
    int blocos = obter_blocos(nfile);
    char *buffer = (char *) calloc((blocos*BUF_SIZE), sizeof(char)), *end, *final;
    fp = fopen(nfile, "rb");
    if (fp != NULL) {
        fread(buffer, blocos*BUF_SIZE, 1, fp);
        tamanhos[0] = obter_tamanho_blocos(nfile);
        for (int i = 0; i < blocos; i++) {
            if (i == 0) {
                strtok(buffer, "@");
                for (int j = 0; j < 2; j++)
                    final = strtok(NULL, "@");
                final = strtok(NULL, "@");
                final = strtok(NULL, "\0");
            }
            else {
                final = strtok(final, "@");
                tamanhos[i] = strtol(final, &end, 10);
                final = strtok(NULL, "\0");
            }
        }
    }
    else
        printf("Erro a abrir o ficheiro.\n");
    free(buffer);
}

//Retira as ocorrencias de arrobas no ficheiro inicial ("@R@2@991@74;0;;;;;;" -> "74;0;;;;;;;").
char *retirar_arrobas_init(char *ficheiro) {
    //João Silva
    char* final;
    strtok(ficheiro, "@");
    for (int i = 0; i < 2; i++) {
        final = strtok(NULL, "@");
    }
    final = strtok(NULL, "\0");
    return final;
}

/*Retira as ocorrencias das arrobas no ficheiro inicial (Quando ja estamos em algum bloco a seguir ao primeiro,
há menos ocorrencias de "@", sendo necessario uma segunda função para as remover).
*/
char *retirar_arrobas_final (char *ficheiro) {
    //João Silva
    char* final;
    strtok(ficheiro, "@");
    final = strtok(NULL, "@");
    final = strtok(NULL, "\0");
    return final;
}

//Ler e abrir ficheiro
void lerfreq(char *nfile, Caracter simb[][256], int blocos) {
    //João Silva
    FILE *fp = fopen(nfile, "rb");
    int tamanho = (obter_tamanho_blocos(nfile))/100;
    //BUF_SIZE * Blocos pq o ficheiro pode de texto pode conter muito mais do que 255 Caracteres por bloco.
    char *documento = (char *) calloc((blocos*5000), sizeof(char)), *end, *doc_format;
    //Coloca o documento no array
    fread(documento, 5000*blocos, 1, fp);
    fclose(fp);
    int anterior, k = 1, j = 0;
    //Começar a analise das frequencias.
    if (fp == NULL)
        printf("Erro a abrir o ficheiro.\n");
    else {
        for (int i = 0; i < blocos; i++) {
            k = 1;
            //Retira a primeira ocorrencia das "@" (para poder converter de char para int com "strtol()").
            if (i == 0)
                doc_format = retirar_arrobas_init(documento);
                //Retira os "@" (para poder converter de str para int).
            else
                doc_format = retirar_arrobas_final(doc_format);
            //Coloca o primeiro elemento do ficheiro no array de frequencias, e define esse valor como valor anterior.
            //printf("%s\n", doc_format);
            simb[i][0].freq = strtol(doc_format, &end, 10);
            anterior = simb[i][0].freq;
            simb[i][0].ind = 0;
            for (j = 1; doc_format[j] != '@'; j++) {
                //No caso de ter ";;" coloca o valor anterior no devido local do array de frequencias.
                if (doc_format[j] == ';' && doc_format[j-1] == ';') {
                    simb[i][k].freq = anterior;
                    simb[i][k].ind = k;
                    k++;
                }
//No caso de se estar a avaliar um numero inteiro, coloca esse valor no devido local do array de frequencias, e atualiza o valor anterior devidamente.
                else if (doc_format[j-1] == ';' && doc_format[j] != ';') {
                    sscanf(&doc_format[j], "%d", &anterior);
                    simb[i][k].freq = anterior;
                    simb[i][k].ind = k;
                    k++;
                }
            }
            if (doc_format[j] == '@' && doc_format[j-1] == ';') {
                simb[i][k].freq = anterior;
                simb[i][k].ind = k;
                k++;
            }
            else if (doc_format[j] == '@' && doc_format[j-1] != ';') {
                sscanf(&doc_format[j], "%d", &anterior);
                simb[i][k].freq = anterior;
                simb[i][k].ind = k;
                k++;
            }
        }
    }
    free(documento);
}

//Ordenar frequencias do ficheiro lido por ordem decrescente
void ordlist(Caracter simb[][BLOCK_SIZE], int n_blocos){
    //Carlos Santos
    //Ordenar cada bloco separadamente
    for(int bloco=0;bloco<n_blocos;bloco++){
        for(int i=0;i<BLOCK_SIZE;i++) simb[bloco][i].ind = i;
        //Segura na primeira frequencia e vai comparando com os seguintes
        for (int f1 = 0; f1 < BLOCK_SIZE; f1++) {
            for (int f2 = f1+1; f2 < BLOCK_SIZE; f2++) {
                //Invoca a maior frquencia encontrada no array para o primeiro lugar
                //Guarda os movimentos feitos no index
                if(simb[bloco][f1].freq < simb[bloco][f2].freq) {
                    Caracter temp = simb[bloco][f1];
                    simb[bloco][f1] = simb[bloco][f2];
                    simb[bloco][f2] = temp;
                }
            }
        }
    }
}

void execsf(Caracter simb[][BLOCK_SIZE], CodigoSF cod[][BLOCK_SIZE], int blocos){
    //Carlos Santos

    int size;
    //Preenche o cod com -1, ou seja, sem freq
    for(int i = 0; i < blocos; i++){
        for(int j = 0; j < BLOCK_SIZE; j++) cod[i][j].codigo[0] = 0x00;
    }
    //Calcula o binario para cada bloco
    for(int j = 0; j < blocos; j++) {
        // Cortar a lista com apenas os Caracteres usados
        struct nodo *raiz = novoNodo(simb[j]);
        distbinary(cod,j,raiz,"");
        free(simb[j]);
    }
}

//Escreve no ficheiro os códigos SF correspondentes a cada ficheiro.
void savedata(CodigoSF cod [][BLOCK_SIZE], char *nfile, char *novo) {
    //João Silva
    FILE *fp;
    int blocos = obter_blocos(nfile);
    //Guarda os tamanhos de cada bloco.
    int tamanhos[blocos];
    guarda_tamanhos(nfile, tamanhos);

    fp = fopen(novo, "wb");
    if (fp != NULL) {
        //Verifica se o ficheiro tem compressão RLE ou não, e esreve no ficheiro o resultado (@R no caso de ter e @N no caso de não ter).
        if (check_rle(nfile) == 1)
            fprintf(fp, "@R@%d@%d@",blocos, tamanhos[0]);
        else
            fprintf(fp, "@N@%d@%d@",blocos, tamanhos[0]);
        //Escreve os códigos no ficheiro.
        for (int i = 0; i < blocos; i++) {
            /*No caso de não estarmos no primeiro bloco (é o único cujo formato da informação do bloco é diferente),
            coloca no ficheiro o tamanho do bloco.*/
            if (i != 0)
                fprintf(fp,"@%d@", tamanhos[i]);

            for (int j = 0; j < 256; j++) {
                if (j == 0) {
                    //No caso de haver um código gerado para o simbolo a ser analisado, coloca esse codigo no ficheiro
                    if (cod[i][0].codigo[0] != 0x00)
                        fprintf(fp, "%s;", cod[i][0].codigo);
                    //No caso de nao haver código gerado para esse simbolo, coloca apenas um ";".
                    else
                        fprintf(fp, ";");
                }
                else if (j != BLOCK_SIZE-1){
                    if (cod[i][j].codigo[0] != 0x00)
                        fprintf(fp, "%s;", cod[i][j].codigo);
                    else
                        fprintf(fp, ";");
                }
                else {
                    if (cod[i][j].codigo[0] != 0x00)
                        fprintf(fp, "%s", cod[i][j].codigo);
                }
            }
        }
        fprintf(fp, "@0");
    }
    fclose(fp);
}

int modulot(char *nfile){
    //João Silva/Carlos Santos
    clock_t begin = clock();
    //Obter numero de blocos e a quantidade de bits em cada um.
    int blocos = obter_blocos(nfile);
    int tamanho_blocos[blocos];
    guarda_tamanhos(nfile, tamanho_blocos);

    Caracter simb [blocos][BLOCK_SIZE];
    CodigoSF cod [blocos][BLOCK_SIZE];
    //Lê as frequencias e coloca-as no array "freqs".
    lerfreq(nfile, simb, blocos);
    //Ordenar a lista de símbolos na tabela pelo
    //valor descendente da sua frequência
    ordlist(simb,blocos);
    //Executar do algoritmo de SF para cada bloco
    execsf(simb, cod, blocos);

    char nome[100];
    gera_novo_nome_t (nfile, nome);

    //Armazenar os resultados
    savedata(cod, nfile, nome);

    clock_t end = clock();
    double tempo_exec = (double) (end - begin) / CLOCKS_PER_SEC;

    //Informações do módulo t
    modinfo(blocos, tamanho_blocos, tempo_exec, nome);

    return 0;
}
