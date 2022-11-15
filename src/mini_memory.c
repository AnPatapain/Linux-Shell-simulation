#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include "mini_lib.h"


#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0

struct malloc_element* malloc_list = NULL;

void* mini_calloc (int size_element, int number_element) {

    int taille_demandee = size_element * number_element;

    if (number_element <= 0) {
        mini_perror("number of element must be greater than 0");
    } 
    // CHECK MALLOC LIST TO FIND AVAILABLE ZONE (ie. statut = 1)
    if (malloc_list != NULL) {
        struct malloc_element* temp = malloc_list;
        
        while(temp != NULL) {
            if(temp->statut == 0 && taille_demandee <= temp->taille) {
                int j = 0;

                while (j < taille_demandee) {
                    *( (char *)temp->zone  + j ) = '\0';
                    j++;
                }
                return temp->zone;
            }
            temp = temp->next_zone;
        }
    }

    // IF THERE ISN'T ANY AVAILABLE ZONE THEN EXECUTE THE CODES BELOW
    void *buffer = sbrk(taille_demandee);

    if (buffer == (void *) - 1) {
        mini_perror("error occur for allocating using sbrk");
        return NULL;
    }

    int i = 0;

    while (i < taille_demandee) {
        *( (char *)buffer  + i ) = '\0';
        i++;
    }

    


    if(malloc_list == NULL) {

        malloc_list = sbrk(sizeof(struct malloc_element));
        malloc_list->zone = buffer;
        malloc_list->taille = taille_demandee;
        malloc_list->statut = 1;
        malloc_list->next_zone = NULL;

    }else {
        
        struct malloc_element* next = sbrk(sizeof(struct malloc_element));
        next->zone = buffer;
        next->taille = taille_demandee;
        next->statut = 1;
        next->next_zone = malloc_list;
        malloc_list = next;
    
    }

    return buffer;
}

void mini_free(void *ptr) {
    struct malloc_element *temp = malloc_list;
    while(temp != NULL) {
        if(temp->zone == ptr) {
            temp->statut = 0;
        }
        temp = temp->next_zone;
    }
}

void mini_exit() {
    mini_exit_string();
    mini_exit_io();
    _Exit(EXIT_SUCCESS);
}
