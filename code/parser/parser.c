#include "parser.h"
#include <arpa/inet.h>

/* **************************************************************
 *      Implementation File:  parser.c
 * 
 *      Programmer:   Norrec Nieh
 *      Course:       CS5008, Summer 2022
 *      Date:         Aug 15, 2022
 *      Email:        nieh.c@northeastern.edu
 * **************************************************************/ 

/* --------------------------------------------------------------
 * `mallocString()` takes a char array and returns a pointer to
 *     memory on the heap containing the char array
 * ------------------------------------------------------------ */
char* mallocString(char inString[]) {

    char* newString = (char*) malloc(sizeof(char) * (strlen(inString) + 1));
    strncpy(newString, inString, strlen(inString) + 1);

    if (newString == NULL) {
        return NULL;
    } // IF

    return newString;

} // FUNCTION `mallocString()` 

/* --------------------------------------------------------------
 * `createCredsNode()` creates a new node to represent a pair of
 *     credentials used in a session. Returns the new node.
 * ------------------------------------------------------------ */
credsNode_t* createCredsNode(char inUsername[], char inPassword[]) {                      

    credsNode_t* newCredsNode = (credsNode_t*) malloc(sizeof(credsNode_t)); 

    if (newCredsNode == NULL) {
        return NULL;
    } // IF         

    newCredsNode->username = mallocString(inUsername);
    newCredsNode->password = mallocString(inPassword);
    return newCredsNode;
                        
} // FUNCTION `createCredsNode()`

/* --------------------------------------------------------------
 * `createCredsList()` creates a new list to hold credsNode objects,
 *     containing credentials used in a session. Returns the node.
 * ------------------------------------------------------------ */
credsList_t* createCredsList() {

    credsList_t* newCredsList = (credsList_t*) malloc(sizeof(credsList_t));

    if (newCredsList == NULL) {
        return NULL;
    } // IF

    newCredsList->head = NULL;
    newCredsList->tail = NULL;
    return newCredsList;

} // FUNCTION `createCredsList()`

/* --------------------------------------------------------------
 * `createSession()` creates a new session_t struct to hold
 *     information gained from a session. Returns the node.
 * ------------------------------------------------------------ */
session_t* createSession(int inID, char* inSourceAddress, char* inDestAddress) { 

    session_t* newSession = (session_t*) malloc(sizeof(session_t));

    if (newSession == NULL) {
        return NULL;
    } // IF

    newSession->ID = inID;
    newSession->sourceAddress = inSourceAddress;
    newSession->destAddress = inDestAddress;
    newSession->credsUsed = createCredsList();
    return newSession;

} // FUNCTION `createSession()`

/* --------------------------------------------------------------
 * `printCredsNode()` prints a single username / password pair
 *     held within a credsNode_t struct.
 * ------------------------------------------------------------ */
void printCredsNode(credsNode_t* inCredsNode) {

    printf("\t\t%s, %s\n", inCredsNode->username, inCredsNode->password);

} // FUNCTION `printCredsNode()`

/* --------------------------------------------------------------
 * `printCredsList()` prints all the username / password pairs
 *     associated with a credsList_t struct.
 * ------------------------------------------------------------ */
void printCredsList(credsList_t* inCredsList) {

    credsNode_t* iterator = inCredsList->head;
    printf("\t\t-------- Credentials Used --------\n");
    while (iterator != NULL) {
        printCredsNode(iterator);
        iterator = iterator->next;
    } // WHILE

} // FUNCTION `printCredsList()`

/* --------------------------------------------------------------
 * `printSession()` prints all the information associated with
 *     a session_t struct.
 * ------------------------------------------------------------ */
void printSession(session_t* inSession) {

    printf("* * * * * SESSION * * * * *\n"
           "Session ID:      %d\n"
           "Source Address:  %s\n"
           "Destin Address:  %s\n",
           inSession->ID, 
	   inSession->sourceAddress,
	   inSession->destAddress);
    printCredsList(inSession->credsUsed);

} // FUNCTION `printSession()`

/* --------------------------------------------------------------
 * `printSessionArray()` prints all sessions in the sessionArray.
 * ------------------------------------------------------------ */
void printSessionArray() {

    printf("= + = + = + = + = + = + = + = + = + = + = + = + =\n"
           "= + = + = +       SESSION ARRAY       + = + = + =\n"
           "= + = + = + = + = + = + = + = + = + = + = + = + =\n\n");
    for (int i = 0; i < NUM_SESSIONS; i++) {
        session_t* currentSession = sessionArray[i];
        printSession(currentSession);
        printf("\n");
    } // FOR

    printf("= + = + = + = + = + = + = + = + = + = + = + = + =\n"
           "= + = + = +          END ARRAY        + = + = + =\n"
           "= + = + = + = + = + = + = + = + = + = + = + = + =\n\n");

} // FUNCTION `printSessionArray()`

/* --------------------------------------------------------------
 * `credsListEmpty()` returns 1 if the list is empty and 0 if not.
 * ------------------------------------------------------------ */
int credsListEmpty(credsList_t* inCredsList) {

    if (inCredsList->head == NULL) {
        return 1;
    } // IF

    else{
        return 0;
    } // ELSE

} // FUNCTION `credsListEmpty()`

/* --------------------------------------------------------------
 * `credsListLength()` returns the number of credsNodes in the list.
 * ------------------------------------------------------------ */
