import turtle
#import os

# Se guarda el estado de las variables
# 0: Estado del Juego, 1: Posición de la raqueta A, 2: Posición de la raqueta B, 3: Posición de la bola en X, 4: Posicion de la bola en Y, 5: Puntaje A, 6: Puntaje B
estado = [0, 0, 0, 0, 0, 0, 0, 0, 0]

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
def paddle_a_up():
    y = paddle_a.ycor()
    y += 20
    paddle_a.sety(y)

def paddle_a_down():
    y = paddle_a.ycor()
    y -= 20
    paddle_a.sety(y)

def paddle_b_up():
    y = paddle_b.ycor()
    y += 20
    paddle_b.sety(y)

def paddle_b_down():
    y = paddle_b.ycor()
    y -= 20
    paddle_b.sety(y)

# Entrada de teclado
wn.listen()
# Con cada entrada de teclado se envía un mensaje al servidor actualizando la posición de la raqueta
wn.onkeypress(paddle_a_up, "w")
wn.onkeypress(paddle_a_down, "s")
wn.onkeypress(paddle_b_up, "Up")
wn.onkeypress(paddle_b_down, "Down")

# Loop principal del juego
while True:
    wn.update()
    
    # Movimiento de la bola
    ball.setx(ball.xcor() + ball.dx)
    ball.sety(ball.ycor() + ball.dy)

    # Bordes

    # Arriba y abajo
    if ball.ycor() > 290:
        ball.sety(290)
        ball.dy *= -1
        #os.system("afplay bounce.wav&")
    
    elif ball.ycor() < -290:
        ball.sety(-290)
        ball.dy *= -1
        #os.system("afplay bounce.wav&")

    # Izquierda y derecha
    if ball.xcor() > 350:
        score_a += 1
        pen.clear()
        # Esto es lo que se envía al servidor
        pen.write("Cliente A: {}  Cliente B: {}".format(score_a, score_b), align="center", font=("Courier", 24, "normal"))
        ball.goto(0, 0)
        ball.dx *= -1

    elif ball.xcor() < -350:
        score_b += 1
        pen.clear()
        # Esto es lo que se envía al servidor
        pen.write("Cliente A: {}  Cliente B: {}".format(score_a, score_b), align="center", font=("Courier", 24, "normal"))
        ball.goto(0, 0)
        ball.dx *= -1

    # Colisiones con las raquetas
    if ball.xcor() < -340 and ball.ycor() < paddle_a.ycor() + 50 and ball.ycor() > paddle_a.ycor() - 50:
        # Esto es lo que se envía al servidor
        ball.dx *= -1 
        #os.system("afplay bounce.wav&")
    
    elif ball.xcor() > 340 and ball.ycor() < paddle_b.ycor() + 50 and ball.ycor() > paddle_b.ycor() - 50:
        # Esto es lo que se envía al servidor
        ball.dx *= -1
        #os.system("afplay bounce.wav&")
    