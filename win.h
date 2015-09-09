#ifndef WIN_H
#define WIN_H

#include <stdint.h>
#include <Windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <vector>
#include "text.h"
#include "io.h"

class win
{
public:
    static wchar_t *getLongString(char *shortStr);
    static uint64_t getFileSize(char *path);
    static bool fileExists(char *path);
    static bool dirExists(char *path);
    static bool createDirectory(char *path);
    static char *getShortString(wchar_t *longStr);
    static char *readFile(char *path,uint64_t &size);
    static bool writeFile(char *path, char *data, uint64_t size);
    static bool deleteFile(char *path);
    static bool deleteDir(char *path);
    static std::vector<char*> *listFilesOnly(char *dir);
    static std::vector<char*> *listDirsOnly(char *dir);
    static std::vector<char*> *listFilesAndDirs(char *dir);
    static std::vector<char*> *listFilesAndDirs(char *dir,uint32_t filter);
};

#endif // WIN_H
