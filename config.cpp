#include "config.h"

config_value_t config::nullValue=new config_value_deref_t(0,0,false);

config_pair_t config::makeKeyValuePair(config_key_t key, config_value_t value)
{
    return new config_pair_deref_t(key,value);
}

config_pair_t config::makeKeyValuePair(config_key_t key, int32_t value)
{
    return new config_pair_deref_t(key,ConfigValue::fromInt32(value));
}

config_pair_t config::makeKeyValuePair(config_key_t key, int64_t value)
{
    return new config_pair_deref_t(key,ConfigValue::fromInt64(value));
}

config_pair_t config::makeKeyValuePair(config_key_t key, double value)
{
    return new config_pair_deref_t(key,ConfigValue::fromDouble(value));
}

config_pair_t config::makeKeyValuePair(config_key_t key, bool value)
{
    return new config_pair_deref_t(key,ConfigValue::fromBool(value));
}

config_pair_t config::makeKeyValuePair(config_key_t key, char *value)
{
    return new config_pair_deref_t(key,ConfigValue::fromString(value));
}

config_pair_t config::makeKeyValuePair(config_key_t key, const char *value)
{
    return new config_pair_deref_t(key,ConfigValue::fromString(value));
}

config_pair_t config::makeKeyValuePair(const char *key, const char *value)
{
    return new config_pair_deref_t(text::duplicateString(key),new ConfigValue(CONFIG_VALUE_TYPE_STRING,strlen(value),text::duplicateString(value)));
}

config_pair_t config::makeKeyValuePair(const char *key, config_value_t value)
{
    return new config_pair_deref_t(text::duplicateString(key),value);
}

config_pair_t config::makeKeyValuePair(const char *key, int32_t value)
{
    return new config_pair_deref_t(text::duplicateString(key),ConfigValue::fromInt32(value));
}

config_pair_t config::makeKeyValuePair(const char *key, int64_t value)
{
    return new config_pair_deref_t(text::duplicateString(key),ConfigValue::fromInt64(value));
}

config_pair_t config::makeKeyValuePair(const char *key, double value)
{
    return new config_pair_deref_t(text::duplicateString(key),ConfigValue::fromDouble(value));
}

config_pair_t config::makeKeyValuePair(const char *key, bool value)
{
    return new config_pair_deref_t(text::duplicateString(key),ConfigValue::fromBool(value));
}

config_pair_t config::makeKeyValuePair(const char *key, char *value)
{
    // Do not duplicate value.
    return new config_pair_deref_t(text::duplicateString(key),new ConfigValue(CONFIG_VALUE_TYPE_STRING,strlen(value),value));
}

std::vector<config_key_t> *config::getKeys(char *source)
{
    return getKeys(source,config_allValues);
}

std::vector<config_key_t> *config::getKeys(char *source, const char *startingWith)
{
    fs_t pos=0;
    std::vector<config_key_t> *out=new std::vector<config_key_t>();
    fs_t valueCount=(fs_t)io::posBasedReadUInt64(source,pos);
    bool allValues=strcmp(startingWith,config_allValues)==0;
    for(fs_t i=0;i<valueCount;i++)
    {
        uint8_t type=io::posBasedReadUInt8(source,pos);
        config_key_t key=io::posBasedReadZeroTerminatedData(source,pos);

        if(type==CONFIG_VALUE_TYPE_INT32)
            pos+=sizeof(int32_t);
        else if(type==CONFIG_VALUE_TYPE_INT64)
            pos+=sizeof(int64_t);
        else if(type==CONFIG_VALUE_TYPE_DOUBLE)
            pos+=sizeof(int64_t)+sizeof(int32_t);
        else if(type==CONFIG_VALUE_TYPE_BOOL)
            pos+=sizeof(uint8_t);
        else if(type==CONFIG_VALUE_TYPE_STRING)
            pos+=strlen(source+pos)+1;
        else if(type==CONFIG_VALUE_TYPE_DATA)
            pos+=io::peekUInt32(source,pos)+sizeof(uint32_t);

        if(!allValues&&!text::startsWith(key,startingWith)) // Needed here, we need to skip the value declaration before moving on to the next key-value-pair.
            continue;
        out->push_back(key);
    }
    return out;
}