int credsListLength(credsList_t* inCredsList) {

    // DECLARE TWO POINTERS TO ITERATE THROUGH LIST
    credsNode_t* currNode = inCredsList->head;
    credsNode_t* nextNode;

    // IF LIST PTR IS A NULL PTR, RETURN -1 (ERROR STATUS)
    if (inCredsList == NULL) {
        return -1;
    } // IF

    // IF HEAD IS NULL AND LIST IS EMPTY, RETURN 0
    if (currNode == NULL) {
        return 0;
    } // IF

    // ELSE, HEAD IS NOT NULL
    else {

        // SET NEXT TO NEXT NODE
        nextNode = currNode->next;    

        // IF NODE 2 IS NULL, HEAD IS THE ONLY NODE; RETURN 1
        if (nextNode == NULL) {
            return 1;
        } // IF

        currNode = nextNode;
        nextNode = nextNode->next;

        // DECLARE AND INITIALIZE COUNTER TO 2, SINCE 
        // `currNode` IS CURRENTLY POINTING TO NODE 2
        int counter = 2;

        // ITERATE UNTIL TAIL NODE
        while (nextNode != NULL) {
            counter++;
            nextNode = nextNode->next;
        } // WHILE

        return counter;
    } // ELSE

} // FUNCTION `credsListLength()`

/* --------------------------------------------------------------
 * `insertFrontCreds()` inserts the creds node at the front of the list.
 *     Returns 1 if successful and -1 if unsuccessful.
 * ------------------------------------------------------------ */
int insertFrontCreds(credsList_t* inCredsList, credsNode_t* inCredsNode) {

    if (inCredsList == NULL) {
        return -1;
    } // IF
   
    // ASSIGN CURR HEAD TO CREDSNODE'S `next` ATTRIBUTE
    inCredsNode->next = inCredsList->head;

    // SET CREDSNODE AS HEAD OF LIST
    inCredsList->head = inCredsNode;

    // IF CREDSNODE IS FIRST NODE, ALSO SET AS TAIL
    if (inCredsNode->next == NULL) {
        inCredsList->tail = inCredsNode;
    } // IF

    return 1; 

} // FUNCTION `insertFrontCreds()`

/*--------------------------------------------------------------
 * `insertRearCreds()` inserts the creds node at the rear of the list.
 *     Returns 1 if successful and -1 if unsuccessful.
 * ------------------------------------------------------------ */
int insertRearCreds(credsList_t* inCredsList, credsNode_t* inCredsNode) {

    if (inCredsList == NULL) {
        return -1; 
    } // IF
  
    // IF THE LIST IS EMPTY, INSERT AT FRONT OF LIST
    if (credsListEmpty(inCredsList)) {
        insertFrontCreds(inCredsList, inCredsNode);
        return 1;
    } // IF

    // ELSE, ASSIGN NEW NODE TO `next` ATTRIBUTE OF CURR TAIL
    // THEN, SET THE NEW NODE AS TAIL OF LIST
    else {
        inCredsList->tail->next = inCredsNode;
        inCredsList->tail = inCredsNode;
    } // ELSE
    
    return 1;

} // FUNCTION `insertRearCreds()`

/*--------------------------------------------------------------
 * `addCredsToSession()` inserts a username / password pair as a
 *     creds node to the end of a credsUsed list within a session.
 *     Returns 1 if successful and -1 if unsuccessful.
 * ------------------------------------------------------------ */
int addCredsToSession(session_t* inSession, char inUsername[], char inPassword[]) {

    if (inSession == NULL) {
        return -1;
    } // IF

    credsNode_t* inCredsNode = createCredsNode(inUsername, inPassword);

    if (inCredsNode == NULL) {
        return -1;
    } // IF

    insertRearCreds(inSession->credsUsed, inCredsNode);
    return 1;

} // FUNCTION `addCredsToSession()`

/*--------------------------------------------------------------
 * `addCredsNodeToSession()` inserts a creds node to the end of
 *     a credsUsed list within a session. Returns 1 if successful 
 *     and -1 if unsuccessful.
 * ------------------------------------------------------------ */
int addCredsNodeToSession(session_t* inSession, credsNode_t* inCredsNode) {

    if (inSession == NULL || inCredsNode == NULL) {
        return -1;
    } // IF

    insertRearCreds(inSession->credsUsed, inCredsNode);
    return 1;

} // FUNCTION `addCredsNodeToSession()` 

/* --------------------------------------------------------------
 * `deleteCredsNode()` deletes the creds node and frees its memory.
 *     Returns 1 if successful and -1 if unsuccessful.
 * ------------------------------------------------------------ */
int deleteCredsNode(credsNode_t* inCredsNode) {

    if (inCredsNode == NULL) {
        return -1;
    } // IF

    free(inCredsNode->username);
    free(inCredsNode->password);
    inCredsNode->next = NULL;
    free(inCredsNode);
    inCredsNode = NULL;
    return 1;

} // FUNCTION `deleteCredsNode()`

/* --------------------------------------------------------------
 * `deleteFrontCreds()` deletes the creds node at the front of the list.
 *     Returns 1 if successful and -1 if unsuccessful.
 * ------------------------------------------------------------ */
