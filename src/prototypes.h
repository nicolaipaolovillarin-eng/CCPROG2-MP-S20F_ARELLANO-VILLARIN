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

/* ─── Size Constants ─── */
#define MAX 100
#define MAXNAME 50
#define MAXDAY 32

#define MAX_FULLNAME 100
#define MAX_SUBJECT  50
#define MAX_DATE     12
#define MAX_USERS    100
#define MAX_RECORDS  1000
#define MAX_LB       3

/* ─── Key Constants ─── */
#define KEY            "study"
#define ADMIN_KEY      "admin_123"
#define MASTER_KEY     '!'

/* ─── Role Constants ─── */
#define ROLE_GUEST   0
#define ROLE_ADMIN   1
#define ROLE_STUDENT 2

/* ─── Date Struct ─── */
typedef struct
{
    int month;
    int day;
    int year;
} date;

/* ─── User Account Struct ─── */
typedef struct
{
    int           userID;
    char          username[MAX];
    unsigned char fullName[MAX_FULLNAME];
    unsigned char password[MAX];
    int           pass_len;
    int           fullName_len;
    int           role;
} UserAccount;

/* ─── Study Record Struct ─── */
typedef struct
{
    int   recordID;
    char  ownerUsername[MAX];
    char  subject[MAX_SUBJECT];
    float hoursStudied;
    int   xpEarned;
    date  session_date;
} StudyRecord;

/* ─── Leaderboard Entry Struct ─── */
typedef struct
{
    char  username[MAX];
    char  fullName[MAX_FULLNAME];
    int   totalXP;
    float totalHours;
    int   userID;
    int   rank;
} LeaderboardEntry;

/* ─── Authentication Functions ─── */
void signin(UserAccount users[], int userCount, int *loggedInIndex);
int  create_acc(UserAccount users[], int *userCount);
void encrypt_decrypt(unsigned char *field, int field_len, const char *key);
int  verify(UserAccount users[], int userCount, const char *inputUsername, const char *inputPassword);
int  reset_password(UserAccount users[], int userCount, int userIndex, const char *adminKey);

/* ─── File Handling Functions ─── */
void loadUsers(UserAccount users[], int *userCount);
void saveUsers(UserAccount users[], int userCount);
void loadRecords(StudyRecord records[], int *recordCount);
void saveRecords(StudyRecord records[], int recordCount);

/* ─── Study Record Functions ─── */
void addRecord(StudyRecord records[], int *recordCount, UserAccount users[], int loggedInIndex);
void editRecord(StudyRecord records[], int recordCount, int recordID);
void deleteRecord(StudyRecord records[], int *recordCount, int recordID);
void listRecords(StudyRecord records[], int recordCount, int viewerRole, const char *filterUsername);

/* ─── Computation Functions ─── */
int  calculateXP(float hoursStudied);
void calculateStats(StudyRecord records[], int recordCount, const char *username,
                    float *totalHours, float *avgHours, int *totalXP);

/* ─── Sort and Search Functions ─── */
// subjectNames[][] is a 2D array extracted from StudyRecord[]
// used as the primary structure for sorting and searching
void extractSubjectNames(StudyRecord records[], int recordCount,
                         char subjectNames[][MAX_SUBJECT]);
void selectionSortBySubject(StudyRecord records[], int recordCount,
                            char subjectNames[][MAX_SUBJECT]);
void selectionSortByXP(StudyRecord records[], int recordCount);
int  binarySearchBySubject(char subjectNames[][MAX_SUBJECT], int recordCount,
                           const char *subject);

/* ─── Leaderboard Functions ─── */
void buildLeaderboard(UserAccount users[], int userCount,
                      StudyRecord records[], int recordCount,
                      LeaderboardEntry leaderboard[], int *leaderboardCount);
void displayLeaderboard(LeaderboardEntry leaderboard[], int leaderboardCount,
                        int viewerRole);

/* ─── Display Functions ─── */
void acc_list(UserAccount users[], int userCount, int viewerRole);

/* ─── Utility Functions ─── */
void clearStdin();

#endif
