#ifndef MOD_F_H_INCLUDED
#define MOD_F_H_INCLUDED

#define FSIZE_DEFAULT_BLOCK_SIZE 524288         // Default block size = 512 KBytes
#define FSIZE_MIN_BLOCK_SIZE 512                // Min block size = 512 Bytes
#define FSIZE_MAX_BLOCK_SIZE 67108864           // Max block size = 64 MBytes
#define FSIZE_MAX_NUMBER_OF_BLOCKS 4294967296   // Max number of blocks that can be returned = 2^32 blocks
#define FSIZE_MAX_SIZE_FSEEK 2147483648         // Max size permitted in fseek/ftell functions = 2 GBytes
#define FSIZE_ERROR_BLOCK_SIZE -1               // Error: Block size is larger than max value
#define FSIZE_ERROR_NUMBER_OF_BLOCKS -2         // Error: Number of Blocks exceeds max value permitted
#define FSIZE_ERROR_IN_FILE -3                  // Error: Opening or reading file
#define FSIZE_ERROR_IN_FTELL -1L                // Error: When using ftell()
#define BLOCK_SIZE 256
#define BUF_SIZE 1024

typedef struct {
    int rle;                //"1" caso tenha sido feita compressao OU "0" caso não tenha sido feita
    int freq;               //valor da frequência em que aparece o símbolo em causa
    unsigned char item;     //símbolo em causa
    int final;              //"1" caso seja o último bloco do ficheiro OU "0" caso não seja
} RLE;


void modinfo_f(char *nfile, int nr_blocos, int tamanho, int ultimo_bloco, float taxa_compressao, int tamanhos_rle[], double T_exec, int rle);

void gera_novo_nome_rle (char *nfile, char novo[]);

void gera_novo_nome_freq (char *nfile, char *novo, int rle);

long long fsize(FILE *fp_in, unsigned char *filename, unsigned long *the_block_size, long *size_of_last_block);

int check_seguidas (char *bloco, int tamanho, int i);

void compressaoRLE (char *bloco, int tamanho, RLE novo[]);

float check_compressao (char *nfile, RLE comp [], int tamanho);

int get_size (RLE array[]);

int get_size_data (RLE array[]);

void conta_frequencias (char *nfile, int freqs[][BLOCK_SIZE], int nr_blocos, int tamanho_primeiro, int tamanho_ultimo, int tamanho_total);

void reset_RLE (RLE comp[], int tamanho);

void compressao (char *nfile, int tamanho_total, char *novo_nome, int tamanho_bloco, int nr_blocos, int freqs[][BLOCK_SIZE], int tamanho_ultimo,
    int tamanhos[]);

void conta_frequenciasRLE (int freqs[][BLOCK_SIZE], int bloco, RLE comp[]);

void escreveRLE (FILE *fp, RLE comp[], int tamanho);

void save_freqs(char *novo_nome, int freqs [][BLOCK_SIZE], int tamanhos[], int rle, int nr_blocos);

void store_tamanhos(int primeiro, int ultimo, int tamanhos[], int blocos);

void store_tamanhosRLE (int tamanhos[], int i, int size);

void modulof (char *nfile, int tamanho_bloco, int comp_forcada);


#endif // MOD_F_H_INCLUDED
