*******************************************************************************/
#include "filemanagementprogram.h"
/*******************************************************************************
 * Function -- Encrypts text contained in file
*******************************************************************************/
void encryptFile(char *content, char newKey[]){
    int keyLen = strlen(newKey); 
    int msgLen = strlen(content); 
    int i, j;
    /*For loop iterates through file contents and extends key based on 
    length of contents*/
    for(i = 0, j = 0; i < msgLen; ++i, ++j) {
        if(j == keyLen)
        j = 0;
        newKey[i] = newKey[j];
    }
    /*Adds null terminator to mark end of string*/
    newKey[i] = '\0';
    /*Stores encrypted message*/
    char encryptedMsg[msgLen];    
    /*For loop that goes through content and applys encryption algorithms*/
    for(i = 0; i < msgLen; ++i){
        /*Applies vigenere encryption to lowercase characters*/
        if (content[i] >= 'a' && content[i] <= 'z') {
            encryptedMsg[i] = ((content[i] - 'a' + (newKey[i] - 'A')) % 26) 
            + 'a';
        /*Applies vigenere encryption to uppercase characters*/
        } else if (content[i] >= 'A' && content[i] <= 'Z') {
            encryptedMsg[i] = ((content[i] - 'A' + (newKey[i] - 'A')) % 26) 
            + 'A';
        /*Copies newline characters and spaces as they are*/
        } else if (content[i] == '\n' || content[i] == ' ') {
            encryptedMsg[i] = content[i];
        } else {
        /*Encrypts any other characters by adding bytes*/
        encryptedMsg[i] = content[i] + 100;
        }       
    }
    /*Adds null terminator to mark end of string*/
    encryptedMsg[msgLen] = '\0';
    strcpy(content, encryptedMsg);
}
