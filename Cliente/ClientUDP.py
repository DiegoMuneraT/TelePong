import socket
import constants

# Create a TCP/IP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Connect the socket to the port where the server is listening
while input != "exit":
    message = input("Client: ")

    # Send data
    client_socket.sendto(message.encode(), (constants.SERVER_IP, constants.SERVER_PORT))

    # Receive message
    data, server_address = client_socket.recvfrom(constants.MAX_MSG_LEN)
    print(f"Server: {data.decode()}")

client_socket.close()