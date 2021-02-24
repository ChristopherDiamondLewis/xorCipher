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


#include "encryptUtil.h"


void restoreTestKeyfile(FILE *fileptr);
void restoreTestPlainFile(FILE *fileptr);

int main(int argc, char *argv[])
{

    if(checkArgs(argc,argv) == EXIT_FAILURE)
    {
        return 1;
    }

    chunkCount = 0;

    // unsigned long int keyfileSizeInBytes;
    // unsigned char *plainTextValues;

    

    // FILE *plainTextValuesFilePtr;
    // keyfilePtr = fopen(argv[1],"r+b");
   
    // if(keyfilePtr == NULL)
    // {
    //     fprintf(stderr, "Error could not open keyfile or plainTextValuesfile!\n");
    //     return 1;
    // }

    // plainTextValuesFilePtr = fopen(argv[2], "r+b");

    // if(keyfilePtr != NULL)
    // {
    //     restoreTestKeyfile(keyfilePtr);
    //     restoreTestPlainFile(plainTextValuesFilePtr);
    //     fprintf(stderr,"FILE RESTORED!\n");
    //     return 1;
    // }
    
    keyfileSizeInBytes = fileSizeInBytes(keyfilePtr);

    numofThreads = 2;

    // get file descriptor for keyfile 
    int fileDes = fileno(keyfilePtr);
    // use mmap to map file to process current memory
    void *fileMap = mmap(NULL, keyfileSizeInBytes, PROT_READ|PROT_WRITE, MAP_SHARED,fileDes,0);
    
    //allocate 'chunk' for plain text array.
    plainTextValues = (unsigned char*)malloc(sizeof(unsigned char) * keyfileSizeInBytes * numofThreads);
 
    // cast file map to array of unsigned chars
    initialkeyFileValues = (unsigned char*)fileMap;

    pthread_t threadArr[numofThreads];

    // keep reading until bytes read less than 0
    bytesReadFromPlain = fread(plainTextValues,1, keyfileSizeInBytes * numofThreads, stdin);
    while(bytesReadFromPlain > 0 )
    {
        for(int i = 0 ; i < bytesReadFromPlain; i++)
        {
            fprintf(stderr,"These were the bytes read - %x\n",plainTextValues[i]);
        }


        fprintf(stderr,"THESE ARE OUR STARTING KEYVALUES\n");

        for(int i = 0 ; i < keyfileSizeInBytes; i++)
        {
            fprintf(stderr,"%x ",initialkeyFileValues[i]);
        }
        fprintf(stderr,"\n\n");

        for(int i = 0; i < numofThreads ; i++)
        {
            pthread_create(&threadArr[i],NULL, &threadFunc, (void*)(long)i);
        }

        for(int i = 0; i < numofThreads; i++)
            {
                pthread_join(threadArr[i], NULL);
            }
        
        // write this result to stdout and do next chunk
        for(int i = 0; i < bytesReadFromPlain; i++)
        {
            fwrite(&plainTextValues[i], sizeof(unsigned char),1,stdout);
            fflush(stdout);
        }
        chunkCount =+numofThreads;

       bytesReadFromPlain = fread(plainTextValues,1, keyfileSizeInBytes * numofThreads, stdin);
    }

    fclose(keyfilePtr);
    free(plainTextValues);

    return 0;
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
