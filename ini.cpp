#include "ini.h"

std::vector<char *> ini::getSections(const char *iniSource)
{
    std::vector<char*> lines=text::split(iniSource,"\n");
    std::vector<char*> out;
    char *thisLine;
    for(int i=0;i<lines.size();i++)
    {
        thisLine=text::trimStart(lines.at(i));
        if(strlen(thisLine)==0)
            continue;
        if(thisLine[0]=='[')
            out.push_back(text::substr(thisLine,1,text::indexOf(thisLine,"]")-1));
        free(thisLine);
    }
    text::freeCharArrayVectorContents(lines);
    return out;
}

bool ini::hasSection(const char *iniSource, const char *section)
{
    std::vector<char*> lines=text::split(iniSource,"\n");
    char *thisLine;
    fs_t size=lines.size();
    for(int i=0;i<size;i++)
    {
        thisLine=text::trimStart(lines.at(i));
        if(strlen(thisLine)==0)
            goto NextIteration;
        if(thisLine[0]=='[')
        {
            char *sstr=text::substr(thisLine,1,text::indexOf(thisLine,"]")-1);
            if(text::iCompare(sstr,section))
            {
                free(sstr);
                free(thisLine);
                text::freeCharArrayVectorContents(lines);
                return true;
            }
            free(sstr);
        }
        NextIteration:
        free(thisLine);
    }
    text::freeCharArrayVectorContents(lines);
    return false;
}

std::vector<char *> ini::getKeys(const char *iniSource)
{
    return getKeys(iniSource,ini_noSection);
}

std::vector<ini_key_t> ini::getKeys(const char *iniSource, const char *section)
{
    std::vector<char*> lines=text::split(iniSource,"\n");
    std::vector<ini_key_t> out;
    char *thisLine;
    char *thisSection=text::duplicateString("");
    bool lookInAllSections=strcmp(section,ini_allSections)==0;
    fs_t size=lines.size();
    for(int i=0;i<size;i++)
    {
        thisLine=text::trimStart(lines.at(i));
        if(strlen(thisLine)==0)
            goto NextIteration;
        if(thisLine[0]=='[')
        {
            if(text::iCompare(thisSection,section))
            {
                free(thisLine);
                break;
            }
            free(thisSection);
            thisSection=text::substr(thisLine,1,text::indexOf(thisLine,"]")-1);
            free(thisLine);
            continue;
        }

        if(thisLine[0]==';')
            goto NextIteration;

        if(!lookInAllSections&&!text::iCompare(thisSection,section))
            goto NextIteration;

        text_t eqIndex=text::indexOf(thisLine,"=");
        if(eqIndex==pos_notFound)
            goto NextIteration;

        out.push_back(text::substr(thisLine,0,eqIndex));
        NextIteration:
        free(thisLine);
    }
    free(thisSection);
    text::freeCharArrayVectorContents(lines);
    return out;
}

ini_value_t ini::getValue(const char *iniSource, const ini_key_t key)
{
    bool found;
    return getValue(iniSource,ini_noSection,key,found);
}

ini_value_t ini::getValue(const char *iniSource, const ini_key_t key, bool &found)
{
    return getValue(iniSource,ini_noSection,key,found);
}

char *ini::getValue(const char *iniSource, const char *section, const ini_key_t key)
{
    bool found;
    return getValue(iniSource,section,key,found);
}

ini_value_t ini::getValue(const char *iniSource, const char *section, const ini_key_t key, bool &found)
{
    found=false;
    std::vector<char*> lines=text::split(iniSource,"\n");
    char *thisLine;
    char *thisSection=text::duplicateString("");
    bool lookInAllSections=strcmp(section,ini_allSections)==0;
    fs_t size=lines.size();
    for(int i=0;i<size;i++)
    {
        thisLine=text::trimStart(lines.at(i));
        if(strlen(thisLine)==0)
            continue;
        if(thisLine[0]=='[')
        {
            if(text::iCompare(thisSection,section))
            {
                free(thisLine);
                break;
            }
            free(thisSection);
            thisSection=text::substr(thisLine,1,text::indexOf(thisLine,"]")-1);
            goto NextIteration;
        }

        if(thisLine[0]==';')
            goto NextIteration;

        if(!lookInAllSections&&!text::iCompare(thisSection,section))
            goto NextIteration;

        text_t eqIndex=text::indexOf(thisLine,"=");
        if(eqIndex==pos_notFound)
            goto NextIteration;

        char *sstr=text::substr(thisLine,0,eqIndex);
        if(text::iCompare(sstr,key))
        {
            free(sstr);
            found=true;
            char *ret = text::substr(thisLine,eqIndex+1);
            free(thisLine);
            return ret;
        }

        free(sstr);
        NextIteration:
        free(thisLine);
    }
    free(thisSection);
    return text::duplicateString("");
}

bool ini::hasKey(const char *iniSource, const ini_key_t key)
{
    return hasKey(iniSource,ini_noSection,key);
}

