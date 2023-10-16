import socket
import threading
import constants

# Variables de estado

estado = [0, None, None, 0, 0, 0, 0, 0, 0]

game_port = 0

you_win = False

# Creamos el socket UDP/IP del cliente
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Set the socket to non-blocking
client_socket.setblocking(0)

print(
    f"Cliente: {str(client_socket.getsockname()[1])} conectado al servidor: {constants.SERVER_IP} en el puerto: {constants.SERVER_PORT}"
)

client_server_port = 0


def get_client_port():
    return str(client_socket.getsockname()[1])


def receive_state(client_socket):
    global estado, game_port, client_server_port, you_win
    contador = 0
    while True:
        try:
            data, server_address = client_socket.recvfrom(constants.MAX_MSG_LEN)
            rawdata = data.decode().split(":")

            print(rawdata)

            if rawdata[0] == "state":
                estado = rawdata[1].split(",")

                if contador == 0 and estado[0] == "0":
                    client_server_port = estado[1]
                    contador += 1
                elif contador == 0:
                    client_server_port = estado[2]
                    contador += 1

            elif rawdata[0] == "port":
                game_port = int(rawdata[1])

        except socket.error:
            pass

        except UnboundLocalError:
            pass


def send_message(client_socket, message, server_port):
    print(
        f"Enviando mensaje: {message} al servidor: {constants.SERVER_IP} en el puerto: {server_port}"
    )
    client_socket.sendto(message.encode(), (constants.SERVER_IP, server_port))


thread = threading.Thread(target=receive_state, args=(client_socket,))


def REQUEST(header, value):
    if header == "GET" and value == "PORT":
        return get_client_port()

    elif header == "GET" and value == "STATE":
        return estado

    elif header == "PADDLE":
        send_message(client_socket, "PM" + str(value), game_port)
        return "PADDLE MOVE OK"

    elif header == "BALLX":
        send_message(client_socket, "BX" + str(value), game_port)
        return "BALL MOVE OK"

    elif header == "BALLY":
        send_message(client_socket, "BY" + str(value), game_port)
        return "BALL MOVE OK"

    elif header == "PLAYER JOIN":
        send_message(client_socket, "PLJ", constants.SERVER_PORT)
        return "PLAYER JOINED"

    elif header == "PLAYER LEAVE":
        send_message(client_socket, "PLV", game_port)
        return "PLAYER LEFT"

    elif header == "SCOREA":
        send_message(client_socket, "SA" + str(value), game_port)

    elif header == "SCOREB":
        send_message(client_socket, "SB" + str(value), game_port)


"""

REQUEST es un método que se encarga de enviar una solicitud al servidor.

GET PORT -> Solicita el puerto del cliente al servidor.

"""
