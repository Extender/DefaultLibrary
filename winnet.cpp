#include "winnet.h"

bool winnet::downloadFile(char *url, char *filePath)
{
    wchar_t *path=win::getLongString(filePath);
    HRESULT result=URLDownloadToFileW(NULL,win::getLongString(url),path,0,NULL);
    return result==S_OK;
}

char *winnet::downloadData(char *url, uint32_t &size)
{
    char *out=NULL;
    BOOL result=FALSE;
    size=0;
    HINTERNET hSession=NULL;
    HINTERNET hConnect=NULL;
    HINTERNET hRequest=NULL;

    wchar_t *longUrl=win::getLongString(url);

    URL_COMPONENTS urlComponents;

    ZeroMemory(&urlComponents,sizeof(urlComponents));
    urlComponents.dwStructSize=sizeof(urlComponents);

    // The lengths of the required components must not be 0

    urlComponents.dwHostNameLength=-1;
    urlComponents.dwUrlPathLength=-1;
    urlComponents.dwExtraInfoLength=-1;
    if(!WinHttpCrackUrl(longUrl,wcslen(longUrl),0,&urlComponents))
        printf("Error (0): %i",GetLastError());

    hSession=WinHttpOpen(L"Download request/1.0",
                         WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                         WINHTTP_NO_PROXY_NAME,
                         WINHTTP_NO_PROXY_BYPASS,
                         0);
    if(hSession)
    {
        hConnect=WinHttpConnect(hSession,
                                text::wFirstChars(urlComponents.lpszHostName,urlComponents.dwHostNameLength),
                                text::iStartsWith(url,"https:")?INTERNET_DEFAULT_HTTPS_PORT:INTERNET_DEFAULT_HTTP_PORT,
                                0);
    }

    if(hConnect)
    {
        hRequest=WinHttpOpenRequest(hConnect,L"GET",urlComponents.lpszUrlPath/*Includes the query string*/,NULL,WINHTTP_NO_REFERER,WINHTTP_DEFAULT_ACCEPT_TYPES,0);
    }

    if(hRequest)
        result=WinHttpSendRequest(hRequest,WINHTTP_NO_ADDITIONAL_HEADERS,0,WINHTTP_NO_REQUEST_DATA,0,0,0);

    if(!result)
        printf("Error (1): %s",GetLastError());
    else
    {
        uint32_t bufferSize=1024;
        out=(char*)malloc(bufferSize);
        result=WinHttpReceiveResponse(hRequest,NULL);
        if(!result)
            printf("Error (2): %s",GetLastError());
        else
        {
            DWORD tempSize,tempDownloaded=0;
            result=WinHttpQueryDataAvailable(hRequest,&tempSize);
            if(!result)
            {
                printf("Error (3): %s",GetLastError());
                return 0;
            }
            char *buffer;
            while(tempSize>0)
            {
                buffer=new char[tempSize];
                ZeroMemory(buffer,tempSize+1);
                WinHttpReadData(hRequest,buffer,tempSize,&tempDownloaded);
                if(bufferSize<size+tempSize)
                {
                    bufferSize=size+tempSize;
                    out=(char*)realloc(out,bufferSize);
                }
                memcpy(out+size,buffer,tempSize);
                size+=tempSize;
                tempSize=0;
                result=WinHttpQueryDataAvailable(hRequest,&tempSize);
                if(!result)
                    printf("Error (3): %s",GetLastError());
            }
        }
    }

    if(hRequest) WinHttpCloseHandle(hRequest);
    if(hConnect) WinHttpCloseHandle(hConnect);
    if(hSession) WinHttpCloseHandle(hSession);

    return out;
}
