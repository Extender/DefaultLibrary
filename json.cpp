#include "json.h"

int32_t json::ValueManager::getInt32FromRaw(char *value)
{
    uint32_t out=(uint32_t)value[0];
    out|=(uint32_t)((uint8_t)value[1])<<8;
    out|=(uint32_t)((uint8_t)value[2])<<16;
    out|=(uint32_t)((uint8_t)value[3])<<24;
    return out;
}

int64_t json::ValueManager::getInt64FromRaw(char *value)
{
    uint64_t out=(uint64_t)value[0];
    out|=(uint64_t)((uint8_t)value[1])<<8;
    out|=(uint64_t)((uint8_t)value[2])<<16;
    out|=(uint64_t)((uint8_t)value[3])<<24;
    out|=(uint64_t)((uint8_t)value[4])<<32;
    out|=(uint64_t)((uint8_t)value[5])<<40;
    out|=(uint64_t)((uint8_t)value[6])<<48;
    out|=(uint64_t)((uint8_t)value[7])<<56;
    return out;
}

int32_t json::ValueManager::getInt32(json::Value *in)
{
    uint32_t out=(uint8_t)in->value[0];
    out|=(uint32_t)((uint8_t)in->value[1])<<8;
    out|=(uint32_t)((uint8_t)in->value[2])<<16;
    out|=(uint32_t)((uint8_t)in->value[3])<<24;
    return out;
}

int64_t json::ValueManager::getInt64(json::Value *in)
{
    uint64_t out=(uint8_t)in->value[0];
    out|=(uint64_t)((uint8_t)in->value[1])<<8;
    out|=(uint64_t)((uint8_t)in->value[2])<<16;
    out|=(uint64_t)((uint8_t)in->value[3])<<24;
    out|=(uint64_t)((uint8_t)in->value[4])<<32;
    out|=(uint64_t)((uint8_t)in->value[5])<<40;
    out|=(uint64_t)((uint8_t)in->value[6])<<48;
    out|=(uint64_t)((uint8_t)in->value[7])<<56;
    return out;
}

double json::ValueManager::getDouble(json::Value *in)
{
    int64_t base=getInt64(in);
    int32_t div=getInt32FromRaw(in->value+sizeof(int64_t));
    return (double)base/pow((double)10,(int32_t)div);
}

bool json::ValueManager::getBool(json::Value *in)
{
    return *(in->value)==1;
}

json::Value ** const json::ValueManager::getConstArrayPointer(json::Value *in,uint32_t &size)
{
    // NOTE: This returns a pointer to the internal array, not a copy of it!
    // Do not free the returned object!

    // The first element always denotes the length of the array.

    Value **arr=(Value**)ValueManager::getInt64(in);
    Value *i0=arr[0];
    size=ValueManager::getInt32(i0);
    return const_cast<Value ** const>(arr+1);
}

json::Value ** const json::ValueManager::getConstMapPointer(json::Value *in,uint32_t &size)
{
    // NOTE: This returns a pointer to the internal map, not a copy of it!
    // Do not free the returned object!

    // The first element always denotes the length of the array.
    Value **arr=(Value**)ValueManager::getInt64(in);
    size=ValueManager::getInt32(arr[0]);
    return const_cast<Value ** const>(arr+1);
}

json::Value **json::ValueManager::getDeepArrayCopy(json::Value *in, uint32_t &size)
{
    Value ** const v=getConstMapPointer(in,size);
    return createDeepCopyOfValueArray(const_cast<Value**>(v),size,false,true);
}

json::Value **json::ValueManager::getDeepMapCopy(json::Value *in, uint32_t &size)
{
    Value ** const v=getConstMapPointer(in,size);
    return createDeepCopyOfValueArray(const_cast<Value**>(v),size,false,true);
}

char *json::ValueManager::getString(json::Value *in)
{
    return text::duplicateString(in->value);
}

