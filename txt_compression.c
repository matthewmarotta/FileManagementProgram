/*******************************************************************************
 * Header files used
*******************************************************************************/
#include "filemanagementprogram.h"
/*******************************************************************************
 * List preprocessing directives 
*******************************************************************************/
HuffmanCode codes[256];
int codesCount = 0;
/*******************************************************************************
 * Function -- Creates nodes for huffman tree
*******************************************************************************/
struct MinHNode *newNode(char item, unsigned freq) {
    /*Dynamically allocate memory for a new Huffman tree node*/
    struct MinHNode *temp = (struct MinHNode *)malloc(sizeof(struct MinHNode));
    /*Set the left and right child pointers to NULL*/
    temp->left = temp->right = NULL;
    /*Initialize the character and frequency from the arguments*/
    temp->item = item;
    temp->freq = freq;
    /*Return a pointer to the newly created node*/
    return temp;
}
/*******************************************************************************
 * Function -- Creates min heap
*******************************************************************************/
struct MinHeap *createMinH(unsigned capacity) {
    /*Dynamically allocate memory for a min heap structure*/
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    /*Initialize the size of the min heap to 0*/
    minHeap->size = 0;
    /*Set the capacity of the min heap with the given value*/
    minHeap->capacity = capacity;
    /*Allocate memory for the array of pointers to tree nodes*/
    minHeap->array = (struct MinHNode **)malloc(minHeap->capacity 
    * sizeof(struct MinHNode *));
    return minHeap;
}
/*******************************************************************************
 * Function -- swaps nodes in tree
*******************************************************************************/
void swapMinHNode(struct MinHNode **a, struct MinHNode **b) {
    /*Swap the pointers of two min heap nodes*/
    struct MinHNode *t = *a;
    *a = *b;
    *b = t;
}
/*******************************************************************************
 * Function -- Heapify which rearanges the tree so parents are always smaller
 in size than the parent node
*******************************************************************************/
void minHeapify(struct MinHeap *minHeap, int idx) {
    /*Perform the heapify operation to maintain the min heap property*/
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    /*Ensure that for any given node, its frequency is less than or 
    equal to its children's frequencies*/
    if (left < minHeap->size && minHeap->array[left]->freq < 
    minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < 
    minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}
/*******************************************************************************
 * Function -- checks if size of tree is 1
*******************************************************************************/
int checkSizeOne(struct MinHeap *minHeap) {
/*Return 1 if the heap size is 1, otherwise return 0*/
    return (minHeap->size == 1);
}
/*******************************************************************************
 * Function -- Extracts the minimum value node from the heap
*******************************************************************************/
struct MinHNode *extractMin(struct MinHeap *minHeap) {
    /*Remove and return the root node of the min heap*/
    struct MinHNode *temp = minHeap->array[0];
    /*Replace the root with the last element in the heap and reduce the size*/
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    /*Heapify the root of the tree to maintain the min heap property*/
    minHeapify(minHeap, 0);

    return temp;
}
/*******************************************************************************
 * Function -- Insertion function that inserts a new node into the min heap
*******************************************************************************/
void insertMinHeap(struct MinHeap *minHeap, struct MinHNode *minHeapNode) {
    /*Insert a new node into the min heap while maintaining the heap property*/
    ++minHeap->size;
    int i = minHeap->size - 1;
    /*Place the new node at the correct position in the heap*/
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

/*******************************************************************************
 * Function -- Function that builds the min heap using the given array of nodes
*******************************************************************************/
void buildMinHeap(struct MinHeap *minHeap) {
    /*Build a min heap from an array of min heap nodes*/
    int n = minHeap->size - 1;
    int i;
/*Call minHeapify for all non-leaf nodes to create a heap from bottom up*/
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}
/*******************************************************************************
 * Function -- Function to check if a given node is a leaf of the Huffman tree
*******************************************************************************/
int isLeaf(struct MinHNode *root) {
    /*Check if a node is a leaf (has no children)*/
    return !(root->left) && !(root->right);
}
/*******************************************************************************
 * Function --to create and build a min heap from character and frequency arrays
*******************************************************************************/
struct MinHeap *createAndBuildMinHeap(char item[], int freq[], int size) {
/*Create a min heap and insert all characters with their frequencies*/
    struct MinHeap *minHeap = createMinH(size);
    int i;
    for (i = 0; i < size; ++i)
        minHeap->array[i] = newNode(item[i], freq[i]);

    minHeap->size = size;
     /*Build the min heap using the created nodes*/
    buildMinHeap(minHeap);

    return minHeap;
}
/*******************************************************************************
 * Function -- Function to build the Huffman tree given 
 character and frequency arrays
*******************************************************************************/
struct MinHNode *buildHuffmanTree(char item[], int freq[], int size) {
    /*Build the Huffman tree and return the root*/
    struct MinHNode *left, *right, *top;
    struct MinHeap *minHeap = createAndBuildMinHeap(item, freq, size);

    while (!checkSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
/*Combine two minimum frequency nodes until only one node is left, 
which becomes the root*/
        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}
/*******************************************************************************
 * Function -- Function to store Huffman codes in an array 
 from the Huffman tree
*******************************************************************************/
void storeHCodes(struct MinHNode *root, int arr[], int top) {
/*Traverse the Huffman tree and store Huffman codes in the 'codes' array*/
    if (root->left) {
        arr[top] = 0;
        storeHCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        storeHCodes(root->right, arr, top + 1);
    }
/*Use a recursive approach to traverse the tree and build codes*/
    if (isLeaf(root)) {
        codes[codesCount].character = root->item;
        int i;
        for (i = 0; i < top; i++) {
            codes[codesCount].huffmanCode[i] = arr[i] + '0'; 
        }
        codes[codesCount].huffmanCode[top] = '\0';
        codes[codesCount].codeLength = top;
        codesCount++;
    }
}

/*******************************************************************************
 * Function -- Wrapper function to generate Huffman codes for given 
 characters and frequencies
*******************************************************************************/
void HuffmanCodes(char item[], int freq[], int size) {
/*Build Huffman tree and store codes using the tree*/
  struct MinHNode *root = buildHuffmanTree(item, freq, size);

  int arr[MAX_TREE_HT], top = 0;

  storeHCodes(root, arr, top);
}

/*******************************************************************************
 * Function -- Function to compress the content using 
 the generated Huffman codes
*******************************************************************************/
char *compressContent(char *content) {
/*Use Huffman codes to compress the content and return the compressed version*/
    char *compressedContent = malloc(strlen(content) * MAX_TREE_HT); 
    int position = 0;
    int i,j;
    for (i = 0; i < strlen(content); i++) {
        for (j = 0; j < codesCount; j++) {
            if (content[i] == codes[j].character) {
                strcpy(&compressedContent[position], codes[j].huffmanCode);
                position += codes[j].codeLength;
                break;
            }
        }
    }
    compressedContent[position] = '\0';
    return compressedContent;
}







/*******************************************************************************
 * Function -- Function to perform compression of content and write to file
*******************************************************************************/
int compress(char *content, char *dbFileName) {
/*Check if the content pointer is NULL, indicating an error 
in retrieving the content.*/    
    if (content == NULL) {
        printf("Failed to get content from file.\n");
        return 1; 
    }
/*Calculate the length of the content*/
    int size = strlen(content);
/*Initialize an array to keep track of the frequency of each ASCII character*/
    int freq[256] = {0};
    int i;
/*Iterate over the content and count the occurrences of each character*/
    for (i = 0; i < size; i++) {
        freq[(unsigned char)content[i]]++;
    }
/*Arrays to store unique characters and their corresponding frequencies*/
    char unique_chars[256]; 
    int unique_freq[256];
    int unique_count = 0;
/*Filter out the unique characters and their frequencies*/
    for (i = 0; i < 256; i++) { 
        if (freq[i] != 0) {
            unique_chars[unique_count] = i;
            unique_freq[unique_count] = freq[i];
            unique_count++;
        }
    }
/*Build Huffman codes for the unique characters based on their frequencies*/
    HuffmanCodes(unique_chars, unique_freq, unique_count);
/*Compress the content using the generated Huffman codes*/
    char *compressedContent = compressContent(content);
/*Write the compressed content to the specified file*/
    writeContent(dbFileName, compressedContent);
/*Free the allocated memory for the compressed content*/
    free(compressedContent);
    return 0;
}
