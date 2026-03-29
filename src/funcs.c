/*

    === funcs.c ===

    Authors: Arellano, Lajon Travis Escalo
             Villarin, Nicolai Paolo Balisado

    Core function definitions for Study Spawn Point
    A Gamified Learning Progress Tracker

*/

#include "prototypes.h"

/*

    @name   calculateXP()

    @brief  Calculates the XP earned based on hours studied
            using a tiered XP system

    @param  hoursStudied    Hours studied in the session

*/
int calculateXP(float hoursStudied) {

    int xp = 0;

    if (hoursStudied <= 0)
        return 0;

    else if (hoursStudied < 1.0)
        xp = (int)(hoursStudied * 50);

    else if (hoursStudied < 2.0)
        xp = (int)(hoursStudied * 75) + 25;

    else if (hoursStudied < 4.0)
        xp = (int)(hoursStudied * 100) + 50;

    else
        xp = (int)(hoursStudied * 100) + 100;

    return xp;
}

/*

    @name   calculateStats()

    @brief  Calculates and displays total hours, average hours,
            and total XP for a specific student or all students.
            Pass NULL for username to calculate for all students

    @param  records       Array of StudyRecord structs
    @param  recordCount   Number of records in the array
    @param  username      Username to filter by, NULL for all
    @param  totalHours    Pointer to store total hours
    @param  avgHours      Pointer to store average hours
    @param  totalXP       Pointer to store total XP

*/
void calculateStats(StudyRecord records[], int recordCount, const char *username,
                    float *totalHours, float *avgHours, int *totalXP) {

    *totalHours = 0;
    *avgHours = 0;
    *totalXP = 0;

    int recordFound = 0;

    for (int i = 0; i < recordCount; i++) {

        if (username != NULL && strcmp(records[i].ownerUsername, username) != 0)
            continue;

        *totalHours += records[i].hoursStudied;
        *totalXP += records[i].xpEarned;
        recordFound++;
    }

    if (recordFound == 0) {
        printf("\nNo records found.\n\n");
        return;
    }

    *avgHours = *totalHours / recordFound;

    if (username != NULL)
        printf("\n--- Stats for %s ---\n\n", username);

    else
        printf("\n--- Stats for All Students ---\n\n");

    printf("Total Records  : %d\n", recordFound);
    printf("Total Hours    : %.2f\n", *totalHours);
    printf("Average Hours  : %.2f\n", *avgHours);
    printf("Total XP       : %d\n\n", *totalXP);
}

/*

    @name   listRecords()

    @brief  Displays study records based on viewer role.
            Admins see all records, students see only their own,
            guests see all records without full names

    @param  records           Array of StudyRecord structs
    @param  recordCount       Number of records in the array
    @param  viewerRole        Role of the viewer
    @param  filterUsername    Username to filter by, NULL for all

*/
void listRecords(StudyRecord records[], int recordCount, int viewerRole,
                 const char *filterUsername) {

    if (recordCount == 0) {
        printf("\nNo records in database.\n\n");
        return;
    }

    int found = 0;

    printf("\n--- Study Records ---\n\n");

    for (int i = 0; i < recordCount; i++) {

        if (filterUsername != NULL &&
            strcmp(records[i].ownerUsername, filterUsername) != 0)
            continue;

        printf("Record ID  : %d\n", records[i].recordID);
        printf("Owner      : %s\n", records[i].ownerUsername);
        printf("Subject    : %s\n", records[i].subject);
        printf("Hours      : %.2f\n", records[i].hoursStudied);
        printf("XP Earned  : %d\n", records[i].xpEarned);
        printf("Date       : %02d/%02d/%04d\n",
               records[i].session_date.month,
               records[i].session_date.day,
               records[i].session_date.year);
        printf("\n");

        found++;
    }

    if (found == 0)
        printf("\nNo records found.\n\n");
}

