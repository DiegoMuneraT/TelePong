# Proyecto #1 - Telemática - Universidad EAFIT 2023-2

# TelePong

## Introducción

El propósito de este proyecto es el desarrollo de habilidades para la programación en red,
aplicaciones concurrentes, así como el diseño e implementación de protocolos.

En el contexto de este curso, un protocolo se entiende como el conjunto de normas, reglas o
parámetros que rigen el intercambio coordinado de mensajes entre dos entidades paritarias. 

Los protocolos son fundamentales para la comunicación entre las aplicaciones que se ejecutan o corren en una red de datos.

Para lograr los objetivos propuestos, se va a emplear la API de Sockets Berkeley para escribir una
aplicación servidor que reciba peticiones desde una aplicación cliente.

----

Este proyecto es un servidor de juego en C que utiliza hilos para manejar múltiples instancias de juego y se comunica a través de sockets UDP con un programa cliente escrito en Python.

## Desarrollo

### Requisitos del sistema

#### Para el cliente

- Python 3.X
- PythonTurtle 0.3.2
- Python Tkinter

#### Para el servidor

- gcc: Se requiere un compilador C para compilar el servidor
- pthread: La biblioteca se utiliza para la gestion de hilos en C

### Compilación del servidor

Para compilar el servidor, siga estos pasos:

1. Abra una terminal en su sistema
2. Dirigase al directorio donde tiene descargado TelePong y ejecute el comando `cd Telepong/Servidor/`
3. Compile el código con el siguiente comando: `gcc server.c -o server -pthread`

**Nota:** La bandera `-pthread` se utiliza para vincular la biblioteca de hilos

### Uso del servidor

Una vez que el servidor se ha compilado con éxito, puedes ejecutarlo con el siguiente comando `./server <PORT> <LogFile>`

En donde PORT y LogFile son el puerto en el que se quiere ejecutar el servidor y el nombre de archivo en donde se guardarán los logs del servidor respectivamente. 

### Uso del cliente

El cliente en Python le permite jugar al juego de Pong. Para ejecutar el cliente, siga estos pasos:

1. Abra una terminal en su sistema
2. Dirigase al directorio donde tiene descargado TelePong y ejecute el comando `cd Telepong/Cliente/`
3. Ejecute el código en el siguiente comando: `python Client.py` o `python3 Client.py`

El cliente le permite controlar una raqueta y jugar al juego contra otro jugador en el servidor. 

**Nota:** El juego no empezará hasta que se encuentre un juego libre y se conecte otro cliente al mismo juego.

**Nota:** Es importante que se consideren las direcciones IP y el puerto al ejecutar el servidor y el cliente localmente o en la nube. Dado el caso se deben cambiar manualmente las constantes en el archivo `constants.py` en la ruta `Telepong/Cliente/`

## Conclusiones

## Referencias