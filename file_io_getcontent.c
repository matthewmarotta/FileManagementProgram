/*******************************************************************************
 * Header files used
*******************************************************************************/
#include "filemanagementprogram.h"
/*******************************************************************************
 * Function -- Gets content from the text file for manipulation
*******************************************************************************/
char *getContent(char *dbFileName) {
    FILE *file;
    char *content;
    long fileSize;

    /*Opens file for reading*/
    file = fopen(dbFileName, "r");
    if (NULL == file) {
        printf("file can't be opened \n");
        return NULL; 
    }
    /*Sets position to end of file*/
    fseek(file, 0, SEEK_END);
    /*Returns number of bytes from current position to beginning of file
    and stores in fileSize*/
    fileSize = ftell(file);
    /*Resets file pointer to the start*/
    rewind(file);    
    /*Dynamically allocates memory for file content plus a null terminator*/
    content = (char *) malloc(sizeof(char) * (fileSize + 1));    
    /*Closes file if memory allocation fails*/
    if (content == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return NULL;
    }
   /*Read the file's content into the allocated buffer*/
    fread(content, sizeof(char), fileSize, file);
    /*Adds null terminator to mark end of string*/
    content[fileSize] = '\0';
    printf("The content of this file before manipulation is \n%s", content);
    /*Closes the file and returns the content*/
    fclose(file);
    return content;
}
