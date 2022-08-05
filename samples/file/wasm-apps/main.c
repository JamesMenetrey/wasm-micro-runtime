/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

#define PATH_TEST_FILE "test.txt"

int
main(int argc, char **argv)
{
    FILE *file;
    const char* text = "Hello, world!";
    char buffer[1000];

    printf("Opening a file..\n");
    file = fopen(PATH_TEST_FILE,"a+");
    if(file == NULL)
    {
        printf("Error! errno: %d\n", errno);   
        exit(1);             
    }

    printf("Writing to the file..\n");
    fprintf(file, "%s", text);
    
    printf("Reading from the file, up to 1000 characters..\n");
    fseek(file, 0, SEEK_SET);
    fread(buffer, 1, sizeof(buffer), file);
    printf("Text read: %s\n", buffer);

    printf("Determine whether we reach the end of the file..\n");
    int is_end_of_file = feof(file);
    printf("Is the end of file? %d\n", is_end_of_file);

    printf("Moving at the end of the file..\n");
    fseek(file, 0, SEEK_END);

    printf("Getting the plaintext size..\n");
    long plaintext_size = ftell(file);
    printf("The plaintext size is %ld.\n", plaintext_size);

    printf("Force actual write of all the cached data to the disk..\n");
    fflush(file);

    printf("Closing from the file..\n");
    fclose(file);

    printf("Getting the ciphertext size..\n");
    struct stat st;
    stat(PATH_TEST_FILE, &st);
    printf("The ciphertext size is %lld.\n", st.st_size);

    return 0;
}
