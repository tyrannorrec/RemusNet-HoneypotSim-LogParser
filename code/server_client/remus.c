/* *************************************************************************************
 *    Program:     remus.c
 *    Purpose:     Implementation of a simple honeypot in c.
 *
 *    Syntax:      remus [ port ]
 *    		   
 *                     port - protocol port number to use. (optional)
 *
 *    NOTE:	   If port not specified, DEFAULT_PORT is used.
 *
 *    Programmer:  Norrec Nieh
 *    Course:      CS 5008, Summer 2022
 *    Date:        Aug 15, 2022
 *    Email:       nieh.c@northeastern.edu
 *
 * *********************************************************************************** */
#include "remus.h"

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
    struct	sockaddr_in clientAddr;		// Client address
    int		serverSD, clientSD;		// Socket descriptors
    int		port = DEFAULT_PORT;		// Port number set to DEFAULT_PORT
    int		addrLen;			// Length of address
    char	buffer[MAX_BUFFER_SIZE];	// Buffer for strings sent and received
    int		numOfCharsRead;			// Number of characters read

    #ifdef WIN32
        WSADATA wsaData;
        WSAStartup(0x0101, &wsaData);
    #endif

    // Initialize server address to local IP address
    memset((char*) &serverAddr, 0,  sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // If argument specified when executing program, overwrite port number with argument
    if (argc > 1) { port = atoi(argv[1]); }

    // If port number is invalid, print error via standard error stream and exit
    if (port < 1) {
        fprintf(stderr, "Invalid port number %s. Exiting program.\n", argv[1]);
        exit(1); 
    }

    // Else, if port number is valid, cast port number to unsigned short and convert to
    // network byte order. Set to in_port of serverAddr struct
    else { serverAddr.sin_port = htons((u_short)port); }

    // Map TCP protocol name to protocol number
    protoEntry_p = getprotobyname("tcp");

    // If mapping failed, print error and exit
    if ((long) protoEntry_p == 0) {
        fprintf(stderr, "Cannot map TCP protocol to protocol number. Exiting program.\n");
        exit(1);
    }

    // Create a socket
    serverSD = socket(PF_INET, SOCK_STREAM, protoEntry_p->p_proto);

    // If socket creation failed, print error and exit
    if (serverSD < 0) {
        fprintf(stderr, "Socket creation failed. Exiting program.\n");
        exit(1);
    }

    // Bind local address to socket
    int bindSuccess = bind(serverSD, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    // If bind failed, print error and exit
    if (bindSuccess < 0) {
        fprintf(stderr, "Bind failed. Exiting program.\n");
        exit(1);
    }

    // Specify size of request queue; exit program if listen failed
    if (listen(serverSD, QUEUE_LENGTH) < 0) {
        fprintf(stderr, "Listen failed. Exiting program.\n");
        exit(1);
    }

    // Main server loop
    while (1) {

        printf("Listening...\n");

        addrLen = sizeof(clientAddr);

        // Accept connection request. If failed, print error and exit
        if ((clientSD = accept(serverSD, (struct sockaddr *) &clientAddr, 
		         (socklen_t*) &addrLen)) < 0) {
            fprintf(stderr, "Accept failed. Exiting program.\n");
            exit(1);
        }

        // Increment connection counter
        conCounter++;

        // Read clientAddr into string
        char clientAddrStr[addrLen];
        inet_ntop(AF_INET, &clientAddr, clientAddrStr, addrLen);

        // Print source IP
        printf("Source IP address: %s\n", clientAddrStr);

for (int j = 0 ; j < 2; j++) {

        // Print header
        sprintf(buffer, "Welcome to RemusNet. This server has been contacted %d time%s\n",
	                 conCounter, conCounter == 1 ? "." : "s.");
        send(clientSD, buffer, strlen(buffer), 0);

        //clear(buffer);
        memset(buffer, 0, sizeof(buffer));

        // Prompt for username
	sprintf(buffer, "Username: ");
        send(clientSD, buffer, strlen(buffer), 0);

        printf("Sent username prompt.\n");

        //clear(buffer);
        memset(buffer, 0, sizeof(buffer));

        printf("Moving from send to receive.\n");

        // Recv username
        numOfCharsRead = recv(clientSD, buffer, sizeof(buffer), 0);
        printf("Username: %s\n", buffer);
                
        //clear(buffer);
        memset(buffer, 0, sizeof(buffer));
	
        // Prompt for password
        sprintf(buffer, "Password: ");
        send(clientSD, buffer, strlen(buffer), 0);

        printf("Sent password prompt. \n");

        //clear(buffer);
        memset(buffer, 0, sizeof(buffer));

	//while ((numOfCharsRead = recv(clientSD, buffer, sizeof(buffer), 0)) > 0) {
        //    write(1, buffer, numOfCharsRead);
        //}

        // Recv password
        numOfCharsRead = recv(clientSD, buffer, sizeof(buffer), 0);
        printf("Password: %s\n", buffer);

} // FOR

        // End connection
        closesocket(clientSD);
    }
    
    return 0;
} 

/* *********************************************************************************** */
