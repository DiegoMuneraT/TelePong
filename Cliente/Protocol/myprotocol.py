import socket
import threading
import constants

# Variables de estado

estado = [ 0, None, None, 0, 0, 0, 0, 0, 0]

# Creamos el socket UDP/IP del cliente
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Set the socket to non-blocking
client_socket.setblocking(0)

client_port = str(client_socket.getsockname()[1])

def get_client_port():
    return str(client_socket.getsockname()[1])

def receive_state(client_socket):
    global estado
    while True:
        try:
            data, server_address = client_socket.recvfrom(constants.MAX_MSG_LEN)
            estado = data.decode().split(",")
        
        except socket.error as e:
            pass

def send_message(client_socket, message):
    client_socket.sendto(message.encode(), (constants.SERVER_IP, constants.SERVER_PORT))

thread = threading.Thread(target=receive_state, args=(client_socket,))

def REQUEST(header, value):

    if header == 'GET' and value == 'PORT':
        return get_client_port()

    elif header == 'GET' and value == 'STATE':
        return estado
    
    elif header == 'PADDLE':
        send_message(client_socket, 'PM'+str(value))
        return 'PADDLE MOVE OK'
    
    elif header == 'PLAYER JOIN':
        send_message(client_socket, header)
        return 'PLAYER JOINED'
    
    elif header == 'SCORE':
        send_message(client_socket, 'SC'+str(value))
        
"""

REQUEST es un método que se encarga de enviar una solicitud al servidor.

GET PORT -> Solicita el puerto del cliente al servidor.

"""