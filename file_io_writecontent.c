/*******************************************************************************
 * Header files used
*******************************************************************************/
#include "filemanagementprogram.h"
/*******************************************************************************
 * Function -- writes manipulated content back to text file
*******************************************************************************/
void writeContent(char *dbFileName, char *content){
    FILE *file;
    /*Opens file for writing*/
    file = fopen(dbFileName, "w");
    if (NULL == file) {
        printf("file can't be opened \n");
        return; 
    }
    /*Writes new content in file at the size of content*/
    fwrite(content, sizeof(char), strlen(content), file); 
    printf("\nThe content of this file after manipulation is \n%s", content);
    fclose(file);
}
