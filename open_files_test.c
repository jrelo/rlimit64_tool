#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_files> <sleep_duration>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_files = atoi(argv[1]);
    int sleep_duration = atoi(argv[2]);

    if (num_files <= 0 || sleep_duration <= 0) {
        fprintf(stderr, "Invalid arguments. Both arguments must be positive integers.\n");
        exit(EXIT_FAILURE);
    }

    FILE **files = malloc(num_files * sizeof(FILE *));
    if (files == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    // generate filenames and open files
    for (int i = 0; i < num_files; i++) {
        char filename[20];
        snprintf(filename, sizeof(filename), "file%d.txt", i + 1);
        printf("Opening file: %s\n", filename);
        files[i] = fopen(filename, "w");
        if (files[i] == NULL) {
            perror("Error opening file");
            // close opened files before exiting
            for (int j = 0; j < i; j++) {
                fclose(files[j]);
            }
            free(files);
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }

    // sleep to keep the files open
    printf("%d files opened successfully. Sleeping for %d seconds...\n", num_files, sleep_duration);
    sleep(sleep_duration);

    for (int i = 0; i < num_files; i++) {
        fclose(files[i]);
    }

    free(files);
    return 0;
}