int deleteFrontCreds(credsList_t* inCredsList) {

    credsNode_t* temp = NULL;

    if (inCredsList == NULL) {
        return -1;
    }

    else if (inCredsList->head == NULL) {
        printf("Creds list is empty -- nothing to delete.\n");
        return -1;
    }

    // ELSE IF LIST HAS ONLY ONE NODE, SET HEAD AND TAIL
    // OF LIST TO NULL AND FREE THE NODE
    else if (credsListLength(inCredsList) == 1) {
        temp = inCredsList->head;
        inCredsList->head = NULL;
        inCredsList->tail = NULL;
        deleteCredsNode(temp);
        return 1;
    } // ELSE IF

    // ELSE IF LIST HAS MORE THAN ONE NODE, SET HEAD TO
    // 2ND NODE AND FREE THE ORIGINAL HEAD NODE
    else {
        temp = inCredsList->head;
        inCredsList->head = temp->next;
        deleteCredsNode(temp);
        return 1;
    } // ELSE

} // FUNCTION `deleteFrontCreds()`

/* --------------------------------------------------------------
 * `deleteRearCreds()` deletes the creds node at the rear of the list.
 *     Returns 1 if successful and -1 if unsuccessful.
 * ------------------------------------------------------------ */
int deleteRearCreds(credsList_t* inCredsList) {

    credsNode_t* temp = NULL;

    if (inCredsList == NULL) {
        return -1;
    } // IF

    else if (inCredsList->head == NULL) {
        printf("Creds list is empty -- nothing to delete.\n");
        return -1;
    } // ELSE IF

    // ELSE, THE LIST HAS AT LEAST ONE NODE
    else {

        // GET LENGTH OF LIST
        int numOfNodes = credsListLength(inCredsList);
        
        // IF LIST HAS ONE NODE, DELETE NODE AT HEAD
        // AND SET BOTH HEAD AND TAIL TO NULL
        if (numOfNodes == 1) {
            temp = inCredsList->head;
            inCredsList->head = NULL;
            deleteCredsNode(temp);
            inCredsList->tail = NULL;
            return 1;
        } // IF

        // ELSE IF LIST HAS TWO NODES, DELETE NODE
        // AT TAIL, SET TAIL OF LIST TO CURR HEAD
        else if (numOfNodes == 2) {
            temp = inCredsList->head->next;
            inCredsList->head->next = NULL;
            deleteCredsNode(temp);
            inCredsList->tail = inCredsList->head;
            return 1;
        } // ELSE IF

        // ELSE IF LIST HAS MORE THAN TWO NODES,
        // ITERATE THROUGH LIST UNTIL NODE `y`
        // IS THE TAIL
        else {
            credsNode_t* x = inCredsList->head;
            credsNode_t* y = x->next;
            credsNode_t* z = y->next;

            while (z != NULL) {
                x = y;
                y = z;
                z = z->next;
            } // WHILE

            // SET NODE BEFORE TAIL AS NEW TAIL
            // AND FREE OLD TAIL
            x->next = NULL;
            deleteCredsNode(y);
            inCredsList->tail = x;
            return 1;
        } // ELSE
    } // ELSE

} // FUNCTION `deleteRearCreds()`

/* --------------------------------------------------------------
 * `deleteCredsList()` deletes and frees all creds nodes in a list.
 *     Returns 1 if successful and -1 if unsuccessful.
 * ------------------------------------------------------------ */
int deleteCredsList(credsList_t* inCredsList) {

    // CREATE TWO NODE POINTERS TO ITERATE THROUGH LIST
    credsNode_t* targetNode = inCredsList->head;
    credsNode_t* nextNode = NULL;

    // IF POINTER TO LIST IS NULL, RETURN -1 (ERROR MESSAGE)
    if (inCredsList == NULL) {
        return -1;
    } // IF

    // IF HEAD IS NOT NULL, AND LIST IS NOT EMPTY,
    if (targetNode != NULL) {
        
        // SET `nextNode` TO NEXT OF HEAD
        nextNode = targetNode->next;

        // WHILE NOT TAIL, DELETE `targetNode` AND
        // MOVE POINTERS ONE STEP TOWARD TAIL
        while (nextNode != NULL) {
            deleteCredsNode(targetNode);
            targetNode = nextNode;
            nextNode = nextNode->next;
        } // WHILE

        // WHEN TAIL, DELETE TAIL
        deleteCredsNode(targetNode);
        // SET HEAD AND TAIL TO NULL
        inCredsList->head = NULL;
        inCredsList->tail = NULL;
    } // IF

    // FREE LIST AND RETURN 1 (SUCCESS MESSAGE)
    free(inCredsList);
    return 1;

} // FUNCTION `deleteCredsList()`

/* --------------------------------------------------------------
 * `deleteSession()` deletes the session node and frees its memory.
 *     Returns 1 if successful and -1 if unsuccessful.
 * ------------------------------------------------------------ */
int deleteSession(session_t* inSession) {

    if (inSession == NULL) {
        return -1;
    } // IF

    free(inSession->sourceAddress);
    inSession->sourceAddress = NULL;
    free(inSession->destAddress);
    inSession->destAddress = NULL;
    deleteCredsList(inSession->credsUsed);
    free(inSession);
    inSession = NULL;
    NUM_SESSIONS--;
    return 1;

} // FUNCTION `deleteSession()`

/* --------------------------------------------------------------
 * `deleteSessionArray()` frees all session nodes in the array and
 *     all creds nodes in every creds list. 
 *     Returns 1 if successful and -1 if unsuccessful.
 * ------------------------------------------------------------ */
