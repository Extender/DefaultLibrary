#ifndef CONFIG_H
#define CONFIG_H

#include "io.h"
#include "text.h"

struct ConfigValue
{
    bool zeroTerminated;
    fs_t valueLength;
    char *value;
};

#define config_key_t char *
#define config_value_t ConfigValue *
#define config_value_deref_t ConfigValue
#define config_allValues ""

#define CONFIG_DEFAULT_BUFFER_SIZE 1024

class config
{
public:
    static config_value_t nullValue;

    static std::vector<config_key_t> *getKeys(char *source);
    static std::vector<config_key_t> *getKeys(char *source,char *startingWith);
    static std::vector<std::pair<config_key_t,config_value_t> *> *getValues(char *source);
    static std::vector<std::pair<config_key_t,config_value_t> *> *getValues(char *source, char *startingWith);
    static config_value_t getValue(char *source, config_key_t key);
    static config_value_t getValue(std::vector<std::pair<config_key_t,config_value_t> *> *values, config_key_t key);
    static void setValue(std::vector<std::pair<config_key_t,config_value_t> *> *values,config_key_t key,config_value_t value);
    static bool hasKey(char *source,config_key_t key);
    static bool hasKey(std::vector<std::pair<config_key_t,config_value_t> *> *values,config_key_t key);
    static bool removeKey(std::vector<std::pair<config_key_t,config_value_t> *> *values,config_key_t key);
    static char *serializeConfig(std::vector<std::pair<config_key_t,config_value_t> *> *values,fs_t &length);
};

#endif // CONFIG_H