/*

    @name   verify()

    @brief  Verifies the inputted username and password against
            the users array. Returns the index of the matched
            account or -1 if no match found

    @param  users           Array of UserAccount structs
    @param  userCount       Number of users in the array
    @param  inputUsername   Username entered by the user
    @param  inputPassword   Password entered by the user

*/
int verify(UserAccount users[], int userCount, const char *inputUsername,
           const char *inputPassword) {

    if (userCount == 0) {
        printf("\nNo accounts in database. Create a new account.\n\n");
        return -1;
    }

    char passKey[MAX + 2];
    snprintf(passKey, sizeof(passKey), "%s%c", inputUsername, MASTER_KEY);

    unsigned char encryptedPass[MAX];
    strncpy((char *)encryptedPass, inputPassword, MAX - 1);
    encrypt_decrypt(encryptedPass, strlen(inputPassword), passKey);

    int found = 0;

    for (int i = 0; i < userCount && found == 0; i++) {

        if (strcmp(users[i].username, inputUsername) == 0) {

            if (strcmp((char *)users[i].password, (char *)encryptedPass) == 0) {
                system("cls");
                printf("\nWelcome, %s!\n\n", users[i].username);
                found = i + 1;
            }
        }
    }

    if (found == 0) {
        printf("\nSign-in failed.\n\n");
        return -1;
    }

    return found - 1;
}

/*

    @name   signin()

    @brief  Prompts the user to enter their username and password
            then calls verify() to check credentials against
            the users array

    @param  users           Array of UserAccount structs
    @param  userCount       Number of users in the array
    @param  loggedInIndex   Pointer to store the index of the
                            matched account

*/
void signin(UserAccount users[], int userCount, int *loggedInIndex) {

    char tempUsername[MAX];
    char tempPassword[MAX];

    printf("\nSign In!\n");
    printf("Enter username: ");
    scanf("%99s", tempUsername);
    clearStdin();

    printf("Enter password: ");
    scanf("%99s", tempPassword);
    clearStdin();

    *loggedInIndex = verify(users, userCount, tempUsername, tempPassword);
}

/*

    @name   create_acc()

    @brief  Creates a new admin or student account. Admin accounts
            require an admin key. Student accounts require a full
            name. Passwords and full names are encrypted before
            storing

    @param  users       Array of UserAccount structs
    @param  userCount   Pointer to the number of users

*/
int create_acc(UserAccount users[], int *userCount) {

    char accChoice;
    int dupe = 0;
    int acc_type = 0;

    printf("\nCreate an Account!\n\n");

    do {
        printf("Are you creating an admin account? Y/N ");
        scanf(" %c", &accChoice);
        clearStdin();

        if (accChoice != 'Y' && accChoice != 'y' &&
            accChoice != 'N' && accChoice != 'n')
            printf("\nInvalid input, please try again.\n\n");

    } while (accChoice != 'Y' && accChoice != 'y' &&
             accChoice != 'N' && accChoice != 'n');

    if (accChoice == 'Y' || accChoice == 'y') {

        char keyInput[MAX];
        acc_type = ROLE_ADMIN;

        printf("Enter admin key: ");
        scanf("%99s", keyInput);
        clearStdin();

        if (strcmp(keyInput, ADMIN_KEY) != 0) {
            printf("\nWrong admin key. Access denied.\n\n");
            return -1;
        }

        printf("\nAdmin Access Granted!\n\n");
        printf("Enter username: ");
        scanf("%99s", users[*userCount].username);
        clearStdin();

        for (int i = 0; i < *userCount; i++) {
            if (strcmp(users[i].username, users[*userCount].username) == 0)
                dupe++;
        }

        if (dupe > 0) {
            printf("\nUsername already exists. Please try again.\n\n");
            return -1;
        }

        char tempPass[MAX];
        printf("Enter password: ");
        scanf("%99s", tempPass);
        clearStdin();

        users[*userCount].pass_len = strlen(tempPass);
        strncpy((char *)users[*userCount].password, tempPass, MAX - 1);

        char passKey[MAX + 2];
        snprintf(passKey, sizeof(passKey), "%s%c",
                 users[*userCount].username, MASTER_KEY);
        encrypt_decrypt(users[*userCount].password,
                        users[*userCount].pass_len, passKey);

        users[*userCount].role = ROLE_ADMIN;
        users[*userCount].userID = *userCount + 1;
        users[*userCount].fullName_len = 0;

        printf("\nAdmin account created successfully!\n\n");
    }

    else {

        acc_type = ROLE_STUDENT;

        printf("\nEnter username: ");
        scanf("%99s", users[*userCount].username);
        clearStdin();

        for (int i = 0; i < *userCount; i++) {
            if (strcmp(users[i].username, users[*userCount].username) == 0)
                dupe++;
        }

        if (dupe > 0) {
            printf("\nUsername already exists. Please try again.\n\n");
            return -1;
        }

        char tempName[MAX_FULLNAME];
        printf("Enter full name: ");
        scanf(" %99[^\n]", tempName);
        clearStdin();

        users[*userCount].fullName_len = strlen(tempName);
        strncpy((char *)users[*userCount].fullName, tempName, MAX_FULLNAME - 1);

        char tempPass[MAX];
        printf("Enter password: ");
        scanf("%99s", tempPass);
        clearStdin();

        users[*userCount].pass_len = strlen(tempPass);
        strncpy((char *)users[*userCount].password, tempPass, MAX - 1);

        char passKey[MAX + 2];
        snprintf(passKey, sizeof(passKey), "%s%c",
                 users[*userCount].username, MASTER_KEY);
        encrypt_decrypt(users[*userCount].password,
                        users[*userCount].pass_len, passKey);

        char nameKey[MAX * 2 + 2];
        snprintf(nameKey, sizeof(nameKey), "%s%s%c",
                 users[*userCount].username, tempPass, MASTER_KEY);
        encrypt_decrypt(users[*userCount].fullName,
                        users[*userCount].fullName_len, nameKey);

        users[*userCount].role = ROLE_STUDENT;
        users[*userCount].userID = *userCount + 1;

        printf("\nStudent account created successfully!\n\n");
        printf("Go back to the menu to sign in!\n\n");
    }

    (*userCount)++;
    return acc_type;
}

