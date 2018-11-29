#include "./main.h"
pid_t pid=0;
void sig_handler(int signo)
{
  if(pid == 0) exit(signo);
  else kill(pid,signo);
}
int main(void)
{ 
    signal(SIGINT,sig_handler);
    signal(SIGTERM,sig_handler);
    char ligneCmd[MAXCMDLENGTH]="";
    printf("Bienvenu dans notre mini shell !\n");
    while (1)
    {
        char hostn[1204] = "";
		gethostname(hostn, sizeof(hostn)); 
		char cwd[1024];
        getcwd(cwd, sizeof(cwd));      
		printf("%s@%s %s > ", getenv("LOGNAME"), hostn, cwd);
        fgets(ligneCmd, MAXCMDLENGTH, stdin);
        if (strcmp(ligneCmd, "exit\n") == 0)
        {
            exit(0);
		}
        if ((strlen(ligneCmd)>1) && (ligneCmd[strlen (ligneCmd) - 1] == '\n') && (strcmp(ligneCmd,"\n")!=0)){
            ligneCmd[strlen (ligneCmd) - 1] = '\0';				
			Commandes* commands = Analyse_Commandes(ligneCmd);
        	Execute_Commandes(commands);
		}
        else
            printf("Erreur! recommancez svp !\n");
    }

    return 0;
}
