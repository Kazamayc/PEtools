#include <stdio.h>
#include <memory>
int getlen(FILE* fp);
char* FileGet(char* PATH);
void PEHeadGet(char* PATH);
int main()
{
    char a[] = {"C:\\test.exe"};
    PEHeadGet(a);
}


void PEHeadGet(char* PATH)
{
    char* FileBuffer = FileGet(PATH);
    if(FileBuffer==NULL)
    {
        return;
    }

    //Determine whether it is a valid MZ mark
    if(*(short*)FileBuffer != 0X5A4D)
    {
        printf("Not a valid MZ mark");
        free(FileBuffer);
		return;
    }

    //Print DOS HEAD
    printf("----------------------------\n");
    printf("----------DOS HEAD----------\n");
    printf("----------------------------\n");
    printf(">>WORD e_magic: 0x%X\n",*(short*)FileBuffer);
    printf(">>DWORD e_lfanew: 0x%X\n",*(int*)(FileBuffer+60));
    int e_lfanew = *(int*)(FileBuffer+60);

    //Determine whether it is a valid PE mark
    if(*(int*)(FileBuffer + e_lfanew) != 0x4550){
        printf("Not a valid PE mark");
        free(FileBuffer);
        return;
    }

    //Print PE HEAD(NT PE OPTION)
    printf("----------------------------\n");
    printf("----------NT HEAD-----------\n");
    printf("----------------------------\n");
    printf(">> DWORD Signature: 0x%X\n",*(int*)(FileBuffer + e_lfanew));


    free(FileBuffer);
    return;
}

char* FileGet(char* PATH)
{
    FILE* fp = NULL;
    int FileSize = 0;
    char* FileBuffer = NULL;

    //Open file
    printf(">>Loading......\n");
    fp = fopen(PATH,"rb");
    if(fp==NULL)
    {
        printf(">>Failed to open file!\n");
        return NULL;
    }

    //Read file size
    FileSize = getlen(fp);
    printf(">>File len: %dByte\n",FileSize);

    //Allocate space in memory
    FileBuffer = (char*)malloc(FileSize);
    if(FileBuffer == NULL)
    {
        printf(">>Failed to allocate space!\n");
        return NULL;
    }
    printf(">>File in memory: %d\n",FileBuffer);

    //Move file data into allocated space
    size_t read = fread(FileBuffer,1,FileSize,fp);

    if(!read)
    {
        printf(">>Failed to read data!\n");
        free(FileBuffer);
        fclose(fp);
        return NULL;
    }

    //Close file
    fclose(fp);
    return FileBuffer;

}
int getlen(FILE* fp)
{
    fseek(fp,0,SEEK_END);
    size_t a = ftell(fp);
    fseek(fp,0,SEEK_SET);
    return a;
}





