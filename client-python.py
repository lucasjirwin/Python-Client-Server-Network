###############################################################################
# client-python.py
# Name: Lucas Irwin
# NetId: ljirwin
###############################################################################

# Imports
import sys
import socket

# Fixed buffer size
SEND_BUFFER_SIZE = 2048

# Method to create a socket and send a message to the server from the client 
def client(server_ip, server_port):
    """TODO: Open socket and send message from sys.stdin"""
    new_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # instantiates the socket 
    new_socket.connect((server_ip, server_port)) # connect to server 

    message = sys.stdin.buffer.read(SEND_BUFFER_SIZE) # read in message from StdIn 

    # Sends chunks of data of required buffer size to server 
    while message: 
        new_socket.sendall(message)
        message = sys.stdin.buffer.read(SEND_BUFFER_SIZE) # reads message from StdIn 

    new_socket.close() # close the socket 

# Main method 
def main():
    """Parse command-line arguments and call client function """
    if len(sys.argv) != 3:
        sys.exit("Usage: python client-python.py [Server IP] [Server Port] < [message]")
    server_ip = sys.argv[1]
    server_port = int(sys.argv[2])
    client(server_ip, server_port)

if __name__ == "__main__":
    main()
