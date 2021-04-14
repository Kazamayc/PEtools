#include <stdio.h>
#include <memory>
#include <memory.h>
//1.新增一个节表，如果后面放不开了，则把整个头往前，然后再放
//2.扩大节，合并节
//这里实现的仅仅是新增一个节

int getlen(FILE* fp);


int add()
{
	char* FileBuffer = NULL;
	FILE* fp = fopen("D:/code/text/ll.exe","rb");
    FILE* fp_w = fopen("D:/code/text/a.exe","wb");
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
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+12) = VirtualAddress+VirtualSize;
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+16) = SectionAlignment;
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+20) = PointerToRawData+SizeOfRawData;
    *(int*)(FileBuffer+e_lfanew+24+SizeOfOptionalHeader+NumberOfSections*40+24) = 0x20000060;


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



int main()
{
	add();
}