char *json::ValueManager::fromInt32(int32_t in)
{
    char *out=(char*)malloc(sizeof(uint32_t)); // Correct order.
    out[0]=(uint8_t)in;
    out[1]=(uint8_t)(in>>8);
    out[2]=(uint8_t)(in>>16);
    out[3]=(uint8_t)(in>>24);
    return out;
}

char *json::ValueManager::fromInt64(uint64_t in)
{
    char *out=(char*)malloc(sizeof(uint64_t)); // Correct order.
    out[0]=(uint8_t)in;
    out[1]=(uint8_t)(in>>8);
    out[2]=(uint8_t)(in>>16);
    out[3]=(uint8_t)(in>>24);
    out[4]=(uint8_t)(in>>32);
    out[5]=(uint8_t)(in>>40);
    out[6]=(uint8_t)(in>>48);
    out[7]=(uint8_t)(in>>56);
    return out;
}

char *json::ValueManager::fromDouble(double in)
{
    bool n=in<0.0;
    if(n)
        in*=-1;
    uint32_t div=0;
    while(in!=floor((double)in))
    {
        in*=10;
        div++;
    }
    int64_t base=((int64_t)in)*(n?-1:1);
    char *out=(char*)calloc(sizeof(uint64_t)+sizeof(uint32_t),1); // Correct order.
    out[0]=(uint8_t)base;
    out[1]=(uint8_t)(base>>8);
    out[2]=(uint8_t)(base>>16);
    out[3]=(uint8_t)(base>>24);
    out[4]=(uint8_t)(base>>32);
    out[5]=(uint8_t)(base>>40);
    out[6]=(uint8_t)(base>>48);
    out[7]=(uint8_t)(base>>56);

    out[8]=(uint8_t)div;
    out[9]=(uint8_t)(div>>8);
    out[10]=(uint8_t)(div>>16);
    out[11]=(uint8_t)(div>>24);

    return out;
}

char *json::ValueManager::fromBool(bool in)
{
    char *out=(char*)malloc(1);
    out[0]=in?1:0;
    return out;
}

char *json::ValueManager::fromArray(json::Value **in, uint32_t size)
{
    // This performs a deep copy of the "in" array instead of using "in" directly.
    // Thus, after calling this method, operations performed with "in" will not affect the array at the address returned.
    // The same as below.
    Value **arr=(Value**)malloc((size+1)*sizeof(Value*));
    Value *sizeValue=new Value(text::duplicateString(""),JSON_VALUE_TYPE_INT32,fromInt32(size));
    arr[0]=sizeValue;
    for(uint32_t i=0;i<size;i++)
        arr[i+1]=in[i];
    return fromInt64((uint64_t)arr);
}

char *json::ValueManager::fromMap(json::Value **in, uint32_t size)
{
    // This performs a deep copy of the "in" map instead of using "in" directly.
    // Thus, after calling this method, operations performed with "in" will not affect the map at the address returned.
    // The same as above.
    Value **arr=(Value**)calloc(size+1,sizeof(Value*));
    Value *sizeValue=new Value(text::duplicateString(""),JSON_VALUE_TYPE_INT32,fromInt32(size));
    arr[0]=sizeValue;
    for(uint32_t i=0;i<size;i++)
        arr[i+1]=in[i]->createDeepCopy();
    uint64_t *address=new uint64_t[1];
    address[0]=(uint64_t)arr;
    return (char*)address;
}

char *json::ValueManager::fromString(char *in)
{
    return text::duplicateString(in);
}

char *json::ValueManager::fromRawData(char *in)
{
    return in;
}

bool json::ValueManager::hasKey(json::Value **array, uint32_t arraySize, const char *key)
{
    for(uint32_t i=0;i<arraySize;i++)
    {
        if(strcmp(array[i]->name,key)==0)
            return true;
    }
    return false;
}

