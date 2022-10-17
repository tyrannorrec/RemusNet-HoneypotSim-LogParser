#include <stdio.h>
#include <stdlib.h>
#include "parser.c"

/* *************************************************************************************
 *    Program:     parserMenu.c
 *    Purpose:     Contains the menu code and main driver of the RemusNet log parser.
 *
 *    Programmer:  Norrec Nieh
 *    Course:      CS 5008, Summer 2022
 *    Date:        Aug 15, 2022
 *    Email:       nieh.c@northeastern.edu
 * *********************************************************************************** */

/* ----------------------------------------------------------------------------------- *
 * flush() flushes the stdin and stdout buffers and forces the user to press enter
 * twice before the screen is cleared after each menu option execution.
 * ----------------------------------------------------------------------------------- */
void flush() {

    fflush(stdin);
    getchar();
    getchar();
    fflush(stdout);

} // FUNCTION flush()

/* ----------------------------------------------------------------------------------- *
 * printBanner() prints the welcome banner for the log parser.
 * ----------------------------------------------------------------------------------- */
void printBanner() {

    system("clear");
    printf("> > >  Welcome to the RemusNet log parser (v1.0).\n" 
	   "> > >  Press enter to access menu.\n");
    flush();

} // FUNCTION printBanner()

/* ----------------------------------------------------------------------------------- *
 * printMenu() prints the menu for the log parser.
 * ----------------------------------------------------------------------------------- */
void printMenu() {

    system("clear");
    printf("--------------------------------------------\n"
           "     1  -- POPULATE SESSION ARRAY\n"
	   "     2  -- FIND ENTRIES BY SOURCE IP ADDRESS\n"
	   "     3  -- FIND ENTRY BY ID\n"
           "     4  -- SORT ENTRIES BY SOURCE IP ADDRESS\n"
           "     5  -- SORT ENTRIES BY ID\n"
	   "     6  -- SORT USERNAMES BY NUM OF OCCURRENCES\n"
           "     7  -- SORT PASSWORDS BY NUM OF OCCURRENCES\n"
           "     8  -- SORT USERNAMES BY LEXICOGRAPHICAL ORDER\n"
           "     9  -- SORT PASSWORDS BY LEXICOGRAPHICAL ORDER\n"
           "     10 -- DELETE ALL SESSIONS\n"
	   "     11 -- QUIT\n"
           "--------------------------------------------\n");

} // FUNCTION printMenu()

/* ----------------------------------------------------------------------------------- *
 * getMenuChoice() gets an integer input from the user and returns that integer.
 * ----------------------------------------------------------------------------------- */

int getMenuChoice() {

    int choice;

    printf("Enter choice (1-11): ");
    fflush(stdin);
    scanf("%d", &choice);
    return choice;

} // FUNCTION getMenuChoice()

/* ----------------------------------------------------------------------------------- *
 * executeMenu() displays the log parser menu, gets a user input, and executes the
 * option requested by the user.
 * ----------------------------------------------------------------------------------- */
int executeMenu() {

    int choice = getMenuChoice();
    char sourceIP[100];
    int sessionID;

    switch (choice) {

        /* CASE 1 -- POPULATE SESSION ARRAY */
        case (1): {
	    resetArrays();
	    populateSessionArray();
	    initializeParserArrays();
            printSessionArray();
	    break;
	} // CASE 1

        /* CASE 2 -- FIND ENTRIES BY SOURCE IP ADDRESS */
        case (2): {
	    printf("Enter source IP: ");
            scanf("%s", sourceIP);
	    if (checkIPAddressValidity(sourceIP) < 1) {
	        printf("Invalid IP address. Returning to menu.\n");
	    } // IF
	    else {
	        quickSortSessionsSource(0, NUM_SESSIONS - 1);
	        printSessionsWithSource(sourceIP);
	    } // ELSE
            break;
        } // CASE 2

        /* CASE 3 -- FIND ENTRY BY ID */
        case (3): {
	    printf("Enter session ID: ");
            scanf("%d", &sessionID);
	    quickSortSessionsID(0, NUM_SESSIONS - 1);
	    printSessionWithID(sessionID);
            break;
        } // CASE 3
       
        /* CASE 4 -- SORT ENTRIES BY SOURCE IP ADDRESS */
        case (4): {
	    quickSortSessionsSource(0, NUM_SESSIONS - 1);
	    printSessionArray();
            break;
        } // CASE 4
        
        /* CASE 5 -- SORT ENTRIES BY ID */
        case (5): {
	    quickSortSessionsID(0, NUM_SESSIONS - 1);
	    printSessionArray();
            break;
        } // CASE 5

        /* CASE 6 -- SORT USERNAMES BY NUM OF OCCURRENCES */
        case (6): {
	    printUserArrayOcc();
            break;
        } // CASE 6

        /* CASE 7 -- SORT PASSWORDS BY NUM OF OCCURRENCES */
        case (7): {
            printPassArrayOcc();
	    break;
	} // CASE 7
        
	/* CASE 8 -- PRINT USERNAMES IN LEXICOGRAPHICAL ORDER */
        case (8): {
            printUserArrayLex();
	    break;
	} // CASE 8        

	/* CASE 9 -- PRINT PASSWORDS IN LEXICOGRAPHICAL ORDER */
        case (9): {
            printPassArrayLex();
	    break;
	} // CASE 9        

	/* CASE 10 -- DELETE ALL SESSIONS */
        case (10): {
	    resetArrays();
	    printf("All sessions deleted.\n");
            break;
	} // CASE 10  

	/* CASE 11 -- QUIT */
        case (11): {
            printf("Quitting program...\n");
            break;
	} // CASE 11  

	/* DEFAULT */
        default: {
            printf("Invalid input. Please try again.\n");
        } // DEFAULT

    } // SWITCH

    flush();
    return choice;

} // FUNCTION executeMenu()

/* ----------------------------------------------------------------------------------- *
 * MAIN
 * ----------------------------------------------------------------------------------- */
int main() {

    int currChoice;

    printBanner();

    do {
        printMenu();
        currChoice = executeMenu();
    } while (currChoice != 11);

    resetArrays();
    return 0;
} // MAIN

/* *********************************************************************************** */
