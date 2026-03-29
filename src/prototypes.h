/*

    === prototypes.h ===

    Authors: Arellano, Lajon Travis Escalo
             Villarin, Nicolai Paolo Balisado

    Header file for Study Spawn Point
    Contains all struct definitions, constants, and function prototypes

*/

#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Size Constants */
#define MAX          100    // maximum size for strings and arrays
#define MAX_FULLNAME 100    // maximum size for full name strings
#define MAX_SUBJECT  50     // maximum size for subject name strings
#define MAX_USERS    100    // maximum number of user accounts
#define MAX_RECORDS  1000   // maximum number of study records
#define MAX_LB       3      // maximum number of leaderboard entries

/* Key Constants */
#define ADMIN_KEY    "admin_123"  // key required to create admin accounts
#define MASTER_KEY   '!'          // master key character appended to encryption keys

/* Role Constants */
#define ROLE_GUEST   0  // guest role — no account required
#define ROLE_ADMIN   1  // admin role — full access
#define ROLE_STUDENT 2  // student role — limited access

/* Date Struct */
typedef struct
{
    int month;  // month of the session (1-12)
    int day;    // day of the session (1-31)
    int year;   // year of the session (1900-9999)
} date;

/* User Account Struct */
typedef struct
{
    int           userID;                   // unique user identifier
    char          username[MAX];            // username for login
    unsigned char fullName[MAX_FULLNAME];   // encrypted full name (students only)
    unsigned char password[MAX];            // encrypted password
    int           pass_len;                 // length of password before encryption
    int           fullName_len;             // length of full name before encryption
    int           role;                     // role of the user (guest, admin, student)
} UserAccount;

/* Study Record Struct */
typedef struct
{
    int   recordID;              // unique record identifier
    char  ownerUsername[MAX];    // username of the record owner
    char  subject[MAX_SUBJECT];  // subject name of the study session
    float hoursStudied;          // hours studied in the session
    int   xpEarned;              // XP earned in the session
    date  session_date;          // date of the study session
} StudyRecord;

/* Leaderboard Entry Struct */
typedef struct
{
    char  username[MAX];          // username of the student
    char  fullName[MAX_FULLNAME]; // decrypted full name of the student
    int   totalXP;                // total XP earned by the student
    float totalHours;             // total hours studied by the student
    int   userID;                 // unique user identifier for tie-breaking
    int   rank;                   // rank of the student in the leaderboard
} LeaderboardEntry;

/* Authentication Functions */
void signin(UserAccount users[], int userCount, int *loggedInIndex);                                                          // prompts user to sign in
int  create_acc(UserAccount users[], int *userCount);                                                                         // creates a new user account
void encrypt_decrypt(unsigned char *field, int field_len, const char *key);                                                   // encrypts or decrypts a field
int  verify(UserAccount users[], int userCount, const char *inputUsername, const char *inputPassword);                        // verifies login credentials
int  reset_password(UserAccount users[], int userCount, int userIndex, const char *adminKey);                                 // resets a user password

/* File Handling Functions */
void loadUsers(UserAccount users[], int *userCount);        // loads users from users.txt
void saveUsers(UserAccount users[], int userCount);         // saves users to users.txt
void loadRecords(StudyRecord records[], int *recordCount);  // loads records from records.txt
void saveRecords(StudyRecord records[], int recordCount);   // saves records to records.txt

/* Study Record Functions */
void addRecord(StudyRecord records[], int *recordCount, UserAccount users[], int loggedInIndex);          // adds a new study record
void editRecord(StudyRecord records[], int recordCount, int recordID);                                    // edits an existing study record
void deleteRecord(StudyRecord records[], int *recordCount, int recordID);                                 // deletes a study record
void listRecords(StudyRecord records[], int recordCount, int viewerRole, const char *filterUsername);     // displays study records

/* Computation Functions */
int  calculateXP(float hoursStudied);                                                                                                    // calculates XP from hours studied
void calculateStats(StudyRecord records[], int recordCount, const char *username, float *totalHours, float *avgHours, int *totalXP);     // calculates study statistics

/* Sort and Search Functions */
// subjectNames[][] is a 2D array extracted from StudyRecord[]
// used as the primary structure for sorting and searching
void extractSubjectNames(StudyRecord records[], int recordCount, char subjectNames[][MAX_SUBJECT]);                    // extracts subject names into 2D array
void selectionSortBySubject(StudyRecord records[], int recordCount, char subjectNames[][MAX_SUBJECT]);                 // sorts records by subject name
void selectionSortByXP(StudyRecord records[], int recordCount);                                                        // sorts records by XP descending
int  binarySearchBySubject(char subjectNames[][MAX_SUBJECT], int recordCount, const char *subject);                   // searches for a subject by name

/* Leaderboard Functions */
void buildLeaderboard(UserAccount users[], int userCount, StudyRecord records[], int recordCount, LeaderboardEntry leaderboard[], int *leaderboardCount);  // builds the leaderboard
void displayLeaderboard(LeaderboardEntry leaderboard[], int leaderboardCount, int viewerRole);                                                             // displays the leaderboard

/* Display Functions */
void acc_list(UserAccount users[], int userCount, int viewerRole);  // displays all user accounts

/* Utility Functions */
void clearStdin();  // clears the input buffer

#endif
