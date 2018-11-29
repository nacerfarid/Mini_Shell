#include "structures.h"
void echoVar(char **);
void setEnvironnement(char* ,char*);
void changerRepertoire(char* );
void getEnvironnement(char *);
void unsetEnvironnement(char *variable);
void Execute_Commandes(Commandes *);
void Execute_With_Pipes(CommandePipe *);
void Analyse_Args(Commande*);
void Execute_Without_Pipes(Commande *);
void Analyse_Background(Commande* );
void Analyse_Input_Output(Commande*);