void deleteSessionArray() {

    for (int i = 0; i < NUM_SESSIONS; i++) {
        deleteSession(sessionArray[i]);
        sessionArray[i] = NULL;
    } // FOR

    NUM_SESSIONS = 0;

} // FUNCTION `deleteSessionArray()`

/* --------------------------------------------------------------
 * `deleteUserArrays()` frees all parser nodes in userArray
 *     and userArrayOcc.
 * ------------------------------------------------------------ */
void deleteUserArrays() {

    for (int i = 0; i < NUM_USERNAMES; i++) {
        deleteParserNode(&userArray[i]);
        deleteParserNode(&userArrayOcc[i]);
	userArray[i] = NULL;
	userArrayOcc[i] = NULL;
    } // FOR
 
    NUM_USERNAMES = 0;

} // FUNCTION `deleteUserArrays()`

/* --------------------------------------------------------------
 * `deletePassArrays()` frees all parser nodes in passArray 
 *     and passArrayOcc.
 * ------------------------------------------------------------ */
void deletePassArrays() {

    for (int i = 0; i < NUM_PASSWORDS; i++) {
        deleteParserNode(&passArray[i]);
        deleteParserNode(&passArrayOcc[i]);
	passArray[i] = NULL;
	passArrayOcc[i] = NULL;
    } // FOR

    NUM_PASSWORDS = 0;

} // FUNCTION `deletePassArrays()`

/* --------------------------------------------------------------
 * `resetArrays()` frees all malloc'd nodes and resets all arrays.
 * ------------------------------------------------------------ */
void resetArrays() {

    deleteSessionArray();
    deleteUserArrays();
    deletePassArrays();

} // FUNCTION `resetArrays()`

// = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = //

/* --------------------------------------------------------------
 * `parseAddressLine()` parses the string passed in as argument
 *     and returns a session obj with the derived data.
 * ------------------------------------------------------------ */
session_t* parseAddressLine(char inLine[300]) {

    char temp[20];
    char inSource[20];
    char inDest[20];
    int inID;

    sscanf(inLine, "%s %s %d %s %s %s %s",
		   temp, temp, &inID, temp, inSource, temp, inDest);
    return createSession(inID, mallocString(inSource), mallocString(inDest));

} // FUNCTION `parseAddressLine()`

/* --------------------------------------------------------------
 * `parseCredsLine()` parses the string passed in as argument
 *     and returns a creds node with the derived data.
 * ------------------------------------------------------------ */
credsNode_t* parseCredsLine(char inLine[300]) {

    char temp;
    char inUser[200];
    char inPass[200];

    sscanf(inLine, "%s %c %s", inUser, &temp, inPass);
    return createCredsNode(inUser, inPass); 

} // FUNCTION `parseCredsLine()`

/* --------------------------------------------------------------
 * `populateSessionArray()` reads in a specially formatted log
 *     file (remusLog.txt) and populates the session array with
 *     entries derived from the file. Returns 1 if successful
 *     and -1 if an error occurred.
 * ------------------------------------------------------------ */
int populateSessionArray() {
    
    char* fileName = (char*) malloc(sizeof(char) * 40);
    printf("Enter file name: ");
    scanf("%s", fileName);

    FILE* fileHandle = fopen(fileName, "r");
    if (fileHandle == NULL) {
        perror("ERROR");
        return -1;
    } // IF
    
    char tempStr[300];
    session_t* currSession;
    credsNode_t* currCredsNode;    

    // FOR EVERY LINE IN FILE
    while (!feof(fileHandle)) {

        fgets(tempStr, 300, fileHandle);
        tempStr[strlen(tempStr)] = '\0';

	// WHILE SESSION END MARKER ('<<') NOT SEEN
        while (tempStr[0] != '<' && tempStr[1] != '<') {

	    // IF SESSION START MARKER ('>>') SEEN, PARSE LINE FOR SESSION DATA 	    
	    if (tempStr[0] == '>' && tempStr[1] == '>') {
		currSession = parseAddressLine(tempStr);
	    } // IF
            
	    // ELSE, CHECK IF LINE LONG ENOUGH TO CONTAIN DATA
	    else {
                // IF TRUE, PARSE LINE FOR CREDS DATA AND ADD TO SESSION
                if (strlen(tempStr) >= 5) {
		    currCredsNode = parseCredsLine(tempStr);
		    addCredsNodeToSession(currSession, currCredsNode);
	        } // IF
            } // ELSE
            
	    fgets(tempStr, 300, fileHandle);
	    tempStr[strlen(tempStr)] = '\0';

	} // WHILE

        sessionArray[NUM_SESSIONS] = currSession;
        NUM_SESSIONS++;
    } // WHILE

    fclose(fileHandle);
    free(fileName);
    return 1;

} // FUNCTION `populateSessionArray()`

// = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = //

/* --------------------------------------------------------------
 * `checkIPAddressValidity()` takes in a string and determines
 *     whether it represents a valid IP address. Returns 1 if
 *     true and 0 or -1 if false.
 * ------------------------------------------------------------ */
int checkIPAddressValidity(char inSourceAddress[]) {

    char* sourceAdd = mallocString(inSourceAddress);
    struct sockaddr_in socketAddress;
    int result = inet_pton(AF_INET, sourceAdd, &(socketAddress.sin_addr));
    return result == 1;

} // FUNCTION `checkIPAddressValidity()`

