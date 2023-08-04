#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mod_f.h"
#include "string.h"
#define BLOCK_SIZE 256

void modinfo_f(char *nfile, int nr_blocos, int tamanho, int ultimo_bloco, float taxa_compressao, int tamanhos_rle[], double T_exec, int rle) {
    // (Vasco Moreno / Rita Teixeira)
    char nome_rle[BUF_SIZE]; strcpy(nome_rle, nfile); strcat(nome_rle, ".rle");
    char freq[BUF_SIZE]; strcpy(freq, nfile); strcat(freq, ".freq");
    char freq_rle[BUF_SIZE]; strcpy(freq_rle, nfile); strcat(freq_rle, ".rle.freq");
    printf("Rita Teixeira, a89494, MIEI/CD, 1-Jan-2021\n");
    printf("Vasco Moreno, a94194, MIEI/CD, 1-Jan-2021\n");
    printf("Numero de blocos: %d\n", nr_blocos);
    printf("Tamanho dos blocos analisados no ficheiro original: ");
    for(int i = 0; i < nr_blocos; i++) {
        if (i == nr_blocos-1)
            printf("%d\n", ultimo_bloco);
        else
            printf("%d/", tamanho);
    }
    printf("Compressao RLE: %s (%.2f%%   compressao)\n", nome_rle, taxa_compressao);
    printf("Tamanho dos blocos analisados no ficheiro RLE: ");
    if (rle == 1) { //Ficheiro submetido a compressão RLE
        for (int i = 0; i < nr_blocos; i++) {
            if (i == nr_blocos-1)
                printf("%d\n", tamanhos_rle[i]);
            else
                printf("%d/", tamanhos_rle[i]);
        }
    }
    else //Ficheiro não submetido a compressão RLE
        printf("%d\n", tamanhos_rle[0]);
    printf("Tempo de execucao do modulo (milissegundos): %.3f\n", T_exec*1000);
    printf("Ficheiros gerados: ");
    if (rle == 1)
        printf("%s, %s\n", freq, freq_rle);
    else
        printf("%s\n", freq);
}

void gera_novo_nome_rle (char *nfile, char novo[]) {
    /* (Rita Teixeira)
     * Gera novo nome no caso de acontecer compressão RLE.
     * Usada em ficheiros comprimidos e toma como input a string apontada por
     * "char *nfile", coloca em "char novo[]", e adiciona ".rle" ao novo nome.
     */
        strcpy(novo, nfile);
        strcat(novo, ".rle");
}

