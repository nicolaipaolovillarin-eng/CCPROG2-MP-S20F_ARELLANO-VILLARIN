/*

    === main.c ===

    Authors: Arellano, Lajon Travis Escalo
             Villarin, Nicolai Paolo Balisado

    Main file for Study Spawn Point
    A Gamified Learning Progress Tracker

*/

#include "prototypes.h"

int main() {

    // arrays to store all users, records, and leaderboard entries
    UserAccount users[MAX_USERS];
    StudyRecord records[MAX_RECORDS];
    LeaderboardEntry leaderboard[MAX_LB];
    char subjectNames[MAX_RECORDS][MAX_SUBJECT];  // 2D array for sort and search

    // counters for each array
    int userCount = 0;
    int recordCount = 0;
    int leaderboardCount = 0;
    int loggedInIndex = -1;  // -1 means no user is logged in

    // menu navigation variables
    int choice = 0;
    int menuChoice = 0;
    char backChoice = 'n';  // controls whether to go back to menu

    // operation variables
    int editID = 0;       // record ID to edit
    int deleteID = 0;     // record ID to delete
    int resetIndex = 0;   // user ID to reset password
    int resetFound = 0;   // flag to check if user ID was found
    int sortChoice = 0;   // sort option choice
    int searchResult = 0; // index returned by binary search
    char searchSubject[MAX_SUBJECT];  // subject name to search for

    // statistics variables
    float totalHours = 0;
    float avgHours = 0;
    int totalXP = 0;

    // load existing data from files on startup
    loadUsers(users, &userCount);
    loadRecords(records, &recordCount);

    do {

        // main menu loop — keeps showing until valid input
        do {
            printf("Welcome to Study Spawn Point!\n");
            printf("[1] - Sign In\n");
            printf("[2] - Create an Account\n");
            printf("[3] - Continue as Guest\n");
            printf("[4] - Exit\n");
            printf("Enter Choice: ");
            scanf("%d", &choice);

            if (choice < 1 || choice > 4)
                printf("\nInvalid input, please try again.\n\n");

        } while (choice < 1 || choice > 4);

        switch (choice) {

            case 1:
                loggedInIndex = -1;  // reset before signing in
                signin(users, userCount, &loggedInIndex);

                // if sign in failed, break out of case 1
                if (loggedInIndex == -1)
                    break;

                // admin menu
                if (users[loggedInIndex].role == ROLE_ADMIN) {
                    backChoice = 'n';  // reset backChoice before entering menu
                    do {
                        printf("\n=== Admin Menu ===\n");
                        printf("[1] - View All Records\n");
                        printf("[2] - Add Record\n");
                        printf("[3] - Edit Record\n");
                        printf("[4] - Delete Record\n");
                        printf("[5] - View Leaderboard\n");
                        printf("[6] - View All Statistics\n");
                        printf("[7] - Sort Records\n");
                        printf("[8] - Search Records\n");
                        printf("[9] - Account List\n");
                        printf("[10] - Reset a Password\n");
                        printf("[11] - Sign Out\n");
                        printf("Enter Choice: ");
                        scanf("%d", &menuChoice);

                        if (menuChoice < 1 || menuChoice > 11) {
                            printf("\nInvalid input, please try again.\n\n");
                            continue;
                        }

                        switch (menuChoice) {

                            case 1:
                                // show all records to admin
                                listRecords(records, recordCount, ROLE_ADMIN, NULL);
                                break;

                            case 2:
                                // add a new record and save immediately
                                addRecord(records, &recordCount, users, loggedInIndex);
                                saveRecords(records, recordCount);
                                break;

                            case 3:
                                // show all records then prompt for ID to edit
                                listRecords(records, recordCount, ROLE_ADMIN, NULL);
                                printf("Enter Record ID to edit: ");
                                scanf("%d", &editID);
                                editRecord(records, recordCount, editID);
                                saveRecords(records, recordCount);
                                break;

                            case 4:
                                // show all records then prompt for ID to delete
                                listRecords(records, recordCount, ROLE_ADMIN, NULL);
                                printf("Enter Record ID to delete: ");
                                scanf("%d", &deleteID);
                                deleteRecord(records, &recordCount, deleteID);
                                saveRecords(records, recordCount);
                                break;

                            case 5:
                                // build and display leaderboard for admin
                                buildLeaderboard(users, userCount, records, recordCount, leaderboard, &leaderboardCount);
                                displayLeaderboard(leaderboard, leaderboardCount, ROLE_ADMIN);
                                break;

                            case 6:
                                // show statistics for all students
                                calculateStats(records, recordCount, NULL, &totalHours, &avgHours, &totalXP);
                                break;

                            case 7:
                                // sort records by subject or XP
                                printf("\nSort by:\n");
                                printf("[1] - Subject Name\n");
                                printf("[2] - XP Earned\n");
                                printf("Enter Choice: ");
                                scanf("%d", &sortChoice);

                                if (sortChoice == 1) {
                                    // extract subject names then sort alphabetically
                                    extractSubjectNames(records, recordCount, subjectNames);
                                    selectionSortBySubject(records, recordCount, subjectNames);
                                }
                                else if (sortChoice == 2)
                                    // sort by XP descending
                                    selectionSortByXP(records, recordCount);
                                else
                                    printf("\nInvalid input, please try again.\n\n");
                                break;

                            case 8:
                                // sort first then binary search by subject
                                extractSubjectNames(records, recordCount, subjectNames);
                                selectionSortBySubject(records, recordCount, subjectNames);
                                printf("Enter Subject to search: ");
                                scanf("%49s", searchSubject);
                                searchResult = binarySearchBySubject(subjectNames, recordCount, searchSubject);

                                if (searchResult == -1)
                                    printf("\nSubject not found.\n\n");
                                else
                                    printf("\nFound at Record ID: %d\n\n", records[searchResult].recordID);
                                break;

                            case 9:
                                // show all accounts to admin
                                acc_list(users, userCount, ROLE_ADMIN);
                                break;

                            case 10:
                                // show accounts then prompt for user ID to reset
                                acc_list(users, userCount, ROLE_ADMIN);
                                printf("Enter User ID to reset password: ");
                                scanf("%d", &resetIndex);
                                resetFound = 0;  // reset flag before searching

                                // find array index matching the entered user ID
                                for (int i = 0; i < userCount; i++) {
                                    if (users[i].userID == resetIndex) {
                                        reset_password(users, userCount, i, ADMIN_KEY);
                                        saveUsers(users, userCount);
                                        resetFound = 1;
                                        break;
                                    }
                                }

                                if (resetFound == 0)
                                    printf("\nUser ID %d not found.\n\n", resetIndex);
                                break;

                            case 11:
                                // sign out and reset logged in index
                                printf("\nSigning out...\n\n");
                                loggedInIndex = -1;
                                break;

                            default:
                                break;
                        }

                        // ask to go back to admin menu after each action
                        if (menuChoice != 11) {
                            do {
                                printf("Go back to Admin Menu? Y/N ");
                                scanf(" %c", &backChoice);

                                if (backChoice != 'Y' && backChoice != 'y' &&
                                    backChoice != 'N' && backChoice != 'n')
                                    printf("\nInvalid input, please try again.\n\n");

                            } while (backChoice != 'Y' && backChoice != 'y' &&
                                     backChoice != 'N' && backChoice != 'n');

                            system("cls");
                        }

                    } while (menuChoice != 11 && (backChoice == 'Y' || backChoice == 'y'));

                }

                // student menu
                else if (users[loggedInIndex].role == ROLE_STUDENT) {
                    backChoice = 'n';  // reset backChoice before entering menu
                    do {
                        printf("\n=== Student Menu ===\n");
                        printf("[1] - Add Study Record\n");
                        printf("[2] - View My Records\n");
                        printf("[3] - View Leaderboard\n");
                        printf("[4] - View My Statistics\n");
                        printf("[5] - Search Records\n");
                        printf("[6] - Reset My Password\n");
                        printf("[7] - Sign Out\n");
                        printf("Enter Choice: ");
                        scanf("%d", &menuChoice);

                        if (menuChoice < 1 || menuChoice > 7) {
                            printf("\nInvalid input, please try again.\n\n");
                            continue;
                        }

                        switch (menuChoice) {

 case 1:
                                // add a new record and save immediately
                                addRecord(records, &recordCount, users, loggedInIndex);
                                saveRecords(records, recordCount);
                                break;

                            case 2:
                                // show only this student's own records
                                listRecords(records, recordCount, ROLE_STUDENT, users[loggedInIndex].username);
                                break;

                            case 3:
                                // build and display leaderboard for student
                                buildLeaderboard(users, userCount, records, recordCount, leaderboard, &leaderboardCount);
                                displayLeaderboard(leaderboard, leaderboardCount, ROLE_STUDENT);
                                break;

                            case 4:
                                // show statistics for this student only
                                calculateStats(records, recordCount, users[loggedInIndex].username, &totalHours, &avgHours, &totalXP);
                                break;

                            case 5:
                                // sort first then binary search by subject
                                extractSubjectNames(records, recordCount, subjectNames);
                                selectionSortBySubject(records, recordCount, subjectNames);
                                printf("Enter Subject to search: ");
                                scanf("%49s", searchSubject);
                                searchResult = binarySearchBySubject(subjectNames, recordCount, searchSubject);

                                if (searchResult == -1)
                                    printf("\nSubject not found.\n\n");
                                else
                                    printf("\nFound at Record ID: %d\n\n", records[searchResult].recordID);
                                break;

                            case 6:
                                // student resets own password using admin key
                                reset_password(users, userCount, loggedInIndex, ADMIN_KEY);
                                saveUsers(users, userCount);
                                break;

                            case 7:
                                // sign out and reset logged in index
                                printf("\nSigning out...\n\n");
                                loggedInIndex = -1;
                                break;

                            default:
                                break;
                        }

                        // ask to go back to student menu after each action
                        if (menuChoice != 7) {
                            do {
                                printf("Go back to Student Menu? Y/N ");
                                scanf(" %c", &backChoice);

                                if (backChoice != 'Y' && backChoice != 'y' &&
                                    backChoice != 'N' && backChoice != 'n')
                                    printf("\nInvalid input, please try again.\n\n");

                            } while (backChoice != 'Y' && backChoice != 'y' &&
                                     backChoice != 'N' && backChoice != 'n');

                            system("cls");
                        }

                    } while (menuChoice != 7 && (backChoice == 'Y' || backChoice == 'y'));

                }

                break;

            case 2:
                // create a new account and save immediately
                create_acc(users, &userCount);
                saveUsers(users, userCount);
                break;

            case 3:
                backChoice = 'n';  // reset backChoice before entering menu
                do {
                    printf("\n=== Guest Menu ===\n");
                    printf("[1] - View All Records\n");
                    printf("[2] - View Leaderboard\n");
                    printf("[3] - Search Records\n");
                    printf("[4] - Exit Guest Mode\n");
                    printf("Enter Choice: ");
                    scanf("%d", &menuChoice);

                    if (menuChoice < 1 || menuChoice > 4) {
                        printf("\nInvalid input, please try again.\n\n");
                        continue;
                    }

                    switch (menuChoice) {

                        case 1:
                            // show all records to guest with limited details
                            listRecords(records, recordCount, ROLE_GUEST, NULL);
                            break;

                        case 2:
                            // build and display leaderboard for guest
                            buildLeaderboard(users, userCount, records, recordCount, leaderboard, &leaderboardCount);
                            displayLeaderboard(leaderboard, leaderboardCount, ROLE_GUEST);
                            break;

                        case 3:
                            // sort first then binary search by subject
                            extractSubjectNames(records, recordCount, subjectNames);
                            selectionSortBySubject(records, recordCount, subjectNames);
                            printf("Enter Subject to search: ");
                            scanf("%49s", searchSubject);
                            searchResult = binarySearchBySubject(subjectNames, recordCount, searchSubject);

                            if (searchResult == -1)
                                printf("\nSubject not found.\n\n");
                            else
                                printf("\nFound at Record ID: %d\n\n", records[searchResult].recordID);
                            break;

                        case 4:
                            // exit guest mode
                            printf("\nExiting Guest Mode...\n\n");
                            break;

                        default:
                            break;
                    }

                    // ask to go back to guest menu after each action
                    if (menuChoice != 4) {
                        do {
                            printf("Go back to Guest Menu? Y/N ");
                            scanf(" %c", &backChoice);

                            if (backChoice != 'Y' && backChoice != 'y' &&
                                backChoice != 'N' && backChoice != 'n')
                                printf("\nInvalid input, please try again.\n\n");

                        } while (backChoice != 'Y' && backChoice != 'y' &&
                                 backChoice != 'N' && backChoice != 'n');

                        system("cls");
                    }

                } while (menuChoice != 4 && (backChoice == 'Y' || backChoice == 'y'));

                break;

            case 4:
                // save all data before exiting
                saveUsers(users, userCount);
                saveRecords(records, recordCount);
                printf("\nExiting program...\n\n");
                break;

            default:
                break;
        }

        // ask to go back to main menu after each action
        if (choice != 4) {
            do {
                printf("Go back to Main Menu? Y/N ");
                scanf(" %c", &backChoice);

                if (backChoice != 'Y' && backChoice != 'y' &&
                    backChoice != 'N' && backChoice != 'n')
                    printf("\nInvalid input, please try again.\n\n");

            } while (backChoice != 'Y' && backChoice != 'y' &&
                     backChoice != 'N' && backChoice != 'n');

            system("cls");
        }

    } while (choice != 4 && (backChoice == 'Y' || backChoice == 'y'));

    return 0;

}
