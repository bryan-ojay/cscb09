#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

/* Add a group with name group_name to the group_list referred to by 
* group_list_ptr. The groups are ordered by the time that the group was 
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.) 
*/
int add_group(Group **group_list_ptr, const char *group_name) {
    //create group
    Group *new_group = malloc(sizeof(Group));
    if (new_group == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(EXIT_FAILURE);    
    }
    new_group->name = malloc(sizeof(group_name));
    if (new_group->name == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(EXIT_FAILURE);    
    }
    strcpy(new_group->name, group_name);

    new_group->users = NULL;
    new_group->xcts = NULL;
    new_group->next = NULL;

    //Check if list is empty
    if (*group_list_ptr == NULL) {
        *group_list_ptr = new_group;
    }
    
    //Else insert into linked list
    else {
        Group *curr = *group_list_ptr;
        while (curr != NULL) {
            if (strcmp(curr->name, group_name) == 0) {
                free(new_group->name);
                free(new_group);
                return -1;
            }
            else if (curr->next == NULL) {
                curr->next = new_group;
                break;
            }
            curr = curr->next;    
        }
    }
    return 0;
}

/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {
    Group *curr = group_list;
    while (curr != NULL) {
        printf("%s\n", curr->name);
        curr = curr->next;
    }
}

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the 
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
    Group *found;
    Group *curr = group_list;
    while (curr != NULL) {
        if (strcmp(curr->name, group_name) == 0) {
            found = curr;
            break;
        }
        else if (curr->next == NULL) {
            found = NULL;
        }
        curr = curr->next;    
    }
    return found;
}

/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {    
    //Create user
    User *new_user = malloc(sizeof(User));
    if (new_user == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(EXIT_FAILURE);    
     }
    new_user->name = malloc(strlen(user_name)+1);
    if (new_user->name == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(EXIT_FAILURE);    
    }
    strcpy(new_user->name, user_name);

    new_user->balance = 0.0;
    new_user->next = NULL;
    
    // Add user to list
    int status;
       
    // Check if list is empty
    if (group->users == NULL) {
        group->users = new_user;
        status = 0;
    }
   
    // Check if "user_name" is first element in list
    else if (strcmp(group->users->name, user_name) == 0) {
    free(new_user->name);
    free(new_user);   
     status = -1;
    }          
   
    // Check if element exists inside the list
    else if (group->users != NULL) {
        User* prev_user = find_prev_user(group, user_name);
        if (prev_user != NULL) {
        free(new_user->name);
        free(new_user);  
         status = -1;
        }
        else {
            // if element doesn't exist inside list, set it as the first element
            new_user->next = group->users;
            group->users = new_user;
            status = 0;
        }
    }
    return status;
}

/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list. 
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you 
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {
    int status;
    User *del_user = NULL;
    
    // check if desired user is at front of list
    if (strcmp(group->users->name, user_name) == 0) {
        del_user = group->users;
        group->users = del_user->next;
        del_user->next = NULL;
    }

    // else check if user is somewhere inside list
    else {
        User *prev_user = find_prev_user(group, user_name);
        if (prev_user == NULL) {
            status = -1;        
        }
        else {
            del_user = prev_user->next;
            prev_user->next = del_user->next;
            del_user->next = NULL;
        }
    }   
    
    // remove user transactions and free from memory
    if (del_user != NULL) {
        remove_xct(group, user_name);
        free(del_user->name);
        free(del_user);
        status = 0;    
    }

    return status;    
}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely 
* lowest payer first.
*/
void list_users(Group *group) {
    User *curr = group->users;
    while (curr != NULL) {
        printf("%s %.2f\n", curr->name, curr->balance);
        curr = curr->next;
    }
}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {
    int status;
    
    // Check if user is the first user
    if (strcmp(group->users->name, user_name) == 0) {
        printf("%.2f\n", group->users->balance);    
        status = 0;
    }
    
    // Else find the previous user of "user_name"
    else {
        User *prev_user = find_prev_user(group, user_name);
        if (prev_user != NULL) {
            printf("%.2f\n", prev_user->next->balance);
            status = 0;
        }
        else {
            status = -1;
        }
    }
    return status;
}

/* Print to standard output the name of the user who has paid the least 
* If there are several users with equal least amounts, all names are output. 
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance). 
*/
int under_paid(Group *group) {
    int status;
    if (group->users != NULL) {
        User *curr = group->users;
        double bal = curr->balance;
        printf("%s\n", curr->name);
        while (curr->next) {
            curr = curr->next;
            if (curr->balance == bal) {
                printf("%s\n", curr->name);
            }        
        }        
        status = 0;
    }
    else {
        status = -1;
    }

    return status;
}

