#ifndef INI_H
#define INI_H

#include "text.h"
#include "io.h"

#define ini_allSections "*" // WARNING: Default: ini_noSection!
#define ini_noSection ""
#define ini_key_t char *
#define ini_value_t char *

#define DEFAULT_INI_BUFFER_SIZE 1024

class ini
{
public:
    static std::vector<char*> getSections(char *iniSource);
    static bool hasSection(char *iniSource,char *section);
    static std::vector<ini_key_t> getKeys(char *iniSource);
    static std::vector<ini_key_t> getKeys(char *iniSource,char *section);
    static bool hasKey(char *iniSource,ini_key_t key);
    static bool hasKey(char *iniSource,char *section,ini_key_t key);
    static ini_value_t getValue(char *iniSource,ini_key_t key);
    static ini_value_t getValue(char *iniSource,ini_key_t key,bool &found);
    static ini_value_t getValue(char *iniSource,char *section,ini_key_t key);
    static ini_value_t getValue(char *iniSource,char *section,ini_key_t key,bool &found);
    static std::vector<std::pair<ini_key_t,ini_value_t> *> *getValues(char *iniSource);
    static std::vector<std::pair<ini_key_t,ini_value_t> *> *getValues(char *iniSource,char *section);
    static void setValue(std::vector<std::pair<ini_key_t,ini_value_t> *> *values,ini_key_t key,ini_value_t value);
    static bool hasKey(std::vector<std::pair<ini_key_t,ini_value_t> *> *values,ini_key_t key);
    static bool removeKey(std::vector<std::pair<ini_key_t,ini_value_t> *> *values,ini_key_t key);
    static char *serializeSection(std::vector<std::pair<ini_key_t,ini_value_t> *> *values);
    static char *serializeSection(std::vector<std::pair<ini_key_t,ini_value_t> *> *values,char *sectionName);
};

#endif // INI_H