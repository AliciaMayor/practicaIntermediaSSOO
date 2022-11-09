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
	int i, j, k, m, ret, estado, overbookingContador;
	int pasajerosTotales = 0;
	int hijos[2];
	int asistentes[*argv[0]];
	int pasajerosPorAsistente[*argv[0]];
	pid_t p, p2, f, f2;
	struct sigaction ss, ss2;

	ss.sa_handler = manejadora;
	sigaction(SIGUSR1, &ss, NULL);

	ss2.sa_handler = manejadora;
	sigaction(SIGUSR2, &ss2, NULL);

	for(i=0; i<2; i++){

		f = fork();

		if(f<0){ // fork()==-1 -> Error

			printf("Se ha producido un error en el fork() para generar los procesos hijos técnico y engargado.");
		}

		if(f==0){ // fork()==0 -> Estamos en los procesos HIJO (Técnico y encargado)

			//switch case i: si es 0 va a ser el hijo tecnico y si es el 1 va a ser el encargado

			switch(i){

				 case 0:   // El primer hijo generado es el proceso técnico

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
					break;

				 case 1:   // El segundo hijo generado es el proceso encargado

					pause(); //El proceso encargado espera por una señal del padre (SIGUSR1)

					//Generamos un aleatorio para ver si hay o no overbooking
					int hayOverbooking = calculaAleatorios(0,1);
					sleep(2); //Duerme 2 segundos

					if(hayOverbooking==0){

						printf("Soy el encargado y no hay overbooking.");
						overbookingContador = 0;
						//int n3kill = kill(getppid(), SIGUSR1); //Señal para proceder al embarque
					}
					else{

						printf("Soy el encargado y hay overbooking.");
						overbookingContador = 10;
						//int n4kill = kill(getppid(), SIGUSR1); //Señal para proceder al embarque
					}
					exit(hayOverbooking);
					break;
			}
			
	//	}
		

		}


		else{ // Estamos en el proceso PADRE

			hijos[i] = f; //Guardamos los pid de los hijos generados en un array		
		}

	} // llave for

	//FUERA DEL FOR: (COORDINADOR)
	//El coordinador manda señales al proceso técnico y al encargado
	int nkill = kill(hijos[0], SIGUSR1);
	int n2kill = kill(hijos[1], SIGUSR1);  

	//Señal SIGUSR1 que el padre envía a los procesos técnico y encargado
	p = wait(&estado);

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
			int n3kill = kill(hijos[1], SIGUSR1);

		}

	}
	//Si venimos del proceso encargado:
	else{

//		//El padre espera la señal para hacer el embarque
//		pause(); 

		//EMBARQUE:

		//Generamos tantos hijos como nos pasan en el argumento del main
		for(j=0; j<*argv[0]; j++){

			f2 = fork();

			if(f2<0){ // fork()==-1 -> Error

				printf("Se ha producido un error en el fork() para generar los asistentes.");
				
			}
			if(f2==0){ 

				pause(); //Los asistentes esperan a la señal que les envía el coordinador

				//Cada asistente tiene un tiempo de embarque de entre 3 y 6 segundos
				int segsEmbarque = calculaAleatorios(3,6);
				sleep(segsEmbarque);

				int numPasajeros = calculaAleatorios(20,30);
				exit(numPasajeros);

			}
			else{ //PADRE: coordinador

				asistentes[j] = f2;
			}
		}

		//Después de crear los asistentes, el coordinador duerme 2 segundos.
		sleep(2);

		//Envía a todos los asistentes una señal
		for(k=0; k<*argv[0]; k++){
			
			int n4kill = kill(asistentes[k], SIGUSR1);

			p2 = wait(&estado); //Recibimos pid del hijo
			pasajerosPorAsistente[k] = WEXITSTATUS(estado); //Guardamos el numero de pasajeros

		}

		//Sumatorio de todos los pasajeros de cada asistente
		for(m=0; m<*argv[0]; m++){

			pasajerosTotales = pasajerosTotales + pasajerosPorAsistente[m];
		}

		//Restamos a los pasajeros totales los necesarios según el overbooking
		pasajerosTotales = pasajerosTotales - overbookingContador;

		printf("El avión tiene %d pasajeros.", pasajerosTotales);

	}

} //llave main


int calculaAleatorios(int min, int max) {

	return rand() % (max-min+1) + min;
}

void manejadora(int s){


}   
