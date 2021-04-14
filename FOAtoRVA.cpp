#include <stdio.h>
#include <memory>
#include <memory.h>
int FOAtoRVA(int FOA,char* FileBuffer);
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
    printf("%x",FOAtoRVA(0xCE200,FileBuffer));

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


int FOAtoRVA(int FOA,char* FileBuffer)
{
    int RVA=FOA;
    int e_lfanew = *(int*)(FileBuffer+60);
    short NumberOfSections = *(short*)(FileBuffer+e_lfanew+6);
    short SizeOfOptionalHeader = *(short*)(FileBuffer+e_lfanew+20);
    int SectionAlignment = *(int*)(FileBuffer+e_lfanew+56);
    int FileAlignment = *(int*)(FileBuffer+e_lfanew+60);
    int SizeOfHeaders = *(int*)(FileBuffer+e_lfanew+84);
    if (SectionAlignment==FileAlignment)
    {
        return RVA;
    }
    if (FOA<=SizeOfHeaders)
    {
        return RVA;
    }

//判断节
    int SectionTable = e_lfanew+24+SizeOfOptionalHeader;
    int VirtualAddress=0;
    int PointerToRawData1=0;
    int PointerToRawData2=0;
    for(int i=0; i<NumberOfSections-1; i++)
    {
        VirtualAddress=*(int*)(FileBuffer+SectionTable+12+i*40);
        PointerToRawData1=*(int*)(FileBuffer+SectionTable+20+i*40);
        PointerToRawData2=*(int*)(FileBuffer+SectionTable+20+(i+1)*40);
        if(FOA>=PointerToRawData1&&FOA<PointerToRawData2)
        {
            RVA=FOA+VirtualAddress-PointerToRawData1;
            return RVA;
        }
    }
    VirtualAddress=*(int*)(FileBuffer+SectionTable+12+(NumberOfSections-1)*40);
    PointerToRawData1=*(int*)(FileBuffer+SectionTable+20+(NumberOfSections-1)*40);
    RVA=FOA+VirtualAddress-PointerToRawData1;
    return RVA;
}


int main()
{
    
    get();
}