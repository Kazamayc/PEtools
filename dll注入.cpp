#include <stdio.h>
#include <memory>
#include <memory.h>
int RVAtoFOA(int RVA,char* FileBuffer);
int FOAtoRVA(int FOA,char* FileBuffer);
int getlen(FILE* fp);
int get();
int add();
int main()
{
    get();
}

int get()
{
    int Section_FOA = add();
    FILE* fp = fopen("D:/code/text/g3.exe","rb");
    FILE* fp2 = fopen("D:/code/text/g4.exe","wb");
    if(fp==NULL||fp2==NULL)
    {
        printf("Error to open file!");
        return 0;
    }
    int len = getlen(fp);
    char* FileBuffer = (char*)malloc(len);
    fread(FileBuffer,1,len,fp);
    if(FileBuffer==NULL)
    {
        printf("Error to read file!");
        return 0;
    }
    int e_lfanew = *(int*)(FileBuffer+60);
    int VirtualAddress = *(int*)(FileBuffer+e_lfanew+120+8);
    int Size = *(int*)(FileBuffer+e_lfanew+120+12);
    int IID_FOA=RVAtoFOA(VirtualAddress,FileBuffer);
    
    int OriginalFirstThunk_RVA = *(int*)(FileBuffer+IID_FOA);
    int FirstThunk = *(int*)(FileBuffer+IID_FOA+16);
    int num = 0;
    while(!(OriginalFirstThunk_RVA==0 && FirstThunk==0))
    {
        num++;
        OriginalFirstThunk_RVA = *(int*)(FileBuffer+IID_FOA+num*20);
        FirstThunk = *(int*)(FileBuffer+IID_FOA+16+num*20);
    }
    //移动导入表
    memcpy((FileBuffer+Section_FOA),(FileBuffer+IID_FOA),(num+2)*20);
    memset((FileBuffer+Section_FOA+(num+1)*20),0,20);
    
    //修改导入表INT 和 IAT
    *(int*)(FileBuffer+Section_FOA+num*20) = FOAtoRVA(Section_FOA+(num+2)*20,FileBuffer);
    *(int*)(FileBuffer+Section_FOA+num*20+16) = FOAtoRVA(Section_FOA+(num+2)*20+8,FileBuffer);
    
    //添加INT和IAT  16
    *(int*)(FileBuffer+Section_FOA+(num+2)*20) = FOAtoRVA(Section_FOA+(num+2)*20+16,FileBuffer);
    *(int*)(FileBuffer+Section_FOA+(num+2)*20+4) = 0;
    *(int*)(FileBuffer+Section_FOA+(num+2)*20+8) = FOAtoRVA(Section_FOA+(num+2)*20+16,FileBuffer);
    *(int*)(FileBuffer+Section_FOA+(num+2)*20+12) = 0;
    
    //添加NAME结构
    char Name[20]=
    {
        0x00,0x00,
        0x45,0x78,0x70,0x6f,0x72,0x74,0x46,0x75,0x6e,0x63,0x74,0x69,0x6f,0x6e,0x00,
        0x00,0x00,0x00
    };
    for(int x=0; x<20; x++)
    {
        *(char*)(FileBuffer+Section_FOA+(num+2)*20+16+x)=Name[x];
    }
    char dllName[14]=
    {
        0x49,0x6e,0x6a,0x65,0x63,0x74,0x44,0x6c,0x6c,0x2e,0x64,0x6c,0x6c,0x00
    };
    //修改导入表中的name
    for(int n=0; n<14; n++)
    {
        *(char*)(FileBuffer+Section_FOA+(num+2)*20+16+20+n)=dllName[n];
    }
    *(int*)(FileBuffer+Section_FOA+num*20+12) = FOAtoRVA(Section_FOA+(num+2)*20+16+20,FileBuffer);
    *(int*)(FileBuffer+e_lfanew+120+8) = FOAtoRVA(Section_FOA,FileBuffer);
    *(int*)(FileBuffer+e_lfanew+120+12) = Size+20;
    
    fwrite(FileBuffer,1,len,fp2);
    
    free(FileBuffer);
    fclose(fp);
    fclose(fp2);
    return 0;
}





int add()
{
    char* FileBuffer = NULL;
    FILE* fp = fopen("D:/code/text/g2.exe","rb");
    FILE* fp_w = fopen("D:/code/text/g3.exe","wb");
    if (fp==NULL || fp_w==NULL)
    {
        printf("Error to open file!");
        return 0;
    }
    int size = getlen(fp);
    FileBuffer = (char*)malloc(size);
    fread(FileBuffer,1,size,fp);
    if (FileBuffer==NULL)
    {
        printf("Error to read file!");
        return 0;
    }

    //定义
    int e_lfanew = *(int*)(FileBuffer+60);
    short NumberOfSections = *(short*)(FileBuffer+e_lfanew+6);
    short SizeOfOptionalHeader = *(short*)(FileBuffer+e_lfanew+20);
    int SectionAlignment = *(int*)(FileBuffer+e_lfanew+56);
    int SizeOfHeaders = *(int*)(FileBuffer+e_lfanew+84);
    int add_section_space = SizeOfHeaders-(e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40);
    if(add_section_space<80)
    {
        printf("Not enough memory space!");
        return 0;
    }
    //分配空间
    FileBuffer = (char*)malloc(size+SectionAlignment);
    memset(FileBuffer,0,size+SectionAlignment);
    fseek(fp, 0, SEEK_SET);
    fread(FileBuffer,1,size,fp);
    if (FileBuffer==NULL)
    {
        printf("Error to read file2!");
        return 0;
    }
    //新增节表
	

    
    *(short*)(FileBuffer+e_lfanew+6) = NumberOfSections+1;
    memcpy((FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40),(FileBuffer+e_lfanew+24+SizeOfOptionalHeader),40);
    
    char Name[8]=
    {
        0x2E,0x63,0x63,0x63,0x00,0x00,0x00,0x00
    };
    for(int x=0; x<8; x++)
    {
        *(char*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+x)=Name[x];
        
    }

    int VirtualSize = *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+(NumberOfSections-1)*40+8);
    int VirtualAddress = *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+(NumberOfSections-1)*40+12);
    int SizeOfRawData = *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+(NumberOfSections-1)*40+16);
    int PointerToRawData = *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+(NumberOfSections-1)*40+20);
    
    
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+8) = SectionAlignment;
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+12) = VirtualAddress+SizeOfRawData;
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+16) = SectionAlignment;
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+20) = PointerToRawData+SizeOfRawData;
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+36) = 0xC0000040;


    int SizeOfImage = *(int*)(FileBuffer+e_lfanew+80);
    *(int*)(FileBuffer+e_lfanew+80) = SizeOfImage+SectionAlignment;

    fwrite(FileBuffer,1,size+SectionAlignment,fp_w);

    free(FileBuffer);
    fclose(fp);
    fclose(fp_w);
    return PointerToRawData+SizeOfRawData;
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
