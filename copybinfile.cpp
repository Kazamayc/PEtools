//
#include <stdio.h>
#include <memory>
int getlen(FILE* fp);
int fun(){
    FILE* fp1 = NULL;
    FILE* fp2 = NULL;
    fp1 = fopen("C:\\Windows\\System32\\notepad.exe","rb");
    fp2 = fopen("D:\\code\\test2.exe","wb");
    if(fp1 == NULL)
    {
        printf("Error to open file1!");
        return 0;
    }
    if (fp2 == NULL)
    {
        printf("Error to open file2!");
        return 0;
    }
    int len1 = getlen(fp1);
    //分配内存空间
    char* FileBuff1 = NULL;
    FileBuff1 = (char*)malloc(len1);
    if(FileBuff1 == NULL)
    {
        printf("Error to allocate space1!");
        return 0;
    }
    //文件读取
    fread(FileBuff1,sizeof(char),len1,fp1);
    fwrite(FileBuff1,sizeof(char),len1,fp2);
    fclose(fp1);
    fclose(fp2);
    free(FileBuff1);
    FileBuff1 = NULL;
    return 0;

}

int getlen(FILE* fp){
    fseek(fp,0,SEEK_END);
    int len = ftell(fp);
    fseek(fp,0,SEEK_SET);
    return len;
}

int main(){
    fun();
}