/*******************************************************************************
 * Header files used
*******************************************************************************/
#include "filemanagementprogram.h"
/*******************************************************************************
 * List preprocessing directives 
*******************************************************************************/
typedef unsigned int u32;
typedef unsigned char u8;
/*******************************************************************************
* Structures used and other definitions
*******************************************************************************/
/*The structure that define each RGB pixel in a .bmp image*/
struct RGB {
    u8 R;
    u8 G;
    u8 B;
};
typedef struct RGB RGB_t; /*defines the struc as RGT_t*/
/*******************************************************************************
 * Function prototypes
*******************************************************************************/
/*Generates values from key1 to be applied to the pixels during XOR decryption*/
u32 generateRandomValueForDecryption(u32 value);
/*Decrypts each pixel of the image back to its original form*/
RGB_t* decodePixels(RGB_t *pixels, u32 height, u32 width, u32 key1, u32 key2);
/*Reverses encryption, by calling decodePixels*/
RGB_t* decrypt(RGB_t *pixels, u32 height, u32 width, u32 key1, u32 key2);
/*Reads the dimensions of the image, including the width and height*/
void readBMPSizeForDecryption(u32 *height, u32 *width, FILE *img_input);
/*Reads in each pixel of the .bmp image to decrypt*/
RGB_t* readPixelsForDecryption(u32 height, u32 width, FILE *img_input);
/*Copies the header information from the file to preserve header infomration*/
void copyHeaderForDecryption(FILE *from, FILE *to);
/*Creates a new .bmp picture pixel by pixel after decryption*/
void writePixelsForDec(RGB_t *pixels, u32 height, u32 width, FILE *img_output);
/*Analyses the value of each pixel from the encrypted image*/
void analyzePixelDistributionForDecryption(RGB_t *pixels, u32 size);
/*Responsible for begining and handling the decryption process*/
void DecryptOperation(FILE *img_input, FILE *img_output, u32 key1, u32 key2);
/*******************************************************************************
 * Main Function -- Decrypts Image File (Restricted to BMP files)
 * Inputs: dbFileName -- File name of image to be encrypted
*******************************************************************************/
int decryptImage(char *dbFileName) {   
    /*Key used for XOR operations during decryption process*/
    u32 key1 = 123456789; 
	/*Another key used, but dosent change in value unlike the other one*/
    u32 key2 = 987654321; 

    /*File path to be saved as after decryption*/
    char destPath[MAX_FILE_NAME_SIZE] = "image_DECRYPTED.bmp"; 

    FILE *img_input = fopen(dbFileName, "rb"); /*Opens the file for decryption*/
    /*Makes and opens a new file to save results after decryption*/
    FILE *img_output = fopen(destPath, "wb"); 
    
    /*Begins decryption process*/
    DecryptOperation(img_input, img_output, key1, key2); 

    /*Closes files*/
    fclose(img_input); 
    fclose(img_output); 

    return 0;
}
/*******************************************************************************
 * Function -- Performs XOR operations
*******************************************************************************/
u32 generateRandomValueForDecryption(u32 value) {
    /*Shifts value left by 13 bits after XOR oprations are done*/
    value ^= value << 13; 
    /*Shifts value right by 17 bits after XOR oprations are done again*/
	value ^= value >> 17; 
    /*Shifts value left by 5 bits after XOR oprations are done again*/
	value ^= value << 5; 

	return value;
}
/*******************************************************************************
 * Function -- Decodes each of the inputed image
*******************************************************************************/
RGB_t* decodePixels(RGB_t *pixels, u32 height, u32 width, u32 key1, u32 key2) {
    int i;
    u32 size = height * width; /*Calcualtes total number of pixel in the image*/
    /*Allocates memory for an array of RGB_t*/
    RGB_t *crackdPxls = (RGB_t*) malloc(size * sizeof(RGB_t)); 

    /*Goes through each pixel from input pixels array and performs decryption*/
    for(i = 0; i < size; i++) {
        
        crackdPxls[i].B = pixels[i].B ^ (key1 & 0xFF) ^ (key2 & 0xFF);
        int A = ((key1 >> 8) & 0xFF);
        int B = ((key2 >> 8) & 0xFF);
        crackdPxls[i].G = pixels[i].G ^ A ^ B;
        int C = ((key1 >> 16) & 0xFF);
        int D = ((key2 >> 16) & 0xFF);
        crackdPxls[i].R = pixels[i].R ^ C ^ D;
        key1 = generateRandomValueForDecryption(key1);
    }

    return crackdPxls;
}
/*******************************************************************************
 * Function -- Decrypts the image
*******************************************************************************/
RGB_t* decrypt(RGB_t *pixels, u32 height, u32 width, u32 key1, u32 key2) {
    /*Calls the function to decrypt each pixel in the image*/
    RGB_t *crackdPxls = decodePixels(pixels, height, width, key1, key2);
    return crackdPxls;
}
/*******************************************************************************
 * Function -- Reads dimensions of the BMP file (width & height)
*******************************************************************************/
void readBMPSizeForDecryption(u32 *height, u32 *width, FILE *img_input) {
	fseek(img_input, 18, SEEK_SET);
    /*Reads the image to set the width*/
	fread(width, sizeof(int), 1, img_input);
	fseek(img_input, 22, SEEK_SET);
    /*Reads the image to set the height*/
	fread(height, sizeof(int), 1, img_input); 
}
/*******************************************************************************
 * Function -- Reads each pixel of the image
*******************************************************************************/
RGB_t* readPixelsForDecryption(u32 height, u32 width, FILE *img_input) {
    int i, j;
    /*Calculates padding required for image data structure*/
    u32 padding = (4 - (width * 3 % 4)) % 4; 
    /*Stores the pixel data of the image*/
    RGB_t *pixels = (RGB_t*) malloc(width * height * sizeof(RGB_t));

    fseek(img_input, HEADER_LENGTH, SEEK_SET);

    /*Iterates through the rows of the pixel data of the image*/
    for(i = height - 1; i >= 0; i--) {
            
        /*Iterates through the columns of the pixel data of the image*/
        for(j = 0; j < width; j++) {
            fread(&pixels[i * width + j].B, 1, 1, img_input);
            fread(&pixels[i * width + j].G, 1, 1, img_input);
            fread(&pixels[i * width + j].R, 1, 1, img_input);
        }
        fseek(img_input, padding, SEEK_CUR);
    }

    return pixels;
}
/*******************************************************************************
 * Function -- Copies the header of the image, ensures header information is 
 *             preserved when decrypting the file
*******************************************************************************/
void copyHeaderForDecryption(FILE *from, FILE *to) {
	/*Sets the pointers from the begining to the end of the file*/
    fseek(from, 0, SEEK_SET);
	fseek(to, 0, SEEK_SET);
    
    /*Allocates the size of the header length to memory*/
	u8 *header = (u8*) malloc(HEADER_LENGTH); 

    /*Writes the contents form header array to the 'to' file*/
	fread(header, HEADER_LENGTH, 1, from);
	fwrite(header, HEADER_LENGTH, 1, to);

    /*Frees memory after copying header information*/
	free(header);
}
/*******************************************************************************
 * Function -- Writes pixel by pixel to a new BMP file after decryption
*******************************************************************************/
void writePixelsForDec(RGB_t *pixels, u32 height, u32 width, FILE *img_output) {
    int i, j;
    u32 padding = (4 - (width * 3 % 4)) % 4; /*Sets padding*/
    u8 *paddingBytes = (u8*) calloc(padding, 1); /*Stores padding data*/

    fseek(img_output, HEADER_LENGTH, SEEK_SET);
    
    /*Goes through the columns of the image, moving from left to right and 
    writes individual colour components to the new file.*/
    for(i = height - 1; i >= 0; i--) {
        /*Goes through the rows of the image, moving from left to right 
        for the same process*/
        for(j = 0; j < width; j++) {
            fwrite(&pixels[i * width + j].B, 1, 1, img_output);
            fwrite(&pixels[i * width + j].G, 1, 1, img_output);
            fwrite(&pixels[i * width + j].R, 1, 1, img_output);
        }
        fwrite(paddingBytes, 1, padding, img_output);
    }

    /*Frees memory after function is completed*/
    free(paddingBytes);
}
/*******************************************************************************
 * Function -- Analyses the distribution of pixel values in the image to gain 
 *             information on where the pixel values are and distributed
*******************************************************************************/
void analyzePixelDistributionForDecryption(RGB_t *pixels, u32 size) {
    /*Stores color channel distributions of (Red, Green & Blue)*/
    u32 **channels = (u32**) malloc(3 * sizeof(u32*)); 
    /*For blue channel distribution*/
    channels[0] = (u32*) calloc(256, sizeof(u32)); 
    /*For green channel distribution*/
    channels[1] = (u32*) calloc(256, sizeof(u32)); 
    /*For red channel distribution*/
    channels[2] = (u32*) calloc(256, sizeof(u32)); 

    int i, j;

    /*Distributes the colour values by blue, red & green for each pixel*/
    for(i = 0; i < size; i++)
    {
        channels[0][pixels[i].B]++;
        channels[1][pixels[i].G]++;
        channels[2][pixels[i].R]++;
    }

    double valB = 0, valG = 0, valR = 0, ft = size / 256.0;
    
    /*Calculates the distrbution variation of each pixel*/
    for(j = 0; j < 256; j++)
    {
        valB += (channels[0][j] - ft) * (channels[0][j] - ft) / ft;
        valG += (channels[1][j] - ft) * (channels[1][j] - ft) / ft;
        valR += (channels[2][j] - ft) * (channels[2][j] - ft) / ft;
    }

    /*Frees memoery after function is completed*/
    free(channels[0]);
    free(channels[1]);
    free(channels[2]);
    free(channels);
}   
/*******************************************************************************
 * Function -- Handles the different functions for the decryption process but 
 *             mainly initialises the decryption process.
*******************************************************************************/
void DecryptOperation(FILE *img_input, FILE *img_output, u32 key1, u32 key2) {
	u32 height, width;

    /*Reads BMP dimensions*/
	readBMPSizeForDecryption(&height, &width, img_input);

    /*Reads and stores pixel data from image to a pointer*/
	RGB_t *pixels = readPixelsForDecryption(height, width, img_input);
	RGB_t *result;

    /*Saves the result of the decryption process*/
	result = decrypt(pixels, height, width, key1, key2);

    /*Preserbes the header information of the image*/
    
	copyHeaderForDecryption(img_input, img_output);
    /*Makes a new image based on the data stored in 'result'*/
	writePixelsForDec(result, height, width, img_output);

    /*Frees memory*/
	free(pixels);
	free(result);
}
