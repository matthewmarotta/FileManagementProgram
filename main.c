/*******************************************************************************
 * Header files used
*******************************************************************************/
#include "filemanagementprogram.h"
/*******************************************************************************
 * Function -- checks if character is uppercase (used for applying the key)
*******************************************************************************/
int upper_case(char c) {
    return (c >= 'A' && c <= 'Z');
}
/*******************************************************************************
 * Function -- checks if every character in the line is uppercase
*******************************************************************************/
int uppercase_line(const char *keyinput) {
    while (*keyinput) {
        if (!upper_case(*keyinput)) {
            return 0;  
        }
        keyinput++;
    }
    return 1;
}
/*******************************************************************************
 * Function -- main which allows command line parsing to call various functions
*******************************************************************************/
int main(int argc, char *argv[]) {

    if (argc < 4) { 
        printf("Fromat: %s <operation> <filename> <key>\n", argv[0]);
        printf("Operations: encrypt_text, decrypt_text, compress_text,\n"
        "encrypt_image, decrypt_image\n");
        return 1;
    }
/*******************************************************************************
* Command-line arguments
*******************************************************************************/
    /*Sets operation as second argument in the command line*/
    char *operation = argv[1];
    /*Sets file name as third argument in the command line*/
    char *dbFileName = argv[2];
    /*Key input from user used for encryption methods, essential for
    vigenere method*/
    char *newKey = argv[3];
    /*Ensures key typed in by user is uppercase*/
        if(!uppercase_line(newKey)){
            printf("Uppercase letters for key only!\n");
            return 1;
        }
         
         /*Call to text encryption function*/
        if (strcmp(operation, "encrypt_text") == 0) {
            char *content = getContent(dbFileName);
            encryptFile(content, newKey);
            writeContent(dbFileName, content);
            free(content);
            printf("Note: type in the same key for decryption");
            printf("as you did for encryption\n");
        } 
        
        /*Call to text decryption function*/
        else if (strcmp(operation, "decrypt_text") == 0) {
            char *content = getContent(dbFileName);
            decryptFile(content, newKey);
            writeContent(dbFileName, content);
            free(content);   
        } 
        
        /*Call to text compression function*/
        else if (strcmp(operation, "compress_text") == 0) {
            char *content = getContent(dbFileName);
            compress(content, dbFileName);
            free(content);
        }
        

        /*Call to image encryption function*/
        else if (strcmp(operation, "encrypt_image") == 0) {
            FILE *file_input = fopen(argv[2], "rb"); 
            /*To check if image inputed is valid*/
            if (file_input == NULL)  {
                printf("Invalid File. File must be a .bmp file");
            }
            else {
                encryptImage(dbFileName);
            }
        }
        
        /*Call to image decryption function*/
        else if (strcmp(operation, "decrypt_image") == 0) {
            FILE *file_input = fopen(argv[2], "rb"); 
            /*To check if image inputed is valid*/
            if (file_input == NULL)  {
                printf("Invalid File. File must be a .bmp file");
            }
            else {
                decryptImage(dbFileName);
            }
        }
        /*If input for operation is not listed, returns*/
        else {
            printf("Invalid operation: %s\n", operation);
            return 1;
        }
        return 0;
    }