uint32_t json::ValueManager::indexOf(json::Value **array, uint32_t arraySize, const char *key)
{
    for(uint32_t i=0;i<arraySize;i++)
    {
        if(strcmp(array[i]->name,key)==0)
            return i;
    }
    return json_invalidIndex;
}

json::Value *json::ValueManager::getValue(json::Value **array, uint32_t arraySize, const char *key)
{
    for(uint32_t i=0;i<arraySize;i++)
    {
        char *name=array[i]->name;
        if(strcmp(name,key)==0)
            return array[i];
    }
    return 0;
}

void json::ValueManager::setValue(Value **&array, uint32_t &arraySize, Value *value)
{
    for(uint32_t i=0;i<arraySize;i++)
    {
        Value *oldValue=array[i];
        if(strcmp(oldValue->name,value->name)==0)
        {
            setValueAt(array,arraySize,i,value);
            return;
        }
    }
    addValue(array,arraySize,value);
}

void json::ValueManager::setValueAt(json::Value **&array, uint32_t &arraySize, uint32_t index, json::Value *value)
{
    delete array[index];
    array[index]=value;
}

void json::ValueManager::addValue(json::Value **&array, uint32_t &arraySize, json::Value *value)
{
    arraySize++;
    Value **newArray=(Value**)malloc(arraySize*sizeof(Value*));
    if(arraySize>1)
        memcpy(newArray,array,(arraySize-1)*sizeof(Value*));
    free(array);
    array=newArray;
    array[arraySize-1]=value;
}

void json::ValueManager::insertValue(json::Value **&array, uint32_t &arraySize, uint32_t index, json::Value *value)
{
    if(index>=arraySize)
    {
        addValue(array,arraySize,value);
        return;
    }
    arraySize++;
    Value **newArray=(Value**)malloc(arraySize*sizeof(Value*));
    for(uint32_t i=0;i<index;i++)
        newArray[i]=array[i];
    newArray[index]=value;
    for(uint32_t i=index+1;i<arraySize;i++)
        newArray[i]=array[i-1];
    free(array);
    array=newArray;
}

bool json::ValueManager::removeKey(json::Value **&array, uint32_t &arraySize, char *key)
{
    for(uint32_t i=0;i<arraySize;i++)
    {
        Value *valueAtI=array[i];
        if(strcmp(array[i]->name,key)==0)
        {
            Value **newArray=(Value**)malloc((arraySize-1)*sizeof(Value*));
            uint32_t c=0;
            for(uint32_t j=0;j<arraySize;j++)
            {
                if(j==i)
                {
                    // Delete and do not insert this value into the new array
                    delete valueAtI;
                    continue;
                }
                newArray[c++]=array[j];
            }
            arraySize--;
            free(array);
            array=newArray;
            return true;
        }
    }
    return false;
}

void json::ValueManager::removeAt(json::Value **&array, uint32_t &arraySize, uint32_t index)
{
    Value **newArray=(Value**)malloc((arraySize-1)*sizeof(Value*));
    uint32_t c=0;
    for(uint32_t j=0;j<arraySize;j++)
    {
        if(j==index)
        {
            delete array[j];
            continue;
        }
        newArray[c++]=array[j];
    }
    arraySize--;
    free(array);
    array=newArray;
}

json::Value **json::ValueManager::createDeepCopyOfValueArray(json::Value **values, uint32_t totalSizeOrNull, bool sizeIsFirstElement, bool copyFirstElement)
{
    if(sizeIsFirstElement)
        totalSizeOrNull=ValueManager::getInt32(values[0]);
    Value **out=(Value**)malloc((totalSizeOrNull-(copyFirstElement?0:1))*sizeof(Value*));
    uint32_t posTracker=0;
    for(uint32_t i=(copyFirstElement?0:1);i<totalSizeOrNull;i++)
    {
        Value *thisValue=values[i];
        out[posTracker++]=thisValue->createDeepCopy();
    }
    return out;
}