/*

    @name   reset_password()

    @brief  Resets the password of a user account after
            verifying the admin key. New password is encrypted
            before storing

    @param  users       Array of UserAccount structs
    @param  userCount   Number of users in the array
    @param  userIndex   Index of the account to reset
    @param  adminKey    Admin key to verify

*/
int reset_password(UserAccount users[], int userCount, int userIndex,
                   const char *adminKey) {

    char keyInput[MAX];

    printf("\nEnter admin key to reset password: ");
    scanf("%99s", keyInput);
    clearStdin();

    if (strcmp(keyInput, adminKey) != 0) {
        printf("\nWrong admin key. Access denied.\n\n");
        return -1;
    }

    char tempPass[MAX];
    printf("\nEnter new password for %s: ", users[userIndex].username);
    scanf("%99s", tempPass);
    clearStdin();

    users[userIndex].pass_len = strlen(tempPass);
    strncpy((char *)users[userIndex].password, tempPass, MAX - 1);

    char passKey[MAX + 2];
    snprintf(passKey, sizeof(passKey), "%s%c",
             users[userIndex].username, MASTER_KEY);
    encrypt_decrypt(users[userIndex].password,
                    users[userIndex].pass_len, passKey);

    printf("\nPassword reset successfully!\n\n");
    return 1;
}

/*

    @name   addRecord()

    @brief  Prompts the logged in user to enter a new study
            record including subject name, hours studied, and
            date. XP is calculated automatically based on
            hours studied using the tiered XP system.
            Record ID is assigned based on current record count
            and owner username is set from the logged in user

    @param  records         Array of StudyRecord structs
    @param  recordCount     Pointer to the number of records
    @param  users           Array of UserAccount structs
    @param  loggedInIndex   Index of the logged in user

*/
void addRecord(StudyRecord records[], int *recordCount, UserAccount users[],
               int loggedInIndex) {

    printf("\n--- Add Study Record ---\n\n");

    printf("Enter subject name: ");
    scanf("%49s", records[*recordCount].subject);
    clearStdin();

    float hours = 0;
    do {
        printf("Enter hours studied: ");
        scanf("%f", &hours);
        clearStdin();

        if (hours <= 0)
            printf("\nHours must be greater than 0. Please try again.\n\n");

    } while (hours <= 0);

    records[*recordCount].hoursStudied = hours;
    records[*recordCount].xpEarned = calculateXP(hours);

    int month, day, year;

    do {
        printf("Enter month (1-12): ");
        scanf("%d", &month);
        clearStdin();

        if (month < 1 || month > 12)
            printf("\nInvalid month. Please try again.\n\n");

    } while (month < 1 || month > 12);

    do {
        printf("Enter day (1-31): ");
        scanf("%d", &day);
        clearStdin();

        if (day < 1 || day > 31)
            printf("\nInvalid day. Please try again.\n\n");

    } while (day < 1 || day > 31);

    do {
        printf("Enter year: ");
        scanf("%d", &year);
        clearStdin();

        if (year < 1900 || year > 9999)
            printf("\nInvalid year. Please try again.\n\n");

    } while (year < 1900 || year > 9999);

    records[*recordCount].session_date.month = month;
    records[*recordCount].session_date.day = day;
    records[*recordCount].session_date.year = year;

    strncpy(records[*recordCount].ownerUsername,
            users[loggedInIndex].username, MAX - 1);

    records[*recordCount].recordID = *recordCount + 1;

    (*recordCount)++;

    printf("\nRecord added successfully!\n");
    printf("XP Earned: %d\n\n", records[*recordCount - 1].xpEarned);
}

