#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 100
#define MAXNAME 50
#define MAXDAY 32

typedef struct{
    char username[MAX];
    char password[MAX];
} user_info;

typedef struct
{
    int month;
    int day;
    int year;
} date;

typedef struct{
    char sub_name[MAXNAME];
    int sub_class[10]; //to check if sub is major or minor
} subject;

typedef struct
{
    char sesh_name[MAX];
    subject sub_list[10];
    int sub_qty;
    date date_list[10];
} study_sessions;

user_info credentials[MAX]; //arrays of struct where each user into is stored
user_info temp[1]; //temp storage for a new user
study_sessions sesh[MAXDAY]; //arrays of struct where no. of sessions is stored

void signin(user_info user_sn[]);
void create_acc(user_info user[], int *user_count);
int verify(user_info user[], user_info temp[], int user_count);
void acc_list(user_info cred_list[], int user_count);
void input_sub(study_sessions session[], int sesh_count);
void input_sesh(study_sessions session[], int *sesh_count);
void sesh_printer(study_sessions session[], int sesh_count);