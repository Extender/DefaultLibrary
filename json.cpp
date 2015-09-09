#include "json.h"

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

bool json::ValueManager::getBool(json::Value *in)
{
    return *(in->value)==1;
}

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

double json::ValueManager::getDouble(json::Value *in)
{
    int64_t base=getInt64(in);
    int32_t div=getInt32FromRaw(in->value+sizeof(int64_t));
    return (double)base/pow((double)10,(int)div);
}

json::Value **json::ValueManager::getArray(json::Value *in,uint32_t &size)
{
    Value **out=(Value**)in->value;
    size=getInt32(out[0]);
    out+=1; // 1*sizeof(Value*) in chars
    return out; // The first element always denotes the length of the array.
}

json::Value **json::ValueManager::getMap(json::Value *in,uint32_t &size)
{
    Value **out=(Value**)in->value;
    size=getInt32(out[0]);
    out+=1; // 1*sizeof(Value*) in chars
    return out; // The first element always denotes the length of the array.
}

char *json::ValueManager::getString(json::Value *in)
{
    return in->value;
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

char *json::ValueManager::fromBool(bool in)
{
    char *out=(char*)malloc(1);
    out[0]=in?1:0;
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

char *json::ValueManager::fromArray(json::Value **in, uint32_t size)
{
    // The same as below.
    Value **out=(Value**)calloc(size+1,sizeof(Value*));
    Value *sizeValue=new Value("",JSON_VALUE_TYPE_INT32,fromInt32(size));
    out[0]=sizeValue;
    for(uint32_t i=0;i<size;i++)
        out[i+1]=in[i];
    return (char*)out;
}

char *json::ValueManager::fromMap(json::Value **in, uint32_t size)
{
    // The same as above.
    Value **out=(Value**)calloc(size+1,sizeof(Value*));
    Value *sizeValue=new Value("",JSON_VALUE_TYPE_INT32,fromInt32(size));
    out[0]=sizeValue;
    for(uint32_t i=0;i<size;i++)
        out[i+1]=in[i];
    return (char*)out;
}

char *json::ValueManager::fromString(char *in)
{
    return in;
}

bool json::ValueManager::hasKey(json::Value **array, uint32_t arraySize, char *key)
{
    for(uint32_t i=0;i<arraySize;i++)
    {
        if(strcmp(array[i]->name,key)==0)
            return true;
    }
    return false;
}

uint32_t json::ValueManager::indexOf(json::Value **array, uint32_t arraySize, char *key)
{
    for(uint32_t i=0;i<arraySize;i++)
    {
        if(strcmp(array[i]->name,key)==0)
            return i;
    }
    return json_invalidIndex;
}

json::Value *json::ValueManager::getValue(json::Value **array, uint32_t arraySize, char *key)
{
    for(uint32_t i=0;i<arraySize;i++)
    {
        if(strcmp(array[i]->name,key)==0)
            return array[i];
    }
    return 0;
}

void json::ValueManager::setValue(Value **&array, uint32_t &arraySize, Value *value)
{
    for(uint32_t i=0;i<arraySize;i++)
    {
        if(strcmp(array[i]->name,value->name)==0)
        {
            array[i]=value;
            return;
        }
    }
    arraySize++;
    Value **newArray=(Value**)malloc(arraySize*sizeof(Value*));
    if(arraySize>1)
        memcpy(newArray,array,(arraySize-1)*sizeof(Value*));
    array=newArray;
    array[arraySize-1]=value;
}

bool json::ValueManager::removeKey(json::Value **&array, uint32_t &arraySize, char *key)
{
    for(uint32_t i=0;i<arraySize;i++)
    {
        if(strcmp(array[i]->name,key)==0)
        {
            Value **newArray=(Value**)malloc((arraySize-1)*sizeof(Value*));
            uint32_t c=0;
            for(uint32_t j=0;j<arraySize;j++)
            {
                if(j==i)
                    continue;
                newArray[c++]=array[j];
            }
            arraySize--;
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
            continue;
        newArray[c++]=array[j];
    }
    arraySize--;
    array=newArray;
}


json::Value::Value()
{
    name="";
    type=0;
    value="";
}

json::Value::Value(char *_name)
{
    name=_name;
    type=0;
    value="";
}

json::Value::Value(char *_name, uint8_t _type)
{
    name=_name;
    type=_type;
    value="";
}

json::Value::Value(char *_name, uint8_t _type, char *_value)
{
    name=_name;
    type=_type;
    value=_value;
}


json::Value **json::parser::parse(char *source, uint32_t &size)
{
    uint32_t temp;
    return parse(source,size,temp);
}

json::Value **json::parser::parse(char *source, uint32_t &size, uint32_t &stoppedAt)
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
    char *thisAttrName="";\
    if(isMap)\
    {\
        "Copy the string, else all names will be that of the last element.";\
        thisAttrName=(char*)calloc(attrNameLength+1,1);\
        memcpy(thisAttrName,attrName,attrNameLength);\
    }\
    else\
        thisAttrName="";\
    if(thisType==JSON_VALUE_TYPE_STRING)\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_STRING,ValueManager::fromString(text::unescape(attrValue)));\
    else if(thisType==JSON_VALUE_TYPE_INT32)\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_INT32,ValueManager::fromInt32(text::intFromString(attrValue)));\
    else if(thisType==JSON_VALUE_TYPE_INT64)\
    {\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_INT64,ValueManager::fromInt64(text::longFromString(attrValue)));\
    }\
    else if(thisType==JSON_VALUE_TYPE_DOUBLE)\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_DOUBLE,ValueManager::fromDouble(text::doubleFromString(attrValue)));\
    else if(thisType==JSON_VALUE_TYPE_BOOL)\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_BOOL,ValueManager::fromBool(stricmp(attrValue,"true")==0));\
    else if(thisType==JSON_VALUE_TYPE_ARRAY)\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_ARRAY,ValueManager::fromArray(subarray,subarraySize));\
    else if(thisType==JSON_VALUE_TYPE_MAP)\
        v=new Value(thisAttrName,JSON_VALUE_TYPE_MAP,ValueManager::fromMap(subarray,subarraySize));\
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
    uint32_t subarraySize;

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
            if(thisChar=='[')
                isMap=false;
            else
                isAttrName=true;
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
            JSON_PARSER_ADD_ATTR;
            stoppedAt=i;
            break;
        }

        if(thisType==JSON_VALUE_TYPE_INT64&&(thisChar<48||thisChar>57)&&thisChar!='-'&&thisChar!='+') // Range of numeric values in ASCII and UTF8
            thisType=JSON_VALUE_TYPE_DOUBLE;
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
        }
        else if(thisChar==stringChar&&prevChar!='\\') // and thisType==JSON_VALUE_TYPE_STRING; !!: \\" handled in JSON_PARSER_NEXT_ITERATION
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
        return isArray?"[]":"{}";
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
        if(thisValue->name==0)
            nameLength=0;
        else
            nameLength=strlen(thisValue->name);
        if(nameLength>0)
        {
            nameStr=text::concat("\"",thisValue->name,"\":");
            io::writeRawDataToBuffer(out,nameStr,nameLength+3,pos,bufferSize); // Include the "":, but not the zero terminator
        }
        if(thisValue->type==JSON_VALUE_TYPE_STRING)
        {
            char *in=text::concat("\"",text::escape(ValueManager::getString(thisValue)),"\"");
            io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
        }
        else if(thisValue->type==JSON_VALUE_TYPE_INT32)
        {
            char *in=text::toString(ValueManager::getInt32(thisValue));
            io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
        }
        else if(thisValue->type==JSON_VALUE_TYPE_INT64)
        {
            char *in=text::longToString(ValueManager::getInt64(thisValue));
            io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
        }
        else if(thisValue->type==JSON_VALUE_TYPE_BOOL)
        {
            char *in=ValueManager::getBool(thisValue)?"true":"false";
            io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
        }
        else if(thisValue->type==JSON_VALUE_TYPE_DOUBLE)
        {
            char *in=text::doubleToString(ValueManager::getDouble(thisValue));
            io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
        }
        else if(thisValue->type==JSON_VALUE_TYPE_ARRAY||thisValue->type==JSON_VALUE_TYPE_MAP)
        {
            uint32_t tempSize;
            Value **arr=ValueManager::getArray(thisValue,tempSize);
            char *in=serialize(arr,tempSize,thisValue->type);
            uint32_t len=strlen(in);
            io::writeRawDataToBuffer(out,in,len,pos,bufferSize);
        }
    }
    io::writeRawCharToBuffer(out,isArray?']':'}',pos,bufferSize);
    io::terminateBuffer(out,pos,bufferSize);
    return out;
}