/* --------------------------------------------------------------
 * `compareIPAddresses()` takes in two strings representing IPv4
 *     addresses. Returns 1 if first add is lexicographically
 *     greater than the second; -1 if it is smaller, and 0 if
 *     the two addresses are equal.
 * ------------------------------------------------------------ */
int compareIPAddresses(char* firstAdd, char* secondAdd) {
    
    char firstAddCopy[100];
    strcpy(firstAddCopy, firstAdd);
    char secondAddCopy[100];
    strcpy(secondAddCopy, secondAdd);

    char* firstAddToken = strtok(firstAddCopy, ".");
    char* firstAddFirstOctet = firstAddToken;
    firstAddToken = strtok(NULL, ".");
    char* firstAddSecondOctet = firstAddToken;
    firstAddToken = strtok(NULL, "."); 
    char* firstAddThirdOctet = firstAddToken;
    firstAddToken = strtok(NULL, ".");
    char* firstAddFourthOctet = firstAddToken;

    char* secondAddToken = strtok(secondAddCopy, ".");
    char* secondAddFirstOctet = secondAddToken;
    secondAddToken = strtok(NULL, ".");
    char* secondAddSecondOctet = secondAddToken;
    secondAddToken = strtok(NULL, ".");
    char* secondAddThirdOctet = secondAddToken;
    secondAddToken = strtok(NULL, ".");
    char* secondAddFourthOctet = secondAddToken;        

    int firstAddFirstOctetInt = atoi(firstAddFirstOctet);
    int firstAddSecondOctetInt = atoi(firstAddSecondOctet);
    int firstAddThirdOctetInt = atoi(firstAddThirdOctet);
    int firstAddFourthOctetInt = atoi(firstAddFourthOctet);
    int secondAddFirstOctetInt = atoi(secondAddFirstOctet);
    int secondAddSecondOctetInt = atoi(secondAddSecondOctet);
    int secondAddThirdOctetInt = atoi(secondAddThirdOctet);
    int secondAddFourthOctetInt = atoi(secondAddFourthOctet);

    if (firstAddFirstOctetInt > secondAddFirstOctetInt) {
        return 1;
    } // IF
    else if (firstAddFirstOctetInt < secondAddFirstOctetInt) {
        return -1;
    } // ELSE IF
    else {
        if (firstAddSecondOctetInt > secondAddSecondOctetInt) {
	    return 1;
        } // IF
        else if (firstAddSecondOctetInt < secondAddSecondOctetInt) {
	    return -1;
	} // ELSE IF
        else {
            if (firstAddThirdOctetInt > secondAddThirdOctetInt) {
	        return 1;
	    } // IF
	    else if (firstAddThirdOctetInt < secondAddThirdOctetInt) {
		return -1;
	    } // ELSE IF
	    else {
		if (firstAddFourthOctetInt > secondAddFourthOctetInt) {
		    return 1;
		} // IF
		else if (firstAddFourthOctetInt < secondAddFourthOctetInt) {
		    return -1;
		} // ELSE IF
		else {
		    return 0;
		} // ELSE
	    } // ELSE
	} // ELSE
    } // ELSE

} // FUNCTION `compareIPAddresses()`

/* --------------------------------------------------------------
 * `swapSessions()` is called by quickSortSessionsSource() to swap 
 *     two sessions in sessionArray while partitioning.
 * ------------------------------------------------------------ */
void swapSessions(session_t** a, session_t** b) {

    session_t* temp = *a;
    *a = *b;
    *b = temp;

} // FUNCTION `swapSessions()`

/* --------------------------------------------------------------
 * `partitionBySource()` is called by quickSortSessionsSource() 
 *     to partition sessionArray.
 * ------------------------------------------------------------ */
int partitionBySource(int low, int high) {

    session_t* pivot = sessionArray[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
	// IF CURRENT SESSION'S SOURCE ADD IS SMALLER THAN PIVOT SOURCE ADD
	if (compareIPAddresses(sessionArray[j]->sourceAddress, pivot->sourceAddress) < 0) {
	    i++;
	    swapSessions(&sessionArray[i], &sessionArray[j]);
	} // IF
    } // FOR

    // MOVE PIVOT TO PIVOT INDEX
    i++;
    swapSessions(&sessionArray[i], &sessionArray[high]);

    return i;

} // FUNCTION `partitionBySource()`

/* --------------------------------------------------------------
 * `quickSortSessionsSource()` invokes mergeSort to reorder the
 *     sessionArray by non-descending source IP addresses.
 * ------------------------------------------------------------ */
void quickSortSessionsSource(int low, int high) {

    // BASE CASE
    if (low >= high || low < 0) {
        return;
    } // IF

    int pivotIndex = partitionBySource(low, high);
    quickSortSessionsSource(low, pivotIndex - 1);
    quickSortSessionsSource(pivotIndex + 1, high);

} // FUNCTION `quickSortSessionsSource()`

/* --------------------------------------------------------------
 * `partitionByID()` is called by quickSortSessionsID() 
 *     to partition sessionArray.
 * ------------------------------------------------------------ */
