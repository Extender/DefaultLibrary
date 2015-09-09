#ifndef WINNET_H
#define WINNET_H

#include "win.h"
#include "text.h"
#pragma comment(lib,"winhttp.lib")
#pragma comment(lib,"Urlmon.lib")

class winnet
{
public:
    static bool downloadFile(char *url,char *filePath);
    static char *downloadData(char *url, uint32_t &size); // Note that this will not work on pages with invalid SSL certificates.
};

#endif // WINNET_H