std::vector<config_pair_t> *config::getValues(char *source)
{
    return getValues(source,config_allValues);
}

std::vector<config_pair_t> *config::getValues(char *source, const char *startingWith)
{
    fs_t pos=0;
    std::vector<config_pair_t> *out=new std::vector<config_pair_t>();
    fs_t valueCount=(fs_t)io::posBasedReadUInt64(source,pos);
    bool allValues=strcmp(startingWith,config_allValues)==0;
    for(fs_t i=0;i<valueCount;i++)
    {
        uint8_t type=io::posBasedReadUInt8(source,pos);
        config_key_t key=io::posBasedReadZeroTerminatedData(source,pos);
        fs_t valueLength;

        if(type==CONFIG_VALUE_TYPE_INT32)
            valueLength=sizeof(int32_t);
        else if(type==CONFIG_VALUE_TYPE_INT64)
            valueLength=sizeof(int64_t);
        else if(type==CONFIG_VALUE_TYPE_DOUBLE)
            valueLength=sizeof(int64_t)+sizeof(int32_t);
        else if(type==CONFIG_VALUE_TYPE_BOOL)
            valueLength=sizeof(uint8_t);
        else if(type==CONFIG_VALUE_TYPE_STRING)
            valueLength=strlen(source+pos)+1/*Final \0*/;
        else if(type==CONFIG_VALUE_TYPE_DATA)
        {
            valueLength=io::peekUInt32(source,pos);
            pos+=sizeof(uint32_t);
        }

        if(!allValues&&!text::startsWith(key,startingWith)) // Needed here, we need to skip the value declaration before moving on to the next key-value-pair.
        {
            pos+=valueLength;
            continue;
        }

        char *value=(char*)malloc(valueLength);
        memcpy(value,source+pos,valueLength);
        pos+=valueLength;

        config_value_t valueProvider=new config_value_deref_t(type,valueLength,value);
        config_pair_t pair=new config_pair_deref_t(key,valueProvider);
        out->push_back(pair);
    }
    return out;
}

config_value_t config::getValue(char *source, const config_key_t key)
{
    fs_t pos=0;
    fs_t valueCount=(fs_t)io::posBasedReadUInt64(source,pos);
    for(fs_t i=0;i<valueCount;i++)
    {
        uint8_t type=io::posBasedReadUInt8(source,pos);
        config_key_t thisKey=io::posBasedReadZeroTerminatedData(source,pos);
        fs_t valueLength;

        if(type==CONFIG_VALUE_TYPE_INT32)
            valueLength=sizeof(int32_t);
        else if(type==CONFIG_VALUE_TYPE_INT64)
            valueLength=sizeof(int64_t);
        else if(type==CONFIG_VALUE_TYPE_DOUBLE)
            valueLength=sizeof(int64_t)+sizeof(int32_t);
        else if(type==CONFIG_VALUE_TYPE_BOOL)
            valueLength=sizeof(uint8_t);
        else if(type==CONFIG_VALUE_TYPE_STRING)
            valueLength=strlen(source+pos)+1/*Final \0*/;
        else if(type==CONFIG_VALUE_TYPE_DATA)
        {
            valueLength=io::peekUInt32(source,pos);
            pos+=sizeof(uint32_t);
        }

        pos+=valueLength;

        if(strcmp(thisKey,key)!=0) // Needed here, we need to skip the value declaration before moving on to the next key-value-pair.
            continue;

        char *value=(char*)malloc(valueLength);
        memcpy(value,source+pos,valueLength);

        config_value_t valueProvider=new config_value_deref_t(type,valueLength,value);
        return valueProvider;
    }
    return nullValue;
}

