#include "prototypes.h"

void signin(user_info user_sn[]) { //this needs to scan for all user and pass in the struct array
        
        printf("\nSign in!\n");  
        printf("Enter username: ");
        scanf("%s", user_sn[0].username);

        printf("Enter password: ");
        scanf("%s", user_sn[0].password);
}

void create_acc(user_info user[], int *user_count) {
    printf("\nCreate an Account!\n");
        
    printf("Enter username: ");
    scanf("%s", credentials[*user_count].username);

    printf("Enter password: ");
    scanf("%s", credentials[*user_count].password);

    printf("Account created! Go back to the menu to login your new account!\n\n");

    (*user_count)++;
}

int verify(user_info user[], user_info temp_user[], int user_count) {

    int return_val;

    if(user_count == 0) {
        printf("No account found in database. Create a new account.\n\n");
        return_val == 2;
    }
        
    else {
        for (int i = 0; i < user_count; i++) {
            if (strcmp(user[i].username, temp_user[0].username) == 0) {
                if (strcmp(user[i].password, temp_user[0].password) == 0) {
                    system("cls");
                    printf("Welcome, %s!\n\n", user[i].username);
                    return_val = 0; 
                } 
                
                else {
                    printf("Wrong password.\n");
                    return_val = 1;
                }
            }
            else
                printf("User does not exist.\n\n");
        }
    }

    return return_val;
    
}
    
void acc_list(user_info cred_list[], int user_count) {

    if(user_count == 0)
        printf("\nNo accounts in database.\n\n");

    else {
        printf("---User List---\n\n");
        for(int i = 0; i < user_count; i++) {
            printf("Account %d:\n", i + 1);
            printf("Username: %s\n", cred_list[i].username);
            printf("Password: %s\n\n", cred_list[i].password);
        }  
    }
}

void input_sub(study_sessions session[], int sesh_count) {

    char choice;
    int confirm;
    int *sub_count = &session[sesh_count].sub_qty;

    do {
        

        printf("Enter Subject Name: ");
        scanf("%s", session[sesh_count].sub_list[*sub_count].sub_name);

        //need to add a feature that detects duplicate subjects using strcmp

        printf("[1] - Major Subject?\n");
        printf("[2] - Minor Subject?\n");
        printf("Enter Input: ");
        scanf("%d", &session[sesh_count].sub_list[*sub_count].sub_class[*sub_count]);
        
        (*sub_count)++;

        do {
            printf("\nDo you want to add another subject? Y/N ");
            scanf(" %c", &choice);

            if (choice != 'n' && choice != 'n' && choice != 'Y' && choice != 'y')
                printf("Invalid input, please try again.\n");

        } while(choice != 'N' && choice != 'n' && choice != 'Y' && choice != 'y' && *sub_count < 10);
        
        system("cls");

        // do {
        //     printf("\nDo you want to add another subject? Y/N ");
        //     scanf(" %c", &choice);

        //     if (choice != 'n' && choice != 'n' && choice != 'Y' && choice != 'y')
        //         printf("Invalid input, please try again.\n");

        // } while(choice != 'n' && choice != 'n' && choice != 'Y' && choice != 'y');

        if(*sub_count == 10) {
            do {
                printf("You have inputted the maximum amount of subjects. Press 1 to continue.");
                scanf("%d", &confirm);

                if(confirm != 1)
                    printf("Invalid input, please try again.");
            } while(confirm != 1);
        }

    } while((choice == 'Y' || choice == 'y') && *sub_count < 10);
        
}

void input_sesh(study_sessions session[], int *sesh_count) {

    char choice;
    int confirm;

    do {
        session[*sesh_count].sub_qty = 0;

        printf("Enter Session Name: ");
        scanf("%s", session[*sesh_count].sesh_name);
        
        input_sub(session, *sesh_count);

        printf("Enter Current Month: ");
        scanf("%d", &session[*sesh_count].date_list[*sesh_count].month);

        printf("Enter Current Day: ");
        scanf("%d", &session[*sesh_count].date_list[*sesh_count].day);

        printf("Enter Current Year: ");
        scanf("%d", &session[*sesh_count].date_list[*sesh_count].year); 

        (*sesh_count)++;

        do {
            printf("Do you want to add another session? Y/N ");
            scanf(" %c", &choice);

            if (choice != 'n' && choice != 'n' && choice != 'Y' && choice != 'y')
                printf("Invalid input, please try again.");

        } while(choice != 'n' && choice != 'n' && choice != 'Y' && choice != 'y' && *sesh_count < 32);

        system("cls");

        if(*sesh_count == 10) {
            do {
                printf("You have inputted the maximum amount of sessions. Press 1 to continue.");
                scanf("%d", &confirm);

                if(confirm != 1)
                    printf("Invalid input, please try again.");
            } while(confirm != 1);
        }
        
    } while((choice == 'Y' || choice == 'y') && *sesh_count < 32);
}


void sesh_printer(study_sessions session[], int sesh_count) {

    if(sesh_count != 0) {
        for(int i = 0; i < sesh_count; i++) {
            printf("Session %d: %s\n", i + 1, session[i].sesh_name);
            printf("Date: %d/%d/%d\n\n", session[i].date_list[i].month, session[i].date_list[i].day, session[i].date_list[i].year);

            for(int j = 0; j < session[i].sub_qty; j++) {
                int sub_type = session[i].sub_list[j].sub_class[j];
                printf("Subject %d: %s\n", j + 1, session[i].sub_list[j].sub_name);
                printf("Subject Type: ");

                if(sub_type == 1)
                    printf("Major\n\n");

                else
                    printf("Minor\n\n");
            }

            printf("\n---------------------\n\n\n");

        }
    }
    
    else
        printf("No session in database, create a new one!\n\n");
}
