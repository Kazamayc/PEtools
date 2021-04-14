#include <stdio.h>
#include <memory.h>
#include <memory>
int RVAtoFOA(int RVA,char* FileBuffer);
int getlen(FILE* fp);
int get();
int main()
{
    get();
}

int get()
{
    FILE* fp = fopen("D:/code/text/notepad.exe","rb");
    if(fp==NULL)
    {
        printf("Error to open file!");
        return 0;
    }
    int size = getlen(fp);
    char* FileBuffer = (char*)malloc(size);
    fread(FileBuffer,1,size,fp);
    if(FileBuffer==NULL)
    {
        printf("Error to read file!");
        return 0;
    }
    int e_lfanew = *(int*)(FileBuffer+60);
    int VirtualAddress = *(int*)(FileBuffer+e_lfanew+120+88);
    int Size = *(int*)(FileBuffer+e_lfanew+120+92);
    printf("VirtualAddress:0x%X\n",VirtualAddress);
    printf("Size:0x%X\n",Size);
    int FOAddress = RVAtoFOA(VirtualAddress,FileBuffer);

    int TimeDateStamp = *(int*)(FileBuffer+FOAddress);
    int OffsetModuleName = *(short*)(FileBuffer+FOAddress+4);
    int NumberOfModuleForwarderRefs = *(short*)(FileBuffer+FOAddress+6);
    int n=1;
    while(!(TimeDateStamp==0&&OffsetModuleName==0&&NumberOfModuleForwarderRefs==0))
    {
        printf("Name:%s\n",(OffsetModuleName+FOAddress+FileBuffer));
        printf("TimeDateStamp:0x%X\n",TimeDateStamp);
        printf("OffsetModuleName:0x%X\n",OffsetModuleName);
        printf("NumberOfModuleForwarderRefs:0x%X\n",NumberOfModuleForwarderRefs);
        TimeDateStamp = *(int*)(FileBuffer+FOAddress+n*8);
        OffsetModuleName = *(short*)(FileBuffer+FOAddress+4+n*8);
        NumberOfModuleForwarderRefs = *(short*)(FileBuffer+FOAddress+6+n*8);
        n++;
    }
    

    free(FileBuffer);
    fclose(fp);
    return 0;


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
