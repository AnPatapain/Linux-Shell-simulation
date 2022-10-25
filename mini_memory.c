#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include "mini_lib.h"
#include<stdio.h>

struct malloc_element {
    void *zone;
    int taille;
    int statut;
    struct malloc_element* next_zone;
};

struct malloc_element* malloc_list = NULL;

void* mini_calloc (int size_element, int number_element) {

    int taille_demandee = size_element * number_element;

    if (number_element <= 0) {
        perror("ERROR");
        return NULL;
    } 

    // CHECK MALLOC LIST TO FIND AVAILABLE ZONE (ie. statut = 1)

    if (malloc_list != NULL) {
        struct malloc_element* temp = malloc_list;
        
        while(temp != NULL) {
            if(temp->statut == 0 && taille_demandee <= temp->taille) {
                return temp->zone;
            }
            temp = temp->next_zone;
        }
    }

    void *buffer = sbrk(taille_demandee);

    if (buffer == (void *) - 1) {
        perror("ERROR");
        return NULL;
    }

    int i = 0;
    while (i < number_element) {
        ( (char *)buffer )[i] = '\0';
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
            printf("\nthe first character of zone will soon be libere: %c\n", ( (char*)(temp->zone) )[0] );
            temp->statut = 0;
        }
        temp = temp->next_zone;
    }

}

void mini_exit() {
    _exit(1);
}