void gera_novo_nome_freq (char *nfile, char *novo, int rle) {
    /* (Rita Teixeira)
     * Gera um novo nome adicionando ".freq" na terminação do ficheiro.
     * Pega num bloco e copia o conteudo de "char *nfile" para "char *novo",
     * de seguida entra num ciclo onde se o ficheiro não foi submetido a
     * compressão RLE concatena ".freq" ao final do novo nome e
     * caso contrário adiciona ".rle.freq" para indicar o tipo de ficheiro
     * que estamos a ver. (Rita Teixeira)
     */
    strcpy(novo, nfile);
    if (rle == 0) {
        strcat(novo, ".freq");
    }
    else
        strcat(novo, ".rle.freq");

}
//Função fsize() dada pelo docente
long long fsize(FILE *fp_in, unsigned char *filename, unsigned long *the_block_size, long *size_of_last_block) {
    unsigned long long total;
    long long n_blocks;
    unsigned long n_read, block_size;
    unsigned char *temp_buffer;
    int fseek_error;
    FILE *fp;

    block_size = *the_block_size;
    if (block_size > FSIZE_MAX_BLOCK_SIZE) return (FSIZE_ERROR_BLOCK_SIZE);
    if (block_size == 0UL) block_size = FSIZE_DEFAULT_BLOCK_SIZE;
    if (block_size < FSIZE_MIN_BLOCK_SIZE) block_size = FSIZE_MIN_BLOCK_SIZE;
    *the_block_size = block_size;

    if (filename == NULL || *filename == 0) fp = fp_in;
    else {
        fp = fopen(filename, "rb");
        if (fp == NULL) return (FSIZE_ERROR_IN_FILE);
    }
    fseek_error = fseek(fp, 0L, SEEK_SET);
    if (fseek_error) return (FSIZE_ERROR_IN_FILE);

    fseek_error = fseek(fp, 0L, SEEK_END);
    if (!fseek_error) {
        total = ftell(fp);
        if (total == FSIZE_ERROR_IN_FTELL) return (FSIZE_ERROR_IN_FILE);
        n_blocks = total / block_size;
        if (n_blocks * block_size == total) *size_of_last_block = block_size;
        else {
            *size_of_last_block = total - n_blocks * block_size;
            n_blocks++;
        }
        return (n_blocks);
    }
    n_blocks = FSIZE_MAX_SIZE_FSEEK / block_size -
               1; // In reality fseek() can't handle FSIZE_MAX_SIZE_FSEEK of 2GBytes, so let's use a smaller size
    fseek_error = fseek(fp, n_blocks * block_size, SEEK_SET);
    if (fseek_error) return (FSIZE_ERROR_IN_FILE);
    temp_buffer = malloc(sizeof(unsigned char) * block_size);
    do {
        n_blocks++;
        n_read = fread(temp_buffer, sizeof(unsigned char), block_size, fp);
    } while (n_read == block_size && n_blocks <= FSIZE_MAX_NUMBER_OF_BLOCKS);
    free(temp_buffer);
    if (n_blocks > FSIZE_MAX_NUMBER_OF_BLOCKS) return (FSIZE_ERROR_NUMBER_OF_BLOCKS);
    if (n_read == 0L) {
        *size_of_last_block = block_size;
        n_blocks--;
    } else *size_of_last_block = n_read;
    if (filename == NULL || *filename == 0) {
        fseek_error = fseek(fp, 0L, SEEK_SET);
        if (fseek_error) return (FSIZE_ERROR_IN_FILE);
    } else fclose(fp);
    return (n_blocks);
}

int check_seguidas (char *bloco, int tamanho, int i) {
    /* (Vasco Moreno)
     * Inicializa um contador a 1 e vai comparando dentro de um
     * ciclo se um elemento é igual ao anterior. Caso isto aconteça
     * adiciona 1 ao contador e volta ao ciclo, caso contrário
     * pára e damos return ao valor guardado em contador.
     * Só entramos no ciclo se o indice a trabalhar for menor ou
     * igual ao tamanho - 4 porque se for menor que isso já não
     * interessa ler visto que só queremos comprimir para caracteres
     * que aparecem 4 vezes ou mais seguidos. (Rita Teixeira)
     */
    int contador = 1;
    if (i <= tamanho - 4) {
        for (int j = i+1; j < tamanho; j++) {
            if (bloco[j] == bloco[i])
                contador++;
            else
                break;
        }
    }
    return contador;
}

void compressaoRLE (char *bloco, int tamanho, RLE novo[]) {
    // (Vasco Moreno)
    /*
     * Com k inicializado a 0, tem-se um ciclo que vai percorrer
     * todos os elementos de um dado bloco. Dentro deste,
     * chamamos a função "checkseguidas()" dando-lhe os respetivos
     * argumentos. Caso o valor recebido por "checkseguidas()" seja
     * maior ou igual a 4, (o que significa que houve uma sequência
     * de 4 ou mais caracteres iguais seguidos) vamos guardar na struct
     * alguns aspetos. Indicamos através de "1" que foi feita a compressão
     * RLE, a frequência do caracter é dada pelo inteiro guardado na
     * variável "seg", o "item" é o caracter em questão e atribuimos o valor
     * 0 a "final" porque nao se trata do último elemento do bloco.
     * Por fim, incrementamos k e passamos para um indice à frente
     * do último indíce analisado, evitando assim que caracteres sejam
     * lidos mais que uma vez.
     */
    int i, k = 0;
        for (i = 0; i < tamanho; i++) {
            int seg = check_seguidas(bloco, tamanho, i);
            if (seg >= 4) {
                novo[k].rle = 1;
                novo[k].freq = seg;
                novo[k].item = (unsigned char) bloco[i];
                novo[k].final = 0;
                k++;
                i = i + (seg-1);
            }
            /*
             * Caso o segmento seja menor que 4, atribuimos o valor 0 a "rle" para
             * indicar que não foi feita compressão RLE. A frequência vai passar a
             * ser 1 porque não ha segmentos de caracteres iguais, o "item" é referente
             * ao char no indice em questão do bloco e atribuimos o valor 0
             * a "final" porque nao se trata do último elemento do bloco.
             * Por fim, incrementamos k para passar ao próximo elemento.
             */
            else {
                novo[k].rle = 0;
                novo[k].freq = 1;
                novo[k].item = (unsigned char) bloco[i];
                novo[k].final = 0;
                k++;
         }
            // Se o indice i for o ultimo elemento do array atribuimos 1 a "final".
            if (i == tamanho - 1)
                novo[k].final = 1;
        }
}