int partitionByID(int low, int high) {

    session_t* pivot = sessionArray[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
	// IF CURRENT SESSION'S ID IS SMALLER THAN OR EQUAL TO PIVOT ID
	if (sessionArray[j]->ID < pivot->ID) {
	    i++;
	    swapSessions(&sessionArray[i], &sessionArray[j]);
	} // IF
    } // FOR

    // MOVE PIVOT TO PIVOT INDEX
    i++;
    swapSessions(&sessionArray[i], &sessionArray[high]);

    return i;

} // FUNCTION `partitionByID()`

/* --------------------------------------------------------------
 * `quickSortSessionsID()` invokes mergeSort to reorder the
 *     sessionArray by non-descending session ID.
 * ------------------------------------------------------------ */
void quickSortSessionsID(int low, int high) {

    // BASE CASE
    if (low >= high || low < 0) {
        return;
    } // IF

    int pivotIndex = partitionByID(low, high);
    quickSortSessionsID(low, pivotIndex - 1);
    quickSortSessionsID(pivotIndex + 1, high);

} // FUNCTION `quickSortSessionsID()`

/* --------------------------------------------------------------
 * `binarySearchSessionByID()` invokes binary search to find a 
 *     sessionArray by non-descending session ID. Returns the
 *     session, or NULL if no matching session is found.
 * ------------------------------------------------------------ */
session_t* binarySearchSessionByID(int inID) {

    session_t* found = NULL;
    int start = 0;
    int end = NUM_SESSIONS - 1;
    int mid;

    // COMPARE EACH MID'S ID WITH KEY
    while (start <= end) {
        mid = (start + end) / 2;

        // IF KEY == MID
        if (inID == sessionArray[mid]->ID) {
            found = sessionArray[mid];
            return found;
        } // IF

        // ELSE IF KEY > MID
        else if (inID > sessionArray[mid]->ID) {
            start = mid + 1;
        } // ELSE IF

        // ELSE IF KEY < MID
        else {
            end = mid - 1; 
        } // ELSE
    } // WHILE

    return found;
    
} // FUNCTION `binarySearchSessionByID()`

/* --------------------------------------------------------------
 * `printSessionWithID()` calls `binarySearchSessionByID()` to 
 *     find a session. Prints the contents of the session, or a
 *     message if the session was not found.
 * ------------------------------------------------------------ */
void printSessionWithID(int inID) {

    session_t* found = binarySearchSessionByID(inID);
    
    if (found == NULL) {
        printf("Session %d was not found.\n", inID);
    } // IF    
    else {
        printSession(found);
    } // ELSE

} // FUNCTION `printSessionWIthID()`

/* --------------------------------------------------------------
 * `printSessionsWithSource()` employs linear search to find and 
 *     print all sessions with a matching source ID. Prints a
 *     message if the session was not found.
 * ------------------------------------------------------------ */
void printSessionsWithSource(char inSourceAddress[]) {

    int count = 0;
    for (int i = 0; i < NUM_SESSIONS; i++) {
        if (compareIPAddresses(sessionArray[i]->sourceAddress, 
			       mallocString(inSourceAddress)) == 0) {
	    printSession(sessionArray[i]);
	    count++;
	} // IF
    } // FOR

    if (count == 0) {
        printf("No sessions were found with the source address %s.\n", 
		inSourceAddress);
    } // IF

} // FUNCTION `printSessionsWithSource()`

// = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = //

/* --------------------------------------------------------------
 * `createParserNode()` creates a new parserNode_t struct to hold
 *     a string ptr as data and its num of occurences. Returns the node.
 * ------------------------------------------------------------ */
parserNode_t* createParserNode(char* inString) {

    parserNode_t* newParserNode = (parserNode_t*) malloc(sizeof(parserNode_t));

    if (newParserNode == NULL) {
        return NULL;
    } // IF

    char copy[100];
    strcpy(copy, inString);
 
    newParserNode->data = mallocString(copy);
    newParserNode->counter = 1;
    return newParserNode;

} // FUNCTION `createParserNode()`

/* --------------------------------------------------------------
 * `deleteParserNode()` deletes the parser node and frees its memory.
 *     Returns 1 if successful and -1 if unsuccessful.
 * ------------------------------------------------------------ */
int deleteParserNode(parserNode_t** inParserNode) {
    
    if (inParserNode == NULL) {
        return -1;
    } // IF

    free((*inParserNode)->data);
    (*inParserNode)->data = NULL;
    free(*inParserNode);
    *inParserNode = NULL;
    return 1;

} // FUNCTION `deleteParserNode()`

/* --------------------------------------------------------------
 * `binarySearchParserArray()` employs binary search to find a
 *     string within the data attribute of parser nodes in an array.
 *     Returns index, or -1 if not in array.
 * ------------------------------------------------------------ */
int binarySearchParserArray(parserNode_t* parserArray[], int size, char key[]) {

    char* keyString = mallocString(key);

    int found = -1;
    int start = 0;
    int end = size - 1;
    int mid;

    // COMPARE EACH MID LEXICOGRAPHICALLY WITH KEY
    while (start <= end) {
        mid = (start + end) / 2;
        int result = strcmp(keyString, parserArray[mid]->data);

        // IF KEY == MID
        if (result == 0) {
            found = mid;
	    free(keyString);
            return found;
        } // IF

        // ELSE IF KEY > MID
        else if (result > 1) {
            start = mid + 1;
        } // ELSE IF

        // ELSE IF KEY < MID
        else {
            end = mid - 1; 
        } // ELSE
    } // WHILE

    free(keyString);
    return found;

} // FUNCTION `binarySearchParserArray()`