config_value_t config::getValue(std::vector<config_pair_t> *values, const config_key_t key)
{
    for(fs_t i=0;i<values->size();i++)
    {
        config_pair_t pair=values->at(i);
        if(strcmp(pair->first,key)==0)
            return pair->second;
    }
    return nullValue;
}

void config::setValue(std::vector<config_pair_t> *values, const config_key_t key, config_value_t value)
{
    for(fs_t i=0;i<values->size();i++)
    {
        config_pair_t pair=values->at(i);
        if(strcmp(pair->first,key)==0)
        {
            pair->second=value;
            return;
        }
    }
    config_pair_t pair=new std::pair<config_key_t, config_value_t>(text::duplicateString(key),value);
    values->push_back(pair);
}

bool config::hasKey(char *source, const config_key_t key)
{
    fs_t pos=0;
    fs_t valueCount=(fs_t)io::posBasedReadUInt64(source,pos);
    for(fs_t i=0;i<valueCount;i++)
    {
        uint8_t type=io::posBasedReadUInt8(source,pos);
        config_key_t thisKey=io::posBasedReadZeroTerminatedData(source,pos);

        if(type==CONFIG_VALUE_TYPE_INT32)
            pos+=sizeof(int32_t);
        else if(type==CONFIG_VALUE_TYPE_INT64)
            pos+=sizeof(int64_t);
        else if(type==CONFIG_VALUE_TYPE_DOUBLE)
            pos+=sizeof(int64_t)+sizeof(int32_t);
        else if(type==CONFIG_VALUE_TYPE_BOOL)
            pos+=sizeof(uint8_t);
        else if(type==CONFIG_VALUE_TYPE_STRING)
            pos+=strlen(source+pos)+1;
        else if(type==CONFIG_VALUE_TYPE_DATA)
            pos+=io::peekUInt32(source,pos)+sizeof(uint32_t);

        // Needed here, we need to skip the value declaration before moving on to the next key-value-pair.
        if(strcmp(thisKey,key)==0)
            return true;
    }
    return false;
}

bool config::hasKey(std::vector<config_pair_t> *values, const config_key_t key)
{
    for(fs_t i=0;i<values->size();i++)
    {
        std::pair<config_key_t,config_value_t> *pair=values->at(i);
        if(strcmp(pair->first,key)==0)
            return true;
    }
    return false;
}

bool config::removeKey(std::vector<std::pair<config_key_t,config_value_t> *> *values, const config_key_t key)
{
    for(fs_t i=0;i<values->size();i++)
    {
        std::pair<config_key_t,config_value_t> *pair=values->at(i);
        if(strcmp(pair->first,key)==0)
        {
            values->erase(values->begin()+i);
            free(pair->first);
            delete pair->second;
            delete pair;
            return true;
        }
    }
    return false;
}

char *config::serializeConfig(std::vector<config_pair_t> *values, fs_t &length)
{
    fs_t valueCount=values->size();
    length=sizeof(uint64_t); // Leave room for the value count.
    fs_t bufferSize=CONFIG_DEFAULT_BUFFER_SIZE;
    char *buffer=(char*)calloc(bufferSize,1);
    for(fs_t i=0;i<valueCount;i++)
    {
        config_pair_t pair=values->at(i);
        config_key_t key=pair->first;
        config_value_t valueProvider=pair->second;
        uint8_t type=valueProvider->type;
        io::writeUInt8ToBuffer(buffer,type,length,bufferSize);
        io::writeZeroTerminatedDataToBuffer(buffer,key,length,bufferSize);
        size_t valueLength=valueProvider->getValueLength();
        if(type==CONFIG_VALUE_TYPE_DATA)
            io::writeUInt32ToBuffer(buffer,valueLength,length,bufferSize); // Write data length
        io::writeRawDataToBuffer(buffer,valueProvider->value,valueLength+(type==CONFIG_VALUE_TYPE_STRING?1/*Final \0*/:0),length,bufferSize);
    }
    io::putUInt64(buffer,valueCount,0);
    return buffer;
}

