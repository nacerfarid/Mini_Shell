#include "execute_commande.h"
/*Cette fonction prend en paramètre un pointeur Commandes
et compte le nombre de pipes et selon le résultat on fait
appel soit à une analayse avec ou sans tubes (si aucune 
pipe on appelle analyse_without_pipe et inversement)
*/
void Execute_Commandes(Commandes *cmds)
{
    int i = 0;
    int nbPipes = 0;
    for (i = 0; i < cmds->nbCommandes; i++)
    {
        	if(cmds->pointeursCommandes[i] != 0x0 && cmds->pointeursCommandes[i]->commande != 0x0){
	nbPipes = Count_Pipes(cmds->pointeursCommandes[i]->commande);
	
	if (nbPipes != 0)
	    Analyse_With_Pipes(cmds->pointeursCommandes[i], nbPipes);
	else
	    Analyse_Without_Pipes(cmds->pointeursCommandes[i]);
    }
    }
}
/*Cette fonction prend en paramètre un pointeur Commande
elle crée un fils et exécute la commande en fore-ground
si la variable booléenne est false (et inversement)
et selon les cas on duplique sur stdin ou stdout 
sur les E/S analysées*/
void Execute_Without_Pipes(Commande *cmd)
{
    pid = fork();
    if (pid == 0)
    {
	if (cmd->input != NULL)
	{
	    dup2(fileno(cmd->input), STDIN_FILENO);
	    close(fileno(cmd->input));
	}
	if (cmd->output != NULL)
	{
	    dup2(fileno(cmd->output), STDOUT_FILENO);
	    close(fileno(cmd->output));
	}
	execvp(cmd->arg[0], cmd->arg);
	exit(EXIT_FAILURE);
    }
    if (cmd->background == false)
    {
	waitpid(pid, NULL, 0);
	pid = 0;
    }
    else
    {
	printf("Processus créer avec le PID: %d\n", pid);
	pid = 0;
    }
}
/*Cette fonction prend en paramètre un pointeur CommandePipe
elle exécute une commande tubé quelque soit le nombre de tubes
grâce à deux descripteurs de fichiers et suivant les tests
sur la parité et le nombre de parité*/

void Execute_With_Pipes(CommandePipe *cmds)
{
	int i;
	int filedes[2]; // Descripteur de fichiers
	int filedes2[2];
	//nombre de commande = nombre de tubes + 1
	for (i = 0; i <= cmds->nbPipe; i++)
	{
		//Selon que nous sommes dans une itération ou d'une autre, 
		//nous allons définir des descripteurs pour les entrées de 
		//tubes et de sortie. De cette façon, un tube sera partagé 
		//entre deux itérations, nous permettant de connecter les 
		//entrées et les sorties des deux commandes différentes.
	    if (i % 2 != 0)
	    {
			pipe(filedes); //pour les i impairs
	    }
	    else
	    {
			pipe(filedes2); //pour les i pairs
	    }
	    pid = fork();
	    if (pid == -1)
	    {
			if (i != cmds->nbPipe)
			{
			    if (i % 2 != 0)
			    {
					close(filedes[1]); //pour les i impairs
				}else{
					close(filedes2[1]); //pour les i pairs
				} 
			}			
			printf("Error fork.\n");
			return;
		}
		if(pid==0){
			//Si on est à la première commande
			if (i == 0){
			dup2(filedes2[1], STDOUT_FILENO);
			}
			//Si nous sommes dans la dernière commande, 
			//selon qu'il est placé dans une position paire 
			//ou impaire, nous remplaçons l'entrée standard 
			//pour une conduite ou d'une autre. La sortie 
			//standard sera intacte parce que nous voulons 
			//voir la sortie dans le terminal
			else if (i == cmds->nbPipe){
				if ((cmds->nbPipe+1) % 2 != 0){ // pour un nombre impair de commandes
					dup2(filedes[0],STDIN_FILENO); 
				}else{ // pour un nombre pair de commandes
					dup2(filedes2[0],STDIN_FILENO);
				}
				
			//Si nous sommes dans une commande qui est au milieu, 
			//nous devrons utiliser deux tubes, l'un pour l'entrée 
			//et l'autre pour la sortie. La position est également 
			//important afin de choisir quel descripteur de fichier 
			//correspond à chaque entrée/sortie
			}else{
				if (i % 2 != 0){ //pour les i impairs
					dup2(filedes2[0],STDIN_FILENO); 
					dup2(filedes[1],STDOUT_FILENO);
				}else{ //pour les i pairs
					dup2(filedes[0],STDIN_FILENO); 
					dup2(filedes2[1],STDOUT_FILENO);					
				} 
			}

			if (execvp(cmds->pointeursCommandesPipe[i]->arg[0],cmds->pointeursCommandesPipe[i]->arg)==-1){
				kill(getpid(),SIGTERM);
			}		
		} else{
			//Fermeture des descripteurs dans le père
			if (i == 0){
				close(filedes2[1]);
			}
			else if (i == cmds->nbPipe){
				if ((cmds->nbPipe+1) % 2 != 0){					
					close(filedes[0]);
				}else{					
					close(filedes2[0]);
				}
				}else{
					if (i % 2 != 0){					
						close(filedes2[0]);
						close(filedes[1]);
					}else{					
						close(filedes[0]);
					close(filedes2[1]);
					}
				}
		}
		waitpid(pid,NULL,0);
	}
}
