#include <stdio.h>
#include <memory>
#include <memory.h>
int GetFunctionAddrByName(char* FileBuffer,int NameAddress);
int GetFunctionAddrByOrdinals(char* FileBuffer,short Ordinals);
int RVAtoFOA(int RVA,char* FileBuffer);
int getlen(FILE* fp);


int GetFunctionAddrByName(char* FileBuffer,int NameAddress)
{
    int e_lfanew = *(int*)(FileBuffer+60);
    int IAMAGE_EXPORT_VirtualAddress = *(int*)(FileBuffer+e_lfanew+120);
    int IAMAGE_EXPORT_FOA = RVAtoFOA(IAMAGE_EXPORT_VirtualAddress,FileBuffer);
    
    int Base = *(int*)(FileBuffer+IAMAGE_EXPORT_FOA+16);
    int NumberOfNames = *(int*)(FileBuffer+IAMAGE_EXPORT_FOA+24);
    int AddressOfFunctions = *(int*)(FileBuffer+IAMAGE_EXPORT_FOA+28);
    int AddressOfNames = *(int*)(FileBuffer+IAMAGE_EXPORT_FOA+32);
    int AddressOfNameOrdinals = *(int*)(FileBuffer+IAMAGE_EXPORT_FOA+36);

    int AddressOfNames_FOA = RVAtoFOA(AddressOfNames,FileBuffer);
    int AddressOfNameOrdinals_FOA = RVAtoFOA(AddressOfNameOrdinals,FileBuffer);
    int AddressOfFunctions_FOA = RVAtoFOA(AddressOfFunctions,FileBuffer);
    
    int num=0;
    for (int i=0; i<NumberOfNames; i++)
    {
        
        if(NameAddress==*(int*)(FileBuffer+AddressOfNames_FOA+num))
        {
            short Ordinals = *(short*)(FileBuffer+AddressOfNameOrdinals_FOA+num/2);
            return *(int*)(FileBuffer+AddressOfFunctions_FOA+Ordinals*4);
        }
        num+=4;
    }
    printf(">>Failed to change.");
    return 0;
    

}

int GetFunctionAddrByOrdinals(char* FileBuffer,short Ordinals)
{
    int e_lfanew = *(int*)(FileBuffer+60);
    int IAMAGE_EXPORT_VirtualAddress = *(int*)(FileBuffer+e_lfanew+120);
    int IAMAGE_EXPORT_FOA = RVAtoFOA(IAMAGE_EXPORT_VirtualAddress,FileBuffer);
    
    int Base = *(int*)(FileBuffer+IAMAGE_EXPORT_FOA+16);
    int AddressOfFunctions = *(int*)(FileBuffer+IAMAGE_EXPORT_FOA+28);
    int AddressOfFunctions_FOA = RVAtoFOA(AddressOfFunctions,FileBuffer);
    
    return *(int*)(FileBuffer+AddressOfFunctions_FOA+(Ordinals-Base)*4);
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

int getlen(FILE* fp)
{
    fseek(fp,0,SEEK_END);
    int size = ftell(fp);
    fseek(fp,0,SEEK_SET);
    return size;
}

int main()
{
    FILE* fp = fopen("D:/code/text/ll.exe","rb");
    if(fp==NULL)
    {
        printf("Error to open file!");
        return 0;
    }
    char* FileBuffer = NULL;
    int len = getlen(fp);
    FileBuffer = (char*)malloc(len);
    fread(FileBuffer,1,len,fp);
    printf("0x%X\n",GetFunctionAddrByName(FileBuffer,0x0010F7A6));
    //0x007F284
    printf("0x%X",GetFunctionAddrByOrdinals(FileBuffer,3));
    fclose(fp);
    free(FileBuffer);
    
}