#ifndef FILEMANAGEMENTPROGRAM_H
#define FILEMANAGEMENTPROGRAM_H
/*******************************************************************************
 * Header files used
*******************************************************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*******************************************************************************
 * Constants/macros
*******************************************************************************/
/*limit for image header size*/
#define HEADER_LENGTH 54
/*limit for image file size*/
#define MAX_FILE_NAME_SIZE 200
/*sets limit for huffman tree size*/
#define MAX_TREE_HT 50
/*******************************************************************************
 * Structs used
*******************************************************************************/
/*Structure to represent a node in the Huffman tree*/
struct MinHNode {
    char item;
    unsigned freq;
    struct MinHNode *left, *right;
};
/*Structure to represent a minimum heap (or priority queue)*/
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHNode **array;
};
/*Structure to store Huffman codes for characters*/
typedef struct {
    char character;
    char huffmanCode[MAX_TREE_HT];
    int codeLength;
} HuffmanCode;
/*******************************************************************************
 * Function prototypes used in the File Management Program
*******************************************************************************/
void encryptFile(char *content, char newKey[]); /*Encrypts text file*/

void decryptFile(char *content, char newKey[]); /*Decrypts text file*/

char *getContent(char *dbFileName); /*Gets content from text file for 
encryption/decryption/compression*/

void writeContent(char *dbFileName, char *content); /*Writes content to text 
file after encryption/decryption/compression*/

int compress(char *content, char *dbFileName); /*Compresses text 
file*/

int encryptImage(char *dbFileName); /*Encrypts Image*/

int decryptImage(char *dbFileName); /*Decrypts Image*/

#endif /*FILEMANAGEMENTPROGRAM_H*/
