#include "funcs.c"

int main() {

    user_info credentials[MAX]; //arrays of struct where each user into is stored
    user_info admin_creds[MAX];
    user_info temp[1]; //temp storage for a new user
    study_sessions sesh[MAXDAY]; //arrays of struct where no. of sessions is stored
    //NOTE: if the struct declarations count as global variables, these are to be moved in main.c

    int choice = 0; //container for user choice
    char bool_choice; //container for continue choice
    int sesh_choice = 0;
    int admin_choice = 0;
    int max_choice = 3; //container for max amount of choices in program

    int user_type = 0; //0 for guest, 1 for admin, 2 for user
    //NOTE: user_type should be in struct so this is placeholder only for logic
    int user_count = 0; //container for user count for database
    int admin_count = 0;
    int sesh_count = 0;
    int verify_val = 0; //to store the return val of verify func
    int create_val = 0;//to store the return val of create_acc func

    do //do while that runs until user doesnt want to go back to menu
    {
        do { //do while that runs until user inputs a valid input
            printf("Welcome to Study Spawn Point!\n");
            printf("[1] - Sign in\n");
            printf("[2] - Create an Account\n");
            printf("[3] - Exit\n");
            printf("Enter Choice: ");
            scanf("%d", &choice);

            if(choice < 1 || choice > max_choice)
                printf("\nInvalid input, please try again.\n\n");

        }while (choice < 1 || choice > max_choice);
       
        if (choice != max_choice) { //if statement to direct program if user chooses not to exit
            switch (choice)
            {
                case 1: //case for signing in
                    signin(temp); //stores in temp struct
                    verify_val = verify(credentials, temp, admin_creds, user_count, admin_count);
                    
                    switch(verify_val) {
                        case 1:
                            do {
                                printf("Admin Menu\n");
                                printf("[1] - Account List\n");
                                printf("[2] - Edit Records\n");
                                printf("[3] - Sign Out\n");
                                printf("Enter Choice: ");
                                scanf("%d", &admin_choice);
                                
                                switch (admin_choice)
                                {
                                case 1:
                                    /* code */
                                    acc_list(credentials, KEY, user_count);
                                    break;

                                case 2:
                                    printf("edit records lol");//placeholder
                                    break;
                                
                                default:
                                    break;
                                }

                                if(admin_choice != 1 && admin_choice != 2 && admin_choice != 3)
                                    printf("Invalid input, please try again.");

                                // do {
                                //     printf("Do you want to go back to the admin menu? Y/N ");
                                //     scanf(" %c", &bool_choice);

                                //     if(bool_choice != 'Y' && bool_choice != 'y' && bool_choice != 'n' && bool_choice != 'N')
                                //         printf("Invalid input, please try again.\n\n");
                                        
                                // } while(bool_choice != 'Y' && bool_choice != 'y' && bool_choice != 'n' && bool_choice != 'N');

                            } while(admin_choice != 1 && admin_choice != 2 && admin_choice != 3);
                        
                            break;

                        case 2:
                            do {
                                printf("Ready to Start?\n");
                                printf("[1] - Input a New Session\n");
                                printf("[2] - Session List\n");
                                printf("[3] - Sign Out\n");
                                printf("Enter Choice: ");
                                scanf("%d", &sesh_choice);

                                switch(sesh_choice) {

                                    case 1:
                                        input_sesh(sesh, &sesh_count);
                                        break;

                                    case 2:
                                        sesh_printer(sesh, sesh_count);
                                        break;

                                    default:
                                        break;
                                }

                                if(sesh_choice != 1 && sesh_choice != 2 && sesh_choice != 3)
                                    printf("Invalid input, please try again.");

                                do {
                                    printf("Do you want to go back to the user menu? Y/N ");
                                    scanf(" %c", &bool_choice);

                                    if(bool_choice != 'Y' && bool_choice != 'y' && bool_choice != 'n' && bool_choice != 'N')
                                        printf("Invalid input, please try again.\n\n");
                                        
                                } while(bool_choice != 'Y' && bool_choice != 'y' && bool_choice != 'n' && bool_choice != 'N');
                                
                                system("cls");

                            } while(sesh_choice != 1 && sesh_choice != 2 && sesh_choice != 3 || bool_choice == 'y' || bool_choice == 'Y'); 

                            break;

                        default:
                            break;
                    }
                        
                    break;

                case 2: //case for creating account
                    create_acc(credentials, admin_creds, &user_count, &admin_count);     
                    break;

                default:
                    break;
            }
            
            do {
                printf("Do you want to go back to the main menu? Y/N ");
                scanf(" %c", &bool_choice);

                if(bool_choice != 'Y' && bool_choice != 'y' && bool_choice != 'n' && bool_choice != 'N')
                    printf("Invalid input, please try again.\n\n");
                    
            } while(bool_choice != 'Y' && bool_choice != 'y' && bool_choice != 'n' && bool_choice != 'N');
            
            system("cls");
        }

        else //else statement if user chooses to exit program
            printf("\nExiting program...\n\n");
        
    } while(bool_choice == 'y' || bool_choice == 'Y' && choice != max_choice);

    return 0;

}