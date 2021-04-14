#include <stdio.h>
#include <memory>
#include <memory.h>
int getlen(FILE* fp);
int read()
{
    FILE* fp = NULL;
    FILE* fp2 = NULL;
    char* FileBuffer = NULL;
    char* ImageBuffer = NULL;
    char* NewBuffer = NULL;
    int FileSize = 0;

    fp = fopen("D:/code/fg.exe","rb");
    fp2 = fopen("D:/code/test.exe","wb");
    if(fp == NULL || fp2 == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }
    FileSize = getlen(fp);

    FileBuffer = (char*)malloc(FileSize);
    if(FileBuffer == NULL)
    {
        printf("Failed to allocate Filebuffer!\n");
        fclose(fp);
        fclose(fp2);
        return -1;
    }
    memset(FileBuffer, 0, FileSize);

    fread(FileBuffer, 1, FileSize, fp);
    fclose(fp);
    int e_lfanew = *(int*)(FileBuffer+60);
    int SizeOfHeaders = *(int*)(FileBuffer+e_lfanew+84);
    int count = 0;
    short NumberOfSections = *(short*)(FileBuffer+e_lfanew+6);
    short SizeOfOptionalHeader = *(short*)(FileBuffer+e_lfanew+20);
    int VirtualAddress = *(int*)(FileBuffer+e_lfanew+SizeOfOptionalHeader+24+12);
    int SizeOfRawData = *(int*)(FileBuffer+e_lfanew+SizeOfOptionalHeader+24+16);
    int PointerToRawData = *(int*)(FileBuffer+e_lfanew+SizeOfOptionalHeader+24+20);
    int SizeOfImage = *(int*)(FileBuffer+e_lfanew+80);
    ImageBuffer = (char*)malloc(SizeOfImage);

    if(ImageBuffer == NULL)
    {
        printf("Failed to allocate ImageBuffer");
        free(FileBuffer);
        fclose(fp);
        fclose(fp2);
        return -1;
    }
    memset(ImageBuffer, 0, SizeOfImage);
    memcpy(ImageBuffer, FileBuffer, SizeOfHeaders);
    for(int i = 0; i < NumberOfSections; i++)
    {
        int VirtualAddress = *(int*)(FileBuffer+e_lfanew+SizeOfOptionalHeader+24+12+count);
        int SizeOfRawData = *(int*)(FileBuffer+e_lfanew+SizeOfOptionalHeader+24+16+count);
        int PointerToRawData = *(int*)(FileBuffer+e_lfanew+SizeOfOptionalHeader+24+20+count);
        memcpy(ImageBuffer+VirtualAddress,FileBuffer+PointerToRawData,SizeOfRawData);
        count+=40;
    }



    NewBuffer = (char*)malloc(FileSize);
    if(NewBuffer == NULL)
    {
        printf("Failed to allocate NewBuffer!");
        free(FileBuffer);
        free(ImageBuffer);
        fclose(fp);
        fclose(fp2);
        return -1;
    }
    memset(NewBuffer, 0, FileSize);
    memcpy(NewBuffer,ImageBuffer,SizeOfHeaders);
    int count2 = 0;
    for(int i = 0; i < NumberOfSections; i++)
    {
        int VirtualAddress = *(int*)(FileBuffer+e_lfanew+SizeOfOptionalHeader+24+12+count2);
        int SizeOfRawData = *(int*)(FileBuffer+e_lfanew+SizeOfOptionalHeader+24+16+count2);
        int PointerToRawData = *(int*)(FileBuffer+e_lfanew+SizeOfOptionalHeader+24+20+count2);
        memcpy(NewBuffer+PointerToRawData,ImageBuffer+VirtualAddress,SizeOfRawData);
        count2 += 40;
    }
    fwrite(NewBuffer,1,FileSize,fp2);
    fclose(fp);
    fclose(fp2);
    free(FileBuffer);
    free(ImageBuffer);
    free(NewBuffer);
    return 0;
}
int getlen(FILE* fp)
{
    fseek(fp,0,SEEK_END);
    int len = ftell(fp);
    fseek(fp,0,SEEK_SET);
    return len;
}
int main()
{
    read();
    return 0;
}