#include <stdio.h>
#include <string.h>
#include "mod_t.h"
#include "mod_f.h"
#include "mod_c.h"


int main (int argc, char *argv[]) {
    if (argc == 4) {
        //ex.txt -m f
        if (strncmp(argv[3], "f", 1) == 0)
            modulof(argv[1], 65536, 0);
            //ex.txt -m t
        else if (strncmp(argv[3], "t", 1) == 0)
            modulot(argv[1]);
            //ex.txt -m c
        else if (strncmp(argv[3], "c", 1) == 0)
            moduloC(argv[1]);
            //ex.txt -m d
        else if (strncmp(argv[3], "d", 1) == 0)
            printf("Modulo D\n");
    }
    else if (argc == 6){
        if (strncmp(argv[3], "f", 1) == 0){
            //ex.txt -m f -c r
            if (strncmp(argv[4], "-c", 2) == 0 && strncmp(argv[5], "r", 1) == 0)
                modulof(argv[1], 65536, 1);

            else if (strncmp(argv[4], "-b", 2) == 0) {
                //ex.txt -m f -b K
                if (strncmp(argv[5], "K", 1) == 0)
                    modulof(argv[1], 655360, 0);
                //ex.txt -m f -b m
                else if (strncmp(argv[5], "m", 1) == 0)
                    modulof(argv[1], 8388608, 0);
                //ex.txt -m f -b M
                else if (strncmp(argv[5], "M", 1) == 0)
                    modulof(argv[1], 67108864, 0);
            }
        }
        else if (strncmp(argv[3], "d", 1) == 0) {
            if (strncmp(argv[4], "-d", 2) == 0) {
                //ex.shaf -m d -d s
                if (strncmp(argv[5], "s", 1) == 0)
                    printf("Descodificacao SF (Gerar ficheiro RLE a partir de ficheiro shaf)\n");
                //ex.rle -m d -d r
                else if (strncmp(argv[5], "r", 1) == 0)
                    printf("Descodificação RLE (Gerar ficheiro original a partir de ficheiro RLE)\n");
            }
        }
    }

    else if (argc == 8) {
        if (strncmp(argv[3], "f", 1) == 0){
            if (strncmp(argv[4], "-b", 2) == 0) {
                if (strncmp(argv[5], "K", 1) == 0) {
                    //ex.txt -m f -b K -c r
                    if (strncmp(argv[6], "-c", 2) == 0 && strncmp(argv[7], "r", 1) == 0)
                        modulof(argv[1], 655360, 1);
                }
                //ex.txt -m f -b m -c r
                else if (strncmp(argv[5], "m", 1) == 0){
                    if (strncmp(argv[6], "-c", 2) == 0 && strncmp(argv[7], "r", 1) == 0)
                        modulof(argv[1], 8388608, 1);
                }
                //ex.txt -m f -b M -c r
                else if (strncmp (argv[5], "M", 1) == 0) {
                    if (strncmp(argv[6], "-c", 2) == 0 && strncmp(argv[7], "r", 1) == 0)
                        modulof(argv[1], 67108864, 1);
                }
            }
        }
    }
    else
        printf("Comando Invalido\n");
    return 0;
}
