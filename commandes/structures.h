#include "variables.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct{
        char* commande;
        int argc;
        FILE* input;
        FILE* output;
        FILE* error;
        char* outputmod;
        bool background;
        char **arg;
} Commande;
typedef struct{
        Commande* pointeursCommandesPipe[MAXNBCOMMANDPIPE];
        int nbPipe;
} CommandePipe;
typedef struct{
	int nbCommandes;
	char lesCommandes[MAXCMDLENGTH] ;
    Commande* pointeursCommandes[MAXNBCOMMAND];
}Commandes;