float check_compressao (char *nfile, RLE comp [], int tamanho) {
    /* (Vasco Moreno)
     *  Usamos a função "calloc" porque no caso de o ficheiro ter grandes
     *  dimensões podemos não ter espaço suficiente na stack para guardar
     *  esse array, por isso, inicializa-mos o array ficheiro como um array
     *  dinâmico e por isso usamos "calloc" para assim guardar tudo em memória.
     *  Nesta função, abre-se o ficheiro no modo leitura em binário "rb" e
     *  através do "fread()" analisamos byte a byte o conteúdo de "fp" e coloca-se
     *  em "ficheiro". Depois disso chama-se a função "compressaoRLE()" e dâo-se os
     *  devidos argumentos. Dizemos que a variável do tipo inteiro "tamanho_novo"
     *  vai ser dada pelo novo tamanho do bloco comprimido.
     *  Como precisamos da taxa de compressão para saber se compensa fazer compressão
     *  para os restantes blocos definimos uma variável "comp" do tipo "float"
     *  que é resultante da divisão entre a diferença do tamanho do bloco original com
     *  o tamanho do bloco comprimido e do tamanho original. Tudo isto multiplicado por
     *  100 para obtermos a taxa de compressão em percentagem.
     *  Libertamos a memória do "ficheiro" e dá-se return à taxa de compressão.
     */
    FILE *fp;
    char *ficheiro = (char*) calloc(tamanho, sizeof(char));
    fp = fopen(nfile, "rb");

    if (fp != NULL) {
        fread(ficheiro, tamanho, 1, fp);
        compressaoRLE(ficheiro, tamanho, comp);
        int tamanho_novo = get_size(comp);
        float comp = ((tamanho - (float) tamanho_novo)/ tamanho) * 100;
        free(ficheiro);
        return comp;
    }
    else
        printf("Erro a abrir o ficheiro\n");
    free(ficheiro);
    return 0;
}

int get_size (RLE array[]) {
    /* (Rita Teixeira)
     * Função que percorre todos os elementos e que para o
     * caso de estarmos perante compressão RLE, adiciona ao contador
     * 3 (que é o tamanho que o rle ocupa em bytes). E para o caso de
     * não ter ocorrido compressão apenas incrementa o contador.
     * O valor de output desta função vai ser o tamanho do array.
     */
    int contador = 0, i = 0;
    while(array[i].final != 1) {
        if (array[i].rle == 1)
            contador = contador + 3;
        else
            contador++;

        i++;
    }
    return contador;
}

int get_size_data (RLE array[]) {
    /* (Rita Teixeira)
     * Esta função percorre o ficheiro inteiro e vai returnar o valor
     * do ficheiro e não do array como faz a "get_size()"
     */
    int contador = 0, i = 0;
    while(array[i].final != 1) {
        contador++;
        i++;
    }
    return contador;
}

