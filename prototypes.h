#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 100
#define MAXNAME 50
#define MAXDAY 32
#define KEY "study"
#define ADMIN_KEY "admin_123"

typedef struct
{
    char username[MAX];
    unsigned char password[MAX];
    int pass_len;
} user_info;

typedef struct
{
    int month;
    int day;
    int year;
} date;

typedef struct
{
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
user_info admin_creds[MAX];
user_info temp[1]; //temp storage for a new user
study_sessions sesh[MAXDAY]; //arrays of struct where no. of sessions is stored
//NOTE: if the struct declarations count as global variables, these are to be moved in main.c

void signin(user_info user_sn[]);
int create_acc(user_info user[], user_info admin_creds[], int *user_count, int *admin_count);
void encrypt_decrypt(user_info *user, char key[], int mode, int len);
int verify(user_info user[], user_info temp[], user_info admin[], int user_count, int admin_count);
void acc_list(user_info cred_list[], char key[], int user_count);
void input_sub(study_sessions session[], int sesh_count);
void input_sesh(study_sessions session[], int *sesh_count);
void sesh_printer(study_sessions session[], int sesh_count);