void json::ValueManager::freeValueArray(json::Value **values, uint32_t size)
{
    for(fs_t i=0;i<size;i++)
        delete values[i];
    free(values);
}

void json::ValueManager::freeInternalArray(json::Value **values)
{
    // The first element always denotes the length of the array.
    freeValueArray(values,ValueManager::getInt32(values[0]));
}

json::Value::Value()
{
    name=text::duplicateString("");
    type=0;
    value=text::duplicateString("");
}

json::Value::Value(char *_name)
{
    name=_name;
    type=0;
    value=text::duplicateString("");
}

json::Value::Value(char *_name, uint8_t _type)
{
    name=_name;
    type=_type;
    value=text::duplicateString("");
}

json::Value::Value(char *_name, uint8_t _type, char *_value)
{
    name=_name;
    type=_type;
    value=_value;
}

json::Value::Value(const char *_name)
{
    name=text::duplicateString(_name);
    type=0;
    value=text::duplicateString("");
}

json::Value::Value(const char *_name, uint8_t _type)
{
    name=text::duplicateString(_name);
    type=_type;
    value=text::duplicateString("");
}

json::Value::Value(const char *_name, uint8_t _type, const char *_value)
{
    name=text::duplicateString(_name);
    type=_type;
    value=text::duplicateString(_value);
}

json::Value::~Value()
{
    free(name);
    if(type==JSON_VALUE_TYPE_ARRAY||type==JSON_VALUE_TYPE_MAP)
        ValueManager::freeInternalArray((Value**)ValueManager::getInt64(this)/*Resolve pointer*/);
    free(value); // An array/map has a int64 value stored in a char*, too!
}

int32_t json::Value::toInt32()
{
    if(type!=JSON_VALUE_TYPE_INT32)
    {
        if(type==JSON_VALUE_TYPE_STRING)
        {
            char *str=ValueManager::getString(this);
            int32_t out=text::intFromString(str);
            free(str);
            return out;
        }
        else if(type==JSON_VALUE_TYPE_INT64)
            return -1;
        else if(type==JSON_VALUE_TYPE_DOUBLE)
            return text::round(ValueManager::getDouble(this));
    }

    return ValueManager::getInt32(this);
}

int64_t json::Value::toInt64()
{
    if(type!=JSON_VALUE_TYPE_INT64)
    {
        if(type==JSON_VALUE_TYPE_STRING)
        {
            char *str=ValueManager::getString(this);
            int64_t out=text::longFromString(str);
            free(str);
            return out;
        }
        else if(type==JSON_VALUE_TYPE_INT32)
            return (int64_t)ValueManager::getInt32(this);
        else if(type==JSON_VALUE_TYPE_DOUBLE)
            return text::roundl(ValueManager::getDouble(this));
    }

    return ValueManager::getInt64(this);
}

double json::Value::toDouble()
{
    if(type!=JSON_VALUE_TYPE_DOUBLE)
    {
        if(type==JSON_VALUE_TYPE_STRING)
        {
            char *str=ValueManager::getString(this);
            double out=atof(str);
            free(str);
            return out;
        }
        else if(type==JSON_VALUE_TYPE_INT32)
            return (double)ValueManager::getInt32(this);
        else if(type==JSON_VALUE_TYPE_INT64)
            return (double)ValueManager::getInt64(this);
        else if(type==JSON_VALUE_TYPE_BOOL)
            return ValueManager::getBool(this)?1.0:0.0;
    }

    return ValueManager::getDouble(this);
}

