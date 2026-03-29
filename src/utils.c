/*

    === utils.c ===

    Authors: Arellano, Lajon Travis Escalo
             Villarin, Nicolai Paolo Balisado

    Utility, encryption, file handling, sort, search,
    leaderboard, and display functions for Study Spawn Point

*/

#include "prototypes.h"

/*

    @name   clearStdin()

    @brief  Clears the input buffer to prevent leftover
            characters from affecting subsequent scanf calls

*/
void clearStdin() {

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

}

/*

    @name   encrypt_decrypt()

    @brief  Encrypts or decrypts a field using XOR encryption
            Since XOR is self-inverse, the same function is
            used for both encrypting and decrypting

    @param  field       The field to encrypt or decrypt
    @param  field_len   The length of the field
    @param  key         The key to use for encryption

*/
void encrypt_decrypt(unsigned char *field, int field_len, const char *key) {

    int key_len = strlen(key);

    for (int i = 0; i < field_len; i++)
        field[i] = field[i] ^ (unsigned char)key[i % key_len];

}

/*

    @name   loadUsers()

    @brief  Loads all user accounts from users.txt into
            the users array. If the file does not exist,
            userCount is set to 0

    @param  users       Array of UserAccount structs to populate
    @param  userCount   Pointer to the number of users loaded

*/
void loadUsers(UserAccount users[], int *userCount) {

    FILE *fp = fopen("users.txt", "r");

    if (fp == NULL) {
        *userCount = 0;
        return;
    }

    char line[512];
    *userCount = 0;

while (fgets(line, sizeof(line), fp) != NULL && *userCount < MAX_USERS) {

        line[strcspn(line, "\n")] = '\0';

        char *token;

        token = strtok(line, "|");
        if (token == NULL) continue;
        users[*userCount].userID = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy(users[*userCount].username, token, MAX - 1);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy((char *)users[*userCount].fullName, token, MAX_FULLNAME - 1);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy((char *)users[*userCount].password, token, MAX - 1);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        users[*userCount].pass_len = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        users[*userCount].fullName_len = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        users[*userCount].role = atoi(token);

        (*userCount)++;
    }

    fclose(fp);
}

/*

    @name   saveUsers()

    @brief  Saves all user accounts from the users array
            into users.txt, overwriting the existing file

    @param  users       Array of UserAccount structs to save
    @param  userCount   Number of users to save

*/
void saveUsers(UserAccount users[], int userCount) {

    FILE *fp = fopen("users.txt", "w");

    if (fp == NULL) {
        printf("\nError saving users.\n\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(fp, "%d|%s|%s|%s|%d|%d|%d\n",
                users[i].userID,
                users[i].username,
                (char *)users[i].fullName,
                (char *)users[i].password,
                users[i].pass_len,
                users[i].fullName_len,
                users[i].role);
    }

    fclose(fp);
}

