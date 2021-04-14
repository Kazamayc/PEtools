#include <stdio.h>
#include <memory>
#include <memory.h>
int RVAtoFOA(int RVA,char* FileBuffer);
int getlen(FILE* fp);
void get()
{
    FILE* fp = fopen("D:/code/text/ll.exe","rb");
    if(fp==NULL)
    {
        printf("Error to open file!");
        return;
    }
    char* FileBuffer = NULL;
    int size = getlen(fp);
    FileBuffer = (char*)malloc(size);
    fread(FileBuffer,1,size,fp);
    if(FileBuffer==NULL)
    {
        printf("Error to read file!");
    }

    int e_lfanew = *(int*)(FileBuffer+60);
    int DATA_DIRECTORY_VirtualAddress_export = *(int*)(FileBuffer+e_lfanew+120);
    int DATA_DIRECTORY_FileAddress_export = RVAtoFOA(DATA_DIRECTORY_VirtualAddress_export,FileBuffer);
    printf("Base:%X\n",*(int*)(DATA_DIRECTORY_FileAddress_export+FileBuffer+16));
    printf("DATA_DIRECTORY_FileAddress_export:%X\n",DATA_DIRECTORY_FileAddress_export);
    printf("DATA_DIRECTORY_VirtualAddress_export:%X\n",DATA_DIRECTORY_VirtualAddress_export);
    





    fclose(fp);
    free(FileBuffer);
}
int getlen(FILE* fp)
{
    fseek(fp,0,SEEK_END);
    int size = ftell(fp);
    fseek(fp,0,SEEK_SET);
    return size;
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


int main()
{
    get();
}