bool json::Value::toBool()
{
    if(type!=JSON_VALUE_TYPE_BOOL)
    {
        if(type==JSON_VALUE_TYPE_STRING)
        {
            char *str=ValueManager::getString(this);
            bool out=text::iCompare(str,"1")||text::iCompare(str,"true");
            free(str);
            return out;
        }
        else if(type==JSON_VALUE_TYPE_INT32)
            return ValueManager::getInt32(this)>0;
        else if(type==JSON_VALUE_TYPE_INT64)
            return ValueManager::getInt64(this)>0;
        else if(type==JSON_VALUE_TYPE_DOUBLE)
            return ValueManager::getDouble(this)>0.0;
    }

    return ValueManager::getBool(this);
}

json::Value **json::Value::toArray(uint32_t &size)
{
    return ValueManager::getDeepArrayCopy(this,size);
}

json::Value **json::Value::toMap(uint32_t &size)
{
    return ValueManager::getDeepMapCopy(this,size);
}

char *json::Value::toString()
{
    if(type!=JSON_VALUE_TYPE_STRING)
    {
        if(type==JSON_VALUE_TYPE_INT32)
            return text::intToString(ValueManager::getInt32(this));
        else if(type==JSON_VALUE_TYPE_INT64)
            return text::longToString(ValueManager::getInt64(this));
        else if(type==JSON_VALUE_TYPE_DOUBLE)
            return text::doubleToString(ValueManager::getDouble(this));
        else if(type==JSON_VALUE_TYPE_BOOL)
            return text::duplicateString(ValueManager::getBool(this)?"true":"false");
        else if(type==JSON_VALUE_TYPE_ARRAY)
        {
            uint32_t size;
            Value ** const values=ValueManager::getConstArrayPointer(this,size);
            char *out=parser::serialize(const_cast<Value**>(values),size,JSON_VALUE_TYPE_ARRAY);
            return out;
        }
        else if(type==JSON_VALUE_TYPE_MAP)
        {
            uint32_t size;
            Value ** const values=ValueManager::getConstArrayPointer(this,size);
            char *out=parser::serialize(const_cast<Value**>(values),size,JSON_VALUE_TYPE_MAP);
            return out;
        }
        else if(type==JSON_VALUE_TYPE_NULL)
            return text::duplicateString("null");

        return text::duplicateString("");
    }

    return ValueManager::getString(this);
}

const char *json::Value::asConstString()
{
    return value;
}

json::Value *json::Value::createDeepCopy()
{
    char *newName=text::duplicateString(name);
    char *newValue=0;
    if(value!=0)
    {
        if(type==JSON_VALUE_TYPE_INT32)
            newValue=ValueManager::fromInt32(ValueManager::getInt32(this));
        else if(type==JSON_VALUE_TYPE_INT64)
            newValue=ValueManager::fromInt64(ValueManager::getInt64(this));
        else if(type==JSON_VALUE_TYPE_DOUBLE)
            newValue=ValueManager::fromDouble(ValueManager::getDouble(this));
        else if(type==JSON_VALUE_TYPE_BOOL)
            newValue=ValueManager::fromBool(ValueManager::getBool(this));
        else if(type==JSON_VALUE_TYPE_STRING)
            newValue=ValueManager::fromString(ValueManager::getString(this));
        else if(type==JSON_VALUE_TYPE_ARRAY)
        {
            uint32_t size;
            Value ** const v=ValueManager::getConstArrayPointer(this,size);
            // "fromArray" creates a deep copy of the array passed.
            newValue=ValueManager::fromArray(const_cast<Value**>(v),size);
        }
        else if(type==JSON_VALUE_TYPE_MAP)
        {
            uint32_t size;
            Value ** const v=ValueManager::getConstArrayPointer(this,size);
            // "fromMap" creates a deep copy of the map passed.
            newValue=ValueManager::fromMap(const_cast<Value**>(v),size);
        }
    }

    return new Value(newName,type,newValue);
}

json::Value **json::parser::parse(const char *source, uint32_t &size)
{
    uint32_t temp;
    return parse(source,size,temp);
}

