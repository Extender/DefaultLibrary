#include "win.h"

wchar_t *win::getLongString(char *shortStr)
{
    int bufferLength=MultiByteToWideChar(CP_UTF8,0,shortStr,-1,NULL,0);
    wchar_t *out=(wchar_t*)calloc(bufferLength+1,sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8,0,shortStr,-1,out,bufferLength);
    return out;
}

uint64_t win::getFileSize(char *path)
{
    wchar_t *longPath=getLongString(path);
    HANDLE f=CreateFile(longPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    DWORD out=GetFileSize(f,NULL);
    CloseHandle(f);
    free(longPath);
    return out;
}

bool win::fileExists(char *path)
{
    wchar_t *longPath=getLongString(path);
    DWORD attrs=GetFileAttributes(longPath);
    free(longPath);
    return attrs!=INVALID_FILE_ATTRIBUTES&&(attrs&FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY;
}

bool win::dirExists(char *path)
{
    wchar_t *longPath=getLongString(path);
    DWORD attrs=GetFileAttributes(longPath);
    free(longPath);
    return attrs!=INVALID_FILE_ATTRIBUTES&&(attrs&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY;
}

bool win::createDirectory(char *path)
{
    path=strdup(path);
    if(text::indexOf(path,"/")!=pos_notFound)
        path=text::replace(path,"/","\\");
    std::vector<char*> parts=text::split(path,"\\");
    fs_t bufferSize=strlen(path)+1;
    fs_t pos=0;
    char *temp=(char*)calloc(bufferSize,1);
    for(uint32_t i=0;i<parts.size()-1;i++)
    {
        char *part=parts.at(i);
        char *append=text::concat(i>0?"\\":"",part);
        io::writeRawDataToBuffer(temp,append,strlen(append),pos,bufferSize);
        if(!dirExists(temp))
            createDirectory(temp);
        free(append);
        free(part);
    }
    free(temp);
    wchar_t *longPath=getLongString(path);
    bool result=CreateDirectory(longPath,NULL);
    free(path);
    free(longPath);
    return result;
}

char *win::getShortString(wchar_t *longStr)
{
    int bufferLength=WideCharToMultiByte(CP_UTF8,0,longStr,-1,NULL,0,NULL,NULL);
    char *out=(char*)calloc(bufferLength+1,1);
    WideCharToMultiByte(CP_UTF8,0,longStr,-1,out,bufferLength,NULL,NULL);
    return out;
}

char *win::readFile(char *path, uint64_t &size)
{
    HANDLE file;
    wchar_t *longPath=getLongString(path);
    file=CreateFile(longPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    free(longPath);
    if(file==INVALID_HANDLE_VALUE)
    {
        size=0;
        return 0;
    }
    LARGE_INTEGER lSize;
    GetFileSizeEx(file,&lSize);
    size=lSize.LowPart;
    char *buffer=(char*)malloc(lSize.LowPart);
    DWORD bytesRead=0;
    bool result=ReadFile(file,buffer,lSize.LowPart,&bytesRead,NULL);
    CloseHandle(file);
    if(!result)
    {
        size=0;
        return 0;
    }
    return buffer;
}

bool win::writeFile(char *path, char *data, uint64_t size)
{
    HANDLE file;
    wchar_t *longPath=getLongString(path);
    file=CreateFile(longPath,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    free(longPath);
    if(file==INVALID_HANDLE_VALUE)
    {
        puts("Error 1.");
        return false;
    }
    DWORD written;
    bool result=WriteFile(file,data,size,&written,NULL);
    CloseHandle(file);
    return result;
}

bool win::deleteFile(char *path)
{
    wchar_t *longPath=getLongString(path);
    bool result=DeleteFile(longPath);
    free(longPath);
    return result;
}

bool win::deleteDir(char *path)
{
    if(!dirExists(path))
        return false;
    wchar_t *longPath=getLongString(path);
    std::vector<char*> *files=listFilesOnly(path);
    std::vector<char*> *dirs=listDirsOnly(path);
    uint32_t fileCount=files->size();
    uint32_t dirCount=dirs->size();
    if(fileCount+dirCount==0)
    {
        bool result=RemoveDirectory(longPath);
        free(longPath);
        return result;
    }
    for(uint32_t i=0;i<fileCount;i++)
    {
        char *elementPath=text::concatPaths(path,files->at(i));
        bool del=deleteFile(elementPath);
        free(elementPath);
        if(!del)
        {
            free(longPath);
            return false;
        }
    }
    for(uint32_t i=0;i<dirCount;i++)
    {
        char *elementPath=text::concatPaths(path,dirs->at(i));
        bool del=deleteDir(elementPath);
        free(elementPath);
        if(!del)
        {
            free(longPath);
            return false;
        }
    }
    bool result=RemoveDirectory(longPath);
    free(longPath);
    return result;

}

std::vector<char *> *win::listFilesOnly(char *dir)
{
    // Special code:
    return listFilesAndDirs(dir,~FILE_ATTRIBUTE_DIRECTORY);
}

std::vector<char *> *win::listDirsOnly(char *dir)
{
    return listFilesAndDirs(dir,FILE_ATTRIBUTE_DIRECTORY);
}

std::vector<char *> *win::listFilesAndDirs(char *dir)
{
    return listFilesAndDirs(dir,0);
}

std::vector<char *> *win::listFilesAndDirs(char *dir, uint32_t filter)
{
    std::vector<char*> *out=new std::vector<char*>();
    wchar_t *longInDir=getLongString(dir);
    wchar_t *longDirName=text::concatWideString(longInDir,L"\\*");
    free(longInDir);
    WIN32_FIND_DATA ffd;
    HANDLE hFind;
    hFind=FindFirstFile(longDirName,&ffd);
    if(hFind==INVALID_HANDLE_VALUE)
    {
        free(longDirName);
        return out;
    }
    bool hasFilter=filter!=0;
    bool filesOnly=filter==~FILE_ATTRIBUTE_DIRECTORY;
    do
    {
        if(hasFilter)
        {
            if(filesOnly)
            {
                if(ffd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
                    continue;
            }
            else if((ffd.dwFileAttributes&filter)!=filter) // () needed!
                    continue;
        }
        if(ffd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
        {
            if(wcscmp(ffd.cFileName,L".")==0||wcscmp(ffd.cFileName,L"..")==0)
                continue;
        }
        out->push_back(getShortString(ffd.cFileName));
    }
    while(FindNextFile(hFind,&ffd)!=0);
    FindClose(hFind);
    free(longDirName);
    return out;
}
