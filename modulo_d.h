#ifndef PROJCD_MODULO_D_H
#define PROJCD_MODULO_D_H

typedef struct padraoRLE{          // Guardar todos os PadroesRLE existentes no file
    char Cod[1000];                // Pré codigo ao padrão
    char simb[10];                 // Guarda o Simbolo a repetir
    int rep;                       // Guarda o numero de vezes a repetir
    struct padraoRLE *prox;        // Aceder ao proximo padraoRLE
}NpadraoRLE, *LpadraoRLE;

typedef struct sKeepCod{           //Uma arvore aonde guardamos o char e o código binário correspondente da codificação SF
    char valor;                    //Char correspondente
    char codBin[8];                //Código binário correspondente
    struct sKeepCod *dir, *esq;    //Se o bit lido for 1 vai para a esquerda se for 0 vai para a direita
}NLKeepCod, *LKeepCod;

typedef struct scodTree{           //Estrutura que guarda o Bloco em que nos emcontramos e a codificação SF dele
    int bloco;                     //Número do bloco a ser lido
    struct scodTree *next;         //Se for um bloco diferente passamos para uma outra estrutura
    NLKeepCod *dir, *esq;          //Se for este o Bloco começamos a analisar o código
}NCodTree, *CodTree;


void modinfo(int blocos, int *tamanho_blocos, double T_exec, char* nome_gerado);

void New_name (char *nfile, char novo[], char ind);

void fileAux (char *nfile, char novo[], char ind);

void newNodo(LpadraoRLE *lre, char *simb, int rep);

LpadraoRLE procuraPadRLE(char *filename, int blocos);

void printFIlE(char *name, LpadraoRLE lre);

LKeepCod newRote(char valor, char *codBin);

CodTree saveSFCod(char *nflie, int bloco, int *keepPos ,int pBloco);

char searchTree(CodTree tree, char *decod, int *val);

void SFtotxt(CodTree tree, char *tmp, char *keepBloco);

void decodShaf(char* nfile, CodTree tree, int bloco, int *keepPos, char *keepBloco, int pBloco);

void decodSF(char *nfile, char*nfileaux, int blocos);
                         
int modulo_d(char *nfile, char *nfileaux, char decod);

#endif //PROJCD_MODULO_D_H