json::Value **json::parser::parse(const char *source, uint32_t &size, uint32_t &stoppedAt)
{
    uint32_t sourceLength=strlen(source);
    char prevChar=0;
    char prevNonWhitespaceChar=0;
    char thisChar;
    bool isWhitespace;

    size=0;

    #define JSON_PARSER_NEXT_ITERATION \
    \
    if(thisChar=='\\'&&prevChar=='\\')\
        prevChar=0;\
    else\
        prevChar=thisChar;\
    if(!isWhitespace)\
        prevNonWhitespaceChar=thisChar;\
    continue;

#define JSON_PARSER_ADD_ATTR \
    \
    if(size==valueBufferSize)\
    {\
        valueBufferSize*=2;\
        values=(Value**)realloc(values,valueBufferSize*sizeof(Value*));\
    }\
    Value *v;\
    io::terminateBuffer(attrValue,attrValueLength,attrValueBufferSize);\
    if(thisType==JSON_VALUE_TYPE_INT64)\
    {\
        int64_t l=text::longFromString(attrValue);\
        if(l>=INT32_MIN&&l<=INT32_MAX)\
            thisType=JSON_VALUE_TYPE_INT32;\
    }\
    char *thisAttrName;\
    if(isMap)\
    {\
        "This is a map, each attribute has a name.";\
        thisAttrName=text::firstChars(attrName,attrNameLength);\
    }\
    else\
    {\
        "This is an array, the individual attributes have no names.";\
        thisAttrName=text::duplicateString("");\
    }\
    if(thisType==JSON_VALUE_TYPE_STRING)\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_STRING,ValueManager::fromRawData(text::unescape(attrValue)));\
    else if(thisType==JSON_VALUE_TYPE_INT32)\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_INT32,ValueManager::fromInt32(atoi(attrValue)));\
    else if(thisType==JSON_VALUE_TYPE_INT64)\
    {\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_INT64,ValueManager::fromInt64(atoll(attrValue)));\
    }\
    else if(thisType==JSON_VALUE_TYPE_DOUBLE)\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_DOUBLE,ValueManager::fromDouble(atof(attrValue)));\
    else if(thisType==JSON_VALUE_TYPE_BOOL)\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_BOOL,ValueManager::fromBool(text::iCompare(attrValue,"true")));\
    else if(thisType==JSON_VALUE_TYPE_ARRAY)\
    {\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_ARRAY,ValueManager::fromArray(subarray,subarraySize));\
        "Do not free the items of \"subarray\"; they are used in \"newValue\".";\
        free(subarray);\
        subarraySize=-1L;\
    }\
    else if(thisType==JSON_VALUE_TYPE_MAP)\
    {\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_MAP,ValueManager::fromMap(subarray,subarraySize));\
        "Do not free the items of \"subarray\"; they are used in \"newValue\".";\
        free(subarray);\
        subarraySize=-1L;\
    }\
    else if(thisType==JSON_VALUE_TYPE_NULL)\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_NULL,0);\
    values[size++]=v;\
    addToValue=true;\
    attrNameLength=0;\
    attrValueLength=0;\
    thisType=JSON_VALUE_TYPE_INT64;

    fs_t attrNameBufferSize=DEFAULT_JSON_PARSER_ATTR_NAME_BUFFER_SIZE;
    fs_t attrNameLength=0;
    char *attrName=(char*)malloc(attrNameBufferSize);

    fs_t attrValueBufferSize=DEFAULT_JSON_PARSER_ATTR_VALUE_BUFFER_SIZE;
    fs_t attrValueLength=0;
    char *attrValue=(char*)malloc(attrValueBufferSize);

    uint8_t thisType=JSON_VALUE_TYPE_INT64;
    Value **subarray;
    uint32_t subarraySize=-1L; // Not 0

    fs_t valueBufferSize=DEFAULT_JSON_PARSER_VALUE_BUFFER_SIZE;
    Value **values=(Value**)malloc(valueBufferSize*sizeof(Value*));

    bool isMap=true; // Specifies whether "values" is a map.
    bool isAttrName=false;

    char stringChar;

    bool addToValue=true;

    for(uint32_t i=0;i<sourceLength;i++)
    {
        thisChar=source[i];
        isWhitespace=thisChar==' '||thisChar=='\n'||thisChar=='\r'||thisChar=='\t';

        if(isAttrName)
        {
            if(thisChar==':')
            {
                isAttrName=false;
                JSON_PARSER_NEXT_ITERATION;
            }
            if(thisChar=='\''||thisChar=='"')
            { // {} needed here.
                JSON_PARSER_NEXT_ITERATION;
            }
            if(isWhitespace)
            { // {} needed here.
                JSON_PARSER_NEXT_ITERATION;
            }
            if(thisChar=='}' || thisChar == ']')
            {
                // Needed for empty arrays/maps.

                // Stop; needed for nested array/map support.
                if(attrNameLength>0||attrValueLength>0)
                {
                    // Check needed to avoid adding empty attributes.
                    JSON_PARSER_ADD_ATTR;
                }
                stoppedAt=i;
                break;
            }
            io::writeRawCharToBuffer(attrName,thisChar,attrNameLength,attrNameBufferSize);
            JSON_PARSER_NEXT_ITERATION;
        }

        if(thisChar==',')
        {
            JSON_PARSER_ADD_ATTR;
            if(isMap)
                isAttrName=true;
            JSON_PARSER_NEXT_ITERATION;
        }
        if(prevNonWhitespaceChar==0) // Keep this above the subarray code.
        {
            if(!isWhitespace)
            {
                if(thisChar=='[')
                    isMap=false;
                else
                    isAttrName=true;
            }
            JSON_PARSER_NEXT_ITERATION;
        }
        if(thisChar=='{')
        {
            thisType=JSON_VALUE_TYPE_MAP;
            uint32_t end;
            subarray=parse(source+i,subarraySize,end);
            i+=end;
            JSON_PARSER_NEXT_ITERATION;
        }

        if(thisChar=='[')
        {
            thisType=JSON_VALUE_TYPE_ARRAY;
            uint32_t end;
            subarray=parse(source+i,subarraySize,end);
            i+=end;
            JSON_PARSER_NEXT_ITERATION;
        }
        if(thisChar=='}'||thisChar==']')
        {
            // Stop; needed for nested array/map support.
            if(attrNameLength>0||attrValueLength>0||subarraySize!=-1) // Not 0
            {
                // Check needed to avoid adding empty attributes.
                JSON_PARSER_ADD_ATTR;
            }
            stoppedAt=i;
            break;
        }

        if(isWhitespace&&(thisType==JSON_VALUE_TYPE_INT32||thisType==JSON_VALUE_TYPE_INT64||thisType==JSON_VALUE_TYPE_DOUBLE))
        {
            JSON_PARSER_NEXT_ITERATION;
        }

        if(thisType==JSON_VALUE_TYPE_INT64&&(thisChar<48||thisChar>57)&&thisChar!='-'&&thisChar!='+') // Range of numeric values in ASCII and UTF8
        {
            thisType=JSON_VALUE_TYPE_DOUBLE;
        }
        if(thisType!=JSON_VALUE_TYPE_STRING)
        {
            if(thisChar=='\''||thisChar=='"')
            {
                thisType=JSON_VALUE_TYPE_STRING;
                stringChar=thisChar;
                attrValueLength=0;
                JSON_PARSER_NEXT_ITERATION;
            }
            else if(thisChar=='t'||thisChar=='f'||thisChar=='T'||thisChar=='F')
                thisType=JSON_VALUE_TYPE_BOOL;
            else if(thisChar=='n')
            {
                addToValue=false;
                thisType=JSON_VALUE_TYPE_NULL;
            }
        }
        else if(thisChar==stringChar&&prevChar!='\\') // and thisType==JSON_VALUE_TYPE_STRING (else if); !!: \\" handled in JSON_PARSER_NEXT_ITERATION
        {
            addToValue=false;
            JSON_PARSER_NEXT_ITERATION; // !!: This is correct! Value adding handled when a , ] } is detected, and not when reaching the end of the string.
        }

        if(addToValue)
            io::writeRawCharToBuffer(attrValue,thisChar,attrValueLength,attrValueBufferSize);

        JSON_PARSER_NEXT_ITERATION;
    }

    #undef JSON_PARSER_NEXT_ITERATION
    #undef JSON_PARSER_ADD_ATTR

    free(attrName);
    free(attrValue);

    return values;
}

