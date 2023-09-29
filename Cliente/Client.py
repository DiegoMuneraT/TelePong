import turtle
import time
#import threading
# Importamos el módulo de protocolo
from Protocol import myprotocol as myp

"""
Variables de estado

0: Estado del juego
1: Cliente A
2: Cliente B
3: Posición de la raqueta A
4: Posición de la raqueta B
5: Posición de la bola en X
6: Posicion de la bola en Y
7: Puntaje A
8: Puntaje B

"""
estado = myp.REQUEST("GET", "STATE")
print("Estado grande: ", estado)

# Dibujamos la pantalla
wn = turtle.Screen()
wn.title("Pong")
wn.bgcolor("black")
wn.setup(width=800, height=600)
wn.tracer(0)

# Puntaje
score_a = 0
score_b = 0

# Raqueta A
paddle_a = turtle.Turtle()
paddle_a.speed(0)
paddle_a.shape("square")
paddle_a.color("white")
paddle_a.shapesize(stretch_wid=5,stretch_len=1)
paddle_a.penup()
paddle_a.goto(-350, 0)

# Raqueta B
paddle_b = turtle.Turtle()
paddle_b.speed(0)
paddle_b.shape("square")
paddle_b.color("white")
paddle_b.shapesize(stretch_wid=5,stretch_len=1)
paddle_b.penup()
paddle_b.goto(350, 0)

# Bola
ball = turtle.Turtle()
ball.speed(0)
ball.shape("square")
ball.color("white")
ball.penup()
ball.goto(0, 0)
ball.dx = 2
ball.dy = 2

# Puntaje
pen = turtle.Turtle()
pen.speed(0)
pen.shape("square")
pen.color("white")
pen.penup()
pen.hideturtle()
pen.goto(0, 260)
pen.write("Cliente A: 0  Cliente B: 0", align="center", font=("Courier", 24, "normal"))

# Movimiento de las raquetas
def paddle_up():

    if estado[1] == myp.REQUEST('GET', 'PORT'):
        y = paddle_a.ycor()
        y += 20
        # Se envia el mensaje al servidor con la cabecera (PM: Paddle Move) y la PDU (String con la posición de la raqueta)
        myp.REQUEST("PADDLE", str(y))
        paddle_a.sety(y)

    elif estado[2] == myp.REQUEST('GET', 'PORT'):
        y = paddle_b.ycor()
        y += 20
        # Se envia el mensaje al servidor con la cabecera (PM: Paddle Move) y la PDU (String con la posición de la raqueta)
        myp.REQUEST("PADDLE", str(y))
        paddle_b.sety(y)


def paddle_down():
    
    if estado[1] == myp.REQUEST('GET', 'PORT'):
        y = paddle_a.ycor()
        y -= 20
        # Se envia el mensaje al servidor con la cabecera (PM: Paddle Move) y la PDU (String con la posición de la raqueta)
        myp.REQUEST("PADDLE", str(y))
        paddle_a.sety(y)
        

    elif estado[2] == myp.REQUEST('GET', 'PORT'):
        y = paddle_b.ycor()
        y -= 20
        # Se envia el mensaje al servidor con la cabecera (PM: Paddle Move ) y la PDU (String con la posición de la raqueta)
        myp.REQUEST("PADDLE", str(y))
        paddle_b.sety(y)

# Actualizar la posición de las raquetas
def update_paddle():
    estado = myp.REQUEST("GET", "STATE")
    print(estado)

    if estado[1] == myp.REQUEST('GET', 'PORT') and paddle_b.ycor() != int(estado[4]):
        paddle_b.sety(int(estado[4]))

    if estado[2] == myp.REQUEST('GET', 'PORT') and paddle_a.ycor() != int(estado[3]):
        paddle_a.sety(int(estado[3]))

def update_score():
    estado = myp.REQUEST("GET", "STATE")

    if estado[1] == myp.REQUEST('GET', 'PORT') and score_a != int(estado[7]):
        score_a = int(estado[7])

    if estado[2] == myp.REQUEST('GET', 'PORT') and score_b != int(estado[8]):
        score_b = int(estado[8])
    
myp.thread.start()

myp.REQUEST("PLAYER JOIN", "NULL")

while estado[1] is None or estado[2] == "0000":
    estado = myp.REQUEST("GET", "STATE")
    continue

# Entrada de teclado
wn.listen()
# Con cada entrada de teclado se envía un mensaje al servidor actualizando la posición de la raqueta
wn.onkeypress(paddle_up, "w")
wn.onkeypress(paddle_down, "s")

# Loop principal del juego
while score_a < 10 and score_b < 10:
    #print(estado)

    wn.update()    

    update_paddle()
    
    # Movimiento de la bolax
    ball.setx(ball.xcor() + ball.dx)
    ball.sety(ball.ycor() + ball.dy)

    # Bordes

    # Arriba y abajo
    if ball.ycor() > 290:
        ball.sety(290)
        # Esto hace que la bola rebote
        ball.dy *= -1

        #os.system("afplay bounce.wav&")
    
    elif ball.ycor() < -290:
        ball.sety(-290)
        # Esto hace que la bola rebote
        ball.dy *= -1

    # Izquierda y derecha
    if ball.xcor() > 350:        
        score_a += 1

        if estado[1] == myp.REQUEST('GET', 'PORT'):
            myp.REQUEST("SCORE", str(score_a))

        pen.clear()
        pen.write("Cliente A: {}  Cliente B: {}".format(score_a, score_b), align="center", font=("Courier", 24, "normal"))
        ball.goto(0, 0)
        ball.dx *= -1

    elif ball.xcor() < -350:
        score_b += 1

        if estado[2] == myp.REQUEST('GET', 'PORT'):
            myp.REQUEST("SCORE", str(score_b))

        pen.clear()
        pen.write("Cliente A: {}  Cliente B: {}".format(score_a, score_b), align="center", font=("Courier", 24, "normal"))
        ball.goto(0, 0)
        ball.dx *= -1

    # Colisiones con las raquetas
    if ball.xcor() < -340 and ball.ycor() < paddle_a.ycor() + 50 and ball.ycor() > paddle_a.ycor() - 50:
        ball.dx *= -1 
    
    elif ball.xcor() > 340 and ball.ycor() < paddle_b.ycor() + 50 and ball.ycor() > paddle_b.ycor() - 50:
        ball.dx *= -1

    time.sleep(0.01)

# Close the socket and wait for the receive thread to finish
myp.client_socket.close()
myp.thread.join()