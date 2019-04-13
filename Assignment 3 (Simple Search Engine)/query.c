#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "freq_list.h"
#include "worker.h"


int main(int argc, char **argv) {

    char ch;
    char path[PATHLENGTH];
    char *startdir = ".";

    while((ch = getopt(argc, argv, "d:")) != -1) {
        switch (ch) {
            case 'd':
            startdir = optarg;
            break;
            default:
            fprintf(stderr, "Usage: query [-d DIRECTORY_NAME]\n");
            exit(1);
        }
    }
    // Open the directory provided by the user (or current working directory)

    DIR *dirp;
    if((dirp = opendir(startdir)) == NULL) {
        perror("opendir");
        exit(1);
    }

    /* For each entry in the directory, eliminate . and .., and check
    * to make sure that the entry is a directory, then call run_worker
    * to process the index file contained in the directory.
     * Note that this implementation of the query engine iterates
    * sequentially through the directories, and will expect to read
    * a word from standard input for each index it checks.
    */
    char inputword[MAXWORD];
    printf("Input a word to search: ");
    scanf("%s", inputword);

    struct dirent *dp;
    FreqRecord master[MAXFILES + 1];
    while((dp = readdir(dirp)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 ||
            strcmp(dp->d_name, "..") == 0 ||
            strcmp(dp->d_name, ".svn") == 0) {
            continue;
        }
        strncpy(path, startdir, PATHLENGTH);
        strncat(path, "/", PATHLENGTH - strlen(path) - 1);
        strncat(path, dp->d_name, PATHLENGTH - strlen(path) - 1);

        int mp[2];
        if (pipe(mp) < 0) {
	    perror("pipe (master)");
            exit(1);
	    }

        int wp[2]; //worker writes to this pipe
        if (pipe(wp) < 0) {
            perror("pipe (worker)");
            exit(1);
        }

        int n = fork();
        if (n < 0) {
            perror("fork()");
            exit(1);
        }
        else if (n > 0) { //master process
            write(mp[1], inputword, MAXWORD);
            //read from worker pipe
            FreqRecord *buffer = malloc(sizeof(FreqRecord));
            while ((read(wp[0], buffer, MAXWORD)) > 0) {
                insert_freq(master, buffer[0]);
            }
        }

        else { //worker process
            struct stat sbuf;
            if(stat(path, &sbuf) == -1) {
                //This should only fail if we got the path wrong
                // or we don't have permissions on this entry.
                perror("stat");
                exit(1);
            }

            // Only call run_worker if it is a directory
            // Otherwise ignore it.
            if(S_ISDIR(sbuf.st_mode)) {
                //read from master pipe, write to worker pipe
	        close(mp[1]);
                run_worker(path, mp[0], wp[1]);
	        close(wp[1]);
            }
            break; //after child is done, prevent it from looping
        }
        //print freq records from master freq array
        //print_freq_records(master);
    }
    return 0;
}