void conta_frequencias (char *nfile, int freqs[][BLOCK_SIZE], int nr_blocos, int tamanho_primeiro, int tamanho_ultimo, int tamanho_total) {
    /* (Vasco Moreno)
     * Função que conta frequencias sem RLE e começa por colocar
     * todos os elementos de freqs a 0.
     */
    FILE *fp;
    char* ficheiro = (char*) calloc(tamanho_total, sizeof (char));
    fp = fopen(nfile, "rb");
    for (int i = 0; i < nr_blocos; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            freqs[i][j] = 0;
        }
    }
    /*
     * Caso consigamos abrir o ficheiro, através de "fread()" vamos copiar byte a byte
     * o conteudo apontado por "fp" para o "ficheiro". Começamos por percorrer bloco a bloco
     * até ao último, fazemos isto porque só o ultimo bloco é que vai ter um tamanho diferente
     * e então tratamos os restantes blocos de igual forma. Entramos num ciclo que percorre todos
     * os indices até um desses ser igual ao tamanho do primeiro bloco. De seguida, entramos num
     * novo ciclo que inicializa "k=0", e que por sua vez vai comparar o indice "ficheiro[j]" a todos
     * os caracteres ASCII até haver uma correspondência. Caso aconteça, acedemos à informação
     * guardada em "freqs[i][j]" e incrementamos.
     */
    if (fp != NULL) {
        fread(ficheiro, tamanho_total, 1, fp);
        for (int i = 0; i < nr_blocos; i++) {
            if (i != nr_blocos-1) {
                for (int j = 0; j < tamanho_primeiro; j++) {
                    for (int k = 0; k < BLOCK_SIZE; k++) {
                        if (ficheiro[j] == k)
                            freqs[i][k]++;
                    }
                }
            }
            else {
                /*
                 * Neste excerto de código estamos a usar o mesmo procedimento que em cima, mas
                 * desta vez estamos só a analisar o último bloco.
                 */
                for (int j = 0; j < tamanho_ultimo; j++) {
                    for (int k = 0; k < BLOCK_SIZE; k++) {
                        if (ficheiro[j] == k)
                            freqs[i][k]++;

                    }
                }
            }
        }
    }
    fclose(fp);
    free(ficheiro);
}
void reset_RLE (RLE comp[], int tamanho) {
    /* (Rita Teixeira)
     * Função que retira toda a informação referente às variaveis
     * "final" e "rle" (inseridas na struct RLE) de um dado indíce.
     */
    for (int i = 0; i < tamanho; i++) {
        comp[i].final = 0;
        comp[i].rle = 0;
    }
}

void compressao (char *nfile, int tamanho_total, char *novo_nome, int tamanho_bloco, int nr_blocos, int freqs[][BLOCK_SIZE], int tamanho_ultimo,
    /* (Vasco Moreno / Rita Teixeira)
     * Faz a compressão do ficheiro inteiro.
     * Abre o ficheiro no modo leitura em binário e lê o ficheiro passando-o
     * para o array.
     * Fechamos o ficheiro e abrimos de novo mas desta vez em modo escrita binária.
     */
    int tamanhos[]) {
    char *ficheiro = (char *) calloc(tamanho_total, sizeof(char));
    FILE *file = fopen(nfile, "rb");
    if (file != NULL)
        fread(ficheiro, tamanho_total, 1, file);
    fclose(file);
    RLE *comp = (RLE*)calloc(tamanho_bloco, sizeof(RLE));
    FILE *fp;
    fp = fopen(novo_nome, "wb");
    /*
     * Começamos por analisar bloco a bloco e recorremos à função
     * "compressaoRLE()" em função da condição dentro do "if".
     * O que difere nessa condição é saber se estamos a trabalhar
     * com o último bloco ou com os restantes.
     * Para a função "compressaoRLE()" passa-se sempre o endereço do
     * primeiro elemento de cada bloco. Isto é feito a partir da
     * multiplicação do indíce i dentro do ciclo pelo tamanho do bloco.
     * ex: (bloco2 -> indíce=1 , logo passa-se &ficheiro[1*tamanho_bloco])
     * ex: (bloco3 -> indíce=2 , logo passa-se &ficheiro[2*tamanho_bloco])
     * Após esta condição, recorremos à função "escreveRLE()" para obtermos
     * a nossa compressão no formato que pretendemos. Visto que queremos
     * obter a compressão do bloco chama-mos a "conta_frequenciasRLE()" para
     * sabermos a frequência de cada caracter no bloco já comprimido.
     * Depois disso apenas guardamos o tamanho dos blocos comprimidos e
     * libertamos a memória de "comp" e "ficheiro" usando o "free()".
     */
    for (int i = 0; i < nr_blocos; i++) {
        reset_RLE(comp, tamanho_bloco);
        if (i != nr_blocos - 1)
            compressaoRLE(&ficheiro[i*tamanho_bloco], tamanho_bloco, comp);
        else
            compressaoRLE(&ficheiro[i*tamanho_bloco], tamanho_ultimo, comp);
        int size = get_size(comp);
        tamanhos[i] = size;
        escreveRLE(fp, comp, size);
        conta_frequenciasRLE(freqs, i, comp);
        store_tamanhosRLE(tamanhos, i, size);
    }
    free(comp);
    free(ficheiro);
    fclose(fp);
}

