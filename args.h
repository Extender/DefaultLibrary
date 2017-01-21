#ifndef ARGS_H
#define ARGS_H

#include <vector>
#include <map>
#include <string>

#include "text.h"

class args
{
public:
    static std::vector<std::string> getFlags(int argc,char *argv[],bool skipFirst=true);
    static std::map<std::string,std::string> getValues(int argc,char *argv[],bool skipFirst=true);
    static bool hasFlag(int argc,char *argv[],const char *flag,bool skipFirst=true);
    static std::string getValue(int argc,char *argv[],const char *flag,bool skipFirst=true);
};

#endif // ARGS_H
