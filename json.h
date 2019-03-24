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
#define JSON_VALUE_TYPE_NULL 128

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

        // Const strings must be duplicated.

        Value(const char *_name);
        Value(const char *_name,uint8_t _type);
        Value(const char *_name,uint8_t _type,const char *_value);

        ~Value();

        int32_t toInt32();
        int64_t toInt64();
        double toDouble();
        bool toBool();
        Value **toArray(uint32_t &size);
        Value **toMap(uint32_t &size);
        char *toString();
        const char *asConstString();

        Value *createDeepCopy();
    };

    class ValueManager
    {
        static int32_t getInt32FromRaw(char *value);
        static int64_t getInt64FromRaw(char *value);

    public:
        static int32_t getInt32(Value *in);
        static int64_t getInt64(Value *in);
        static double getDouble(Value *in);
        static bool getBool(Value *in);
        static Value **getDeepArrayCopy(Value *in,uint32_t &size);
        static Value **getDeepMapCopy(Value *in,uint32_t &size);
        static char *getString(Value *in);

        static char *fromInt32(int32_t in);
        static char *fromInt64(uint64_t in);
        static char *fromDouble(double in);
        static char *fromBool(bool in);
        static char *fromArray(Value **in,uint32_t size);
        static char *fromMap(Value **in,uint32_t size);
        static char *fromString(char *in);
        static char *fromRawData(char *in);

        static bool hasKey(Value **array,uint32_t arraySize,const char *key);
        static uint32_t indexOf(Value **array,uint32_t arraySize,const char *key);
        static Value *getValue(Value **array, uint32_t arraySize, const char *key);
        static void setValue(Value **&array,uint32_t &arraySize,Value *value);
        static void setValueAt(Value **&array,uint32_t &arraySize,uint32_t index,Value *value);
        static void addValue(Value **&array,uint32_t &arraySize,Value *value);
        static void insertValue(Value **&array,uint32_t &arraySize,uint32_t index,Value *value);
        static bool removeKey(Value **&array,uint32_t &arraySize,char *key);
        static void removeAt(Value **&array,uint32_t &arraySize,uint32_t index);

        static Value **createDeepCopyOfValueArray(Value **values,uint32_t totalSizeOrNull,bool sizeIsFirstElement=false,bool copyFirstElement=true);
        static void freeValueArray(Value **values,uint32_t size);
        static void freeInternalArray(Value **values);

        // Do not use the const pointers returned by these methods to add/remove objects.
        // Note that when an element is added (to the array/map returned by these functions),
        // the pointer in the Value object will not be changed, and will thus still
        // point to the old array, which will not contain the new element.
        static Value ** const getConstArrayPointer(Value *in,uint32_t &size);
        static Value ** const getConstMapPointer(Value *in,uint32_t &size);
    };

    class parser
    {
    public:
        static Value **parse(const char *source, uint32_t &size, uint32_t &stoppedAt);
        static Value **parse(const char *source, uint32_t &size);
        static char *serialize(Value **values,uint32_t size);
        static char *serialize(Value **values,uint32_t size,uint8_t baseType);
        static void freeValueArray(Value **values,uint32_t size);
    };
}

#endif // JSON_H
