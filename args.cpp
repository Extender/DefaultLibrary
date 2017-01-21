#include "args.h"

std::vector<std::string> args::getFlags(int argc, char *argv[], bool skipFirst)
{
    std::vector<std::string> out;
    for(size_t i=(skipFirst?1:0);i<(size_t)argc;i++)
    {
        char *c=text::trim(argv[i]);
        size_t l=strlen(c);
        if(l==0||(c[0]!='-'&&c[0]!='/'))
        {
            free(c);
            continue;
        }
        size_t n=text::indexOf(c,'=');
        if(n!=pos_notFound)
        {
            c[n]=0; // Trim end
            l=n;
        }
        size_t dashCount=0;
        for(size_t p=0;p<l;p++)
        {
            char chr=c[p];
            if(chr!='-'&&chr!='/')
                break;
            dashCount++;
        }
        out.push_back(std::string(c+dashCount));
        free(c);
    }
    return out;
}

std::map<std::string, std::string> args::getValues(int argc, char *argv[], bool skipFirst)
{
    std::map<std::string,std::string> out;
    for(size_t i=(skipFirst?1:0);i<(size_t)argc;i++)
    {
        char *c=text::trim(argv[i]);
        size_t l=strlen(c);
        if(l==0||(c[0]!='-'&&c[0]!='/'))
        {
            free(c);
            continue;
        }
        size_t n=text::indexOf(c,'=');
        if(n==pos_notFound)
        {
            free(c);
            continue;
        }
        size_t dashCount=0;
        for(size_t p=0;p<l;p++)
        {
            char chr=c[p];
            if(chr!='-'&&chr!='/')
                break;
            dashCount++;
        }
        std::string name=std::string(c+dashCount,n-dashCount);
        std::string value=l==n-1?std::string():std::string(c+n+1,l-n);
        out.insert(std::pair<std::string,std::string>(name,value));
    }
    return out;
}

bool args::hasFlag(int argc, char *argv[], const char *flag, bool skipFirst)
{
    for(size_t i=(skipFirst?1:0);i<(size_t)argc;i++)
    {
        char *c=text::trim(argv[i]);
        size_t l=strlen(c);
        if(l==0||(c[0]!='-'&&c[0]!='/'))
        {
            free(c);
            continue;
        }
        size_t n=text::indexOf(c,'=');
        if(n!=pos_notFound)
        {
            c[n]=0; // Trim end
            l=n;
        }
        size_t dashCount=0;
        for(size_t p=0;p<l;p++)
        {
            char chr=c[p];
            if(chr!='-'&&chr!='/')
                break;
            dashCount++;
        }
        if(text::iCompare((c+dashCount),flag))
        {
            free(c);
            return true;
        }
        free(c);
    }
    return false;
}

std::string args::getValue(int argc, char *argv[], const char *flag, bool skipFirst)
{
    std::string flagStr=std::string(flag);
    for(size_t i=(skipFirst?1:0);i<(size_t)argc;i++)
    {
        char *c=text::trim(argv[i]);
        size_t l=strlen(c);
        if(l==0||(c[0]!='-'&&c[0]!='/'))
        {
            free(c);
            continue;
        }
        size_t n=text::indexOf(c,'=');
        if(n==pos_notFound)
        {
            free(c);
            continue;
        }
        size_t dashCount=0;
        for(size_t p=0;p<l;p++)
        {
            char chr=c[p];
            if(chr!='-'&&chr!='/')
                break;
            dashCount++;
        }
        std::string name=std::string(c+dashCount,n-dashCount);
        if(text::iCompare(name,flagStr))
        {
            std::string value=l==n-1?std::string():std::string(c+n+1,l-n);
            free(c);
            return value;
        }
        free(c);
    }
    return std::string();
}
