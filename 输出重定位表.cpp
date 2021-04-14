#include <stdio.h>
#include <memory>
#include <memory.h>
int getlen(FILE* fp);
int RVAtoFOA(int RVA,char* FileBuffer);
int get()
{
    FILE* fp = fopen("D:/code/text/a.exe","rb");
    if(fp==NULL)
    {
        printf("Error to open file!\n");
        return 0;
    }
    char* FileBuffer = NULL;
    int size = getlen(fp);
    FileBuffer = (char*)malloc(size);
    fread(FileBuffer,1,size,fp);
    if(FileBuffer==NULL)
    {
        printf("Error to read file!");
        return 0;
    }
    int e_lfanew = *(int*)(FileBuffer+60);
    int RELOCATION_VirtualAddress = *(int*)(FileBuffer+e_lfanew+120+40);
    int RELOCATION_Size = *(int*)(FileBuffer+e_lfanew+120+44);
    printf("RELOCATION_VirtualAddress:0x%X\n",RELOCATION_VirtualAddress);
    printf("RELOCATION_Size:0x%X\n",RELOCATION_Size);
    int RELOCATION_FOA = RVAtoFOA(RELOCATION_VirtualAddress,FileBuffer);
    int VirtualAddress = *(int*)(RELOCATION_FOA+FileBuffer);
    int SizeOfBlock = *(int*)(RELOCATION_FOA+FileBuffer+4);

    int num = 0;
    while(VirtualAddress!=0&&SizeOfBlock!=0)
    {
        printf("VirtualAddress:0x%X\n",VirtualAddress);
        printf("SizeOfBlock:0x%X\n",SizeOfBlock);
        int a = (SizeOfBlock-8)/2;
        for(int i=0;i<a;i++)
        {
            int offset = *(short*)(RELOCATION_FOA+FileBuffer+8+2*i+num);
            int type = *(short*)(RELOCATION_FOA+FileBuffer+8+2*i+num);
            printf("offset:0x%X\n",offset&0x0FFF);
            printf("type:0x%X\n",type>>12);
        }
        num+=SizeOfBlock;
        VirtualAddress=*(int*)(RELOCATION_FOA+FileBuffer+num);
        SizeOfBlock=*(int*)(RELOCATION_FOA+FileBuffer+num+4);
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

int main()
{
    get();
}