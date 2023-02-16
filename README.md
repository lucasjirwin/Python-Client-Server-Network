# Python-Client-Server-Network

Creates a Python and C client-server pair to read in data from StdIn and send the data from the client to the server to print to the command-line.

**Server (Python/C)**
Each server program listens on a socket, waits for a client to connect, receives a message from the client, prints the message to stdout, and then waits for the next client indefinitely.
Each server takes one command-line argument: the port number to listen on for client connections.
Each server maintains a short (5-10) client queue and handles multiple client connection attempts sequentially.
Each server gracefully handles error values potentially returned by socket programming library functions. 


**Client (Python/C)**
Each client program contacts a server, reads a message from stdin, sends the message, and exits. 
Each client takes two command-line arguments: the IP address of the server and the port number of the server.
Each client can handle arbitrarily large messages by iteratively reading and sending chunks of the message. 
Each client gracefully handles error values potentially returned by socket programming library functions.
