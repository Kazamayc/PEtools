//将txt文件读取到内存，并返回读取后在内存中的地址和数据
#include <memory>
#include <iostream>
#include <stdio.h>
//把文件内容传输到分配的内存中。
int file_length(FILE *fp);
int fun(){
    //读取文件
    FILE* fp = NULL;
    int FpSize = 0;
    fp = fopen("D:\\code\\books.txt","r+");
    if(fp == NULL){
        printf("Error to open file!");
        return 0;
    }
    //获取文件大小
    FpSize = file_length(fp);
    char* FileBuffer = (char*)malloc(FpSize);
    if(FileBuffer == NULL)
    {
        printf("Error to allocate space!");
        return 0;
    }
    fgets(FileBuffer,FpSize,fp);
    int addr = (long long)FileBuffer;
    //返回文件在内存中的地址
	printf("memory:%d\n",FileBuffer);
    printf("memory:%d\n",addr);
    //返回文件内容
    printf("file:%s\n",FileBuffer);

	free(FileBuffer);
    FileBuffer = NULL;
    fclose(fp);
    return 0;

}

int file_length(FILE *fp)
{
    //初始化一个计数器
    int num;
	fseek(fp,0,SEEK_END);
	num = ftell(fp);
	// 使用完毕后，要将文件指针指向文件开始
	fseek(fp,0,SEEK_SET);
	return num;
}

int main(){
    fun();


}