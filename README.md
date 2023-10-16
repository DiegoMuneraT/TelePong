# Proyecto #1 - Telemática - Universidad EAFIT 2023-2

## TelePong- Introducción

El propósito de este proyecto es el desarrollo de habilidades para la programación en red,
aplicaciones concurrentes, así como el diseño e implementación de protocolos.

En el contexto de este curso, un protocolo se entiende como el conjunto de normas, reglas o
parámetros que rigen el intercambio coordinado de mensajes entre dos entidades paritarias. 

Los protocolos son fundamentales para la comunicación entre las aplicaciones que se ejecutan o corren en una red de datos.

Para lograr los objetivos propuestos, se va a emplear la API de Sockets Berkeley para escribir una
aplicación servidor que reciba peticiones desde una aplicación cliente.

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

En donde `PORT` y `LogFile` son el puerto en el que se quiere ejecutar el servidor y el nombre de archivo en donde se guardarán los logs del servidor respectivamente. 

### Uso del cliente

El cliente en Python le permite jugar al juego de Pong. Para ejecutar el cliente, siga estos pasos:

1. Abra una terminal en su sistema
2. Dirigase al directorio donde tiene descargado TelePong y ejecute el comando `cd Telepong/Cliente/`
3. Ejecute el código en el siguiente comando: `python Client.py` o `python3 Client.py`

El cliente le permite controlar una raqueta y jugar al juego contra otro jugador en el servidor. 

**Nota:** El juego no empezará hasta que se encuentre un juego libre y se conecte otro cliente al mismo juego.

**Nota:** Es importante que se consideren las direcciones IP y el puerto al ejecutar el servidor y el cliente localmente o en la nube. Dado el caso se deben cambiar manualmente las constantes en el archivo `constants.py` en la ruta `Telepong/Cliente/`

### Arquitectura

El sistema del juego consta de dos componentes principales: el servidor implementado en C y el cliente desarrollado en Python. La arquitectura se basa en el modelo cliente-servidor, donde el servidor gestiona las instancias de juego y se comunica con los clientes para mantener la sincronización del juego.

--imagen--

#### Servidor

- Hilos: El servidor utiliza hilos POSIX para gestionar multiples instancias de juego de forma concurrente. Cada instancia de juego se ejecuta en un hilo independiente, lo que permite que varios juegos se desarrollen simultáneamente.

- Comunicación: La comunicación entre el servidor y los clientes se realiza a través de sockets UDP. Este enfoque nos proporciona una comunicación de baja latencia y es adecuado para este juego en tiempo real.

- Estado del juego: El servidor controla el estado del juego, incluyendo el movimiento de la bola, las raquetas y el seguimiento de las puntuaciones. Además, el servidor coordina la interacción de los clientes en el juego y garantiza que las acciones de un jugador se reflejen en el juego del otro.

#### Cliente

- Interfaz gráfica: El cliente en Python utiliza el módulo `turtle` para crear una interfaz gráfica simple en la que los jugadores controlan las raquetas y ven el estado del juego.

- Comunicación con el servidor: El cliente se comunica con el servidor a través de un protocolo personalizado implementado en el módulo `myprotocol`. Esto permite que el cliente envie y reciba información en el juego, así como actualizaciones sobre el estado del juego.

- Interacción del jugador: Los jugadores pueden controlar las raquetas a través de las teclas `W` y `S` en el teclado. Cada acción del jugador se refleja en el juego a través de la comunicación con el servidor.

- Puntuación y estado del juego: El cliente muestra la puntuación actual y el estado del juego en la interfaz gráfica, permitiendo a los jugadores seguir el progreso del juego.

### Protocolo

#### Protocolo del servidor

El archivo `myprotocol.h` define funciones y estructuras que ayudan a establecer un protocolo de comunicación personalizado entre el servidor y el cliente. 

Las funciones en este modulo implementan las siguientes cabeceras de protocolo:

- `GET` utilzado para obtener un mensaje del cliente nuevo 
- `SND` utilizado para enviar información a los clientes
- `MSG` utilizado para recibir los mensajes de los clientes con juego asignado
- `UPD` utilizado para actualizar el juego a los clientes con juego asignado

#### Protocolo del cliente

- `PLJ` Player Join utilizado para indicar que un jugador entró
- `PM` Paddle utilizado para indicar el movimiento de una raqueta
- `BX` Ball X utilizado para indicar el movimiento de la bola en el eje X
- `BY` Ball Y utilizado para indicar el movimiento de la bola en el eje Y
- `SA` Score A utilizado para indicar el cambio en el puntaje del player A
- `SB` Score B utilizado para indicar el cambio en el puntaje del player B

## Conclusiones

El proyecto fue una buen reto para nosotros ya que combina de manera efectiva dos componentes fundamentales vistos en el curso: un servidor en C y un cliente en Python, para crear una experiencia de juego de Pong multijugador. La arquitectura client-servidor, junto con la implementación de hilos en el servidor, permite gestionar múltiples instancias de juego de forma concurrente y brinda a los jugadores la oportunidad de competir en tiempo real.

La creación de un protocolo de comunicación personalizado demuestra una comprensión sólida de los desafíos asociados con la sincronización de juegos en tiempo real. Este protocolo facilita la transmisión de información relevante, como el estado del juego, las acciones de los jugadores y las puntuaciones, lo que contribuye a una experiencia de juego fluida y sincronizada.

El enfoque en la escalabilidad y personalización del servidor en C permite que el proyecto se adapte a diferentes necesidades y reglas del juego. Además, la comunicación eficiente entre el servidor y el cliente se basa en mensajes definidos en el protocolo personalizado, lo que garantiza una interacción precisa y eficaz.

En conjunto, este proyecto demuestra habilidades en programación en C, implementación de hilos, desarrollo de protocolos de comunicación y programación en Python. La combinación de estos elementos crea una experiencia de juego dinámica y ofrece una base sólida para el desarrollo de aplicaciones multijugador en tiempo real. Este proyecto no solo proporciona una implementación funcional, sino que también refleja un profundo entendimiento de los principios de programación y la arquitectura de sistemas, lo que lo convierte en un logro significativo en el ámbito del desarrollo de servidores y aplicaciones distribuidas.


## Referencias

- https://beej.us/guide/bgc/
- https://beej.us/guide/bgnet/
- https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
- https://www.ibm.com/docs/es/aix/7.3?topic=protocols-socket-types
- https://www.it.uc3m.es/pbasanta/asng/CES/M2/concurrent_1_es.pdf


