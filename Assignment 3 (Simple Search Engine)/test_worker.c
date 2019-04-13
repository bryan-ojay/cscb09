#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "freq_list.h"
#include "worker.h"

int main() {

    Node *node1 = malloc(sizeof(Node));
    Node *node2 = malloc(sizeof(Node));
    Node *node3 = malloc(sizeof(Node));
    if ((node1 == NULL) || (node2 == NULL) || (node3 == NULL)) {
        fprintf(stderr, "Unable to malloc.");
        exit(1);
    }

    strcpy(node1->word, "apple");
    node1->freq[0] = 1;
    node1->freq[1] = 2;
    node1->freq[2] = 0;
    node1->freq[3] = 2;
    node1->freq[4] = 2;
    node1->next = node2;

    strcpy(node2->word, "banana");
    node2->freq[0] = 0;
    node2->freq[1] = 2;
    node2->freq[2] = 1;
    node2->freq[3] = 0;
    node2->freq[4] = 5;
    node2->next = node3;

    strcpy(node3->word, "carrot");
    node3->freq[0] = 0;
    node3->freq[1] = 0;
    node3->freq[2] = 0;
    node3->freq[3] = 0;
    node3->freq[4] = 0;
    node3->next = NULL;

    char **filenames = malloc(5 * sizeof(char *));
    if (filenames == NULL) {
        fprintf(stderr, "Unable to malloc filenames.");
        exit(1);
    }
    filenames[0] = "bob.txt";
    filenames[1] = "bob2.txt";
    filenames[2] = "bob3.txt";
    filenames[3] = "bob4.txt";
    filenames[4] = "bob5.txt";


    FreqRecord *rec1 = get_word("apple", node1, filenames);
    FreqRecord *rec2 = get_word("banana", node1, filenames);
    FreqRecord *rec3 = get_word("carrot", node1, filenames);

    printf("Apple:\n");
    print_freq_records(rec1);

    printf("\nBanana:\n");
    print_freq_records(rec2);

    printf("\nCarrot:\n");
    print_freq_records(rec3);

    return 0;

}
