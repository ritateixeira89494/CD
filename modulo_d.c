#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "modulo_d.h"
#include "mod_t.h"

void modinfo(int blocos, int *tamanho_blocos, double T_exec, char* nome_gerado){
    printf("Luís Fernandes, a88539, MIEI/CD, 1-jan-2021\n"
           "Raul Parente, a88321, MIEI/CD, 1-jan-2021\n"
           "Módulo: d (descodificação dum ficheiro shaf)\n"
           "Número de blocos: %d\n"
           "Tamanho dos blocos analisados no ficheiro de símbolos: ", blocos);
    for (int i = 0; i < blocos; i++) {
        printf("%d", tamanho_blocos[i]);
        if (i < blocos-1)
            printf("/");
    }
    printf(" bytes.");
    printf("Tempo de execução do módulo (milissegundos): %f\n"
           "Ficheiro gerado: %s\n", T_exec, nome_gerado);
}

void New_name (char *nfile, char novo[], char ind) {
    if (ind == 's' &&  check_rle(nfile) == 1){
        strcpy(novo, nfile);
        strtok(novo, ".");
        strcat(novo, ".txt.rle");
    }
    else{
        strcpy(novo, nfile);
        strtok(novo, ".");
        strcat(novo, ".txt");
    }
}

void fileAux (char *nfile, char novo[], char ind) {
    
    char *aux;
    
    if (ind == 's' &&  (aux = strstr(nfile, "rle") != NULL){
        strcpy(novo, nfile);
        strtok(novo, ".");
        strcat(novo, ".txt.rle.cod");
    }
    else if(ind == 's'){
        strcpy(novo, nfile);
        strtok(novo, ".");
        strcat(novo, ".txt.cod");
    }else if (ind == 'r' &&  (aux = strstr(nfile, "rle") != NULL){
        strcpy(novo, nfile);
        strtok(novo, ".");
        strcat(novo, ".txt.rle.freq");
    } else {
        strcpy(novo, nfile);
        strtok(novo, ".");
        strcat(novo, ".txt.freq");
    }
}

void newNodo(LpadraoRLE *lre, char *simb, int rep, char *cod){  // Insere um novo padrao na lista de padroes

    LpadraoRLE novonodo;

    novonodo = malloc(sizeof(NpadraoRLE));
    strcpy(novonodo->simb ,simb);
    novonodo->rep = rep;
    strcpy(novonodo->Cod, cod);
    novonodo->prox = NULL;

    *lre = novonodo;


}

LpadraoRLE procuraPadRLE(char *filename,int blocos){ //erros em certos padrões

    FILE *fp;
    fp = fopen(filename, "rb");

    char documento [BUF_SIZE*blocos];
    //Coloca o documento no array
    fread(documento, BUF_SIZE*blocos, 1, fp);

    fclose(fp);

    char cod[1000];
    int bit = 0, tam = 0, cont = 0;
    LpadraoRLE r = NULL, *sitio;
    sitio = &r;

    char aux;
    while(cont != 1 && bit < BUF_SIZE*blocos){
        aux = documento[bit++];
        cod[tam] = aux; //guarda numa lista o codigo nao inserido num padrao

        if(aux == '\0'){ // se existir então existe um padrao RLE
            cod[tam] = '\0';
            tam = 0;
            char crep;
            char simb[2];
            aux = documento[bit++];
            if(aux == '\0') { // se existir coloca com simbolo
                simb[0] = '\0';
            }else { // caso o contrário
                simb[0] = aux; // retira o simbolo que se repete
            }

            crep = documento[bit++]; // Numero de vezes que repete o simb
            int rep = crep;

            newNodo(&(*sitio), simb, rep, cod);
            sitio = &((*sitio)->prox);
            
            if(rep == 0){
                cont = 1;
            }

        } else 
            tam++;
    }
    if(tam != 0)
        newNodo(&(*sitio), "FIM", 0, cod); //Insere mais um nodo só para guardar o restante código

    return r;

}

void decodFile(char *name, LpadraoRLE lre) { // Imprime um ficheiro apos a descodificação RLE

    FILE *fp;
    fp = fopen(name, "wb");

    for(;lre != NULL; lre = lre->prox){

        fprintf(fp, "%s", lre->Cod);

        if (strcmp(lre->simb, "FIM") != 0) {
            for (int i = 0; i < lre->rep; i++)
                fprintf(fp, "%s", lre->simb);
        }

    }

    fclose(fp);

}

//Descodificação SF

LKeepCod newRote(char valor, char *codBin){

    LKeepCod novonodo;

    novonodo = malloc(sizeof(NLKeepCod));
    novonodo->valor = valor;
    strcpy(novonodo->codBin, codBin);
    novonodo->esq = novonodo->dir = NULL;

    return novonodo;

};

CodTree saveSFCod(char *nflie, int bloco, int *keepPos ,int pBloco){//Armazena numa árvore binária o código de cada bloco

    int i = 0;
    char codbin[8], aux;
    int cont = 0;
    char documento[BUF_SIZE*1];
    CodTree tree = malloc(sizeof(NCodTree)), *sitio;
    sitio = &tree;
    (*sitio)->bloco = bloco;
    (*sitio)->dir = (*sitio)->esq = NULL;
    LKeepCod *pos;
    
    //abre o ficheiro e passa o bloco para memoria
    FILE *fp = fopen(nflie,"rb");

    if (fp == NULL)
        printf("Erro a abrir o ficheiro.\n");
    else if(bloco == 1)
        fread(documento,BUF_SIZE*1,1,fp);
    else {
        fseek(fp,pBloco,SEEK_SET);
        fread(documento,BUF_SIZE*1,1,fp);
    }
    fclose(fp);
    
    
    while((aux = documento[i++]) != ';');

    while(i < (BUF_SIZE*1)){

        LKeepCod *dir = &((*sitio)->dir), *esq = &((*sitio)->esq);
        int cb = 0;
        
        //Se encontrar um 0 ou um 1 começa a guarda o código
        while(aux == '0' || aux == '1'){
            codbin[cb++] = aux;
            aux = documento[i++];
        }

        char c = cont;
       
        //Guarda o Simbolo
        if(aux == ';') {
            cont++;
            codbin[cb] = '\0';
        }
        
        //Se encontrou uma codificaçao binária para um simbolo adicionar ambos á arvore
        if(cb != 0){
            cb = 0;

            if(codbin[cb] == '1')     //Se for 1 desce pela direita
                pos = &(*dir);
            else if(codbin[cb] == '0')//Se for 0 desce pela esquerda
                pos = &(*esq);
            cb++;

            while(codbin[cb] && *pos){
                if (codbin[cb++] == '1'){
                    if (strcmp((*pos)->codBin, codbin) != 0)
                        pos = &((*pos)->dir);
                } else if (codbin[cb++] == '0'){
                    if (strcmp((*pos)->codBin, codbin) != 0)
                        pos = &((*pos)->esq);
                }
            }

            LKeepCod new = NULL;
            new = newRote(c, codbin);

            *pos = new;
            /*if(!(*pos)){ //Caso a string acabasse inserir um nodo na pos e o que estava em pos passar a folha

            } else {
                if (((*pos)->codBin[0]) == '0')
                    new->esq = pos;
                else
                    new->dir = pos;

                *pos = new;
            }*/
        }

        if(cont == 255)
            break;

        aux = documento[i++];

    }

    *keepPos = i;
    return tree;

}
    
char searchTree(CodTree tree, char *decod, int *val){//Procura na aŕvore se existe o codigo binário

    int i = 0;
    char r = 'NULL';
    LKeepCod aux;

    if(decod[i] == '0')
        aux = tree->esq;
    else if(decod[i] == '1')
        aux = tree->dir;
    i++;

    while(decod[i] != '\0' && strcmp(decod, aux->codBin) != 0){
        if(decod[i] == '1')
            aux = aux->dir;
        else
            aux = aux->esq;

        i++;
    }

    if(strcmp(decod, aux->codBin) != 0) // Val serve para identificar se encontrou um simbolo ou não
        *val = 0;
    else{
        *val = 1;
        r = aux->valor;
    }

    return r;
}

void SFtotxt(CodTree tree, char *tmp, char *keepBloco){//Procura na arvore a informação do bloco em analise

    char dec[16];
    int j = 0, k = 0;

    while(j<(BUF_SIZE)){
        int val;
        char caracter;
        dec[k++] = tmp[j++];

        caracter = searchTree(tree, dec, &val);

        if(val){
            strcpy(keepBloco,caracter); //Guardar o simbolo num array para despois guardar no file
        }

    }
}

void decodShaf(char* nfile, CodTree tree, int bloco, int *keepPos, char *keepBloco, int pBloco){//Descodifica o ficheiro Shaf

    int i = 0;
    char documento[BUF_SIZE*1], tmp[BUF_SIZE];
    char aux, bits[CHAR_BIT];
    
    //Guarda o conteudo do ficheiro por bloco na memoŕia
    FILE *fp = fopen(nfile,"rb");

    if (fp == NULL)
        printf("Erro a abrir o ficheiro.\n");
    else if(bloco == 1)
        fread(documento,BUF_SIZE*1,1,fp);
    else {
        fseek(fp,pBloco,SEEK_SET);
        fread(documento,BUF_SIZE*1,1,fp);
    }
    fclose(fp);

    aux = documento[i++];
    while(aux != '0' && aux != '1')
        
    //Guarda numa string os bytes de cada char retirado do bloco 
    while(i < (BUF_SIZE*1) && aux != '@') {

        for(int b = 0; b < CHAR_BIT; b++)
            bits[b] = '0' + ((aux & (1 << b)) > 0);

        strcat(tmp,bits);
        aux = documento[i++];
    }

    SFtotxt(tree, tmp, keepBloco);

    *keepPos = i;
}

char *decodSF(char *nfile, char*nfileaux, int blocos){//Descodifica o ficheiro

    int bloco = 1;
    int cnewbloco, snewbloco;
    int cpos = 0, spos = 0;
    char decodfile[BUF_SIZE*blocos];
    char temp[BUF_SIZE];

    while(bloco <= blocos){
        CodTree tree = saveSFCod(nfileaux, bloco, &cnewbloco, cpos);
        decodShaf(nfile, tree, bloco, &snewbloco, temp, spos);
        cpos += cnewbloco;
        spos += snewbloco;
        strcat(decodfile,temp);
        bloco++;
    }
    
    return decodfile;
}

int modulo_d(char *nfile, char decod){ // NOTA: APenas está para padrões RLE, ainda falta inserir código SF
    clock_t begin = clock();
    //Buscar o ficheiro Cod ou Freq
    
    char nfileaux[100];
    fileAux (nfile, nfileaux, decod);
    
    //Obter numero de blocos e a quantidade de bits em cada um.
    int blocos = obter_blocos(nfileaux);
    int tamanho_blocos[blocos];

    guarda_tamanhos(nfileaux, tamanho_blocos);

    char name[100];
    New_name (nfileaux, name, decod);

    if(decod == 'r'){
        //Lê o ficheiro e guarda os padrões RLE e o código numa struct
        LpadraoRLE lre = procuraPadRLE(nfile,blocos);

        //Armazena os dados descodificados
        decodFile(name, lre);
    } else {
        //Descodifica o ficheiro shal e guarda o código num string
        char *decodfile = decodSF(nfile, nfileaux, blocos);
        
        //Armazena os dados num ficheiro
        FILE *fp = fopen(name,"wb");

        if (fp == NULL)
            printf("Erro a abrir o ficheiro.\n");
        else 
            fwrite(decodfile,BUF_SIZE*blocos,1,fp);
        fclose(fp);
    }


    clock_t end = clock();
    double temp_exec = (double) (end - begin) / CLOCKS_PER_SEC;

    //Informação do modulo d
    modinfo(blocos, tamanho_blocos,temp_exec,name);

    return 0;
}