/* --------------------------------------------------------------
 * `mergeOcc()` merges two halves on an array and returns the
 *     resulting array in non-increasing sorted order.
 * ------------------------------------------------------------ */
void mergeOcc(parserNode_t* parserArray[], int left, int mid, int right) {

    int tempLength = right - left + 1;
    parserNode_t* temp[tempLength];

    // DECLARE COUNTERS FOR LEFT HALF (i), RIGHT HALF (j), AND TEMP ARR (k)
    int i = left;
    int j = mid + 1;
    int k = 0;

    int status;

    while (i <= mid && j <= right) {
        if (parserArray[i]->counter >= parserArray[j]->counter) {
            temp[k] = parserArray[i];
            i++;
        } // IF
        else {
            temp[k] = parserArray[j];
            j++;
        } // ELSE
        k++;
    } // WHILE

    while (i <= mid) {
        temp[k] = parserArray[i];
        k++;
        i++;
    } // WHILE

    while (j <= right) {
        temp[k] = parserArray[j];
        k++;
        j++;
    } // WHILE

    for (k = 0, i = left; k < tempLength; k++, i++) {
        parserArray[i] = temp[k];
    } // FOR

} // FUNCTION `mergeOcc()`

/* --------------------------------------------------------------
 * `mergeSortOcc()` employs mergesort to rearrange parser nodes
 *     in an array by non-increasing num of occurrences.
 * ------------------------------------------------------------ */
void mergeSortOcc(parserNode_t* parserArray[], int left, int right) {

    // IF NOT BASE CASE
    if (left < right) {
    
        int mid = (left + right) / 2;
        mergeSortOcc(parserArray, left, mid);
        mergeSortOcc(parserArray, mid + 1, right);
        mergeOcc(parserArray, left, mid, right);

    } // IF

} // FUNCTION `mergeSortOcc()`

/* --------------------------------------------------------------
 * `populateUserArrayLex()` extracts the username from every creds
 *     node and adds it to userArray or updates the count of
 *     existing parserNodes in userArray. 
 *     Returns 1 if successful and -1 if unsuccessful.
 * ------------------------------------------------------------ */
int populateUserArrayLex() {

    // FOR EVERY SESSION, TRAVERSE CREDSLIST TO GET USERNAMES
    for (int i = 0; i < NUM_SESSIONS; i++) {
        credsNode_t* iterator = sessionArray[i]->credsUsed->head;      

        while (iterator != NULL) {

            int status = binarySearchParserArray(userArray, NUM_USERNAMES, iterator->username);
	    int indexToInsert = 0;

            // IF NEW USERNAME NOT IN USER ARRAY, ADD TO ARRAY IN ORDER
	    if (status == -1) {
                parserNode_t* newParserNode = createParserNode(iterator->username);

   		// FIND INDEX TO INSERT NEW NODE IN ORDER
                for (int j = 0; j < NUM_USERNAMES; j++) {
                    int result = strcmp(userArray[j]->data, newParserNode->data);
                    
                    if (result > 0) {
                        indexToInsert = j;
			break;
		    } // IF
		    if (j == NUM_USERNAMES - 1) {
		        indexToInsert = j + 1;
		    } // IF
		} // FOR

		// IF NEW NODE TO BE PLACED AT END OF ARRAY
		if (indexToInsert == NUM_USERNAMES) {
		    userArray[indexToInsert] = newParserNode;
		} // IF
		
		// ELSE, MOVE ELEMENTS TO MAKE ROOM
		else {
                    //for (int k = indexToInsert; k < NUM_USERNAMES; k++) {
                    for (int k = NUM_USERNAMES - 1; k >= indexToInsert; k--) {
		        userArray[k+1] = userArray[k];
	            } // FOR

                    userArray[indexToInsert] = newParserNode;
		} // ELSE		

		NUM_USERNAMES++;
	    } // IF

            // ELSE, INCREMENT COUNTER FOR THAT PARSER NODE
            else {
                (userArray[status]->counter)++;
            } // ELSE

	    iterator = iterator->next;
        } // WHILE
    } // FOR

    return 1;

} // FUNCTION `populateUserArrayLex()`

/* --------------------------------------------------------------
 * `populatePassArrayLex()` extracts the password from every creds
 *     node and adds it to passArray or updates the count of
 *     existing parserNodes in passArray. 
 *     Returns 1 if successful and -1 if unsuccessful.
 * ------------------------------------------------------------ */
