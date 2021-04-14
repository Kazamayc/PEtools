//修改imagebase，然后修改重定位表，然后存盘，看他能否正常使用
//代码有问题，可能是环境导致，不过想法应该是对的
//修改imagebase，假如把值增加了10000，则在重定位表中的所有RVA加10000即可

#include <stdio.h>
#include <memory>
#include <memory.h>
int RVAtoFOA(int RVA,char* FileBuffer);
int getlen(FILE* fp);
int repair();
int main()
{
    repair();
}

int repair()
{
    FILE* fp = fopen("D:/code/text/ll.exe","rb");
    FILE* fp_w = fopen("D:/code/text/a.exe","wb");
    if (fp==NULL || fp_w==NULL)
    {
        printf("Error to open file!");
        return 0;
    }
    char* FileBuffer = NULL;
    int size = getlen(fp);
    FileBuffer = (char*)malloc(size);
    fread(FileBuffer,1,size,fp);
    if (FileBuffer==NULL)
    {
        printf("Error to read file!");
        return 0;
    }
    int e_lfanew = *(int*)(FileBuffer+60);
    int RELOCATION_VirtualAddress = *(int*)(FileBuffer+e_lfanew+120+40);
    int RELOCATION_Size = *(int*)(FileBuffer+e_lfanew+120+44);
    int RELOCATION_FOA = RVAtoFOA(RELOCATION_VirtualAddress,FileBuffer);
    int VirtualAddress = *(int*)(RELOCATION_FOA+FileBuffer);
    int SizeOfBlock = *(int*)(RELOCATION_FOA+FileBuffer+4);
    int num = 0;
    while(VirtualAddress!=0&&SizeOfBlock!=0)
    {
        VirtualAddress=*(int*)(RELOCATION_FOA+FileBuffer+num);
        *(int*)(RELOCATION_FOA+FileBuffer+num)+=0x10000;
        SizeOfBlock=*(int*)(RELOCATION_FOA+FileBuffer+num+4);
        num+=SizeOfBlock;
    }
    *(int*)(FileBuffer+e_lfanew+52)+=0x10000;
    fwrite(FileBuffer,1,size,fp_w);

    return 0;

    fclose(fp);
    fclose(fp_w);
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

