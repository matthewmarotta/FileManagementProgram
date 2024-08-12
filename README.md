README

/*******************************************************************************
* Program description
*
* File Management Program
* Author: Matthew Marotta
* Description:
* This program is designed for encrypting, decrypting, and compressing TXT files and
* BMP images. The program uses command-line arguments to specify the operation
* and the file upon which to operate. It is especially useful for securing the contents of files
* by encryption and for saving disk space through compression.
* 
* Intended Use:
* To encrypt and decrypt text files using a Vigenère cipher where the key is
* required to be in uppercase.
* To compress text files to reduce their size.
* To encrypt and decrypt bitmap image files (.bmp) to secure their contents.
* 
* How to Run:
* Compile the program by typing "make" in the terminal, linking any necessary files and libraries.
* Run the program from the command line or terminal with the appropriate arguments.
* The general format for running the program is as follows:
* 
* ./main.out <operation> <filename> <key>
*   
* 
* Where:
* - The operation you wish to perform. Options include:
* encrypt_text, decrypt_text, compress_text, encrypt_image, decrypt_image.
*   
* - The name of the file on which the operation will be performed.
* - A sequence of uppercase letters that will serve as the key for
* encryption or decryption operations.
*   
* 
* Examples:
* To encrypt a text file named 'example.txt' with the key 'KEY':
*  ./main.out encrypt_text example.txt KEY
*   
* 
* To decrypt the same file with the same key:
*  ./main.out decrypt_text example.txt KEY
*   
* 
* To compress the same text file:
*  ./main.out compress_text example.txt KEY
*   
* 
* Notes:
* Ensure the key for encryption is the same as for decryption.
* For image encryption and decryption, the file must be a valid .bmp file.
* The program does not perform file format validation for text encryption and
* decryption.
* 
*******************************************************************************/
