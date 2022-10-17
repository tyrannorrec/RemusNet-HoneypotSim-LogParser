#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* **************************************************************
 *      Header File:  parser.h
 * 
 *      Programmer:   Norrec Nieh
 *      Course:       CS5008, Summer 2022
 *      Date:         Aug 15, 2022
 *      Email:        nieh.c@northeastern.edu
 * **************************************************************/

typedef struct credsNode {
    char*         username;
    char*         password;
    struct credsNode* next;
} credsNode_t;

typedef struct credsList {
    credsNode_t*      head;
    credsNode_t*      tail;
} credsList_t;

typedef struct session {
    int           ID;
    char*         sourceAddress;
    char*         destAddress;
    credsList_t*  credsUsed;  
} session_t;

// Used to store either usernames or passwords in data attribute
typedef struct parserNode {
    char*         data;
    int           counter;
} parserNode_t;

/* ------------------------------------------------------------- */

#define       MAX_SESSIONS 100
session_t*    sessionArray[MAX_SESSIONS];      // Stores session_t* nodes
parserNode_t* userArray[MAX_SESSIONS * 10];    // Stores all usernames, sorted lexicographically
parserNode_t* passArray[MAX_SESSIONS * 10];    // Stores all passwords, sorted lexicographically
parserNode_t* userArrayOcc[MAX_SESSIONS * 10]; // Stores all usernames, sorted by occurrences
parserNode_t* passArrayOcc[MAX_SESSIONS * 10]; // Stores all passwords, sorted by occurrences
int	      NUM_SESSIONS = 0;
int	      NUM_USERNAMES = 0;
int	      NUM_PASSWORDS = 0;

/* --------------------------------------------------------------
 * Struct and List Manipulation
 * ------------------------------------------------------------- */
char* mallocString(char inString[]);
credsNode_t* createCredsNode(char inUsername[], char inPassword[]);
credsList_t* createCredsList();
session_t* createSession(int inID, char* inSourceAddress, char* inDestAddress);
void printCredsNode(credsNode_t* inCredsNode);
void printCredsList(credsList_t* inCredsList);
void printSession(session_t* inSession);
void printSessionArray();
int credsListEmpty(credsList_t* inCredsList);
int credsListLength(credsList_t* inCredsList);
int insertFrontCreds(credsList_t* inCredsList, credsNode_t* inCredsNode);
int insertRearCreds(credsList_t* inCredsList, credsNode_t* inCredsNode);
int addCredsToSession(session_t* inSession, char inUsername[], char inPassword[]);
int addCredsNodeToSession(session_t* inSession, credsNode_t* inCredsNode);
int deleteCredsNode(credsNode_t* inCredsNode);
int deleteFrontCreds(credsList_t* inCredsList);
int deleteRearCreds(credsList_t* inCredsList);
int deleteCredsList(credsList_t* inCredsList);
int deleteSession(session_t* inSession);
void deleteSessionArray();
void deleteUserArrays();
void deletePassArrays();
void resetArrays();
session_t* parseAddressLine(char inLine[300]);
credsNode_t* parseCredsLine(char inLine[300]);
int populateSessionArray();

/* --------------------------------------------------------------
 * IP Address Manipulation
 * ------------------------------------------------------------- */

int checkIPAddressValidity(char inSourceAddress[]);
int compareIPAddresses(char* firstAdd, char* secondAdd);
void swapSessions(session_t** a, session_t** b);
int partitionBySource(int low, int high);
void quickSortSessionsSource(int low, int high);
int partitionByID(int low, int high);
void quickSortSessionsID(int low, int high);
session_t* binarySearchSessionByID(int inID);
void printSessionWithID(int inID);
void printSessionsWithSource(char inSourceAddress[]);

/* --------------------------------------------------------------
 * Username and Password Manipulation
 * ------------------------------------------------------------- */

parserNode_t* createParserNode(char* inString);
int deleteParserNode(parserNode_t** inParserNode);
int binarySearchParserArray(parserNode_t* parserArray[], int size, char key[]);
void mergeOcc(parserNode_t* parserArray[], int left, int mid, int right);
void mergeSortOcc(parserNode_t* parserArray[], int left, int right);
int populateUserArrayLex();
int populatePassArrayLex();
void populateUserArrayOcc();
void populatePassArrayOcc();
void initializeParserArrays();
void printUserArrayLex();
void printPassArrayLex();
void printUserArrayOcc();
void printPassArrayOcc();

/* ------------------------------------------------------------- */
