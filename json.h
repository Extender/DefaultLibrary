#ifndef JSON_H
#define JSON_H

#include <stdint.h>
#include <string>
#include "text.h"
#include "io.h"

#define JSON_VALUE_TYPE_MAP 1
#define JSON_VALUE_TYPE_ARRAY 2
#define JSON_VALUE_TYPE_STRING 4
#define JSON_VALUE_TYPE_BOOL 8
#define JSON_VALUE_TYPE_DOUBLE 16
#define JSON_VALUE_TYPE_INT32 32
#define JSON_VALUE_TYPE_INT64 64

#define DEFAULT_JSON_PARSER_BUFFER_SIZE 1024
#define DEFAULT_JSON_PARSER_ATTR_NAME_BUFFER_SIZE 32
#define DEFAULT_JSON_PARSER_ATTR_VALUE_BUFFER_SIZE 64
#define DEFAULT_JSON_PARSER_VALUE_BUFFER_SIZE 16
#define DEFAULT_JSON_PARSER_ARRAY_BUFFER_SIZE 16
#define DEFAULT_JSON_SERIALIZER_BUFFER_SIZE 512

#define json_invalidIndex UINT32_MAX

namespace json
{
    struct Value
    {
        char *name;
        uint8_t type;
        char *value;

        Value();
        Value(char *_name);
        Value(char *_name,uint8_t _type);
        Value(char *_name,uint8_t _type,char *_value);
    };

    class ValueManager
    {
        static int32_t getInt32FromRaw(char *value);
        static int64_t getInt64FromRaw(char *value);

    public:
        static int32_t getInt32(Value *in);
        static int64_t getInt64(Value *in);
        static bool getBool(Value *in);
        static double getDouble(Value *in);
        static Value **getArray(Value *in,uint32_t &size);
        static Value **getMap(Value *in,uint32_t &size);
        static char *getString(Value *in);


        static char *fromInt32(int32_t in);
        static char *fromInt64(uint64_t in);
        static char *fromBool(bool in);
        static char *fromDouble(double in);
        static char *fromArray(Value **in,uint32_t size);
        static char *fromMap(Value **in,uint32_t size);
        static char *fromString(char *in);

        static bool hasKey(Value **array,uint32_t arraySize,char *key);
        static uint32_t indexOf(Value **array,uint32_t arraySize,char *key);
        static Value *getValue(Value **array,uint32_t arraySize,char *key);
        static void setValue(Value **&array,uint32_t &arraySize,Value *value);
        static bool removeKey(Value **&array,uint32_t &arraySize,char *key);
        static void removeAt(Value **&array,uint32_t &arraySize,uint32_t index);
    };

    class parser
    {
    public:
        static Value **parse(char *source, uint32_t &size);
        static Value **parse(char *source, uint32_t &size, uint32_t &stoppedAt);
        static char *serialize(Value **values,uint32_t size);
        static char *serialize(Value **values,uint32_t size,uint8_t baseType);
    };
};

#endif // JSON_H
