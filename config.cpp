#include "config.h"

config_value_t config::nullValue=new config_value_deref_t();

std::vector<char *> *config::getKeys(char *source)
{
    return getKeys(source,config_allValues);
}

std::vector<char *> *config::getKeys(char *source, char *startingWith)
{
    fs_t pos=0;
    std::vector<config_key_t> *out=new std::vector<config_key_t>();
    fs_t valueCount=io::posBasedReadFsT(source,pos);
    bool allValues=strcmp(startingWith,config_allValues)==0;
    for(fs_t i=0;i<valueCount;i++)
    {
        bool zeroTerminated=io::posBasedReadUInt8(source,pos)==1;
        config_key_t key=io::posBasedReadZeroTerminatedData(source,pos);
        fs_t valueLength;
        if(zeroTerminated)
            io::posBasedReadZeroTerminatedData(source,pos);
        else
            io::posBasedReadFixedLengthData(source,pos,valueLength);
        if(!allValues&&!text::startsWith(key,startingWith)) // Needed here, we need to skip the value declaration before moving on to the next key-value-pair.
            continue;
        out->push_back(key);
    }
    return out;
}

std::vector<std::pair<config_key_t, config_value_t> *> *config::getValues(char *source)
{
    return getValues(source,config_allValues);
}

std::vector<std::pair<config_key_t, config_value_t> *> *config::getValues(char *source, char *startingWith)
{
    fs_t pos=0;
    std::vector<std::pair<config_key_t, config_value_t> *> *out=new std::vector<std::pair<config_key_t, config_value_t> *>();
    fs_t valueCount=io::posBasedReadFsT(source,pos);
    bool allValues=strcmp(startingWith,config_allValues)==0;
    for(fs_t i=0;i<valueCount;i++)
    {
        bool zeroTerminated=io::posBasedReadUInt8(source,pos)==1;
        config_key_t key=io::posBasedReadZeroTerminatedData(source,pos);
        fs_t valueLength;
        char *valueData=zeroTerminated?io::posBasedReadZeroTerminatedData(source,pos):io::posBasedReadFixedLengthData(source,pos,valueLength);
        if(!allValues&&!text::startsWith(key,startingWith)) // Needed here, we need to skip the value declaration before moving on to the next key-value-pair.
            continue;
        config_value_t valueProvider=new config_value_deref_t();
        valueProvider->valueLength=zeroTerminated?strlen(valueData):valueLength;
        valueProvider->value=valueData;
        valueProvider->zeroTerminated=zeroTerminated;
        std::pair<config_key_t, config_value_t> *pair=new std::pair<config_key_t, config_value_t>(key,valueProvider);
        out->push_back(pair);
    }
    return out;
}

config_value_t config::getValue(char *source, config_key_t key)
{
    fs_t pos=0;
    fs_t valueCount=io::posBasedReadFsT(source,pos);
    for(fs_t i=0;i<valueCount;i++)
    {
        bool zeroTerminated=io::posBasedReadUInt8(source,pos)==1;
        config_key_t thisKey=io::posBasedReadZeroTerminatedData(source,pos);
        fs_t valueLength;
        char *valueData=zeroTerminated?io::posBasedReadZeroTerminatedData(source,pos):io::posBasedReadFixedLengthData(source,pos,valueLength);
        if(stricmp(thisKey,key)!=0) // Needed here, we need to skip the value declaration before moving on to the next key-value-pair.
            continue;
        config_value_t valueProvider=new config_value_deref_t();
        valueProvider->valueLength=zeroTerminated?strlen(valueData):valueLength;
        valueProvider->value=valueData;
        valueProvider->zeroTerminated=zeroTerminated;
        return valueProvider;
    }
    return nullValue;
}

config_value_t config::getValue(std::vector<std::pair<config_key_t, config_value_t> *> *values, config_key_t key)
{
    for(fs_t i=0;i<values->size();i++)
    {
        std::pair<config_key_t, config_value_t> *pair=values->at(i);
        if(stricmp(pair->first,key)==0)
            return pair->second;
    }
    return nullValue;
}

void config::setValue(std::vector<std::pair<config_key_t, config_value_t> *> *values, config_key_t key, config_value_t value)
{
    for(fs_t i=0;i<values->size();i++)
    {
        std::pair<config_key_t, config_value_t> *pair=values->at(i);
        if(stricmp(pair->first,key)==0)
        {
            pair->second=value;
            return;
        }
    }
    std::pair<config_key_t, config_value_t> *pair=new std::pair<config_key_t, config_value_t>(key,value);
    values->push_back(pair);
}

bool config::hasKey(char *source, config_key_t key)
{
    fs_t pos=0;
    fs_t valueCount=io::posBasedReadFsT(source,pos);
    for(fs_t i=0;i<valueCount;i++)
    {
        bool zeroTerminated=io::posBasedReadUInt8(source,pos)==1;
        config_key_t thisKey=io::posBasedReadZeroTerminatedData(source,pos);
        fs_t valueLength;
        if(zeroTerminated)
            io::posBasedReadZeroTerminatedData(source,pos);
        else
            io::posBasedReadFixedLengthData(source,pos,valueLength);
        if(stricmp(thisKey,key)==0)
            return true;
    }
    return false;
}

bool config::hasKey(std::vector<std::pair<config_key_t, config_value_t> *> *values, config_key_t key)
{
    for(fs_t i=0;i<values->size();i++)
    {
        std::pair<config_key_t,config_value_t> *pair=values->at(i);
        if(stricmp(pair->first,key)==0)
            return true;
    }
    return false;
}

bool config::removeKey(std::vector<std::pair<config_key_t,config_value_t> *> *values, config_key_t key)
{
    for(fs_t i=0;i<values->size();i++)
    {
        std::pair<config_key_t,config_value_t> *pair=values->at(i);
        if(stricmp(pair->first,key)==0)
        {
            values->erase(values->begin()+i);
            return true;
        }
    }
    return false;
}

char *config::serializeConfig(std::vector<std::pair<config_key_t, config_value_t> *> *values, fs_t &length)
{
    fs_t valueCount=values->size();
    length=sizeof(fs_t); // Leave some room for the value count.
    fs_t bufferSize=CONFIG_DEFAULT_BUFFER_SIZE;
    char *buffer=(char*)calloc(bufferSize,1);
    for(fs_t i=0;i<valueCount;i++)
    {
        std::pair<config_key_t, config_value_t> *pair=values->at(i);
        config_key_t key=pair->first;
        config_value_t valueProvider=pair->second;
        io::writeUInt8ToBuffer(buffer,valueProvider->zeroTerminated?1:0,length,bufferSize);
        io::writeZeroTerminatedDataToBuffer(buffer,key,length,bufferSize);
        if(valueProvider->zeroTerminated)
            io::writeZeroTerminatedDataToBuffer(buffer,valueProvider->value,length,bufferSize);
        else
            io::writeFixedLengthDataToBuffer(buffer,valueProvider->valueLength,valueProvider->value,length,bufferSize);
    }
    fs_t zeroPos=0;
    io::writeFsT(buffer,valueCount,zeroPos);
    return buffer;
}
