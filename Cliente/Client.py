import turtle
import time

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
paddle_a.color("red")
paddle_a.shapesize(stretch_wid=5, stretch_len=1)
paddle_a.penup()
paddle_a.goto(-350, 0)

# Raqueta B
paddle_b = turtle.Turtle()
paddle_b.speed(0)
paddle_b.shape("square")
paddle_b.color("blue")
paddle_b.shapesize(stretch_wid=5, stretch_len=1)
paddle_b.penup()
paddle_b.goto(350, 0)

# Bola
ball = turtle.Turtle()
ball.speed(0)
ball.shape("circle")
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


def restart_variables():
    global wn, score_a, score_b, paddle_a, paddle_b, ball, pen
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
    paddle_a.color("red")
    paddle_a.shapesize(stretch_wid=5, stretch_len=1)
    paddle_a.penup()
    paddle_a.goto(-350, 0)

    # Raqueta B
    paddle_b = turtle.Turtle()
    paddle_b.speed(0)
    paddle_b.shape("square")
    paddle_b.color("blue")
    paddle_b.shapesize(stretch_wid=5, stretch_len=1)
    paddle_b.penup()
    paddle_b.goto(350, 0)

    # Bola
    ball = turtle.Turtle()
    ball.speed(0)
    ball.shape("circle")
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


# Mostrar Ganador
def display_winner(winner):
    play_again = False
    # Continuar Juego
    continue_game = True

    # Clear the screen
    wn.clear()
    wn.bgcolor("black")

    # Display the winner
    pen.goto(0, 200)  # Adjusted position for the winner text
    pen.write(f"{winner}", align="center", font=("Courier", 36, "normal"))

    # Display Play Again button
    play_again_button = turtle.Turtle()
    play_again_button.speed(0)
    play_again_button.color("blue")
    play_again_button.goto(0, 50)  # Adjusted position for the Play Again button
    play_again_button.shapesize(4, 5)  # Larger button
    pen.goto(0, 70)
    pen.write("Play Again", align="center", font=("Courier", 24, "normal"))

    # Display Leave button
    leave_button = turtle.Turtle()
    leave_button.speed(0)
    leave_button.color("red")
    leave_button.goto(0, -50)  # Adjusted position for the Leave button
    leave_button.shapesize(4, 5)  # Larger button
    pen.goto(0, -30)
    pen.write("Leave", align="center", font=("Courier", 24, "normal"))

    # Define the function to restart the game
    def restart_game(x, y):
        nonlocal play_again
        print("Jugar otra vez")  # Modify this function as needed
        # You can add logic here to restart the game
        nonlocal continue_game
        continue_game = False  # Set the flag to end the game
        # Close the socket and wait for the receive thread to finish
        # myp.client_socket.close()
        play_again = True

    # Define the function to leave the game
    def leave_game(x, y):
        nonlocal continue_game
        continue_game = False  # Set the flag to end the game
        wn.bye()  # This will close the turtle window
        # Close the socket and wait for the receive thread to finish

    # Set up button click events
    play_again_button.onclick(restart_game)
    leave_button.onclick(leave_game)

    while continue_game:
        wn.update()

    return play_again


