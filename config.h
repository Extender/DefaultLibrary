#ifndef CONFIG_H
#define CONFIG_H

#include "io.h"
#include "text.h"

#define CONFIG_VALUE_TYPE_STRING 4
#define CONFIG_VALUE_TYPE_BOOL 8
#define CONFIG_VALUE_TYPE_DOUBLE 16
#define CONFIG_VALUE_TYPE_INT32 32
#define CONFIG_VALUE_TYPE_INT64 64
#define CONFIG_VALUE_TYPE_DATA 128

#define config_allValues ""
#define CONFIG_DEFAULT_BUFFER_SIZE 1024

class ConfigValue
{
private:
    fs_t valueLength;

public:
    char *value;
    uint8_t type;

    ConfigValue(uint8_t _type, fs_t _valueLengthOrZero, char *_value);
    ConfigValue(const char *_value);
    ~ConfigValue();

    int32_t toInt32();
    int64_t toInt64();
    double toDouble();
    bool toBool();
    char* toString();
    char* toRawData();

    size_t getValueLength();

    static ConfigValue *fromInt32(int32_t in);
    static ConfigValue *fromInt64(int64_t in);
    static ConfigValue *fromDouble(double in);
    static ConfigValue *fromBool(bool in);
    static ConfigValue *fromString(char *in);
    static ConfigValue *fromString(const char *in);
    static ConfigValue *fromRawData(char *in,fs_t size);
};

typedef char* config_key_t;
typedef ConfigValue config_value_deref_t;
typedef config_value_deref_t* config_value_t;
typedef std::pair<config_key_t,config_value_t> config_pair_deref_t;
typedef config_pair_deref_t* config_pair_t;

class config
{
public:
    static config_value_t nullValue;

    static config_pair_t makeKeyValuePair(config_key_t key,config_value_t value);
    static config_pair_t makeKeyValuePair(config_key_t key,int32_t value);
    static config_pair_t makeKeyValuePair(config_key_t key,int64_t value);
    static config_pair_t makeKeyValuePair(config_key_t key,double value);
    static config_pair_t makeKeyValuePair(config_key_t key,bool value);
    static config_pair_t makeKeyValuePair(config_key_t key,char *value);
    static config_pair_t makeKeyValuePair(config_key_t key,const char *value);


    static config_pair_t makeKeyValuePair(const char *key,const char *value);
    static config_pair_t makeKeyValuePair(const char *key,config_value_t value);
    static config_pair_t makeKeyValuePair(const char *key,int32_t value);
    static config_pair_t makeKeyValuePair(const char *key,int64_t value);
    static config_pair_t makeKeyValuePair(const char *key,double value);
    static config_pair_t makeKeyValuePair(const char *key,bool value);
    static config_pair_t makeKeyValuePair(const char *key,char *value);

    static std::vector<config_key_t> *getKeys(char *source);
    static std::vector<config_key_t> *getKeys(char *source,char *startingWith);
    static std::vector<config_pair_t> *getValues(char *source);
    static std::vector<config_pair_t> *getValues(char *source, char *startingWith);
    static config_value_t getValue(char *source, config_key_t key);
    static config_value_t getValue(std::vector<config_pair_t> *values, config_key_t key);
    static void setValue(std::vector<config_pair_t> *values,config_key_t key,config_value_t value);
    static bool hasKey(char *source,config_key_t key);
    static bool hasKey(std::vector<config_pair_t> *values,config_key_t key);
    static bool removeKey(std::vector<config_pair_t> *values,config_key_t key);
    static char *serializeConfig(std::vector<config_pair_t> *values,fs_t &length);
    static void freeValueArray(std::vector<config_pair_t> *values);
};

#endif // CONFIG_H
