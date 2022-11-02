#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int calculaAleatorios(int min, int max);
void manejadora(int s);

int main(int argc, char *argv[]){

	srand (time(NULL));

	//El proceso coordinador (PADRE) crea los procesos técnico y encargado.
	int i, j, ret, estado;
	int hijos[2];
	pid_t p = wait(&estado);
	pid_t f;
	struct sigaction ss;

	ss.sa_handler = manejadora;
	sigaction(SIGUSR1, &ss, NULL);

	for(i=0; i<2; i++){

		f = fork();

		if(f<0){ // fork()==-1 -> Error

			printf("Se ha producido un error en el fork() para generar los procesos hijos técnico y engargado.");
		}

		if(f==0){ // fork()==0 -> Estamos en los procesos HIJO (Técnico y encargado)

			if(p==hijos[0]){ // El primer hijo generado es el proceso técnico

				pause(); //El proceso técnico espera por una señal del padre (SIGUSR1)
				printf("Soy el proceso técnico y he recibido la señal del proceso coordinador.");

				//Generamos número aleatorio para que el proceso técnico duerma.
				int segsDormir = calculaAleatorios(3,6);
				sleep(segsDormir);

				//Generamos un número aleatorio entre 0 y 1 para que devuelva el proceso técnico.
				int vueloViable = calculaAleatorios(0,1);

				if(vueloViable==0){

					printf("Soy el técnico y el vuelo no es viable.");
				}
				else{

					printf("Soy el técnico y el vuelo es viable.");
				}
				exit(vueloViable);
			}
	//	}

			else{ // El segundo hijo generado es el proceso encargado

				pause(); //El proceso encargado espera por una señal del padre (SIGUSR1)

				//Generamos un aleatorio para ver si hay o no overbooking
				int hayOverbooking = calculaAleatorios(0,1);
				sleep(2); //Duerme 2 segundos

				if(hayOverbooking==0){

					printf("Soy el encargado y no hay overbooking.");
					int n3kill = kill(getppid(), SIGUSR1); //Señal para proceder al embarque
				}
				else{

					printf("Soy el encargado y hay overbooking.");
					int n4kill = kill(getppid(), SIGUSR1); //Señal para proceder al embarque
				}
			}

		}


		else{ // Estamos en el proceso PADRE (Coordinador)

			hijos[i] = f;
			int nkill = kill(hijos[i], SIGUSR1); //Señal SIGUSR1 que el padre envía a los procesos técnico y encargado

			//Si venimos del proceso técnico: (p (pid del proceso hijo del que se viene) == hijos[0] (pid del primer hijo generado))
			if(p == hijos[0]){

				//Si el valor recibido es 0, el vuelo no es viable: matamos a los procesos técnico y encargado.
				if(WEXITSTATUS(estado) == 0){

					kill(hijos[0], SIGTERM); //Matamos al proceso técnico
					kill(hijos[1], SIGTERM); //Matamos al proceso encargado

					printf("Finaliza el programa ya que el vuelo no es viable.");
					exit(-1); //Finalizamos el programa ???????????????????????????????????''
				}
				//Si el valor recibido es 1, el vuelo es viable: 
				else{

					//Si el vuelo es viable, enviamos señal al proceso encargado.
					int n2kill = kill(hijos[1], SIGUSR1);

				}

			}
			//Si venimos del proceso encargado:
			else{

				//El padre espera la señal para hacer el embarque
				pause(); 

				//EMBARQUE:

				//Generamos tantos hijos como nos pasan en el argumento del main
				for(j=0; j<*argv[0]; j++){


				}
			}
			

		}

	}
}


int calculaAleatorios(int min, int max) {

	return rand() % (max-min+1) + min;
}

void manejadora(int s){


}   