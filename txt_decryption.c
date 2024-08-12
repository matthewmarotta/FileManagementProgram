#include "filemanagementprogram.h"
/*******************************************************************************
 * Function -- Decrypts text contained in file
*******************************************************************************/
void decryptFile(char *content, char newKey[]){
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
    char decryptedMsg[msgLen];       
    /*For loop that goes through content and applys encryption algorithms*/
    for(i = 0; i < msgLen; ++i){
        /*Removes vigenere encryption from lowercase characters*/
        if (content[i] >= 'a' && content[i] <= 'z') {
            decryptedMsg[i] = ((content[i] - 'a' - (newKey[i] - 'A') +26 ) % 26) + 'a';
        /*Removes vigenere encryption to uppercase characters*/
        } else if (content[i] >= 'A' && content[i] <= 'Z') {
            decryptedMsg[i] = ((content[i] - 'A' - (newKey[i] - 'A') + 26) % 26)
            + 'A';
        } else if (content[i] == '\n' || content[i] == ' ') {
        /*Copies newline characters and spaces as they are*/
            decryptedMsg[i] = content[i];
        } else {
        /*Any other characters are decrypted by removing bytes*/
        decryptedMsg[i] = content[i] - 100;   
        }       
    }
    /*Adds null terminator to mark end of string*/
    decryptedMsg[msgLen] = '\0';
    strcpy(content, decryptedMsg);
}
