#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "mod_c.h"

/*
* Função que cria o array com os códigos binários de cada caractér ascii (index do array corresponde ao código ascii do caractér em questão).
*/
char* codes(char codigos[256][TAM_CODIGO], char bloco[TAMANHO]){


        for(int i=0, j=0, ascii=0; bloco[i] != NULL; i++, ascii++){
        char temp[TAM_CODIGO] = "";

        for(j=0; isdigit(bloco[i+j]); j++){
            temp[j] = bloco[i+j];
        }
        strcpy(codigos[ascii],temp);
        i += j;
    }

    return *codigos;
}


int moduloC(char *path)
{
    clock_t begin = clock();
    FILE *fptr;
    char buffer[TAMANHO];
    char pathCod[30];

    strcpy(pathCod,path);

    fptr = fopen(strcat(pathCod,".cod"), "r");

    if(fptr == NULL){
        printf("Falha na abertura do ficheiro .cod !!\n");
        return 1;
    }

    fgets(buffer,TAMANHO,fptr);
    //printf("buffer:%s|.\n",buffer);

    fclose(fptr);

    char codigos[256][TAM_CODIGO];
    char *token;
    const char s[2] = "@";


    char tipo=' ';
    int nBlocos=0,tamBloco=0;

    token = strtok(buffer, s);
    tipo = token[0];

    token = strtok(NULL, s);
    nBlocos = atoi(token);

    int infoAntes[nBlocos], infoDepois[nBlocos];

    FILE *fich;
    fich = fopen(path,"r");
    if(fich == NULL){
        printf("Falha na abertura do ficheiro original !!\n");
        return 1;
    }

    FILE *output;
    char fileOutput[30];

    strcpy(fileOutput, strcat(path,".shaf"));

    output = fopen(fileOutput, "w");


    if(fich == NULL){
        printf("Falha na criação do ficheiro .shaf!\n");
        return 1;
    }


    char saveBuff[20];
    snprintf(saveBuff, sizeof(buffer), "@%d",nBlocos);
    fwrite(saveBuff, strlen(saveBuff), 1, output);

for(int main_i=0; main_i < nBlocos; main_i++){

    token = strtok(NULL, s);
    tamBloco = atoi(token);

    infoAntes[main_i] = tamBloco;

    token = strtok(NULL, s);
    //printf("token:%s|.\n",token);

    codes(codigos, token);

char bloco[tamBloco];

fgets(bloco,tamBloco,fich);
//printf("bloco:%s|.",bloco);

char bits[tamBloco];
int bit_offset = 8;


    int b = 0;
    bits[b] = bits[b] & 0; //limpar a posição de memória.

    for(int i=0; bloco[i] != NULL; i++){

        for(int len=0; len < strlen(codigos[bloco[i]]); len++){

            if(codigos[bloco[i]][len] == '0'){
                if(bit_offset == 1){
                    b++;
                    bits[b] = bits[b] & 0;
                    bit_offset = 8;
                }else{
                    bits[b] <<= 1;
                    bit_offset--;
                }

            }else{ // == 1
                if(bit_offset == 1){
                    bits[b] = bits[b] | 1;

                    b++;
                    bits[b] = bits[b] & 0;
                    bit_offset = 8;
                }else{
                    bits[b] <<= 1;
                    bit_offset--;
                    bits[b] = bits[b] | 1;
                }//end else2.
            }//end else1.
        }//end for2.
    }//end for1.



    b++; //incrementar o b para contar com a posição 0 do array.

    if(bit_offset == 8){
        b--;
        snprintf(saveBuff, sizeof(buffer), "@%d@",b);
        fwrite(saveBuff, strlen(saveBuff), 1, output);
    }else{
        snprintf(saveBuff, sizeof(buffer), "@%d@",b);
        fwrite(saveBuff, strlen(saveBuff), 1, output);
    }
    fwrite(bits, b, 1, output);
    infoDepois[main_i]=b;

}//end of for

clock_t end = clock();
double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

time_t t;
struct tm* current_time;

t = time(NULL);
current_time = localtime(&t);
char mes[4];

switch(current_time->tm_mon+1)
{
case 1:
    strcpy(mes, "jan");
    break;
case 2:
    strcpy(mes, "fev");
    break;
case 3:
    strcpy(mes, "mar");
    break;
case 4:
    strcpy(mes, "abr");
    break;
case 5:
    strcpy(mes, "mai");
    break;
case 6:
    strcpy(mes, "jun");
    break;
case 7:
    strcpy(mes, "jul");
    break;
case 8:
    strcpy(mes, "ago");
    break;
case 9:
    strcpy(mes, "set");
    break;
case 10:
    strcpy(mes, "out");
    break;
case 11:
    strcpy(mes, "nov");
    break;
case 12:
    strcpy(mes, "dez");
    break;
}

    printf("\n Tiago Ribeiro, a76420, MIEI/CD, %d-%s-%d", current_time->tm_mday, mes, current_time->tm_year+1900);
    printf("\n Modulo: c (codificacao dum ficheiro de simbolos)");
    printf("\n Numero de blocos: %d", nBlocos);

    double taxaCompressao = 0.0, totalAntes=0.0, totalDepois = 0.0;

    for(int i=0; i<nBlocos; i++){
        printf("\n Tamanho antes/depois & taxa de compressao (bloco %d): %d/%d", i, infoAntes[i], infoDepois[i]);
        totalAntes += infoAntes[i];
        totalDepois += infoDepois[i];
    }

    taxaCompressao = ( (totalAntes - totalDepois) / totalAntes ) * 100;

    printf("\n Taxa de compressao global: %.0f%%", taxaCompressao);
    printf("\n Tempo de execucao do modulo (milissegundos): %f", time_spent);
    printf("\n Ficheiro gerado: %s\n", fileOutput);

    fclose(fich);
    fclose(output);

    return 0;
}