char *json::parser::serialize(json::Value **values, uint32_t size)
{
    return serialize(values,size,JSON_VALUE_TYPE_MAP);
}

char *json::parser::serialize(json::Value **values, uint32_t size, uint8_t baseType)
{
    bool isArray=baseType==JSON_VALUE_TYPE_ARRAY;
    if(size==0)
        return text::duplicateString(isArray?"[]":"{}");
    fs_t bufferSize=DEFAULT_JSON_SERIALIZER_BUFFER_SIZE;
    fs_t pos=1; // [
    char *out=(char*)malloc(bufferSize);
    char *nameStr;
    out[0]=isArray?'[':'{';
    uint32_t nameLength;
    for(uint32_t i=0;i<size;i++)
    {
        if(i>0)
            io::writeRawCharToBuffer(out,',',pos,bufferSize);
        Value *thisValue=values[i];

        if(isArray)
            nameLength=0;
        else
            nameLength=strlen(thisValue->name);
        if(nameLength>0)
        {
            nameStr=text::concat("\"",thisValue->name,"\":");
            io::writeRawDataToBuffer(out,nameStr,nameLength+3,pos,bufferSize); // Include the "":, but not the zero terminator
            free(nameStr);
        }
        if(thisValue->type==JSON_VALUE_TYPE_STRING)
        {
            char *in=text::concat("\"",text::escape(ValueManager::getString(thisValue)),"\"");
            io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
            free(in);
        }
        else if(thisValue->type==JSON_VALUE_TYPE_INT32)
        {
            char *in=text::toString(ValueManager::getInt32(thisValue));
            io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
            free(in);
        }
        else if(thisValue->type==JSON_VALUE_TYPE_INT64)
        {
            char *in=text::longToString(ValueManager::getInt64(thisValue));
            io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
            free(in);
        }
        else if(thisValue->type==JSON_VALUE_TYPE_BOOL)
        {
            char *in=text::duplicateString(ValueManager::getBool(thisValue)?"true":"false");
            io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
            free(in);
        }
        else if(thisValue->type==JSON_VALUE_TYPE_DOUBLE)
        {
            char *in=text::doubleToString(ValueManager::getDouble(thisValue));
            io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
            free(in);
        }
        else if(thisValue->type==JSON_VALUE_TYPE_ARRAY||thisValue->type==JSON_VALUE_TYPE_MAP)
        {
            uint32_t tempSize;
            Value ** const arr=ValueManager::getConstArrayPointer(thisValue,tempSize);
            char *in=serialize(const_cast<Value**>(arr),tempSize,thisValue->type);
            size_t len=strlen(in);
            io::writeRawDataToBuffer(out,in,len,pos,bufferSize);
            free(in);
        }
        else if(thisValue->type==JSON_VALUE_TYPE_NULL)
        {
            char *in=text::duplicateString("null");
            io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
            free(in);
        }
    }
    io::writeRawCharToBuffer(out,isArray?']':'}',pos,bufferSize);
    io::terminateBuffer(out,pos,bufferSize);
    return out;
}

void json::parser::freeValueArray(json::Value **values, uint32_t size)
{
    ValueManager::freeValueArray(values,size);
}
