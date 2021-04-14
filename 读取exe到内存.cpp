//将记事本的.exe文件读取到内存，并返回读取后在内存中的地址
#include <stdio.h>
#include <memory>
int getlen(FILE *fp);
int fun(){
	FILE* fp = NULL;
	int len = 0;
	fp = fopen("C:\\Windows\\System32\\notepad.exe","rb");
	if(fp == NULL){
		printf("Error to open file!");
        return 0;
	}
	len = getlen(fp);

	char* ptr;
	ptr = (char*)malloc(len);
	if(ptr == NULL){
		printf("Error to allocate space!");
        return 0;
	}
	fgets(ptr,len,fp);
	printf("%d",ptr);

	fclose(fp);
	free(ptr);
	ptr = NULL;
	return 0;
}
int getlen(FILE *fp){
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);
	return len;
}
int main(){
	fun();
}