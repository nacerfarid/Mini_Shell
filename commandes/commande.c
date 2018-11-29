#include "commande.h"
/*Cette fonction prend en paramètre un double
pointeur sur la variable et affiche sa valeur
si elle existe, NULL sinon, et si pas de dollar
elle réecrit l'argument dans le terminal*/
void echoVar(char **variables){
    int i;
    for(i = 1; variables[i] != NULL; i++){
            char *str = variables[i];
            if(*str == '$'){
                    variables[i] = getenv(str + 1);
            }
        }
    for (i = 2; variables[i] != NULL; i++) {
	    printf("%s\n ", variables[i]);
	}
}
/*Cette fonction prend en paramètre un pointeur
variable et un pointeur valeur et selon les 
tests on affecte la valeur à cette variable*/
void setEnvironnement(char* variable,char* valeur){
		if(getenv(variable) != NULL){
			printf("%s", "La variable est modifiee\n");
		}else{
			printf("%s", "La variable est cree\n");
		}
		if (valeur == NULL){
			setenv(variable, "", 1);
		}else{
			setenv(variable, valeur, 1);
		}
}
/*Cette fonction prend en argument un pointeur
vers chemin de dossier et selon la valeur on se
déplace vers ce dossier ou message d'erreur sinon*/
void changerRepertoire(char* directory){
    if (directory == NULL)
        chdir(getenv("HOME"));
    else{
        if (chdir(directory) == -1){
            char error[100];
            snprintf(error, sizeof error, "%s", directory);
            perror(error);
        }
    }
}
/*Cette fonction prend en argument un nom de variable
d'environnement et affiche la valeur de cette variable*/
void getEnvironnement(char *variable){
    if(getenv(variable) == NULL){
			printf("\n");
		} else{
			printf("%s\n",getenv(variable));
		}
}
/*Cette fonction prend en argument un nom de variable
d'environnement et efface cette variable*/
void unsetEnvironnement(char *variable){
       
		if(getenv(variable) != NULL){
			unsetenv(variable);
			printf("%s", "La variable est supprimee\n");
		}else{
			printf("%s", "La variable n'existe pas\n");
		}
}
/*Cette fonction prend en argument un pointeur
vers une chaine de caractère, calcule le
nombre de '|' dans cette chaine et le renvoie*/
int Count_Pipes(char *sstring)
{
    int i;
    int nbocc = 0;
    int strlength = strlen(sstring);
    for (i = 0; i < strlength; i++)
    {
        if (sstring[i] == '|')
            nbocc++;
    }
    return nbocc;
}
/*Cette fonction prend en paramètre un pointeur Commande
et un entier qui vaut le nombre de pipes dans la commande
elle fait une analyse suivant les délimiteurs et récupère
les arguments et les stockent avant de les envoyer à la 
fonction d'execution avec pipes*/
void Analyse_With_Pipes(Commande *cmd, int nbPipes)
{
   	CommandePipe *cmds = (CommandePipe *)malloc(sizeof(CommandePipe));
    cmds->nbPipe = nbPipes;
    char *commands_copy = strdup(cmd->commande);
    char delimiter[] = "|";
    Commande *firstCmd = (Commande *)malloc(sizeof(Commande));
    firstCmd->commande = strtok(commands_copy, delimiter);
    cmds->pointeursCommandesPipe[0] = firstCmd;
    int i;
    bool nextCommandValid = true;
    for (i = 1; i < MAXNBPIPE && nextCommandValid != false; i++)
    {
        Commande *currentCommand = (Commande *)malloc(sizeof(Commande));
        currentCommand->commande = strtok(NULL, delimiter);
        if (currentCommand->commande == 0x0)
            nextCommandValid = false;
        else
        {
            cmds->pointeursCommandesPipe[i] = currentCommand;
        }
    }
    for (i = 0; i <= cmds->nbPipe;i++){
    	if(cmds->pointeursCommandesPipe[i] != 0x0 && cmds->pointeursCommandesPipe[i]->commande != 0x0){
			Analyse_Args(cmds->pointeursCommandesPipe[i]);
		}
    }
    Execute_With_Pipes(cmds);
}
/*Cette fonction prend en paramètre un pointeur Commande
et analyse les arguments avec le délimiteur espace*/
void Analyse_Args(Commande* commande){
	char *command_copy = strdup(commande->commande);
    char delimiter[] = " ";
    commande->arg = malloc(ARGLENGTH * sizeof(char**));
    commande->arg[0] = strtok(command_copy, delimiter);
    int i;
    bool nextArgValid = true;
    for (i = 1; i < ARGLENGTH && nextArgValid!=false; i++) {
        commande->arg[i] = (char*)strtok(NULL, delimiter);
        if (commande->arg[i] == 0x0)  nextArgValid = false;
        else commande->argc++;
    }
}
/*Cette fonction prend en paramètre un pointeur Commande
et effectue trois vérifications avant d'appliquer le 
délimiteur: si la commande est en background ou en 
forground, si elle contient une redirection et les
arguments de cette commande avant de les envoyer à la
fonction d'exécution sans tubes*/
void Analyse_Without_Pipes(Commande *cmd)
{
    Analyse_Background(cmd);
    Analyse_Input_Output(cmd);
    Analyse_Args(cmd);
    if (cmd->arg[0] !=NULL)
    {
    if (strcmp(cmd->arg[0], "cd") == 0)
        changerRepertoire(cmd->arg[1]);
    if(strcmp(cmd->arg[0],"setenv")==0){
		if((cmd->arg[1] == NULL) && cmd->arg[2] == NULL) printf("%s","Trop peu d'arguments\n");
		 else setEnvironnement(cmd->arg[1],cmd->arg[2]);
	}
	if(strcmp(cmd->arg[0],"unsetenv")==0){
         if(cmd->arg[1] == NULL)
			printf("%s","Trop peu d'arguments\n");
		 else
         	unsetEnvironnement(cmd->arg[1]);
	}
	if(strcmp(cmd->arg[0],"getenv")==0 && cmd->arg[1]!=NULL) getEnvironnement(cmd->arg[1]);
    if (strcmp(cmd->arg[0], "echo") == 0) echoVar(cmd->arg);
    Execute_Without_Pipes(cmd);
    }
}
/*Cette fonction prend en paramètre un pointeur Commande
et vérifie l'existence d'un '&' si ça existe on passe 
en background sinon exécution normale*/
void Analyse_Background(Commande* cmd){
    char *ampersand = strchr(cmd->commande, '&');
    if (ampersand != NULL)
    {
        *ampersand= ' ';
        cmd->background = true;
    }
    else
    {
        cmd->background = false;
    }
}
