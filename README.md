# Tarea2: Tarea de programacion RA2
Alumnos: Gabriela Carrasco - Bayron Mardones

 //Seccion: 1

 Objetivos:

 -Utilizar diversos mecanismos relacionados con los procesos incluyendo la creacionn y finalizacion de procesos, y los mecanismos de comunicacion.

-Definir e implementar un protocolo para la capa de aplicacion.

-Utilizar el protocolo orientado a la conexion TCP de la capa de transporte.

# Requisitos
-Compilador de C++

-make //Herramienta de apoyo para compilar

# Ejecucion 
Para compilar el proyecto es necesita estar hubicado en el directorio del proyecto y ejecutar "make"

-Para iniciar el servidor se ejecuta:

./servidor [puerto]

Donde el [puerto] es el puerto TCP que escuchara las conexiones.

-Para iniciar el cliente se ejecuta:

./cliente [direccion_ip_servidor][puerto]

# Ejemplo:

./servidor 7777

./cliente 192.168.1.100 7777

make clean 