bool ini::hasKey(const char *iniSource, const char *section, const ini_key_t key)
{
    std::vector<char*> lines=text::split(iniSource,"\n");
    char *thisLine;
    char *thisSection=text::duplicateString("");
    bool lookInAllSections=strcmp(section,ini_allSections)==0;
    fs_t size=lines.size();
    for(int i=0;i<size;i++)
    {
        thisLine=text::trimStart(lines.at(i));
        if(strlen(thisLine)==0)
            goto NextIteration;
        if(thisLine[0]=='[')
        {
            if(thisSection==section)
            {
                free(thisLine);
                break;
            }
            free(thisSection);
            thisSection=text::substr(thisLine,1,text::indexOf(thisLine,"]")-1);
            goto NextIteration;
        }

        if(thisLine[0]==';')
            goto NextIteration;

        if(!lookInAllSections&&!text::iCompare(thisSection,section))
            goto NextIteration;

        text_t eqIndex=text::indexOf(thisLine,"=");
        if(eqIndex==pos_notFound)
            goto NextIteration;

        char *sstr=text::substr(thisLine,0,eqIndex);
        if(text::iCompare(sstr,key))
        {
            free(sstr);
            free(thisLine);
            text::freeCharArrayVectorContents(lines);
            return true;
        }
        free(sstr);
        NextIteration:
        free(thisLine);
    }
    text::freeCharArrayVectorContents(lines);
    free(thisSection);
    return false;
}

std::vector<std::pair<ini_key_t, ini_value_t> *> *ini::getValues(const char *iniSource)
{
    return getValues(iniSource,ini_noSection);
}

std::vector<std::pair<ini_key_t, ini_value_t> *> *ini::getValues(const char *iniSource, const char *section)
{
    std::vector<char*> lines=text::split(iniSource,"\n");
    std::vector<std::pair<ini_key_t,ini_value_t> *> *out=new std::vector<std::pair<ini_key_t,ini_value_t> *>();
    char *thisLine;
    char *thisSection=text::duplicateString("");
    bool lookInAllSections=strcmp(section,ini_allSections)==0;
    fs_t size=lines.size();
    for(int i=0;i<size;i++)
    {
        thisLine=text::trimStart(lines.at(i));
        if(strlen(thisLine)==0)
            goto NextIteration;
        if(thisLine[0]=='[')
        {
            if(text::iCompare(thisSection,section))
            {
                free(thisLine);
                break;
            }
            free(thisSection);
            thisSection=text::substr(thisLine,1,text::indexOf(thisLine,"]")-1);
            goto NextIteration;
        }

        if(thisLine[0]==';')
            goto NextIteration;

        if(!lookInAllSections&&!text::iCompare(thisSection,section))
            goto NextIteration;

        text_t eqIndex=text::indexOf(thisLine,"=");
        if(eqIndex==pos_notFound)
            goto NextIteration;

        char *key=text::substr(thisLine,0,eqIndex);
        char *value=text::substr(thisLine,eqIndex+1);
        std::pair<ini_key_t,ini_value_t> *keyValuePair=
                new std::pair<ini_key_t,ini_value_t>(
                    text::trim(key),
                    text::trim(value)
                    );
        free(key);
        free(value);
        out->push_back(keyValuePair);
        NextIteration:
        free(thisLine);
    }
    free(thisSection);
    text::freeCharArrayVectorContents(lines);
    return out;
}

void ini::setValue(std::vector<std::pair<ini_key_t, ini_value_t> *> *values, const ini_key_t key, const ini_value_t value)
{
    for(int i=0;i<values->size();i++)
    {
        std::pair<ini_key_t, ini_value_t> *pair=values->at(i);
        if(text::iCompare(pair->first,key))
        {
            free(pair->second);
            pair->second=text::duplicateString(value);
            return;
        }
    }
    std::pair<ini_key_t, ini_value_t> *newPair=new std::pair<ini_key_t, ini_value_t>(text::duplicateString(key),text::duplicateString(value));
    values->push_back(newPair);
}

bool ini::hasKey(std::vector<std::pair<ini_key_t, ini_value_t> *> *values, const ini_key_t key)
{
    fs_t size=values->size();
    for(int i=0;i<size;i++)
    {
        std::pair<ini_key_t, ini_value_t> *pair=values->at(i);
        if(text::iCompare(pair->first,key))
            return true;
    }
    return false;
}

bool ini::removeKey(std::vector<std::pair<ini_key_t, ini_value_t> *> *values, const ini_key_t key)
{
    fs_t size=values->size();
    for(int i=0;i<size;i++)
    {
        std::pair<ini_key_t, ini_value_t> *pair=values->at(i);
        if(text::iCompare(pair->first,key))
        {
            free(pair->first);
            free(pair->second);
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

char *ini::serializeSection(std::vector<std::pair<ini_key_t, ini_value_t> *> *values, const char *sectionName)
{
    fs_t bufferSize=DEFAULT_INI_BUFFER_SIZE;
    fs_t pos=0;
    char *out=(char*)calloc(bufferSize,1);
    if(strcmp(sectionName,ini_noSection)!=0)
    {
        char *in=text::concat("[",sectionName,"]\n");
        io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
        free(in);
    }
    for(int i=0;i<values->size();i++)
    {
        std::pair<ini_key_t, ini_value_t> *pair=values->at(i);
        char *in=text::concat(i>0?"\n":"",pair->first,"=",pair->second);
        io::writeRawDataToBuffer(out,in,strlen(in),pos,bufferSize);
        free(in);
    }
    io::terminateBuffer(out,pos,bufferSize);
    return out;
}

void ini::freeAndDeleteCharArrayValueVector(std::vector<std::pair<char *,char *> *> *values)
{
    fs_t count=values->size();
    for(fs_t i=0;i<count;i++)
    {
        std::pair<ini_key_t,ini_value_t> *p=values->at(i);
        free(p->first);
        free(p->second);
        delete p;
    }
    delete values;
}
