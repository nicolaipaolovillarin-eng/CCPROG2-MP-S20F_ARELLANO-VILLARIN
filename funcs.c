#include "prototypes.h"

void signin(user_info user_sn[]) { //this needs to scan for all user and pass in the struct array
        
        printf("\nSign in!\n");  
        printf("Enter username: ");
        scanf("%s", user_sn[0].username);

        printf("Enter password: ");
        scanf("%s", user_sn[0].password);
}

int create_acc(user_info user[], user_info admin[], int *user_count, int *admin_count) {
    char admin_choice;
    int acc_type = 0;
    int return_val;
    printf("\nCreate an Account!\n\n");

    do {
        printf("Are you creating an admin account? Y/N ");
        scanf(" %c", &admin_choice);
    } while(admin_choice != 'Y' && admin_choice != 'y' && admin_choice != 'N' && admin_choice != 'n');
    
    if(admin_choice == 'Y' || admin_choice == 'y') {
        char key_input[MAX];
        acc_type = 1;

        printf("Enter admin key: ");
        scanf("%s", key_input);

        if(strcmp(key_input, ADMIN_KEY) == 0) {
            printf("\nAccess Granted!\n\n");
            printf("Enter username: ");
            scanf("%s", admin[*admin_count].username);

            printf("Enter password: ");
            scanf("%s", admin[*admin_count].password);
            admin[*admin_count].pass_len = strlen(admin[*admin_count].password);
            encrypt_decrypt(&admin[*admin_count], KEY, 1, admin[*admin_count].pass_len);

            (*admin_count)++;
        }

        else
            printf("Wrong admin key. Access denied.\n");
    }
    
    else {
        acc_type = 2;
        printf("Enter username: ");
        scanf("%s", user[*user_count].username);

        printf("Enter password: ");
        scanf("%s", user[*user_count].password);
        user[*user_count].pass_len = strlen(user[*user_count].password);
        encrypt_decrypt(&user[*user_count], KEY, 1, user[*user_count].pass_len);

        printf("Account created! Go back to the menu to login your new account!\n\n");

        (*user_count)++;
    }

    return acc_type;
}

void encrypt_decrypt(user_info *user, char key[], int mode, int len) {
    int key_len = strlen(key);

    for(int i = 0; i < len; i++) {
        if (mode == 1) { //encrypt mode
            int val = user->password[i] - 32; //shift to printable ASCII then turn it into an int
            val = val ^ key[i % key_len]; //xor shift occurs
            val = (val + 67) % 95; //extra shift on top of xor
            user->password[i] = (unsigned char)(val + 32); //edits the char to the encrypted one
        }

        else { //decrypt mode
            int val = user->password[i] - 32; //same thing as first
            val = (val - 67 + 95) % 95; //reverse the extra shift but add 95 after in case of negative values
            val = val ^ key[i % key_len]; //then xor shift again
            user->password[i] = (unsigned char)(val + 32); //edits the char to the decrypted version
        }
    }
}


int verify(user_info user[], user_info temp[], user_info admin[], int user_count, int admin_count) {

    int acc_type = 0; //0 for guest, 1 for admin, 2 for user
    /*1 = acc in user database but wrong pass, 2 = acc not in user database
     3 = acc in admin database but wrong pass, 4 = acc does not exist at all*/
    int found = 0; //0 means not found, 1 means found

    if(user_count == 0 && admin_count == 0) 
        printf("No account found in database. Create a new account.\n\n");

    else {
        for (int i = 0; i < admin_count && found == 0; i++) {
            encrypt_decrypt(&admin[i], KEY, 0, admin[i].pass_len);
            if (strcmp((char*)admin[i].username, (char*)temp[0].username) == 0) {
                if (strcmp((char*)admin[i].password, (char*)temp[0].password) == 0) {
                    system("cls");
                    printf("Welcome, %s!\n\n", admin[i].username);
                    acc_type = 1; 
                    found++;
                }
            }
            encrypt_decrypt(&admin[i], KEY, 1, admin[i].pass_len);
        }

        for (int i = 0; i < user_count && found == 0; i++) {
            encrypt_decrypt(&user[i], KEY, 0, user[i].pass_len);
            if (strcmp((char*)user[i].username, (char*)temp[0].username) == 0) {
                if (strcmp((char*)user[i].password, (char*)temp[0].password) == 0) {
                    system("cls");
                    printf("Welcome, %s!\n\n", user[i].username);
                    acc_type = 2; 
                    found++;
                }
            }
            encrypt_decrypt(&user[i], KEY, 1, user[i].pass_len);
        }

        if(found == 0) {
           printf("Sign-in failed.\n\n"); 
           acc_type = -1;
        }
    }
    return acc_type;
    
}
    
void acc_list(user_info cred_list[], char key[], int user_count) {

    if(user_count == 0)
        printf("\nNo accounts in database.\n\n");

    else {
        printf("\n---User List---\n");
        for(int i = 0; i < user_count; i++) {
            printf("Account %d:\n", i + 1);
            printf("Username: %s\n", cred_list[i].username);
            printf("Encrypted Password: %s\n", cred_list[i].password);
            encrypt_decrypt(&cred_list[i], KEY, 0, cred_list[i].pass_len);
            printf("Actual Password: %s\n\n", cred_list[i].password);
            encrypt_decrypt(&cred_list[i], KEY, 1, cred_list[i].pass_len);
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

// void
