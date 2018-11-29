cc=gcc -c -Wall -g
main : commandes.o main.o commande.o input_output.o execute_commande.o
	gcc -g -Wall -o main main.o commandes.o commande.o input_output.o execute_commande.o
	
input_output.o : ./commandes/input_output.c commande.o commandes.o execute_commande.o
	$(cc) ./commandes/input_output.c

commandes.o : ./commandes/commandes.c commande.o
	$(cc) ./commandes/commandes.c

commande.o : ./commandes/commande.c execute_commande.o
	$(cc) ./commandes/commande.c

execute_commande.o : ./commandes/execute_commande.c
	$(cc) ./commandes/execute_commande.c

main.o : main.c 
	$(cc) main.c

clean :
	rm -f *.o main