void conta_frequenciasRLE (int freqs[][BLOCK_SIZE], int bloco, RLE comp[]) {
    // (Rita Teixeira)
    // Guarda as frequencias do ficheiro
    int tamanho = get_size_data(comp);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        freqs[bloco][i] = 0;
    }
    freqs[bloco][0] = 0;
    for (int i = 0; i < tamanho; i++) {
        for (int k = 0; k < BLOCK_SIZE; k++) {
            if (comp[i].item == k) {
                if (comp[i].rle == 1) {
                    freqs[bloco][comp[i].freq]++; //vê o indice e incrementa a frequencia
                    freqs[bloco][k]++; //incrementa ao caracter em questão
                    freqs[bloco][0] += 1; //incrementa 1 à frequência do "NULL"
                }
                else if (comp[i].item == k)
                    freqs[bloco][k]++;
            }
        }
    }
}

void escreveRLE (FILE *fp, RLE comp[], int tamanho) {
    /* (Vasco Moreno)
     * Esta função vai escrever no ficheiro o bloco que lhe é passado,
     * no formato que foi pedido. Quando o bloco foi submetido
     * à compressão RLE, vai ficar na forma de {NULL}{ITEM}{FREQ}. Caso contrário
     * fica apenas {ITEM}.
     */
    for (int i = 0; i < tamanho; i++) {
        if (comp[i].rle == 1) {
            fprintf(fp, "%c%c%c", 0x00, comp[i].item, comp[i].freq);
        }
        else
            fprintf(fp, "%c",comp[i].item);
    }
}

// Função que vai escrever no ficheiro as frequencias.
void save_freqs(char *novo_nome, int freqs [][BLOCK_SIZE], int tamanhos [], int rle, int nr_blocos) {
    // (Vasco Moreno / Rita Teixeira)
    FILE *fp;
    fp = fopen(novo_nome, "wb");
    if (fp != NULL) {
    if (rle == 0)
        //Escreve-se @N quando se trata do ficheiro original.
        fprintf(fp, "@N@%d@%d@", nr_blocos, tamanhos[0]);
    else
        //Escreve-se @R quando se trata do ficheiro comprimido.
        fprintf(fp, "@R@%d@%d@", nr_blocos, tamanhos[0]);
    for (int i = 0; i < nr_blocos; i++) {
        if (i != 0)
            //Quando mudamos de bloco escrevemos @{tamanho_do_bloco}@
            fprintf(fp, "@%d@", tamanhos[i]);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (j == 0)
                //Só acontece para o primeiro simbolo
                fprintf(fp, "%d", freqs[i][j]);
            else {
                if (freqs[i][j] != freqs[i][j-1])
                    //Quando ocorre um valor de frequência diferente da frequência do símbolo anterior
                    fprintf(fp, ";%d", freqs[i][j]);
                else if (freqs[i][j] == freqs[i][j-1])
                    //Quando ocorre um valor de frequência igual da frequência do símbolo anterior
                    fprintf(fp, ";");
            }
        }
    }
    //Imprime @0 no final do bloco comprimido para representar a terminação do array.
    fprintf(fp, "@0");
    }
    else
        printf("Erro a abrir ficheiro.\n");
    fclose(fp);
}


