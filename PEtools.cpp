#include "PEtools.h"

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
    printf("----------OPTION HEAD-------\n");
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
    int SizeOfPE = *(short*)(FileBuffer + e_lfanew + 20) + e_lfanew + 24;
    int NumberOfSections = *(short*)(FileBuffer + e_lfanew + 6);
    int count = 0;
    while(NumberOfSections>0 )
    {
        char Name[8];
        printf(">> BYTE Name[IMAGE_SIZEOF_SHORT_NAME]: 0x");
        for(int x=0, y=7; x<8; x++,y--)
        {
            Name[x] = *(char*)(FileBuffer+SizeOfPE+y+40*count);
            printf("%x",Name[x]);

        }
        printf("\n>> DWORD VirtualSize: 0x%X\n",*(int*)(FileBuffer+SizeOfPE+8+40*count));
        printf(">> DWORD VirtualAddress: 0x%X\n",*(int*)(FileBuffer+SizeOfPE+12+40*count));
        printf(">> DWORD SizeOfRawData: 0x%X\n",*(int*)(FileBuffer+SizeOfPE+16+40*count));
        printf(">> DWORD PointerToRawData: 0x%X\n",*(int*)(FileBuffer+SizeOfPE+20+40*count));
        printf(">> DWORD Characteristics: 0x%X\n",*(int*)(FileBuffer+SizeOfPE+36+40*count));
        NumberOfSections--;
        count++;
    }


    int DATA_DIRECTORY_VirtualAddress_export = *(int*)(FileBuffer+e_lfanew+120);
    int DATA_DIRECTORY_FileAddress_export = RVAtoFOA(DATA_DIRECTORY_VirtualAddress_export,FileBuffer);
    
    printf("----------------------------\n");
    printf("----------DATA_DIRECTORY----\n");
    printf("----------------------------\n");

    printf("----------------------------\n");
    printf("----------EXPORT_DIRECTORY--\n");
    printf("----------------------------\n");
    printf(">> DWORD VirtualAddress: 0x%X\n",DATA_DIRECTORY_VirtualAddress_export);
    printf(">> DWORD Size: 0x%X\n",*(int*)(FileBuffer+e_lfanew+124));
    printf("----------------------------");
    printf(">> DWORD Name: 0x%X\n",*(int*)(DATA_DIRECTORY_FileAddress_export+FileBuffer+12));
    printf(">> DWORD Base: 0x%X\n",*(int*)(DATA_DIRECTORY_FileAddress_export+FileBuffer+16));
    printf(">> DWORD NumberOfFunctions: 0x%X\n",*(int*)(DATA_DIRECTORY_FileAddress_export+FileBuffer+20));
    printf(">> DWORD NumberOfNames: 0x%X\n",*(int*)(DATA_DIRECTORY_FileAddress_export+FileBuffer+24));
    printf(">> DWORD AddressOfFunctions: 0x%X\n",*(int*)(DATA_DIRECTORY_FileAddress_export+FileBuffer+28));
    printf(">> DWORD AddressOfNames: 0x%X\n",*(int*)(DATA_DIRECTORY_FileAddress_export+FileBuffer+32));
    printf(">> DWORD AddressOfNameOrdinals: 0x%X\n",*(int*)(DATA_DIRECTORY_FileAddress_export+FileBuffer+36));


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

int RVAtoFOA(int RVA,char* FileBuffer)
{
    int FOA=RVA;
    int e_lfanew = *(int*)(FileBuffer+60);
    short NumberOfSections = *(short*)(FileBuffer+e_lfanew+6);
    short SizeOfOptionalHeader = *(short*)(FileBuffer+e_lfanew+20);
    int SectionAlignment = *(int*)(FileBuffer+e_lfanew+56);
    int FileAlignment = *(int*)(FileBuffer+e_lfanew+60);
    int SizeOfHeaders = *(int*)(FileBuffer+e_lfanew+84);
    if (SectionAlignment==FileAlignment)
    {
        return FOA;
    }
    if (RVA<=SizeOfHeaders)
    {
        return FOA;
    }

//判断节表
    int SectionTable = e_lfanew+24+SizeOfOptionalHeader;
    int VirtualAddress1=0;
    int VirtualAddress2=0;
    int PointerToRawData=0;
    for(int i=0; i<NumberOfSections-1; i++)
    {
        VirtualAddress1=*(int*)(FileBuffer+SectionTable+12+i*40);
        VirtualAddress2=*(int*)(FileBuffer+SectionTable+12+(i+1)*40);
        PointerToRawData=*(int*)(FileBuffer+SectionTable+20+(i)*40);
        if(RVA>=VirtualAddress1&&RVA<VirtualAddress2)
        {
            FOA=RVA-VirtualAddress1+PointerToRawData;
            return FOA;
        }
    }
    VirtualAddress1=*(int*)(FileBuffer+SectionTable+12+(NumberOfSections-1)*40);
    PointerToRawData=*(int*)(FileBuffer+SectionTable+20+(NumberOfSections-1)*40);
    FOA=RVA-VirtualAddress1+PointerToRawData;
    return FOA;


}

