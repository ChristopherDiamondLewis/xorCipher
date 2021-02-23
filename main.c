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

#include "encryptUtil.h"


unsigned long int fileSizeInBytes(char *file);
void restoreTestfile(FILE *fileptr);

void leftRotate(void *fileMap, unsigned long int fileSize);

int main(int argc, char *argv[])
{
    unsigned long int sizeInBytes;

    unsigned char plainText[2];
    unsigned char keyFileText[2];

    FILE *keyfilePtr;

    sizeInBytes = fileSizeInBytes(argv[1]);

    printf("This is the size of the KEYFILE in bytes %ld\n", sizeInBytes);


    keyfilePtr = fopen(argv[1],"r+b");

    if(keyfilePtr == NULL)
    {
        fprintf(stderr, "Error could not open keyfile!\n");
        return 1;
    }

    // restoreTestfile(keyfilePtr);

    // get file descriptor
    int fileDes = fileno(keyfilePtr);
    // use mmap to map file to process curren memory
    void *fileMap = mmap(NULL, sizeInBytes, PROT_READ|PROT_WRITE, MAP_SHARED,fileDes,0);
    
    leftRotate(fileMap,sizeInBytes);
    

    // change stdin to read in binary mode
    //freopen(NULL, "r+b", stdin);

    


    //read in 2 bytes, chunk amount of times
    //fgets(plainText, 2, stdin); 


    // fgets(keyFileText, 2, keyfilePtr);

    // printf("This is what we got from stdin in first spot %x\n", *plainText);
    // printf("This is what we got from keyfile in first spot %x\n", *keyFileText);
    

   // move file pointer to next two btyes and repeat.
    //fseek(stdin, 2 ,SEEK_CUR);
    //  fseek(keyfilePtr, 0, SEEK_CUR);

   

    // fgets(keyFileText, 2, keyfilePtr);
    //fgets(plainText, 2, stdin); 

    // printf("This is what we got from keyfile in second spot %x\n", *keyFileText);

    // unsigned char cipherResult[2];
    // *cipherResult = *plainText ^= *keyFileText;

    // printf("This is stdin XOR'd with keyfile value %x\n", *cipherResult);

    return 0;
}
void leftRotate(void *fileMap, unsigned long int fileSize)
{
    // cast map to unsigned char ptrs
    unsigned char *hexValues = (unsigned char*)fileMap;

    unsigned char prev;
    unsigned char next;


    // will left rotate entire file
    for(unsigned long int i = 0; i < fileSize; i++)
    {
        prev = ( (hexValues[i] >> 7 ) & 1);
        printf("This is current data: %x\n", hexValues[i]);
        next = hexValues[i];
        hexValues[i] = (hexValues[i] << 1 | prev ); 
        prev = next;
        printf("This is current data after modifying: %x\n", hexValues[i]);
    }
}
void restoreTestfile(FILE *file)
{

    unsigned char value = strtol("11110000", NULL, 2);

    fwrite(&value, sizeof(value),1,file);
    fwrite(&value, sizeof(value),1,file);

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