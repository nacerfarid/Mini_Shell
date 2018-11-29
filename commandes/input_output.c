#include "input_output.h"

/*Cette fonction prend en paramètre un pointeur Commande
et vérifie l'existence de '>', '>>','2>' ou '<' pour diriger les 
entrées sorties de la commande suivant les arguments*/
void Analyse_Input_Output(Commande *cmd)
{
    char *command_copy_input = strdup(cmd->commande);
    char *command_copy_output = strdup(cmd->commande);
    strtok(command_copy_input, "<");
    char *input = strtok(NULL, " ");
    strtok(command_copy_output, ">");
    char *output = strtok(NULL, " ");
    if (input != 0x0)
    {
        cmd->commande = replace_string(cmd->commande, input, " ", 0);
        cmd->commande = replace_string(cmd->commande, "<", " ", 0);
        char repertoireCourant[512];
        getcwd(repertoireCourant, 512);
        strcat(repertoireCourant, "/");
        strcat(repertoireCourant, input);
        cmd->input = fopen(repertoireCourant, "r");
    }
    if (output != 0x0)
    {
        cmd->commande = replace_string(cmd->commande, output, " ", 0);
        cmd->commande = replace_string(cmd->commande, ">", " ", 0);
        if (*output == '>'){
            output = strtok(output, ">");
            cmd->outputmod = "a";
            cmd->commande = replace_string(cmd->commande, ">", " ", 0);
        }
        else{
            cmd->outputmod = "w+";
        }
        char repertoireCourant[512];
        getcwd(repertoireCourant, 512);
        strcat(repertoireCourant, "/");
        strcat(repertoireCourant, output);
        cmd->output = fopen(repertoireCourant, cmd->outputmod);
    }
}
/*Cette fonction prend en paramètre un pointeur sur
une chaine de caractère, une chaine de caractère de recherche 
une chaine de caractère de remplacement si le premier 
est trouvé on le remplace par le deuxième*/
char *replace_string(char *string, char *find, char *replace, int start)
{
  static char temp[4096];
  static char buffer[4096];
  char *p;
  strcpy(temp, string + start);

  if(!(p = strstr(temp, find)))
    return temp;
  strncpy(buffer, temp, p-temp);
  buffer[p-temp] = '\0';
  sprintf(buffer + (p - temp), "%s%s", replace, p + strlen(find));
  sprintf(string + start, "%s", buffer);    
  return string;
}