/*
*   @author Christopher Lewis
*
*   @date - 2/22/2021
*
*   @brief XOR cypher.
*
*   @TODD lots of documentation
*   
*/


#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h> //< for mmap
#include <stdlib.h>

#include "encryptUtil.h"


unsigned long int fileSizeInBytes(char *file);
void restoreTestKeyfile(FILE *fileptr);
void restoreTestPlainFile(FILE *fileptr);

void leftRotate(unsigned char *keyFileHexValues, unsigned long int fileSize);


int main(int argc, char *argv[])
{
    unsigned long int sizeInBytes;
    unsigned long int chunkSize;

    unsigned char *plainText;

    FILE *keyfilePtr;
    FILE *plainTextFilePtr;

    sizeInBytes = fileSizeInBytes(argv[1]);

    //printf("This is the size of the KEYFILE in bytes %ld\n", sizeInBytes);

    keyfilePtr = fopen(argv[1],"r+b");
   
    if(keyfilePtr == NULL)
    {
        fprintf(stderr, "Error could not open keyfile or plainTextfile!\n");
        return 1;
    }

    // plainTextFilePtr = fopen(argv[2], "r+b");

    // if(keyfilePtr != NULL)
    // {
    //     restoreTestKeyfile(keyfilePtr);
    //     restoreTestPlainFile(plainTextFilePtr);
    //     fprintf(stderr,"FILE RESTORED!\n");
    //     return 1;
    // }
    

    // get file descriptor
    int fileDes = fileno(keyfilePtr);
    // use mmap to map file to process curren memory
    void *fileMap = mmap(NULL, sizeInBytes, PROT_READ|PROT_WRITE, MAP_SHARED,fileDes,0);
    

    // change stdin to read in binary mode
    freopen(NULL, "r+b", stdin);


    //allocate 'chunk' for plain text array.
    // allocateChunk();
    chunkSize = sizeInBytes;
    plainText = (unsigned char*)malloc(sizeof(unsigned char) * chunkSize);

   
    
    //printf("This was read in from stdin - %x\n", plainText[0]); 
    // change file map to char ptrs;
    unsigned char *hexValues = (unsigned char*)fileMap;
    fgets(plainText, 2 * chunkSize, stdin);
    fprintf(stderr,"OUR CHUNK SIZE IS - %ld \n", chunkSize);
    size_t chunkReadFromPlain = 0;

    
    while(0 < (chunkReadFromPlain = fread(plainText,1, chunkSize, stdin)) != 0)//<WHAT HAPPENS WHEN  NOT CLEAN MULTIPLES OF CHUNKS   
    {
        feof(stdin);
        
        fprintf(stderr,"How many bytes read from fread() - %ld\n", chunkReadFromPlain);

        for(int i = 0; i < chunkReadFromPlain ; i++)
        {            
            fprintf(stderr,"current hex value in keyfile is - %x\n", hexValues[i]);
            fprintf(stderr,"current hex value in plaintext file is - %x\n", plainText[i]);
            unsigned char currValue = hexValues[i] ^ plainText[i];
            fprintf(stderr,"This is the XOR'd value - %x\n", currValue);
            fwrite(&currValue, sizeof(currValue),1,stdout);
            fflush(stdout);

        }

        leftRotate(hexValues,sizeInBytes);
        
    }

    

    
    return 0;
}
void leftRotate(unsigned char *keyFileHexValues, unsigned long int fileSize)
{
    // cast map to unsigned char ptrs
    // unsigned char *hexValues = (unsigned char*)fileMap;

    unsigned char prev;
    unsigned char next;


    // will left rotate entire file
    for(unsigned long int i = 0; i < fileSize; i++)
    {
        prev = ( (keyFileHexValues[i] >> 7 ) & 1);
       // printf("This is current data before rotate: %x\n", keyFileHexValues[i]);
        next = keyFileHexValues[i];
        keyFileHexValues[i] = (keyFileHexValues[i] << 1 | prev ); 
        prev = next;
       // printf("This is current data after rotate: %x\n", keyFileHexValues[i]);
    }

}
void restoreTestKeyfile(FILE *file)
{

    unsigned char value = strtol("11110000", NULL, 2);

    fwrite(&value, sizeof(value),1,file);
    fwrite(&value, sizeof(value),1,file);

}
void restoreTestPlainFile(FILE *fileptr)
{
    unsigned char value1 = strtol("00000001", NULL, 2);
    unsigned char value2 = strtol("00000010", NULL, 2);
    unsigned char value3 = strtol("00000011", NULL, 2);
    unsigned char value4 = strtol("00000100", NULL, 2);
    unsigned char value5 = strtol("00010001", NULL, 2);
    unsigned char value6 = strtol("00010010", NULL, 2);
    unsigned char value7 = strtol("00010011", NULL, 2);
    unsigned char value8 = strtol("00010100", NULL, 2);
    unsigned char value9 = strtol("01000100", NULL, 2);
    unsigned char value10 = strtol("11010100", NULL, 2);
    unsigned char value11 = strtol("10010100", NULL, 2);



    fwrite(&value1, sizeof(value1),1,fileptr);
    fwrite(&value2, sizeof(value2),1,fileptr);
    fwrite(&value3, sizeof(value3),1,fileptr);
    fwrite(&value4, sizeof(value4),1,fileptr);
    fwrite(&value5, sizeof(value5),1,fileptr);
    fwrite(&value6, sizeof(value6),1,fileptr);
    fwrite(&value7, sizeof(value7),1,fileptr);
    fwrite(&value8, sizeof(value8),1,fileptr);
    fwrite(&value9, sizeof(value8),1,fileptr);
    fwrite(&value10, sizeof(value8),1,fileptr);
    fwrite(&value11, sizeof(value8),1,fileptr);

}
unsigned long int fileSizeInBytes(char *filename)
{

    FILE *currentFile = fopen(filename, "r+b");

    if(currentFile == NULL)
    {
        fprintf(stderr, "ERROR: Unable to open file - %s\n",filename);
        return -1; 
    }

    // fseek to end of file
    fseek(currentFile,0 , SEEK_END);


    unsigned long int sizeInBytes = ftell(currentFile);

    //close our file.

    fclose(currentFile);

    return sizeInBytes;
}