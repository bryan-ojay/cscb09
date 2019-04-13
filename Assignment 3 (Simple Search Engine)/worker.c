#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "freq_list.h"
#include "worker.h"

/* The function get_word should be added to this file */
FreqRecord *get_word(char *word, Node *index, char **filenames) {
    Node *curr = index;
    FreqRecord *record_list = malloc((MAXFILES+1) * sizeof(FreqRecord));
    if (record_list == NULL) {
        fprintf(stderr, "Out of memory.");
        exit(EXIT_FAILURE);
    }
    while (curr != NULL) {
        if (strcmp(word, curr->word) == 0) {
            int i;
            int j = 0;
            for (i = 0; i < MAXFILES; i++) {
		if (filenames[i] == NULL) break;
                if (curr->freq[i] != 0) {
                    record_list[i-j].freq = curr->freq[i];
                    strncpy(record_list[i-j].filename, filenames[i],
                            sizeof(record_list[i-j].filename) - 1);
                }
		else j++;
            }
            //set the freq of the last freqrecord to 0
            record_list[i-j].freq = 0;
            return record_list;
        }
        curr = curr->next;
    }
    return NULL;
}


/* Print to standard output the frequency records for a word.
* Used for testing.
*/
void print_freq_records(FreqRecord *frp) {
    int i = 0;
    while(frp != NULL && frp[i].freq != 0) {
        printf("%d    %s\n", frp[i].freq, frp[i].filename);
        i++;
    }
}

/* run_worker
* - load the index found in dirname
* - read a word from the file descriptor "in"
* - find the word in the index list
* - write the frequency records to the file descriptor "out"
*/
void run_worker(char *dirname, int in, int out){
    //dirname index path
    char indexpath[PATHLENGTH];
    strncpy(indexpath, dirname, sizeof(indexpath) - 1);
    strncat(indexpath, "/index", sizeof(indexpath) - strlen(indexpath) - 1);

    //filenames path
    char namepath[PATHLENGTH];
    strncpy(namepath, dirname, sizeof(namepath) - 1);
    strncat(namepath, "/filenames", sizeof(namepath) - strlen(namepath) - 1);

    printf("%s %s\n", indexpath, namepath);

    //read index and filenames files
    Node *head = NULL;
    char **files = init_filenames();
    read_list(indexpath, namepath, &head, files);

    //read word from input
    char thisword[MAXWORD];
    ssize_t num;
    while ((num = read(in, thisword, MAXWORD)) > 0) {

        //'close' string
        thisword[num - 1] = '\0';
        //printf("%s %lu\n", thisword, strlen(thisword)); //debug

        //get FreqRecord list
        FreqRecord *record_list = get_word(thisword, head, files);
        int ind = 0;
        while(record_list != NULL && record_list[ind].freq != 0) {
            write(out, &record_list[ind], sizeof(FreqRecord));
            ind++;
        }
        //print_freq_records(record_list);
        free(record_list);
    }

}

/* Helper for master freq record */
void insert_freq(FreqRecord *array, FreqRecord new_freq) {
    int i = 0;
    while(array != NULL && array[i].freq != 0) {
        i++; //more to be added to the loop
    }
    if (i < MAXFILES) {
        array[i].freq = new_freq.freq;
        strncpy(array[i].filename, new_freq.filename, PATHLENGTH - strlen(array[i].filename) - 1);
        array[i+1].freq = 0;
    }
}