/*

    @name   loadRecords()

    @brief  Loads all study records from records.txt into
            the records array. If the file does not exist,
            recordCount is set to 0

    @param  records       Array of StudyRecord structs to populate
    @param  recordCount   Pointer to the number of records loaded

*/
void loadRecords(StudyRecord records[], int *recordCount) {

    FILE *fp = fopen("records.txt", "r");

    if (fp == NULL) {
        *recordCount = 0;
        return;
    }

    char line[512];
    *recordCount = 0;

while (fgets(line, sizeof(line), fp) != NULL && *recordCount < MAX_RECORDS) {

        line[strcspn(line, "\n")] = '\0';

        char *token;
        token = strtok(line, "|");
        if (token == NULL) continue;
        records[*recordCount].recordID = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy(records[*recordCount].ownerUsername, token, MAX - 1);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy(records[*recordCount].subject, token, MAX_SUBJECT - 1);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        records[*recordCount].hoursStudied = atof(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        records[*recordCount].xpEarned = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        records[*recordCount].session_date.month = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        records[*recordCount].session_date.day = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        records[*recordCount].session_date.year = atoi(token);

        (*recordCount)++;
    }

    fclose(fp);
}

/*

    @name   saveRecords()

    @brief  Saves all study records from the records array
            into records.txt, overwriting the existing file

    @param  records       Array of StudyRecord structs to save
    @param  recordCount   Number of records to save

*/
void saveRecords(StudyRecord records[], int recordCount) {

    FILE *fp = fopen("records.txt", "w");

    if (fp == NULL) {
        printf("\nError saving records.\n\n");
        return;
    }

    for (int i = 0; i < recordCount; i++) {
        fprintf(fp, "%d|%s|%s|%.2f|%d|%d|%d|%d\n",
                records[i].recordID,
                records[i].ownerUsername,
                records[i].subject,
                records[i].hoursStudied,
                records[i].xpEarned,
                records[i].session_date.month,
                records[i].session_date.day,
                records[i].session_date.year);
    }

    fclose(fp);
}

/*

    @name   extractSubjectNames()

    @brief  Extracts subject names from the records array
            into a 2D array for use in sort and search

    @param  records         Array of StudyRecord structs
    @param  recordCount     Number of records in the array
    @param  subjectNames    2D array to store subject names

*/
void extractSubjectNames(StudyRecord records[], int recordCount,
                         char subjectNames[][MAX_SUBJECT]) {

    for (int i = 0; i < recordCount; i++)
        strncpy(subjectNames[i], records[i].subject, MAX_SUBJECT - 1);

}

/*

    @name   selectionSortBySubject()

    @brief  Sorts the records array and subjectNames 2D array
            alphabetically by subject name using selection sort

    @param  records         Array of StudyRecord structs to sort
    @param  recordCount     Number of records in the array
    @param  subjectNames    2D array of subject names to sort in sync

*/
void selectionSortBySubject(StudyRecord records[], int recordCount,
                            char subjectNames[][MAX_SUBJECT]) {

    int minIndex;
    StudyRecord tempRecord;
    char tempSubject[MAX_SUBJECT];

    for (int i = 0; i < recordCount - 1; i++) {

        minIndex = i;

        for (int j = i + 1; j < recordCount; j++) {
            if (strcmp(subjectNames[j], subjectNames[minIndex]) < 0)
                minIndex = j;
        }

        if (minIndex != i) {

            tempRecord = records[i];
            records[i] = records[minIndex];
            records[minIndex] = tempRecord;

            strncpy(tempSubject, subjectNames[i], MAX_SUBJECT - 1);
            strncpy(subjectNames[i], subjectNames[minIndex], MAX_SUBJECT - 1);
            strncpy(subjectNames[minIndex], tempSubject, MAX_SUBJECT - 1);
        }
    }
}

/*

    @name   selectionSortByXP()

    @brief  Sorts the records array by XP earned in
            descending order using selection sort

    @param  records       Array of StudyRecord structs to sort
    @param  recordCount   Number of records in the array

*/
void selectionSortByXP(StudyRecord records[], int recordCount) {

    int maxIndex;
    StudyRecord tempRecord;

    for (int i = 0; i < recordCount - 1; i++) {

        maxIndex = i;

        for (int j = i + 1; j < recordCount; j++) {
            if (records[j].xpEarned > records[maxIndex].xpEarned)
                maxIndex = j;
        }

        if (maxIndex != i) {
            tempRecord = records[i];
            records[i] = records[maxIndex];
            records[maxIndex] = tempRecord;
        }
    }
}

/*

    @name   binarySearchBySubject()

    @brief  Searches the subjectNames 2D array for a subject
            using binary search. Array must be sorted first
            before calling this function

    @param  subjectNames    2D array of subject names to search
    @param  recordCount     Number of records in the array
    @param  subject         Subject name to search for

*/
int binarySearchBySubject(char subjectNames[][MAX_SUBJECT], int recordCount,
                          const char *subject) {

    int low = 0;
    int high = recordCount - 1;
    int mid;

    while (low <= high) {

        mid = (low + high) / 2;

        int cmp = strcmp(subjectNames[mid], subject);

        if (cmp == 0)
            return mid;

        else if (cmp < 0)
            low = mid + 1;

        else
            high = mid - 1;
    }

    return -1;
}

/*

    @name   buildLeaderboard()

    @brief  Builds the leaderboard by aggregating XP and hours
            per student, sorting by XP descending, and taking
            the top MAX_LB entries. Tie-breaking is done by
            total hours then userID

    @param  users               Array of UserAccount structs
    @param  userCount           Number of users in the array
    @param  records             Array of StudyRecord structs
    @param  recordCount         Number of records in the array
    @param  leaderboard         Array of LeaderboardEntry structs to populate
    @param  leaderboardCount    Pointer to the number of leaderboard entries

*/
void buildLeaderboard(UserAccount users[], int userCount,
                      StudyRecord records[], int recordCount,
                      LeaderboardEntry leaderboard[], int *leaderboardCount) {

    *leaderboardCount = 0;

    for (int i = 0; i < userCount; i++) {

        if (users[i].role != ROLE_STUDENT)
            continue;

        int totalXP = 0;
        float totalHours = 0;
        int recordFound = 0;

        for (int j = 0; j < recordCount; j++) {
            if (strcmp(records[j].ownerUsername, users[i].username) == 0) {
                totalXP += records[j].xpEarned;
                totalHours += records[j].hoursStudied;
                recordFound++;
            }
        }

        if (recordFound == 0)
            continue;

        strncpy(leaderboard[*leaderboardCount].username, users[i].username, MAX - 1);
        unsigned char tempName[MAX_FULLNAME]; 
        
        strncpy((char *)tempName, (char *)users[i].fullName, MAX_FULLNAME - 1);
        char nameKey[MAX * 2 + 2];
        snprintf(nameKey, sizeof(nameKey), "%s%s%c", users[i].username, (char *)users[i].password, MASTER_KEY);
        encrypt_decrypt(tempName, users[i].fullName_len, nameKey);
        
        strncpy(leaderboard[*leaderboardCount].fullName, (char *)tempName, MAX_FULLNAME - 1);

        leaderboard[*leaderboardCount].totalXP = totalXP;
        leaderboard[*leaderboardCount].totalHours = totalHours;
        leaderboard[*leaderboardCount].userID = users[i].userID;

        (*leaderboardCount)++;
    }

    for (int i = 0; i < *leaderboardCount - 1; i++) {

        int bestIndex = i;

        for (int j = i + 1; j < *leaderboardCount; j++) {

            int better = 0;

            if (leaderboard[j].totalXP > leaderboard[bestIndex].totalXP)
                better = 1;

            else if (leaderboard[j].totalXP == leaderboard[bestIndex].totalXP) {
                if (leaderboard[j].totalHours > leaderboard[bestIndex].totalHours)
                    better = 1;
                else if (leaderboard[j].totalHours == leaderboard[bestIndex].totalHours) {
                    if (leaderboard[j].userID < leaderboard[bestIndex].userID)
                        better = 1;
                }
            }

            if (better)
                bestIndex = j;
        }

        if (bestIndex != i) {
            LeaderboardEntry temp = leaderboard[i];
            leaderboard[i] = leaderboard[bestIndex];
            leaderboard[bestIndex] = temp;
        }
    }

    if (*leaderboardCount > MAX_LB)
        *leaderboardCount = MAX_LB;

    for (int i = 0; i < *leaderboardCount; i++)
        leaderboard[i].rank = i + 1;
}

/*

    @name   displayLeaderboard()

    @brief  Displays the top students in the leaderboard.
            Admins see username, full name, and total XP.
            Guests and students see username and total XP only

    @param  leaderboard         Array of LeaderboardEntry structs
    @param  leaderboardCount    Number of leaderboard entries
    @param  viewerRole          Role of the viewer

*/
void displayLeaderboard(LeaderboardEntry leaderboard[], int leaderboardCount,
                        int viewerRole) {

    if (leaderboardCount == 0) {
        printf("\nNo students available for leaderboard.\n\n");
        return;
    }

    printf("\n--- Top %d Students ---\n\n", leaderboardCount);

    if (viewerRole == ROLE_ADMIN)
        printf("%-6s %-20s %-20s %-10s\n", "RANK", "USERNAME", "FULL NAME", "TOTAL XP");

    else
        printf("%-6s %-20s %-10s\n", "RANK", "USERNAME", "TOTAL XP");

    printf("------------------------------------------------------\n");

    for (int i = 0; i < leaderboardCount; i++) {

        if (viewerRole == ROLE_ADMIN)
            printf("%-6d %-20s %-20s %-10d\n",
                   leaderboard[i].rank,
                   leaderboard[i].username,
                   leaderboard[i].fullName,
                   leaderboard[i].totalXP);

        else
            printf("%-6d %-20s %-10d\n",
                   leaderboard[i].rank,
                   leaderboard[i].username,
                   leaderboard[i].totalXP);
    }

    printf("\n");
}

/*

    @name   acc_list()

    @brief  Displays all user accounts in the database.
            Admins see username, full name, and role.
            Non-admins see username and role only

    @param  users       Array of UserAccount structs
    @param  userCount   Number of users in the array
    @param  viewerRole  Role of the viewer

*/
void acc_list(UserAccount users[], int userCount, int viewerRole) {

    if (userCount == 0) {
        printf("\nNo accounts in database.\n\n");
        return;
    }

    printf("\n--- Account List ---\n\n");

    for (int i = 0; i < userCount; i++) {

        printf("Account %d:\n", i + 1);
        printf("User ID  : %d\n", users[i].userID);
        printf("Username : %s\n", users[i].username);

        if (viewerRole == ROLE_ADMIN) {

            unsigned char tempName[MAX_FULLNAME];
            strncpy((char *)tempName, (char *)users[i].fullName, MAX_FULLNAME - 1);

            char nameKey[MAX * 2 + 2];
            snprintf(nameKey, sizeof(nameKey), "%s%s%c", users[i].username, (char *)users[i].password, MASTER_KEY);
            encrypt_decrypt(tempName, users[i].fullName_len, nameKey);

            printf("Full Name: %s\n", tempName);
        }

        printf("Role     : ");

        if (users[i].role == ROLE_ADMIN)
            printf("Admin\n");

        else if (users[i].role == ROLE_STUDENT)
            printf("Student\n");

        else
            printf("Guest\n");

        printf("\n");
    }
}
