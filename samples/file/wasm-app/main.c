/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define PATH_TEST_FILE "test.txt"

int
main(int argc, char **argv)
{
    FILE *file;
    const char* text = "Hello, world!";
    char buffer[1000];

    printf("Opening a file..\n");
    file = fopen(PATH_TEST_FILE,"w+");
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

    printf("File current offset: %ld\n", ftell(file));
    off_t world_offset = ftell(file) - 6;

    printf("Writing 5 characters at offset %lld..\n", world_offset);
    pwrite(fileno(file), "James", 5, world_offset);

    printf("File current offset: %ld\n", ftell(file));

    printf("Reading 5 characters at offset %lld..\n", world_offset);
    buffer[5] = '\0';
    pread(fileno(file), buffer, 5, world_offset);
    printf("Text read: %s\n", buffer);

    printf("File current offset: %ld\n", ftell(file));

    printf("Allocate more space to the file..\n");
    posix_fallocate(fileno(file), ftell(file) - 2, 10);

    printf("File current offset: %ld\n", ftell(file));

    printf("Closing from the file..\n");
    fclose(file);

    printf("Getting the ciphertext size..\n");
    struct stat st;
    stat(PATH_TEST_FILE, &st);
    printf("The ciphertext size is %lld.\n", st.st_size);

    return 0;
}
