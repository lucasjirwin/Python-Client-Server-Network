###############################################################################
# server-python.py
# Name: Lucas Irwin
# NetId: ljirwin
###############################################################################

# Imports
import sys
import socket

# Fixed buffer size and queue length for clients
RECV_BUFFER_SIZE = 2048
QUEUE_LENGTH = 10

# Method to bind to the socket and accept messages from clients which 
# are then written to stdout
def server(server_port):
    """TODO: Listen on socket and print received message to sys.stdout"""
    server_host  = "localhost"
    new_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    new_socket.bind((server_host, server_port))
    print("server binding")

    # Listens to a maximum number of clients euqal to the Queue length
    new_socket.listen(QUEUE_LENGTH)
    print("server listening")

    # Loops over client sockets while they exist 
    # and writes sent messages to stdout 
    while True:
        conn, addr = new_socket.accept()
        message = conn.recv(RECV_BUFFER_SIZE)
        while message:
            sys.stdout.buffer.write(message)
            sys.stdout.buffer.flush()
            message = conn.recv(RECV_BUFFER_SIZE)
        conn.close()


# Main method
def main():
    """Parse command-line argument and call server function """
    if len(sys.argv) != 2:
        sys.exit("Usage: python server-python.py [Server Port]")
    server_port = int(sys.argv[1])
    server(server_port)

if __name__ == "__main__":
    main()
