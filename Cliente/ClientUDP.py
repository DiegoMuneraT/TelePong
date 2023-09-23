import socket
import threading
import constants

def receive_messages(client_socket):
    while True:
        try:
            data, server_address = client_socket.recvfrom(constants.MAX_MSG_LEN)
            print(f"Server: {data.decode()}")
        except socket.error as e:
            pass  # Ignore socket errors

# Create a UDP/IP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Set the socket to non-blocking
client_socket.setblocking(0)

# Start a thread to receive messages from the server
receive_thread = threading.Thread(target=receive_messages, args=(client_socket,))
receive_thread.start()

try:
    while True:
        message = input("Client: ")

        if message.lower() == "exit":
            break  # Exit the loop if the user types 'exit'

        try:
            # Send data
            client_socket.sendto(message.encode(), (constants.SERVER_IP, constants.SERVER_PORT))
        except socket.error as e:
            print(f"Error sending data: {e}")
except KeyboardInterrupt:
    pass  # Handle Ctrl+C

# Close the socket and wait for the receive thread to finish
client_socket.close()
receive_thread.join()