//Guarda os tamanhos de cada bloco (não pode ter compressao RLE)
void store_tamanhos(int primeiro, int ultimo, int tamanhos[], int blocos) {
    //(Rita Teixeira)
    for (int i = 0; i < blocos; i++) {
        if (i == blocos - 1)
            tamanhos[i] = ultimo;
        else
            tamanhos[i] = primeiro;
    }
}

//Guarda os tamanhos dos blocos com rle.
void store_tamanhosRLE (int tamanhos[], int i, int size) {
    //(Rita Teixeira)
    tamanhos[i] = size;
}

void modulof (char *nfile, int tamanho_bloco, int comp_forcada) {
        // (Vasco Moreno / Rita Teixeira)
    clock_t begin = clock();
    unsigned long tam_bloco = (unsigned long) tamanho_bloco;
    long long n_blocos;
    int primeiro = (int) tam_bloco;
    long tam_ultimo_bloco;
    unsigned long long tamanho;
    FILE *fp;
    /*
    * Este excerto de código serve para recebermos o numero
    * de blocos e o tamanho total do ficheiro original.
    * Definimos também que a "checkcompressao()" vai ser dada por "comp".
    */
    fp = fopen(nfile, "rb");
    n_blocos = fsize(fp, NULL, &tam_bloco, &tam_ultimo_bloco);
    tamanho = (n_blocos-1) * tam_bloco + tam_ultimo_bloco;
    tamanho = (int) tamanho;
    int tamanhos[(int) n_blocos];
    RLE *teste_compressao = (RLE*)calloc(primeiro, sizeof(RLE));
    float comp = check_compressao(nfile, teste_compressao, primeiro);
    fclose(fp);
    int freqs[(int) n_blocos][BLOCK_SIZE];
    int tamanhos_rle[(int) n_blocos];
    tamanhos_rle[0] = get_size(teste_compressao);
    char novo[BUF_SIZE];

    if (comp >= 5 || comp_forcada == 1) { //Para o caso de a taxa de compressão do primeiro bloco ser maior que 5%
        gera_novo_nome_rle(nfile, novo); //gera um novo ficheiro com extensão .rle
        compressao(nfile, tamanho, novo, primeiro, (int) n_blocos, freqs, (int) tam_ultimo_bloco, tamanhos_rle);
        //executa a compressão para o ficheiro
        char novo_nome[BUF_SIZE];
        gera_novo_nome_freq(nfile, novo_nome, 1); //gera um novo ficheiro com extensão .freq
        save_freqs(novo_nome, freqs, tamanhos_rle, 1, (int) n_blocos); //escreve as frequências no ficheiro .freq
    }
    conta_frequencias(nfile, freqs, (int) n_blocos, primeiro, (int) tam_ultimo_bloco, tamanho);
    store_tamanhos(primeiro, (int) tam_ultimo_bloco, tamanhos, (int) n_blocos);

    strcpy(novo, nfile);
    strcat(novo,".freq\0");
    save_freqs(novo, freqs, tamanhos, 0, (int) n_blocos);
    clock_t end = clock();
    double tempo_exec = (double) (end - begin) / CLOCKS_PER_SEC; //definimos o tempo de execução

    //Chama a "modinfo_f()", da-mos os devidos argumentos e imprimimos tudo que é necessário para este módulo.
    if (comp >= 5 || comp_forcada == 1)
        modinfo_f(nfile, (int) n_blocos, primeiro, (int) tam_ultimo_bloco, comp, tamanhos_rle, tempo_exec, 1);
    else if (comp < 5 && comp_forcada != 1)
        modinfo_f(nfile, (int) n_blocos, primeiro, (int) tam_ultimo_bloco, comp, tamanhos_rle, tempo_exec, 0);

    free(teste_compressao);
}
