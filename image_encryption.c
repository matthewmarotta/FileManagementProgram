/*******************************************************************************
 * Header files used
*******************************************************************************/
#include "filemanagementprogram.h"
/*******************************************************************************
 * List preprocessing directives 
*******************************************************************************/
typedef unsigned char u8;
typedef unsigned int u32;
/*******************************************************************************
* Structures used and other definitions
*******************************************************************************/
struct RGB {
    u8 R;
    u8 G;
    u8 B;
};
typedef struct RGB RGB_t;
/*******************************************************************************
 * Function prototypes
*******************************************************************************/
/*Generates values from key1 to be applied to the pixels during XOR encryption*/
u32 generateRandomValueForEncryption(u32 value);
/*Encrypts each pixel of the image*/
RGB_t* encodePixels(RGB_t *pixels, u32 height, u32 width, u32 key1, u32 key2);
/*Encrypts image, by calling encodePixels*/
RGB_t* encrypt(RGB_t *pixels, u32 height, u32 width, u32 key1, u32 key2);
/*Reads the dimensions of the image, including the width and height*/
void readBMPSizeForEncryption(u32 *height, u32 *width, FILE *img_inp);
/*Reads in each pixel of the .bmp image to encrypt*/
RGB_t* readPixelsForEncryption(u32 height, u32 width, FILE *img_inp);
/*Copies the header information of the image to preserve it*/
void copyHeaderForEncryption(FILE *from, FILE *to);
/*Creates a new .bmp picture pixel by pixel after encryption*/
void writePixelsForEnc(RGB_t *pixels, u32 height, u32 width, FILE *img_out);
/*Analyses the value of each pixel from the image*/
void analyzePixelDistributionForEncryption(RGB_t *pixels, u32 size);
/*Responsible for begining and handling the encryption process*/
void EncryptOperation(FILE *img_inp, FILE *img_out, u32 key1, u32 key2);
/*******************************************************************************
 * Main Function -- Encrypts Image File (Restricted to BMP files)
 * Inputs: dbFileName -- File name of image to be encrypted
*******************************************************************************/
int encryptImage(char *dbFileName) {
    /*Key used for XOR operations during decryption process*/
    u32 key1 = 123456789; 
	/*Another key used, but dosent change in value unlike the other one*/
    u32 key2 = 987654321; 

    /*File path to be saved as after decryption*/
    char destPath[MAX_FILE_NAME_SIZE] = "image_ENCRYPTED.bmp"; 

    FILE *img_inp = fopen(dbFileName, "rb"); /*Opens the file*/
    /*Makes and opens a new file to save results after decryption*/
    FILE *img_out = fopen(destPath, "wb"); 

    /*Begins encryption process*/
    EncryptOperation(img_inp, img_out, key1, key2); 
    
    fclose(img_inp); /*Closes the file to be encrypted after use*/
    fclose(img_out); /*Closes the file after encryption is completed*/
    
    return 0;
}
/*******************************************************************************
 * Function -- Performs XOR operations, and shifts either left or right
*******************************************************************************/
u32 generateRandomValueForEncryption(u32 value) {
    value ^= value << 13; /*XOR performed then shifted left by 13 bits*/
	value ^= value >> 17; /*XOR performed again then shifted right by 17 bits*/
	value ^= value << 5; /*XOR performed again then shifted left by 5 bits*/

	return value;
}
/*******************************************************************************
 * Function -- Encodes each of the inputed image
*******************************************************************************/
RGB_t* encodePixels(RGB_t *pixels, u32 height, u32 width, u32 key1, u32 key2) {
    int i;
    u32 size = height * width; /*Calcualtes total numb of pixels in the image*/
    /* allocates memory for an array of RGB_t*/
    RGB_t *encodedPixels = (RGB_t*) malloc(size * sizeof(RGB_t)); 

    /*Goes through each pixel in the input pixels array and performs 
    encryption*/
    for(i = 0; i < size; i++) {
        encodedPixels[i].B = pixels[i].B ^ (key1 & 0xFF) ^ (key2 & 0xFF);
        int A = ((key1 >> 8) & 0xFF);
        int B = ((key2 >> 8) & 0xFF);
        encodedPixels[i].G = pixels[i].G ^ A ^ B;
        int C = ((key1 >> 16) & 0xFF);
        int D = ((key2 >> 16) & 0xFF);
        encodedPixels[i].R = pixels[i].R ^ C ^ D;
        key1 = generateRandomValueForEncryption(key1);
    }

    return encodedPixels;
}
/*******************************************************************************
 * Function -- Encrypts the image
*******************************************************************************/
RGB_t* encrypt(RGB_t *pixels, u32 height, u32 width, u32 key1, u32 key2) {
    /*Calls the function to encrypt each pixel in the image*/
    RGB_t *encodedPixels = encodePixels(pixels, height, width, key1, key2);
    return encodedPixels;
}
/*******************************************************************************
 * Function -- Reads dimensions of the BMP file (width & height)
*******************************************************************************/
void readBMPSizeForEncryption(u32 *height, u32 *width, FILE *img_inp) {
	fseek(img_inp, 18, SEEK_SET);
	/*Reads the image to set the width*/
    fread(width, sizeof(int), 1, img_inp); 
	fseek(img_inp, 22, SEEK_SET);
	/*Reads the image to set the height*/
    fread(height, sizeof(int), 1, img_inp); 
}
/*******************************************************************************
 * Function -- Reads each pixel from the BMP file 
*******************************************************************************/
RGB_t* readPixelsForEncryption(u32 height, u32 width, FILE *img_inp) {
    int i, j;
    /*Calculates padding required for image data structure*/
    u32 padding = (4 - (width * 3 % 4)) % 4; 
    /*Stores the pixel data of the image*/
    RGB_t *pixels = (RGB_t*) malloc(width * height * sizeof(RGB_t));

    fseek(img_inp, HEADER_LENGTH, SEEK_SET);

    /*Iterates through the rows of the pixel data of the image*/
    for(i = height - 1; i >= 0; i--) {
            
        /*Iterates through the columns of the pixel data of the image*/
        for(j = 0; j < width; j++) {
            fread(&pixels[i * width + j].B, 1, 1, img_inp);
            fread(&pixels[i * width + j].G, 1, 1, img_inp);
            fread(&pixels[i * width + j].R, 1, 1, img_inp);
        }
        fseek(img_inp, padding, SEEK_CUR);
    }

    return pixels;
}
/*******************************************************************************
 * Function -- Copies the header of the image, ensures header information is 
 *             preserved when encrypting the file
*******************************************************************************/
void copyHeaderForEncryption(FILE *from, FILE *to) {
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
 * Function -- Writes pixels to a new BMP file after encryption
*******************************************************************************/
void writePixelsForEnc(RGB_t *pixels, u32 height, u32 width, FILE *img_out) {
    int i, j;
    u32 padding = (4 - (width * 3 % 4)) % 4; /*Sets padding*/
    u8 *paddingBytes = (u8*) calloc(padding, 1); /*Stores padding data*/

    fseek(img_out, HEADER_LENGTH, SEEK_SET);
    
    /*Goes through the columns of the image, moving from left to right and 
    writes individual colour components to the new file.*/
    for(i = height - 1; i >= 0; i--)
    {
        /*Goes through the rows of the image, moving from left to right 
        for the same process*/
        for(j = 0; j < width; j++)
        {
            fwrite(&pixels[i * width + j].B, 1, 1, img_out);
            fwrite(&pixels[i * width + j].G, 1, 1, img_out);
            fwrite(&pixels[i * width + j].R, 1, 1, img_out);
        }
        fwrite(paddingBytes, 1, padding, img_out);
    }

    /*Frees memory after function is completed*/
    free(paddingBytes);
}
/*******************************************************************************
 * Function -- Analyses the distribution of pixel values in the image to gain 
 *             information on where the pixel values are and distributed
*******************************************************************************/
void  analyzePixelDistributionForEncryption(RGB_t *pixels, u32 size) {
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
void EncryptOperation(FILE *img_inp, FILE *img_out, u32 key1, u32 key2) {
	u32 height, width;

    /*Reads BMP dimensions*/
	readBMPSizeForEncryption(&height, &width, img_inp);

	RGB_t *pixels = readPixelsForEncryption(height, width, img_inp);
	RGB_t *result;

    /*Saves the result of the decryption process*/
	result = encrypt(pixels, height, width, key1, key2);

    /*Preserbes the header information of the image*/

	copyHeaderForEncryption(img_inp, img_out);
    /*Makes a new image based on the data stored in 'result'*/
	writePixelsForEnc(result, height, width, img_out);
    
    /*Frees memory*/
	free(pixels);
	free(result);
}
