#include <stdio.h>
#include <memory>
int getlen(FILE* fp);
char* FileGet(char* PATH);
void PEHeadGet(char* PATH);
int main()
{
    char PATH[] = {"C:\\test.exe"};
    PEHeadGet(PATH);
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
    printf("----------------------------\n");
    printf("----------PE HEAD-----------\n");
    printf("----------------------------\n");
    printf(">> WORD Machine: 0x%X\n",*(short*)(FileBuffer + e_lfanew + 4));
    printf(">> WORD NumberOfSections: 0x%X\n",*(short*)(FileBuffer + e_lfanew + 6));
    printf(">> DWORD TimeDataStamp: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 8));
    printf(">> WORD SizeOfOptionalHeader: 0x%X\n",*(short*)(FileBuffer + e_lfanew + 20));
    printf(">> WORD Characteristics: 0x%X\n",*(short*)(FileBuffer + e_lfanew + 22));
    printf("----------------------------\n");
    printf("-----------OPTION HEAD------\n");
    printf("----------------------------\n");
    printf(">> WORD Magic: 0x%X\n",*(short*)(FileBuffer + e_lfanew + 24));
    printf(">> DWORD SizeOfCode: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 28));
    printf(">> DWORD SizeOfInitializedData: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 32));
    printf(">> DWORD SizeOfUninitializedData: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 36));
    printf(">> DWORD AddressOfEntryPoint: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 40));
    printf(">> DWORD BaseOfCode: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 44));
    printf(">> DWORD BaseOfData: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 48));
    printf(">> DWORD ImageBase: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 52));
    printf(">> DWORD SectionAlignment: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 56));
    printf(">> DWORD FileAlignment: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 60));
    printf(">> DWORD SizeOfImage: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 80));
    printf(">> DWORD SizeOfHeaders: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 84));
    printf(">> DWORD NumberOfRvaAndSizes: 0x%X\n",*(int*)(FileBuffer + e_lfanew + 116));

    //Print section table
    printf("----------------------------\n");
    printf("----------SECTION TABLE-----\n");
    printf("----------------------------\n");
    int SizeOfOptionalHeader = *(short*)(FileBuffer + e_lfanew + 20) + e_lfanew + 24;
    char Name[8];
    printf(">> BYTE Name[IMAGE_SIZEOF_SHORT_NAME]: 0x");
    for(int x=0, y=7; x<8; x++,y--)
    {
        Name[y] = *(char*)(FileBuffer+SizeOfOptionalHeader+x);
        printf("%x",Name[x]);
    }
    printf("\n>> DWORD VirtualAddress: 0x%X\n",*(int*)(FileBuffer+SizeOfOptionalHeader+12));
    printf(">> DWORD SizeOfRawData: 0x%X\n",*(int*)(FileBuffer+SizeOfOptionalHeader+16));
    printf(">> DWORD PointerToRawData: 0x%X\n",*(int*)(FileBuffer+SizeOfOptionalHeader+20));
    printf(">> DWORD Characteristics: 0x%X\n",*(int*)(FileBuffer+SizeOfOptionalHeader+36));


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





