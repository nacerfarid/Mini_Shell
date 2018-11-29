#include "./structures.h"
void Execute_Commandes(Commandes *);
void Execute_Without_Pipes(Commande *);
void Execute_With_Pipes(CommandePipe *);
int Count_Pipes(char *);
void Analyse_With_Pipes(Commande *, int );
void Analyse_Without_Pipes(Commande *);