def main():
    global continue_game, score_a, score_b, wn, paddle_a, paddle_b, ball
    estado = myp.REQUEST("GET", "STATE")

    print(myp.REQUEST("PLAYER JOIN", "NULL"))

    # Movimiento de las raquetas
    def paddle_up():
        if estado[1] == myp.client_server_port:
            y = paddle_a.ycor()
            y += 20
            # Se envia el mensaje al servidor con la cabecera (PM: Paddle Move) y la PDU (String con la posición de la raqueta)
            myp.REQUEST("PADDLE", str(y))
            paddle_a.sety(y)

        elif estado[2] == myp.client_server_port:
            y = paddle_b.ycor()
            y += 20
            # Se envia el mensaje al servidor con la cabecera (PM: Paddle Move) y la PDU (String con la posición de la raqueta)
            myp.REQUEST("PADDLE", str(y))
            paddle_b.sety(y)

    def paddle_down():
        if estado[1] == myp.client_server_port:
            y = paddle_a.ycor()
            y -= 20
            # Se envia el mensaje al servidor con la cabecera (PM: Paddle Move) y la PDU (String con la posición de la raqueta)
            myp.REQUEST("PADDLE", str(y))
            paddle_a.sety(y)

        elif estado[2] == myp.client_server_port:
            y = paddle_b.ycor()
            y -= 20
            # Se envia el mensaje al servidor con la cabecera (PM: Paddle Move ) y la PDU (String con la posición de la raqueta)
            myp.REQUEST("PADDLE", str(y))
            paddle_b.sety(y)

    # Actualizar la posición de las raquetas
    def update_paddle():
        estado = myp.REQUEST("GET", "STATE")
        # print(estado)

        if estado[1] == myp.client_server_port and paddle_b.ycor() != int(estado[4]):
            paddle_b.sety(int(estado[4]))

        if estado[2] == myp.client_server_port and paddle_a.ycor() != int(estado[3]):
            paddle_a.sety(int(estado[3]))

    def update_ball():
        estado = myp.REQUEST("GET", "STATE")

        # Movimiento de la bola
        if estado[1] == myp.client_server_port:
            ball.setx(ball.xcor() + ball.dx)
            ball.sety(ball.ycor() + ball.dy)
            myp.REQUEST("BALLX", ball.xcor())
            myp.REQUEST("BALLY", ball.ycor())
        else:
            ball.setx(int(estado[5]))
            ball.sety(int(estado[6]))

        # ball.setx(ball.xcor() + ball.dx)
        # ball.sety(ball.ycor() + ball.dy)

    while estado[1] is None or estado[2] == "0000":
        estado = myp.REQUEST("GET", "STATE")
        continue

    if estado[1] == myp.client_server_port:
        pen.write(
            "YOU: 0  THE ENEMY: 0", align="center", font=("Courier", 24, "normal")
        )
        paddle_a.color("blue")
        paddle_b.color("red")
    else:
        pen.write(
            "THE ENEMY: 0  YOU: 0", align="center", font=("Courier", 24, "normal")
        )
        paddle_a.color("red")
        paddle_b.color("blue")

    # Entrada de teclado
    wn.listen()
    # Con cada entrada de teclado se envía un mensaje al servidor actualizando la posición de la raqueta
    wn.onkeypress(paddle_up, "w")
    wn.onkeypress(paddle_down, "s")

    if estado[2] == myp.client_server_port:
        myp.REQUEST("PADDLE", str(0))

    continue_game = True

    # Loop principal del juego
    while score_a < 5 and score_b < 5 and myp.you_win == False:
        wn.update()

        update_paddle()

        update_ball()

        if estado[1] == myp.client_server_port:
            # Bordes
            # Arriba y abajo
            if ball.ycor() > 290:
                ball.sety(290)
                # Esto hace que la bola rebote
                ball.dy *= -1

            elif ball.ycor() < -290:
                ball.sety(-290)
                # Esto hace que la bola rebote
                ball.dy *= -1

            # Izquierda y derecha
            if ball.xcor() > 350:
                score_a += 1

                myp.REQUEST("SCOREA", str(score_a))

                ball.goto(0, 0)
                ball.dx *= -1

            elif ball.xcor() < -350:
                score_b += 1

                myp.REQUEST("SCOREB", str(score_b))

                ball.goto(0, 0)
                ball.dx *= -1

            # Colisiones con las raquetas
            if (
                ball.xcor() < -340
                and ball.ycor() < paddle_a.ycor() + 50
                and ball.ycor() > paddle_a.ycor() - 50
            ):
                ball.dx *= -1

            elif (
                ball.xcor() > 340
                and ball.ycor() < paddle_b.ycor() + 50
                and ball.ycor() > paddle_b.ycor() - 50
            ):
                ball.dx *= -1
            pen.clear()
            pen.write(
                "YOU: {}  THE ENEMY: {}".format(score_a, score_b),
                align="center",
                font=("Courier", 24, "normal"),
            )

        if estado[2] == myp.client_server_port:
            estado = myp.REQUEST("GET", "STATE")
            score_a = int(estado[7])
            score_b = int(estado[8])
            pen.clear()
            pen.write(
                "THE ENEMY: {}  YOU: {}".format(score_a, score_b),
                align="center",
                font=("Courier", 24, "normal"),
            )

        time.sleep(0.01)

    if estado[1] == myp.client_server_port:
        winner = "You Won!" if score_a == 5 else "You Lost :("
    else:
        winner = "You Won!" if score_b == 5 else "You Lost :("

    myp.REQUEST("PLAYER LEAVE", 0)
    play_again = display_winner(winner)
    myp.estado = [0, None, None, 0, 0, 0, 0, 0, 0]
    return play_again


if __name__ == "__main__":
    myp.thread.start()
    play = main()
    while play:
        wn.clear()
        restart_variables()
        play = main()
    myp.thread.join()
    myp.client_socket.close()
