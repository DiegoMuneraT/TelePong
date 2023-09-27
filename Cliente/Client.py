import turtle
import threading
import constants
import socket


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
estado = [ 0, None, None, 0, 0, 0, 0, 0, 0 ]
waiting_for_start = True
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client_socket.setblocking(0)

def receive_state(client_socket):
    while True:
        try:
            data, server_address = client_socket.recvfrom(constants.MAX_MSG_LEN)
            estado = data.decode().split(",")

        except socket.error as e:
            print('Error', e)







