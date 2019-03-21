#ifndef INI_H
#define INI_H

#include <stdlib.h>

#include "text.h"
#include "io.h"

#define ini_allSections "*" // WARNING: Default: ini_noSection!
#define ini_noSection ""

#define DEFAULT_INI_BUFFER_SIZE 1024

#define ini_value_t char*
#define ini_key_t char*

class ini
{
public:
    static std::vector<char*> getSections(const char *iniSource);
    static bool hasSection(const char *iniSource,const char *section);
    static std::vector<ini_key_t> getKeys(const char *iniSource);
    static std::vector<ini_key_t> getKeys(const char *iniSource,const char *section);
    static bool hasKey(const char *iniSource,const ini_key_t key);
    static bool hasKey(const char *iniSource, const char *section, const ini_key_t key);
    static ini_value_t getValue(const char *iniSource,const ini_key_t key);
    static ini_value_t getValue(const char *iniSource,const ini_key_t key,bool &found);
    static ini_value_t getValue(const char *iniSource,const char *section,const ini_key_t key);
    static ini_value_t getValue(const char *iniSource,const char *section,const ini_key_t key,bool &found);
    static std::vector<std::pair<ini_key_t,ini_value_t> *> *getValues(const char *iniSource);
    static std::vector<std::pair<ini_key_t,ini_value_t> *> *getValues(const char *iniSource,const char *section);
    static void setValue(std::vector<std::pair<ini_key_t,ini_value_t> *> *values,const ini_key_t key,const ini_value_t value);
    static bool hasKey(std::vector<std::pair<ini_key_t,ini_value_t> *> *values, const ini_key_t key);
    static bool removeKey(std::vector<std::pair<ini_key_t,ini_value_t> *> *values, const ini_key_t key);
    static char *serializeSection(std::vector<std::pair<ini_key_t,ini_value_t> *> *values);
    static char *serializeSection(std::vector<std::pair<ini_key_t,ini_value_t> *> *values,const char *sectionName);
    static void freeAndDeleteCharArrayValueVector(std::vector<std::pair<char*,char*> *> *values);
};

#endif // INI_H
