/*

    === main.c ===

    Authors: Arellano, Lajon Travis Escalo
             Villarin, Nicolai Paolo Balisado

    Main file for Study Spawn Point
    A Gamified Learning Progress Tracker

*/

#include "prototypes.h"

int main() {

    UserAccount users[MAX_USERS];
    StudyRecord records[MAX_RECORDS];
    LeaderboardEntry leaderboard[MAX_LB];
    char subjectNames[MAX_RECORDS][MAX_SUBJECT];

    int userCount = 0;
    int recordCount = 0;
    int leaderboardCount = 0;
    int loggedInIndex = -1;

    int choice = 0;
    int menuChoice = 0;
    char backChoice = 'n';

    int editID = 0;
    int deleteID = 0;
    int resetIndex = 0;
    int sortChoice = 0;
    int searchResult = 0;
    char searchSubject[MAX_SUBJECT];

    float totalHours = 0;
    float avgHours = 0;
    int totalXP = 0;

    loadUsers(users, &userCount);
    loadRecords(records, &recordCount);

    do {

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
                loggedInIndex = -1;
                signin(users, userCount, &loggedInIndex);

                if (loggedInIndex == -1)
                    break;

                if (users[loggedInIndex].role == ROLE_ADMIN) {

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
                                listRecords(records, recordCount, ROLE_ADMIN, NULL);
                                break;

                            case 2:
                                addRecord(records, &recordCount, users, loggedInIndex);
                                saveRecords(records, recordCount);
                                break;

                            case 3:
                                listRecords(records, recordCount, ROLE_ADMIN, NULL);
                                printf("Enter Record ID to edit: ");
                                scanf("%d", &editID);
                                editRecord(records, recordCount, editID);
                                saveRecords(records, recordCount);
                                break;

                            case 4:
                                listRecords(records, recordCount, ROLE_ADMIN, NULL);
                                printf("Enter Record ID to delete: ");
                                scanf("%d", &deleteID);
                                deleteRecord(records, &recordCount, deleteID);
                                saveRecords(records, recordCount);
                                break;

                            case 5:
                                buildLeaderboard(users, userCount, records, recordCount, leaderboard, &leaderboardCount);
                                displayLeaderboard(leaderboard, leaderboardCount, ROLE_ADMIN);
                                break;

                            case 6:
                                calculateStats(records, recordCount, NULL, &totalHours, &avgHours, &totalXP);
                                break;

                            case 7:
                                printf("\nSort by:\n");
                                printf("[1] - Subject Name\n");
                                printf("[2] - XP Earned\n");
                                printf("Enter Choice: ");
                                scanf("%d", &sortChoice);

                                if (sortChoice == 1) {
                                    extractSubjectNames(records, recordCount, subjectNames);
                                    selectionSortBySubject(records, recordCount, subjectNames);
                                }
                                else if (sortChoice == 2)
                                    selectionSortByXP(records, recordCount);
                                else
                                    printf("\nInvalid input, please try again.\n\n");
                                break;

                            case 8:
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
                                acc_list(users, userCount, ROLE_ADMIN);
                                break;

                            case 10:
                                acc_list(users, userCount, ROLE_ADMIN);
                                printf("Enter User ID to reset password: ");
                                scanf("%d", &resetIndex);
                                reset_password(users, userCount, resetIndex, ADMIN_KEY);
                                saveUsers(users, userCount);
                                break;

                            case 11:
                                printf("\nSigning out...\n\n");
                                loggedInIndex = -1;
                                break;

                            default:
                                break;
                        }

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

                else if (users[loggedInIndex].role == ROLE_STUDENT) {

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
                                addRecord(records, &recordCount, users, loggedInIndex);
                                saveRecords(records, recordCount);
                                break;

                            case 2:
                                listRecords(records, recordCount, ROLE_STUDENT, users[loggedInIndex].username);
                                break;

                            case 3:
                                buildLeaderboard(users, userCount, records, recordCount, leaderboard, &leaderboardCount);
                                displayLeaderboard(leaderboard, leaderboardCount, ROLE_STUDENT);
                                break;

                            case 4:
                                calculateStats(records, recordCount, users[loggedInIndex].username, &totalHours, &avgHours, &totalXP);
                                break;

                            case 5:
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
                                reset_password(users, userCount, loggedInIndex, ADMIN_KEY);
                                saveUsers(users, userCount);
                                break;

                            case 7:
                                printf("\nSigning out...\n\n");
                                loggedInIndex = -1;
                                break;

                            default:
                                break;
                        }

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
                create_acc(users, &userCount);
                saveUsers(users, userCount);
                break;

            case 3:

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
                            listRecords(records, recordCount, ROLE_GUEST, NULL);
                            break;

                        case 2:
                            buildLeaderboard(users, userCount, records, recordCount, leaderboard, &leaderboardCount);
                            displayLeaderboard(leaderboard, leaderboardCount, ROLE_GUEST);
                            break;

                        case 3:
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
                            printf("\nExiting Guest Mode...\n\n");
                            break;

                        default:
                            break;
                    }

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
                saveUsers(users, userCount);
                saveRecords(records, recordCount);
                printf("\nExiting program...\n\n");
                break;

            default:
                break;
        }

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