void config::freeValueArray(std::vector<config_pair_t> *values)
{
    fs_t size=values->size();
    for(fs_t i=0;i<size;i++)
    {
        config_pair_t pair=values->at(i);
        free(pair->first);
        delete pair->second;
        delete pair;
    }
    delete values;
}

ConfigValue::ConfigValue(uint8_t _type, fs_t _valueLengthOrZero, char *_value)
{
    type=_type;
    valueLength=_valueLengthOrZero;
    value=_value;
}

ConfigValue::ConfigValue(const char *_value)
{
    type=CONFIG_VALUE_TYPE_STRING;
    valueLength=strlen(_value);
    value=text::duplicateString(_value);
}

ConfigValue::~ConfigValue()
{
    free(value);
}

int32_t ConfigValue::toInt32()
{
    if(type!=CONFIG_VALUE_TYPE_INT32)
    {
        if(type==CONFIG_VALUE_TYPE_STRING)
        {
            char *str=this->toString();
            int32_t out=text::intFromString(str);
            free(str);
            return out;
        }
        else if(type==CONFIG_VALUE_TYPE_INT64)
            return -1;
        else if(type==CONFIG_VALUE_TYPE_DOUBLE)
            return text::round(this->toDouble());
    }

    uint32_t out=(uint8_t)value[0];
    out|=(uint32_t)((uint8_t)value[1])<<8;
    out|=(uint32_t)((uint8_t)value[2])<<16;
    out|=(uint32_t)((uint8_t)value[3])<<24;
    return out;
}

int64_t ConfigValue::toInt64()
{
    if(type!=CONFIG_VALUE_TYPE_INT64)
    {
        if(type==CONFIG_VALUE_TYPE_STRING)
        {
            char *str=this->toString();
            int64_t out=text::longFromString(str);
            free(str);
            return out;
        }
        else if(type==CONFIG_VALUE_TYPE_INT32)
            return (int64_t)this->toInt32();
        else if(type==CONFIG_VALUE_TYPE_DOUBLE)
            return text::roundl(this->toDouble());
    }

    uint64_t out=(uint8_t)value[0];
    out|=(uint64_t)((uint8_t)value[1])<<8;
    out|=(uint64_t)((uint8_t)value[2])<<16;
    out|=(uint64_t)((uint8_t)value[3])<<24;
    out|=(uint64_t)((uint8_t)value[4])<<32;
    out|=(uint64_t)((uint8_t)value[5])<<40;
    out|=(uint64_t)((uint8_t)value[6])<<48;
    out|=(uint64_t)((uint8_t)value[7])<<56;
    return out;
}

double ConfigValue::toDouble()
{
    if(type!=CONFIG_VALUE_TYPE_DOUBLE)
    {
        if(type==CONFIG_VALUE_TYPE_STRING)
        {
            char *str=this->toString();
            double out=text::doubleFromString(str);
            free(str);
            return out;
        }
        else if(type==CONFIG_VALUE_TYPE_INT32)
            return (double)this->toInt32();
        else if(type==CONFIG_VALUE_TYPE_INT64)
            return (double)this->toInt64();
        else if(type==CONFIG_VALUE_TYPE_BOOL)
            return this->toBool()?1.0:0.0;
    }

    int64_t base=io::peekUInt64(value,0);
    int32_t div=io::peekUInt32(value,sizeof(uint64_t));
    return (double)base/pow((double)10,(int32_t)div);
}

bool ConfigValue::toBool()
{
    if(type!=CONFIG_VALUE_TYPE_BOOL)
    {
        if(type==CONFIG_VALUE_TYPE_STRING)
        {
            char *str=this->toString();
            bool out=text::iCompare(str,"1")||text::iCompare(str,"true");
            free(str);
            return out;
        }
        else if(type==CONFIG_VALUE_TYPE_INT32)
            return this->toInt32()>0;
        else if(type==CONFIG_VALUE_TYPE_INT64)
            return this->toInt64()>0;
        else if(type==CONFIG_VALUE_TYPE_DOUBLE)
            return this->toDouble()>0.0;
    }

    return *(value)==1;
}

