#include <stdio.h>
#include <memory>
#include <memory.h>
int RVAtoFOA(int RVA,char* FileBuffer);
int getlen(FILE* fp);
int FOAtoRVA(int FOA,char* FileBuffer);
int add();
int movIBR();
int main()
{
    movIBR();
}
int movIBR()
{
    int section_FOA=add();
	FILE* fp = fopen("D:/code/text/c.exe","rb");
    FILE* fp_w = fopen("D:/code/text/d.exe","wb");
    if (fp==NULL||fp_w==NULL)
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
    short NumberOfSections = *(short*)(FileBuffer+e_lfanew+6);
    int RELOCATION_VirtualAddress = *(int*)(FileBuffer+e_lfanew+120+40);
    int RELOCATION_Size = *(int*)(FileBuffer+e_lfanew+120+44);
    
    int RELOCATION_FOA = RVAtoFOA(RELOCATION_VirtualAddress,FileBuffer);
    
    
    memcpy(FileBuffer+section_FOA,FileBuffer+RELOCATION_FOA,RELOCATION_Size+8);
    int section_RVA=FOAtoRVA(section_FOA,FileBuffer);
    *(int*)(FileBuffer+e_lfanew+120+40) = section_RVA;
    
    fwrite(FileBuffer,1,size,fp_w);
    fclose(fp);
    fclose(fp_w);
    free(FileBuffer);
    return 0;

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
int add()
{
	char* FileBuffer = NULL;
	FILE* fp = fopen("D:/code/text/ll.exe","rb");
    FILE* fp_w = fopen("D:/code/text/c.exe","wb");
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
    FileBuffer = (char*)malloc(size+SectionAlignment*2);
    memset(FileBuffer,0,size+SectionAlignment*2);
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


    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+8) = SectionAlignment*2;
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+12) = VirtualAddress+VirtualSize;
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+16) = SectionAlignment*2;
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+20) = PointerToRawData+SizeOfRawData;
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+24) = 0x20000060;


    int SizeOfImage = *(int*)(FileBuffer+e_lfanew+80);
    *(int*)(FileBuffer+e_lfanew+80) = SizeOfImage+SectionAlignment*2;

    fwrite(FileBuffer,1,size+SectionAlignment*2,fp_w);
    free(FileBuffer);
    fclose(fp);
    fclose(fp_w);

    return PointerToRawData+SizeOfRawData;
}