/*

    @name   editRecord()

    @brief  Edits an existing study record by record ID.
            Displays current values before prompting for
            new values. XP is recalculated after editing

    @param  records       Array of StudyRecord structs
    @param  recordCount   Number of records in the array
    @param  recordID      ID of the record to edit

*/
void editRecord(StudyRecord records[], int recordCount, int recordID) {

    int index = -1;

    for (int i = 0; i < recordCount; i++) {
        if (records[i].recordID == recordID) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nRecord ID %d not found.\n\n", recordID);
        return;
    }

    printf("\n--- Edit Record %d ---\n\n", recordID);
    printf("Current Subject  : %s\n", records[index].subject);
    printf("Current Hours    : %.2f\n", records[index].hoursStudied);
    printf("Current XP       : %d\n", records[index].xpEarned);
    printf("Current Date     : %02d/%02d/%04d\n\n",
           records[index].session_date.month,
           records[index].session_date.day,
           records[index].session_date.year);

    printf("Enter new subject name: ");
    scanf("%49s", records[index].subject);
    clearStdin();

    float hours = 0;
    do {
        printf("Enter new hours studied: ");
        scanf("%f", &hours);
        clearStdin();

        if (hours <= 0)
            printf("\nHours must be greater than 0. Please try again.\n\n");

    } while (hours <= 0);

    records[index].hoursStudied = hours;
    records[index].xpEarned = calculateXP(hours);

    int month, day, year;

    do {
        printf("Enter new month (1-12): ");
        scanf("%d", &month);
        clearStdin();

        if (month < 1 || month > 12)
            printf("\nInvalid month. Please try again.\n\n");

    } while (month < 1 || month > 12);

    do {
        printf("Enter new day (1-31): ");
        scanf("%d", &day);
        clearStdin();

        if (day < 1 || day > 31)
            printf("\nInvalid day. Please try again.\n\n");

    } while (day < 1 || day > 31);

    do {
        printf("Enter new year: ");
        scanf("%d", &year);
        clearStdin();

        if (year < 1900 || year > 9999)
            printf("\nInvalid year. Please try again.\n\n");

    } while (year < 1900 || year > 9999);

    records[index].session_date.month = month;
    records[index].session_date.day = day;
    records[index].session_date.year = year;

    printf("\nRecord updated successfully!\n");
    printf("New XP Earned: %d\n\n", records[index].xpEarned);
}

/*

    @name   deleteRecord()

    @brief  Deletes a study record by record ID by shifting
            all records after it left by one position

    @param  records       Array of StudyRecord structs
    @param  recordCount   Pointer to the number of records
    @param  recordID      ID of the record to delete

*/
void deleteRecord(StudyRecord records[], int *recordCount, int recordID) {

    int index = -1;

    for (int i = 0; i < *recordCount; i++) {
        if (records[i].recordID == recordID) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nRecord ID %d not found.\n\n", recordID);
        return;
    }

    for (int i = index; i < *recordCount - 1; i++)
        records[i] = records[i + 1];

    (*recordCount)--;

    printf("\nRecord ID %d deleted successfully.\n\n", recordID);
}
