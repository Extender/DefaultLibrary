#include "ini.h"

std::vector<char *> ini::getSections(char *iniSource)
{
    std::vector<char*> lines=text::split(iniSource,"\n");
    std::vector<char*> out;
    char *thisLine;
    for(int i=0;i<lines.size();i++)
    {
        thisLine=text::trimStart(lines.at(i));
        if(thisLine=="")
            continue;
        if(thisLine[0]=='[')
            out.push_back(text::substr(thisLine,1,text::indexOf(thisLine,"]")-1));
    }
    return out;
}

bool ini::hasSection(char *iniSource, char *section)
{
    std::vector<char*> lines=text::split(iniSource,"\n");
    std::vector<char*> out;
    char *thisLine;
    for(int i=0;i<lines.size();i++)
    {
        thisLine=text::trimStart(lines.at(i));
        if(thisLine=="")
            continue;
        if(thisLine[0]=='[')
        {
            if(stricmp(text::substr(thisLine,1,text::indexOf(thisLine,"]")-1),section)==0)
                return true;
        }
    }
    return false;
}

std::vector<char *> ini::getKeys(char *iniSource)
{
    return getKeys(iniSource,ini_noSection);
}

std::vector<ini_key_t> ini::getKeys(char *iniSource, char *section)
{
    std::vector<char*> lines=text::split(iniSource,"\n");
    std::vector<ini_key_t> out;
    char *thisLine;
    char *thisSection="";
    bool lookInAllSections=strcmp(section,ini_allSections)==0;
    for(int i=0;i<lines.size();i++)
    {
        thisLine=text::trimStart(lines.at(i));
        if(thisLine=="")
            continue;
        if(thisLine[0]=='[')
        {
            if(thisSection==section)
                break;
            thisSection=text::substr(thisLine,1,text::indexOf(thisLine,"]")-1);
            continue;
        }
        if(thisLine[0]==';')
            continue;
        if(!lookInAllSections&&stricmp(thisSection,section)!=0)
            continue;
        text_t eqIndex=text::indexOf(thisLine,"=");
        if(eqIndex==pos_notFound)
            continue;
        out.push_back(text::substr(thisLine,0,eqIndex));
    }
    return out;
}

ini_value_t ini::getValue(char *iniSource, ini_key_t key)
{
    bool found;
    return getValue(iniSource,ini_noSection,key,found);
}

ini_value_t ini::getValue(char *iniSource, ini_key_t key, bool &found)
{
    return getValue(iniSource,ini_noSection,key,found);
}

char *ini::getValue(char *iniSource, char *section, char *key)
{
    bool found;
    return getValue(iniSource,section,key,found);
}

ini_value_t ini::getValue(char *iniSource, char *section, ini_key_t key, bool &found)
{
    found=false;
    std::vector<char*> lines=text::split(iniSource,"\n");
    char *thisLine;
    char *thisSection="";
    bool lookInAllSections=strcmp(section,ini_allSections)==0;
    for(int i=0;i<lines.size();i++)
    {
        thisLine=text::trimStart(lines.at(i));
        if(thisLine=="")
            continue;
        if(thisLine[0]=='[')
        {
            if(thisSection==section)
                break;
            thisSection=text::substr(thisLine,1,text::indexOf(thisLine,"]")-1);
            continue;
        }
        if(thisLine[0]==';')
            continue;
        if(!lookInAllSections&&stricmp(thisSection,section)!=0)
            continue;
        text_t eqIndex=text::indexOf(thisLine,"=");
        if(eqIndex==pos_notFound)
            continue;
        if(stricmp(text::substr(thisLine,0,eqIndex),key)==0)
        {
            found=true;
            return text::substr(thisLine,eqIndex+1);
        }
    }
    return "";
}

bool ini::hasKey(char *iniSource, ini_key_t key)
{
    return hasKey(iniSource,ini_noSection,key);
}

