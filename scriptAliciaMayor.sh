#!/bin/bash

function mostrarCodigo(){

	cat practicaIntermediaAliciaMayor.c
}

function compilar(){

	gcc practicaIntermediaAliciaMayor.c -o practicaIntermediaAliciaMayor
	echo Programa compilado
}

function ejecutar(){

	echo Introduce el numero de asistentes de vuelo:
	read asistentes
	if  test $asistentes -lt 1
	then
		echo El número de asistentes no puede ser menor de 1.
	else
		./practicaIntermediaAliciaMayor $asistentes
	fi
}

while true

echo " --------------------------------------"
echo " Escribe una opción del 1 al 4, siendo: "
echo " 1) Mostrar el código del programa. "
echo " 2) Compilar el programa. "
echo " 3) Ejecutar el programa. "
echo " 4) Salir del script. "

read input

do
	case $input in

		1) mostrarCodigo;;
	
		2) compilar;;

		3) compilar
		   ejecutar;;

		4) echo Has salido del script
		   exit 0;;

		*) echo El numero introducido tiene que ser entre 1 y 4
	esac
done