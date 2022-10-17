/* *************************************************************************************
 *    Program:     romulus.h
 *    Purpose:     Header file for romulus.c, the client side of remusNet.
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>		// Provides POSIX-compliant functions

#define unix			// Define Unix
#ifndef unix				// If Unix not defined, define WIN32
    #define WIN32
    #include <windows.h>		// Windows API
    #include <winsock.h>		// Windows Sockets 2
#else
    #define closesocket close	// Redefines Win closesocket() as close() for Unix
    #include <sys/types.h>	// Provides typedef symbols and structures
    #include <sys/socket.h>	// Provides socket definitions
    #include <netinet/in.h>	// Provides definitions for internet address family
    #include <arpa/inet.h>	// Provides definitions for internet operations
    #include <netdb.h>		// Provides definitions for network database operations
#endif

/* ----------------------------------------------------------------------------------- *
 * CONFIG
 * ----------------------------------------------------------------------------------- */
#define DEFAULT_PORT 8080
#define MAX_BUFFER_SIZE 1024

extern int errorNum;
char localhost[] = "localhost";		 

/* *********************************************************************************** */