bool ini::hasKey(char *iniSource, char *section, ini_key_t key)
{
    std::vector<char*> lines=text::split(iniSource,"\n");
    char *thisLine;
    char *thisSection="";
    bool lookInAllSections=strcmp(section,ini_allSections)==0;
    for(int i=0;i<lines.size();i++)
    {
        thisLine=text::trimStart(lines.at(i));
        if(thisLine=="")
            continue;
        if(thisLine[0]=='[')
        {
            if(thisSection==section)
                break;
            thisSection=text::substr(thisLine,1,text::indexOf(thisLine,"]")-1);
            continue;
        }
        if(thisLine[0]==';')
            continue;
        if(!lookInAllSections&&stricmp(thisSection,section)!=0)
            continue;
        text_t eqIndex=text::indexOf(thisLine,"=");
        if(eqIndex==pos_notFound)
            continue;
        if(stricmp(text::substr(thisLine,0,eqIndex),key)==0)
            return true;
    }
    return false;
}

std::vector<std::pair<ini_key_t, ini_value_t> *> *ini::getValues(char *iniSource)
{
    return getValues(iniSource,ini_noSection);
}

std::vector<std::pair<ini_key_t, ini_value_t> *> *ini::getValues(char *iniSource, char *section)
{
    std::vector<char*> lines=text::split(iniSource,"\n");
    std::vector<std::pair<ini_key_t,ini_value_t> *> *out;
    char *thisLine;
    char *thisSection="";
    bool lookInAllSections=strcmp(section,ini_allSections)==0;
    for(int i=0;i<lines.size();i++)
    {
        thisLine=text::trimStart(lines.at(i));
        if(thisLine=="")
            continue;
        if(thisLine[0]=='[')
        {
            if(thisSection==section)
                break;
            thisSection=text::substr(thisLine,1,text::indexOf(thisLine,"]")-1);
            continue;
        }
        if(thisLine[0]==';')
            continue;
        if(!lookInAllSections&&stricmp(thisSection,section)!=0)
            continue;
        text_t eqIndex=text::indexOf(thisLine,"=");
        if(eqIndex==pos_notFound)
            continue;
        std::pair<ini_key_t,ini_value_t> *keyValuePair=
                new std::pair<ini_key_t,ini_value_t>(
                    text::substr(thisLine,0,eqIndex),
                    text::substr(thisLine,eqIndex+1)
                    );
        out->push_back(keyValuePair);
    }
    return out;
}

void ini::setValue(std::vector<std::pair<ini_key_t, ini_value_t> *> *values, ini_key_t key, ini_value_t value)
{
    for(int i=0;i<values->size();i++)
    {
        std::pair<ini_key_t, ini_value_t> *pair=values->at(i);
        if(stricmp(pair->first,key)==0)
        {
            pair->second=value;
            return;
        }
    }
    std::pair<ini_key_t, ini_value_t> *newPair=new std::pair<ini_key_t, ini_value_t>(key,value);
    values->push_back(newPair);
}

bool ini::hasKey(std::vector<std::pair<ini_key_t, ini_value_t> *> *values, ini_key_t key)
{
    for(int i=0;i<values->size();i++)
    {
        std::pair<ini_key_t, ini_value_t> *pair=values->at(i);
        if(stricmp(pair->first,key)==0)
        {
            return true;
        }
    }
    return false;
}

bool ini::removeKey(std::vector<std::pair<ini_key_t, ini_value_t> *> *values, ini_key_t key)
{
    for(int i=0;i<values->size();i++)
    {
        std::pair<ini_key_t, ini_value_t> *pair=values->at(i);
        if(stricmp(pair->first,key)==0)
        {
            values->erase(values->begin()+i);
            return true;
        }
    }
    return false;
}

char *ini::serializeSection(std::vector<std::pair<ini_key_t, ini_value_t> *> *values)
{
    return serializeSection(values,ini_noSection);
}

char *ini::serializeSection(std::vector<std::pair<ini_key_t, ini_value_t> *> *values, char *sectionName)
{
    fs_t bufferSize=DEFAULT_INI_BUFFER_SIZE;
    fs_t pos=0;
    char *out=(char*)calloc(bufferSize,1);
    if(strcmp(sectionName,ini_noSection)!=0)
    {
        char *in=text::concat("[",sectionName,"]\n");
        io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
    }
    for(int i=0;i<values->size();i++)
    {
        std::pair<ini_key_t, ini_value_t> *pair=values->at(i);
        char *in=text::concat(i>0?"\n":"",pair->first,"=",pair->second);
        io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
    }
    io::terminateBuffer(out,pos,bufferSize);
    return out;
}