int populatePassArrayLex() {

    // FOR EVERY SESSION, TRAVERSE CREDSLIST TO GET PASSWORDS
    for (int i = 0; i < NUM_SESSIONS; i++) {
        credsNode_t* iterator = sessionArray[i]->credsUsed->head;      

        while (iterator != NULL) {

            int status = binarySearchParserArray(passArray, NUM_PASSWORDS, iterator->password);
	    int indexToInsert = 0;

            // IF NEW PASSWORD NOT IN PASS ARRAY, ADD TO ARRAY IN ORDER
	    if (status == -1) {
                parserNode_t* newParserNode = createParserNode(iterator->password);
		
   		// FIND INDEX TO INSERT NEW NODE IN ORDER
                for (int j = 0; j < NUM_PASSWORDS; j++) {
                    int result = strcmp(passArray[j]->data, newParserNode->data);
                    
                    if (result > 0) {
                        indexToInsert = j;
			break;
		    } // IF
		    if (j == NUM_PASSWORDS - 1) {
		        indexToInsert = j + 1;
		    } // IF
		} // FOR

		// IF NEW NODE TO BE PLACED AT END OF ARRAY
		if (indexToInsert == NUM_PASSWORDS) {
		    passArray[indexToInsert] = newParserNode;
		} // IF
		
		// ELSE, MOVE ELEMENTS TO MAKE ROOM
		else {
                    for (int k = NUM_PASSWORDS - 1; k >= indexToInsert; k--) {
		        passArray[k+1] = passArray[k];
	            } // FOR

                    passArray[indexToInsert] = newParserNode;
		} // ELSE		

		NUM_PASSWORDS++;
	    } // IF

            // ELSE, INCREMENT COUNTER FOR THAT PARSER NODE
            else {
                (passArray[status]->counter)++;
            } // ELSE

	    iterator = iterator->next;
        } // WHILE
    } // FOR

    return 1;

} // FUNCTION `populatePassArrayLex()`

/* --------------------------------------------------------------
 * `populateUserArrayOcc()` makes a copy of userArray and
 *     sorts it by num of occurrences. Stores in userArrayOcc.
 * ------------------------------------------------------------ */
void populateUserArrayOcc() {

    for (int i = 0; i < NUM_USERNAMES; i++) {
        parserNode_t* iterator = userArray[i];
        parserNode_t* newParserNode = createParserNode(iterator->data);
	newParserNode->counter = iterator->counter;
	userArrayOcc[i] = newParserNode;
    } // FOR

    mergeSortOcc(userArrayOcc, 0, NUM_USERNAMES - 1);

} // FUNCTION `populateUserArrayOcc()`

/* --------------------------------------------------------------
 * `populatePassArrayOcc()` makes a copy of passArray and
 *     sorts it by num of occurrences. Stores in passArrayOcc.
 * ------------------------------------------------------------ */
void populatePassArrayOcc() {

    for (int i = 0; i < NUM_PASSWORDS; i++) {
        parserNode_t* iterator = passArray[i];
        parserNode_t* newParserNode = createParserNode(iterator->data);
	newParserNode->counter = iterator->counter;
        passArrayOcc[i] = newParserNode;
    } // FOR

    mergeSortOcc(passArrayOcc, 0, NUM_PASSWORDS - 1);

} // FUNCTION `populatePassArrayOcc()`

/* --------------------------------------------------------------
 * `initializeParserArrays()` populates userArray, userArrayOcc,
 *     passArray, and passArrayOcc.
 * ------------------------------------------------------------ */
void initializeParserArrays() {

    populateUserArrayLex();
    populatePassArrayLex();
    populateUserArrayOcc();
    populatePassArrayOcc();

} // FUNCTION `initializeParserArrays()`

/* --------------------------------------------------------------
 * `printUserArrayLex()` prints every username found in the
 *     sessionArray in lexicographical order.
 * ------------------------------------------------------------ */
void printUserArrayLex() {
    
    printf("\nPrinting USERNAMES in lexicographical order...\n"
           "* * * * * * * * * * * * * * * * * * * * * * * *\n\n");

    for (int i = 0; i < NUM_USERNAMES; i++) {
	   printf("%s   --   %d\n", userArray[i]->data, userArray[i]->counter);
    } // FOR

    printf("\n");

} // FUNCTION `printUserArrayLex()`

/* --------------------------------------------------------------
 * `printPassArrayLex()` prints every password found in the
 *     sessionArray in lexicographical order.
 * ------------------------------------------------------------ */
void printPassArrayLex() {
    
    printf("\nPrinting PASSWORDS in lexicographical order...\n"
           "* * * * * * * * * * * * * * * * * * * * * * * *\n\n");

    for (int i = 0; i < NUM_PASSWORDS; i++) {
	   printf("%s   --   %d\n", passArray[i]->data, passArray[i]->counter);
    } // FOR

    printf("\n");

} // FUNCTION `printPassArrayLex()`

/* --------------------------------------------------------------
 * `printUserArrayOcc()` prints every username found in the
 *     sessionArray in non-increasing order of occurrences.
 * ------------------------------------------------------------ */
void printUserArrayOcc() {

    printf("\nPrinting USERNAMES in order of occurrences...\n"
           "* * * * * * * * * * * * * * * * * * * * * * * *\n\n");

    for (int i = 0; i < NUM_USERNAMES; i++) {
	   printf("%s   --   %d\n", userArrayOcc[i]->data, userArrayOcc[i]->counter);
    } // FOR

    printf("\n");
    
} // FUNCTION `printUserArrayOcc()`

/* --------------------------------------------------------------
 * `printPassArrayOcc()` prints every password found in the
 *     sessionArray in non-increasing order of occurrences.
 * ------------------------------------------------------------ */
void printPassArrayOcc() {
    
    printf("\nPrinting PASSWORDS in order of occurrences...\n"
           "* * * * * * * * * * * * * * * * * * * * * * * *\n\n");

    for (int i = 0; i < NUM_PASSWORDS; i++) {
	   printf("%s   --   %d\n", passArrayOcc[i]->data, passArrayOcc[i]->counter);
    } // FOR

    printf("\n");

} // FUNCTION `printPassArrayOcc()`

/* ************************************************************** */
