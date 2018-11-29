#include "commandes.h"
/*Cette fonction prend en paramètre un pointeur sur
la ligne de commande et vérifie l'existence de ';'
pour séparer les différentes commandes saisies*/
Commandes* Analyse_Commandes(char *ligne_commande){
    Commandes* cmds = (Commandes*)malloc(sizeof(Commandes));
    strcpy(cmds->lesCommandes, ligne_commande);
    char delimiter[] = ";";
    int i=0;
    Commande *firstCmd=(Commande*)malloc(sizeof(Commande));
    firstCmd->commande = strtok(ligne_commande, delimiter);
    cmds->pointeursCommandes[i] = firstCmd;
    cmds->nbCommandes++;
    bool nextCommandValid = true;
    for (i = 1; nextCommandValid!=false && i < MAXNBCOMMAND ; i++){
        Commande *cmd=(Commande*)malloc(sizeof(Commande));
        cmd->commande = strtok(NULL, delimiter);
        cmds->pointeursCommandes[i] = cmd;
        if (cmds->pointeursCommandes[i]->commande == 0x0)
            nextCommandValid = false;
        else
            cmds->nbCommandes++;
    }
    return cmds;
}
