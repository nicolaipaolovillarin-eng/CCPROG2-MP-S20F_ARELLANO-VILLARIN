#include "funcs.c"

int main() {

    int choice; //container for user choice
    char bool_choice; //container for continue choice
    int sesh_choice;
    int max_choice = 4; //container for max amount of choices in program

    int user_count = 0; //container for user count for database
    int sesh_count = 0;

    do //do while that runs until user doesnt want to go back to menu
    {
        do { //do while that runs until user inputs a valid input
            /* code */
            printf("Welcome to Study Spawn Point!\n");
            printf("[1] - Sign in\n");
            printf("[2] - Create an Account\n");
            printf("[3] - Account List\n");
            printf("[4] - Exit\n");
            printf("Enter Choice: ");
            scanf("%d", &choice);

            if(choice < 1 || choice > max_choice)
                printf("\nInvalid input, please try again.\n\n");

        }while (choice < 1 || choice > max_choice);
       
        if (choice != max_choice) { //if statement to direct program if user chooses not to exit
            switch (choice)
            {
                case 1: //case for signing in
                    signin(temp); //stores in temp array
                    verify(credentials, temp, user_count); //verifies info in temp array to credentials array (see funcs.c for structs)
                    if(verify(credentials, temp, user_count) == 0) {
                        do {
                            printf("[1] - Input a New Session\n");
                            printf("[2] - Session List\n");
                            scanf("%d", &sesh_choice);

                            if(sesh_choice == 1)
                                input_sesh(sesh, &sesh_count);
                            
                            else if(sesh_choice == 2)
                                sesh_printer(sesh, sesh_count);

                            else
                                printf("Invalid input, please try again.");
                        } while(sesh_choice != 1 || sesh_choice != 2); 
                    }
                    break;

                case 2: //case for creating account
                    create_acc(credentials, &user_count);
                    break;

                case 3: //case for showing list of accounts created
                    acc_list(credentials, user_count);
        
                default:
                    break;
            }
            
            do {
                printf("Do you want to go back to the menu? Y/N ");
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