char *ConfigValue::toString()
{
    if(type!=CONFIG_VALUE_TYPE_STRING)
    {
        if(type==CONFIG_VALUE_TYPE_INT32)
            return text::intToString(this->toInt32());
        else if(type==CONFIG_VALUE_TYPE_INT64)
            return text::longToString(this->toInt64());
        else if(type==CONFIG_VALUE_TYPE_DOUBLE)
            return text::doubleToString(this->toDouble());
        else if(type==CONFIG_VALUE_TYPE_BOOL)
            return text::duplicateString(this->toBool()?"true":"false");

        return text::duplicateString("");
    }

    return text::duplicateString(value);
}

size_t ConfigValue::getValueLength()
{
    if(type==CONFIG_VALUE_TYPE_INT32)
        return sizeof(int32_t);
    else if(type==CONFIG_VALUE_TYPE_INT64)
        return sizeof(int64_t);
    else if(type==CONFIG_VALUE_TYPE_DOUBLE)
        return sizeof(int32_t)+sizeof(int64_t);
    else if(type==CONFIG_VALUE_TYPE_BOOL)
        return sizeof(uint8_t);
    else if(type==CONFIG_VALUE_TYPE_STRING)
        return strlen(value);
    return valueLength; // CONFIG_VALUE_TYPE_DATA
}

ConfigValue *ConfigValue::fromInt32(int32_t in)
{
    char *data=(char*)malloc(sizeof(uint32_t));
    data[0]=(uint8_t)in;
    data[1]=(uint8_t)(in>>8);
    data[2]=(uint8_t)(in>>16);
    data[3]=(uint8_t)(in>>24);
    return new ConfigValue(CONFIG_VALUE_TYPE_INT32,0,data);
}

ConfigValue *ConfigValue::fromInt64(int64_t in)
{
    char *data=(char*)malloc(sizeof(uint64_t));
    data[0]=(uint8_t)in;
    data[1]=(uint8_t)(in>>8);
    data[2]=(uint8_t)(in>>16);
    data[3]=(uint8_t)(in>>24);
    data[4]=(uint8_t)(in>>32);
    data[5]=(uint8_t)(in>>40);
    data[6]=(uint8_t)(in>>48);
    data[7]=(uint8_t)(in>>56);
    return new ConfigValue(CONFIG_VALUE_TYPE_INT64,0,data);
}

ConfigValue *ConfigValue::fromDouble(double in)
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
    char *data=(char*)calloc(sizeof(uint64_t)+sizeof(uint32_t),1); // Correct order.
    data[0]=(uint8_t)base;
    data[1]=(uint8_t)(base>>8);
    data[2]=(uint8_t)(base>>16);
    data[3]=(uint8_t)(base>>24);
    data[4]=(uint8_t)(base>>32);
    data[5]=(uint8_t)(base>>40);
    data[6]=(uint8_t)(base>>48);
    data[7]=(uint8_t)(base>>56);

    data[8]=(uint8_t)div;
    data[9]=(uint8_t)(div>>8);
    data[10]=(uint8_t)(div>>16);
    data[11]=(uint8_t)(div>>24);

    return new ConfigValue(CONFIG_VALUE_TYPE_DOUBLE,0,data);
}

ConfigValue *ConfigValue::fromBool(bool in)
{
    char *data=(char*)malloc(1);
    data[0]=in?1:0;
    return new ConfigValue(CONFIG_VALUE_TYPE_BOOL,0,data);
}

ConfigValue *ConfigValue::fromString(char *in)
{
    // Duplicate non-const string, too
    return new ConfigValue(CONFIG_VALUE_TYPE_STRING,0,text::duplicateString(in));
}

ConfigValue *ConfigValue::fromString(const char *in)
{
    return new ConfigValue(CONFIG_VALUE_TYPE_STRING,0,text::duplicateString(in));
}

ConfigValue *ConfigValue::fromRawData(char *in, fs_t size)
{
    return new ConfigValue(CONFIG_VALUE_TYPE_DATA,size,in);
}
