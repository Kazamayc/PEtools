#include <stdio.h>
#include <memory>
#include <memory.h>
int RVAtoFOA(int RVA,char* FileBuffer);
int getlen(FILE* fp);
int get();
int main()
{
    get();
}

int get()
{
    FILE* fp = fopen("D:/code/text/ll2.exe","rb");
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
    int VirtualAddress = *(int*)(FileBuffer+e_lfanew+120+8);
    int Size = *(int*)(FileBuffer+e_lfanew+120+12);
    printf("VirtualAddress:0x%X\n",VirtualAddress);
    printf("Size:0x%X\n",Size);
    int IID_FOA=RVAtoFOA(VirtualAddress,FileBuffer);

    int OriginalFirstThunk_RVA = *(int*)(FileBuffer+IID_FOA);
    int OriginalFirstThunk_FOA = RVAtoFOA(OriginalFirstThunk_RVA,FileBuffer);
    int TimeDateStamp = *(int*)(FileBuffer+IID_FOA+4);
    int ForwarderChain = *(int*)(FileBuffer+IID_FOA+8);
    int Name = *(int*)(FileBuffer+IID_FOA+12);
    int FirstThunk = *(int*)(FileBuffer+IID_FOA+16);
    
    int num = 1;

    while(!(OriginalFirstThunk_RVA==0 && TimeDateStamp==0 && ForwarderChain==0 && Name==0 && FirstThunk==0))
    {
        printf("DLL_Name:%s\n",(char*)(FileBuffer+RVAtoFOA(Name,FileBuffer)));
        printf("OriginalFirstThunk_RVA:0x%X\n",OriginalFirstThunk_RVA);
        printf("TimeDateStamp:0x%X\n",TimeDateStamp);
        printf("ForwarderChain:0x%X\n",ForwarderChain);
        printf("Name_RVA:0x%X\n",Name);
        printf("FirstThunk_RVA:0x%X\n",FirstThunk);

        int OriginalFirstThunk_VALUE = *(int*)(FileBuffer+OriginalFirstThunk_FOA);
        int num2 = 0;
        while(OriginalFirstThunk_VALUE)
        {
            if(OriginalFirstThunk_VALUE&0x80000000)
            {
                printf("Import using serial number:0x%X\n",OriginalFirstThunk_VALUE&0x7FFFFFFF);
            }else
            {
                int name_FOA = RVAtoFOA(OriginalFirstThunk_VALUE,FileBuffer);
                printf("Hint:0x%X\n",*(short*)(FileBuffer+name_FOA));
                printf("Import using name:%s\n",(char*)(FileBuffer+name_FOA+2));
            }
            num2+=4;
            OriginalFirstThunk_VALUE = *(int*)(FileBuffer+OriginalFirstThunk_FOA+num2);
        }
        

        OriginalFirstThunk_RVA = *(int*)(FileBuffer+IID_FOA+num*20);
        OriginalFirstThunk_FOA = RVAtoFOA(OriginalFirstThunk_RVA,FileBuffer);
        TimeDateStamp = *(int*)(FileBuffer+IID_FOA+4+num*20);
        ForwarderChain = *(int*)(FileBuffer+IID_FOA+8+num*20);
        Name = *(int*)(FileBuffer+IID_FOA+12+num*20);
        FirstThunk = *(int*)(FileBuffer+IID_FOA+16+num*20);
        num++;
    }



    fclose(fp);
    free(FileBuffer);
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