/* Return a pointer to the user prior to the one in group with user_name. If 
* the matching user is the first in the list (i.e. there is no prior user in 
* the list), return a pointer to the matching user itself. If no matching user 
* exists, return NULL. 
*
* The reason for returning the prior user is that returning the matching user 
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {
    User *found;
    User *curr = group->users;
    // check if list is empty
    if (curr == NULL) {
        found = NULL;        
    }
    // check for element in list
    else {
        while (curr->next != NULL) {
            if (strcmp(curr->next->name, user_name) == 0) {
                found = curr;
                break;
            }
            else if (curr->next->next == NULL) {
                found = NULL;
            }
            curr = curr->next;    
        }
    }
    return found;
}

/* Add the transaction represented by user_name and amount to the appropriate 
* transaction list, and update the balances of the corresponding user and group. 
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {
    int status;
    User *prev_user = NULL;
    User *xct_user = NULL;

    // Check if user list is empty
    if (group->users == NULL) {
        status = -1;
    }
    
    // Check if "user_name" is first element in list
    else if (strcmp(group->users->name, user_name) == 0) {
        xct_user = group->users;
        status = 0;
    }    
    
    // Check if user exists inside the list
    else if (group->users != NULL) {
        prev_user = find_prev_user(group, user_name);
        if (prev_user != NULL) {
            xct_user = prev_user->next;
            status = 0;
        }
        else {
            status = -1;
        }
    }

    if (xct_user != NULL) {
        //create xct
        Xct *new_xct = malloc(sizeof(Xct));
        if (new_xct == NULL) {
            fprintf(stderr, "Out of memory.\n");
            exit(EXIT_FAILURE);    
        }
        new_xct->name = malloc(strlen(user_name)+1);
        if (new_xct->name == NULL) {
            fprintf(stderr, "Out of memory.\n");
            exit(EXIT_FAILURE);    
        }
        strcpy(new_xct->name, user_name);
        new_xct->amount = amount;
        new_xct->next = NULL;

        //Check if list is non-empty
        if (group->xcts != NULL) {
            new_xct->next = group->xcts;
        }
        group->xcts = new_xct;
        
        //sort user in list
        xct_user->balance += new_xct->amount;

        //check if the user is not the last user in the list
        if (xct_user->next != NULL) {
            //check if the next user's balance is lower than the xct user
            if (xct_user->balance > xct_user->next->balance) {
                //check if the user is not the first element in the list
                if (prev_user != NULL) {
                    prev_user->next = xct_user->next;
                }
                else {
                    group->users = xct_user->next;
                }
                
                //loop through the list to find xct_user's new successor
                User *curr = xct_user->next;
                while (curr->next != NULL) {
                    if (xct_user->balance < curr->next->balance) { break; }
                    curr = curr->next;
                }
                //insert the user into the list
                xct_user->next = curr->next;
                curr->next = xct_user;
            }
        }
    }

    return status;
}

/* Print to standard output the num_xct most recent transactions for the 
* specified group (or fewer transactions if there are less than num_xct 
* transactions posted for this group). The output should have one line per 
* transaction that prints the name and the amount of the transaction. If 
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long num_xct) {
    Xct *curr = group->xcts;
    for (int i = 0; i < num_xct; i++) {
	    if (curr != NULL) {
		printf("%s %.2f\n", curr->name, curr->amount);
            curr = curr->next;
	    }
        else {break;}
    }
}

/* Remove all transactions that belong to the user_name from the group's 
* transaction list. This helper function should be called by remove_user. 
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/
void remove_xct(Group *group, const char *user_name) {
    Xct *del_xct;

    /* check xcts at beginning of list
       (i.e. if 1 or more xcts at beginning of list belong to user) */
    if (group->xcts != NULL) {
        while (strcmp(group->xcts->name, user_name) == 0) {
            del_xct = group->xcts;
            group->xcts = del_xct->next;
            free(del_xct->name);
            free(del_xct);
            if (group->xcts == NULL) {break;}
        }
    }
    // check xcts after beginning of list
    if (group->xcts != NULL) {
        Xct *curr = group->xcts;
        while (curr->next != NULL) {
            if (strcmp(curr->next->name, user_name) == 0) {
                del_xct = curr->next;
                curr->next = del_xct->next;
                free(del_xct->name);
                free(del_xct);
            }
            else {
                curr = curr->next;
            }
        }
    }
}

