/* *************************************************************************************
 *    Program:     romulus.c
 *    Purpose:     Implementation of a simple client in c.
 *
 *    Syntax:      romulus [ host [ port ] ]
 *
 * 		       host - name of computer on which server is running. (optional)	
 *                     port - protcol port number to use.		   (optional)
 *
 *    NOTE:        If host not specified, "localhost" used as hostname. 
 *                 If port not specified, DEFAULT_PORT is used.
 *
 *    Programmer:  Norrec Nieh
 *    Course:      CS 5008, Summer 2022
 *    Date:        Aug 15, 2022
 *    Email:       nieh.c@northeastern.edu
 *
 * *********************************************************************************** */
#include "romulus.h" 

#define END_MESSAGE "!@#END_MESSAGE!@#"

/* ----------------------------------------------------------------------------------- *
 * clear() resets all bits in the buffer to 0.
 * ----------------------------------------------------------------------------------- */
void clear(char* buffer) {

    memset(buffer, 0, sizeof(*buffer));
}        

/* ----------------------------------------------------------------------------------- *
 * MAIN
 * ----------------------------------------------------------------------------------- */
int main(int argc, char* argv[]) {

    struct	hostent *hostEntry_p;		// Pointer to host table entry
    struct	protoent *protoEntry_p;		// Pointer to protocol table entry
    struct	sockaddr_in serverAddr;		// Server address
    int		SD;				// Socket descriptor
    int         port = DEFAULT_PORT;		// Port number set to DEFAULT_PORT
    char*       host;				// pointer to host name
    char	buffer[MAX_BUFFER_SIZE];	// Buffer for strings sent and received
    int		numOfCharsRead;			// Number of characters read
    //int		temp;

    #ifdef WIN32
        WSADATA wsaData;
        WSAStartup(0x0101, &wsaData);
    #endif

for(int i = 0; i < 5; i++) {

    // Initialize server address to local IP address
    memset((char *) &serverAddr, 0,  sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    
    // If port specified when executing program, overwrite port number with argument
    if (argc > 2) { port = atoi(argv[2]); }

    // If port number is invalid, print error via standard error stream and exit
    if (port < 1) {
        fprintf(stderr, "Invalid port number %s. Exiting program.\n", argv[2]);
        exit(1);
    }

    // Else, if port number is valid, cast port number to unsigned short and convert to
    // network byte order. Set to in_port of serverAddr struct
    else { serverAddr.sin_port = htons((u_short) port); }

    // If host specified when executing program, set to `host` var
    if (argc > 1) { host = argv[1]; }
    
    // Else, set host to `localhost`;
    else { host = localhost; }

    // Convert host name to IP address
    hostEntry_p = gethostbyname(host);

    // If invalid host, print error and exit
    if (((char *) hostEntry_p) == NULL) {
        fprintf(stderr, "Invalid host. Exiting program.\n");
        exit(1);
    }

    // Copy host address to serverAddr object
    memcpy(&serverAddr.sin_addr, hostEntry_p->h_addr, hostEntry_p->h_length);

    // Map TCP protocol name to protocol number
    protoEntry_p = getprotobyname("tcp");

    // If mapping failed, print error and exit
    if ((long) protoEntry_p == 0) {
        fprintf(stderr, "Cannot map TCP protocol to protocol number. Exiting program.\n");
        exit(1);
    }

    // Create a socket
    SD = socket(PF_INET, SOCK_STREAM, protoEntry_p->p_proto);

    // If socket creation failed, print error and exit
    if (SD < 0) {
        fprintf(stderr, "Socket creation failed. Exiting program.\n");
        exit(1);
    }

    // Connect to server
    if (connect(SD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        fprintf(stderr, "Connect failed. Exiting program.\n");
        exit(1);
    }

for (int j = 0; j < 2; j++) {

    // Read header
    numOfCharsRead = recv(SD, buffer, sizeof(buffer), 0);
    write(1, buffer, numOfCharsRead);    

    //clear(buffer);
    memset(buffer, 0, sizeof(buffer));

    // Read username prompt
    numOfCharsRead = recv(SD, buffer, 10, 0);
    printf("***Between receive and write\n");
    int temp = write(1, buffer, numOfCharsRead);    
 

    printf("status of write = %d\n", temp);
    printf("Username prompt has been read.\n");   

    //clear(buffer);
    memset(buffer, 0, sizeof(buffer));

    fgets(buffer, MAX_BUFFER_SIZE - 1, stdin);
    printf("***Just read, about to send!\n");
    if ((send(SD, buffer, strlen(buffer), 0)) == -1) {
        fprintf(stderr, "Failed to send message.\n");
        close(SD);
        exit(1);
    }
    printf("Message being sent: %s", buffer);

    //clear(buffer);
    memset(buffer, 0, sizeof(buffer));

    numOfCharsRead = recv(SD, buffer, 10, 0);
    write(1, buffer, numOfCharsRead);    
    
    //clear(buffer);
    memset(buffer, 0, sizeof(buffer));
   
    fgets(buffer, MAX_BUFFER_SIZE - 1, stdin);
    if ((send(SD, buffer, strlen(buffer), 0)) == -1) {
        fprintf(stderr, "Failed to send message.\n");
        close(SD);
        exit(1);
    }
    printf("Message being sent: %s", buffer);

} // FOR

    // Close socket
    closesocket(SD);


}

    exit(0);
}

/* *********************************************************************************** */
