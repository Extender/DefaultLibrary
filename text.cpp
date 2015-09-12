#include "text.h"

int32_t text::int32Pow(int32_t base, int32_t exp)
{
    int32_t out=1;
    for(int32_t i=0;i<exp;i++)
        out=out*base;
    return out;
}

int64_t text::int64Pow(int64_t base, int64_t exp)
{
    int64_t out=1;
    for(int64_t i=0;i<exp;i++)
        out=out*base;
    return out;
}

uint32_t text::uint32Pow(uint32_t base, uint32_t exp)
{
    uint32_t out=1;
    for(uint32_t i=0;i<exp;i++)
        out=out*base;
    return out;
}

uint64_t text::uint64Pow(uint64_t base, uint64_t exp)
{
    uint64_t out=1;
    for(uint64_t i=0;i<exp;i++)
        out=out*base;
    return out;
}

char *text::mkstr(uint32_t length)
{
    char *out=(char*)malloc(length+1); // Zero-terminator
    out[length]=0;
    return out;
}

wchar_t *text::mkwstr(text_t length)
{
    wchar_t *out=(wchar_t*)malloc((length+1)*sizeof(wchar_t)); // Zero-terminator
    out[length]=0;
    return out;
}

char *text::cloneString(char *in)
{
    text_t len=strlen(in);
    char *out=(char*)mkstr(len);
    memcpy(out,in,len);
    return out;
}

char *text::concat(char *part1, char *part2)
{
    text_t part1Len=strlen(part1);
    char *out=mkstr(part1Len+strlen(part2));
    strcpy(out,part1);
    strcpy(out+part1Len,part2);
    return out;
}

char *text::concat(char *part1, char *part2, char *part3)
{
    text_t part1Len=strlen(part1);
    text_t part2Len=strlen(part2);
    char *out=mkstr(part1Len+part2Len+strlen(part3));
    strcpy(out,part1);
    strcpy(out+part1Len,part2);
    strcpy(out+part1Len+part2Len,part3);
    return out;
}

char *text::concat(char *part1, char *part2, char *part3, char *part4)
{
    text_t part1Len=strlen(part1);
    text_t part2Len=strlen(part2);
    text_t part3Len=strlen(part3);
    char *out=mkstr(part1Len+part2Len+part3Len+strlen(part4));
    strcpy(out,part1);
    strcpy(out+part1Len,part2);
    strcpy(out+part1Len+part2Len,part3);
    strcpy(out+part1Len+part2Len+part3Len,part4);
    return out;
}

char *text::concat(char *part1, char *part2, char *part3, char *part4, char *part5)
{
    text_t part1Len=strlen(part1);
    text_t part2Len=strlen(part2);
    text_t part3Len=strlen(part3);
    text_t part4Len=strlen(part4);
    char *out=mkstr(part1Len+part2Len+part3Len+part4Len+strlen(part5));
    strcpy(out,part1);
    strcpy(out+part1Len,part2);
    strcpy(out+part1Len+part2Len,part3);
    strcpy(out+part1Len+part2Len+part3Len,part4);
    strcpy(out+part1Len+part2Len+part3Len+part4Len,part5);
    return out;
}

char *text::concat(char *part1, char *part2, char *part3, char *part4, char *part5, char *part6)
{
    text_t part1Len=strlen(part1);
    text_t part2Len=strlen(part2);
    text_t part3Len=strlen(part3);
    text_t part4Len=strlen(part4);
    text_t part5Len=strlen(part5);
    char *out=mkstr(part1Len+part2Len+part3Len+part4Len+part5Len+strlen(part6));
    strcpy(out,part1);
    strcpy(out+part1Len,part2);
    strcpy(out+part1Len+part2Len,part3);
    strcpy(out+part1Len+part2Len+part3Len,part4);
    strcpy(out+part1Len+part2Len+part3Len+part4Len,part5);
    strcpy(out+part1Len+part2Len+part3Len+part4Len+part5Len,part6);
    return out;
}

char *text::concat(char *part1, char *part2, char *part3, char *part4, char *part5, char *part6, char *part7)
{
    text_t part1Len=strlen(part1);
    text_t part2Len=strlen(part2);
    text_t part3Len=strlen(part3);
    text_t part4Len=strlen(part4);
    text_t part5Len=strlen(part5);
    text_t part6Len=strlen(part6);
    char *out=mkstr(part1Len+part2Len+part3Len+part4Len+part5Len+part6Len+strlen(part7));
    strcpy(out,part1);
    strcpy(out+part1Len,part2);
    strcpy(out+part1Len+part2Len,part3);
    strcpy(out+part1Len+part2Len+part3Len,part4);
    strcpy(out+part1Len+part2Len+part3Len+part4Len,part5);
    strcpy(out+part1Len+part2Len+part3Len+part4Len+part5Len,part6);
    strcpy(out+part1Len+part2Len+part3Len+part4Len+part5Len+part6Len,part7);
    return out;
}

char *text::concat(char *part1, char *part2, char *part3, char *part4, char *part5, char *part6, char *part7, char *part8)
{
    text_t part1Len=strlen(part1);
    text_t part2Len=strlen(part2);
    text_t part3Len=strlen(part3);
    text_t part4Len=strlen(part4);
    text_t part5Len=strlen(part5);
    text_t part6Len=strlen(part6);
    text_t part7Len=strlen(part7);
    char *out=mkstr(part1Len+part2Len+part3Len+part4Len+part5Len+part6Len+part7Len+strlen(part8));
    strcpy(out,part1);
    strcpy(out+part1Len,part2);
    strcpy(out+part1Len+part2Len,part3);
    strcpy(out+part1Len+part2Len+part3Len,part4);
    strcpy(out+part1Len+part2Len+part3Len+part4Len,part5);
    strcpy(out+part1Len+part2Len+part3Len+part4Len+part5Len,part6);
    strcpy(out+part1Len+part2Len+part3Len+part4Len+part5Len+part6Len,part7);
    strcpy(out+part1Len+part2Len+part3Len+part4Len+part5Len+part6Len+part7Len,part8);
    return out;
}

wchar_t *text::concatWideString(wchar_t *part1, wchar_t *part2)
{
    text_t part1Len=wcslen(part1);
    wchar_t *out=(wchar_t*)mkwstr(part1Len+wcslen(part2));
    wcscpy(out,part1);
    wcscpy(out+part1Len,part2);
    return out;
}

wchar_t *text::concatWideString(wchar_t *part1, wchar_t *part2, wchar_t *part3)
{
    text_t part1Len=wcslen(part1);
    text_t part2Len=wcslen(part2);
    wchar_t *out=(wchar_t*)mkwstr(part1Len+part2Len+wcslen(part3));
    wcscpy(out,part1);
    wcscpy(out+part1Len,part2);
    wcscpy(out+part1Len+part2Len,part3);
    return out;
}

wchar_t *text::concatWideString(wchar_t *part1, wchar_t *part2, wchar_t *part3, wchar_t *part4)
{
    text_t part1Len=wcslen(part1);
    text_t part2Len=wcslen(part2);
    text_t part3Len=wcslen(part3);
    wchar_t *out=(wchar_t*)mkwstr(part1Len+part2Len+part3Len+wcslen(part4));
    wcscpy(out,part1);
    wcscpy(out+part1Len,part2);
    wcscpy(out+part1Len+part2Len,part3);
    wcscpy(out+part1Len+part2Len+part3Len,part4);
    return out;
}

char *text::concatPaths(char *part1, char *part2)
{
    if(part1[0]=='\\')
        part1++;
    return concat(part1,part1[strlen(part1)-1]=='\\'?"":"\\",part2);
}

char *text::concatPaths(char *part1, char *part2, char *part3)
{
    if(part1[0]=='\\')
        part1++;
    if(part2[0]=='\\')
        part2++;
    return concat(part1,part1[strlen(part1)-1]=='\\'?"":"\\",part2,part2[strlen(part2)-1]=='\\'?"":"\\",part3);
}

char *text::concatPaths(char *part1, char *part2, char *part3, char *part4)
{
    if(part1[0]=='\\')
        part1++;
    if(part2[0]=='\\')
        part2++;
    if(part3[0]=='\\')
        part3++;
    if(part4[0]=='\\')
        part4++;
    return concat(part1,part1[strlen(part1)-1]=='\\'?"":"\\",part2,part2[strlen(part2)-1]=='\\'?"":"\\",part3,part3[strlen(part3)-1]=='\\'?"":"\\",part4);
}

char *text::concatPaths(char *part1, char *part2, char *part3, char *part4, char *part5)
{
    if(part1[0]=='\\')
        part1++;
    if(part2[0]=='\\')
        part2++;
    if(part3[0]=='\\')
        part3++;
    if(part4[0]=='\\')
        part4++;
    if(part5[0]=='\\')
        part5++;
    return concat(part1,part1[strlen(part1)-1]=='\\'?"":"\\",part2,part2[strlen(part2)-1]=='\\'?"":"\\",part3,part3[strlen(part3)-1]=='\\'?"":"\\",part4,text::concat(part4[strlen(part4)-1]=='\\'?"":"\\",part5));
}

char *text::concatPaths(char *part1, char *part2, char *part3, char *part4, char *part5, char *part6)
{
    if(part1[0]=='\\')
        part1++;
    if(part2[0]=='\\')
        part2++;
    if(part3[0]=='\\')
        part3++;
    if(part4[0]=='\\')
        part4++;
    if(part5[0]=='\\')
        part5++;
    if(part6[0]=='\\')
        part6++;
    return concat(part1,part1[strlen(part1)-1]=='\\'?"":"\\",part2,part2[strlen(part2)-1]=='\\'?"":"\\",part3,part3[strlen(part3)-1]=='\\'?"":"\\",part4,text::concat(part4[strlen(part4)-1]=='\\'?"":"\\",part5,part5[strlen(part5)-1]=='\\'?"":"\\",part6));
}

char *text::concatPaths(char *part1, char *part2, char *part3, char *part4, char *part5, char *part6, char *part7)
{
    if(part1[0]=='\\')
        part1++;
    if(part2[0]=='\\')
        part2++;
    if(part3[0]=='\\')
        part3++;
    if(part4[0]=='\\')
        part4++;
    if(part5[0]=='\\')
        part5++;
    if(part6[0]=='\\')
        part6++;
    if(part7[0]=='\\')
        part7++;
    return concat(part1,part1[strlen(part1)-1]=='\\'?"":"\\",part2,part2[strlen(part2)-1]=='\\'?"":"\\",part3,part3[strlen(part3)-1]=='\\'?"":"\\",text::concat(part4,part4[strlen(part4)-1]=='\\'?"":"\\",part5,part5[strlen(part5)-1]=='\\'?"":"\\",part6,part6[strlen(part6)-1]=='\\'?"":"\\",part7));
}

char *text::concatPaths(char *part1, char *part2, char *part3, char *part4, char *part5, char *part6, char *part7, char *part8)
{
    if(part1[0]=='\\')
        part1++;
    if(part2[0]=='\\')
        part2++;
    if(part3[0]=='\\')
        part3++;
    if(part4[0]=='\\')
        part4++;
    if(part5[0]=='\\')
        part5++;
    if(part6[0]=='\\')
        part6++;
    if(part7[0]=='\\')
        part7++;
    if(part8[0]=='\\')
        part8++;
    return concat(part1,part1[strlen(part1)-1]=='\\'?"":"\\",part2,part2[strlen(part2)-1]=='\\'?"":"\\",part3,part3[strlen(part3)-1]=='\\'?"":"\\",text::concat(part4,part4[strlen(part4)-1]=='\\'?"":"\\",part5,part5[strlen(part5)-1]=='\\'?"":"\\",part6,part6[strlen(part6)-1]=='\\'?"":"\\",part7,text::concat(part7[strlen(part7)-1]=='\\'?"":"\\",part8)));
}

char *text::toString(int32_t in)
{
    /*bool n=in<0;
    if(n)
        in=-in;
    text_t len=in==0?1:(int)ceil((double)log((double)in));
    char *out=mkstr(len+(n?1:0));
    itoa(in*(n?-1:1),out,10);
    return out;*/

    if(in<0)
        return text::concat("-",unsignedIntToString(-in));
    return unsignedIntToString(in);
}

char *text::longToString(int64_t in)
{
    if(in<0)
        return text::concat("-",unsignedLongToString(-in));
    return unsignedLongToString(in);
}

char *text::doubleToString(double in) // Warning: precision loss (inavoidable)
{
    std::ostringstream strm;
    strm<<in;
    std::string str=strm.str();
    char *out=(char*)calloc(32,1);
    memcpy(out,str.c_str(),str.length());
    return out;
}

char *text::unsignedIntToString(uint32_t in)
{
    char *out=(char*)calloc(11,1); // 10+1
    uint32_t *temp=(uint32_t*)calloc(sizeof(uint32_t)*8,sizeof(uint32_t)); // Retrieve a single value for each position.
    temp[0]=in;
    for(char i=0;i<32;i++)
    {
        uint32_t t=temp[i];
        uint32_t r=t%10;
        temp[i+1]=(t-r)/10;
        temp[i]=r;
    }
    char reachedValueAt=-1;
    for(char i=31;i>=0;i--)
    {
        uint32_t t=temp[i];
        if(reachedValueAt==-1)
        {
            if(t==0)
                continue;
            reachedValueAt=i;
        }
        out[reachedValueAt-i]=(char)48+t; // 48: ASCII 0
    }
    if(reachedValueAt==-1)
        return "0";
    return out;
}

char *text::unsignedLongToString(uint64_t in)
{
    char *out=(char*)calloc(20,1); // 19+1
    uint64_t *temp=(uint64_t*)calloc(sizeof(uint64_t)*8,sizeof(uint64_t)); // Retrieve a single value for each position.
    temp[0]=in;
    for(char i=0;i<64;i++)
    {
        uint64_t t=temp[i];
        uint64_t r=t%10;
        temp[i+1]=(t-r)/10;
        temp[i]=r;
    }
    char reachedValueAt=-1;
    for(char i=63;i>=0;i--)
    {
        uint64_t t=temp[i];
        if(reachedValueAt==-1)
        {
            if(t==0)
                continue;
            reachedValueAt=i;
        }
        out[reachedValueAt-i]=(char)48+t; // 48: ASCII & UTF8 0
    }
    if(reachedValueAt==-1)
        return "0";
    return out;
}

int32_t text::intFromString(char *in)
{
    int32_t out=0;
    int len=strlen(in);
    uint8_t n=in[0]=='-'?1:0;
    if(n||in[0]=='+')
    {
        in++;
        len--;
    }
    for(int i=0;i<len;i++)
        out+=(in[i]-48)*int32Pow(10,len-i-1); // 48: ASCII & UTF8 0
    return out*(n?-1:1);
}

int64_t text::longFromString(char *in)
{
    int64_t out=0;
    int len=strlen(in);
    uint8_t n=in[0]=='-'?1:0;
    if(n||in[0]=='+')
    {
        in++;
        len--;
    }
    for(int i=0;i<len;i++)
    {
        out+=(in[i]-48)*int64Pow(10,len-i-1); // 48: ASCII & UTF8 0
    }
    return out*(n?-1:1);
}

char *text::bytesToHexString(char *in, text_t len, bool addSpaces)
{
    char *table="0123456789ABCDEF";
    char *out=mkstr(len*2+(addSpaces?len-1:0));
    text_t pos=0;
    uint8_t thisChar;
    for(text_t i=0;i<len;i++)
    {
        thisChar=(uint8_t)in[i];
        if(addSpaces&&i>0)
            out[pos++]=' ';
        out[pos++]=table[(thisChar&0xF0)>>4];
        out[pos++]=table[thisChar&0x0F];
    }
    return out;
}

char *text::bytesFromHexString(char *in, uint32_t &size)
{
    // Could contain spaces or other irrelevant chars. Use a buffer system.
    text_t len=strlen(in);
    size=0;
    char *out=(char*)malloc(len); // Larger than the max value.
    bool first=true;
    uint8_t next=0;
    char thisChar;
    for(text_t i=0;i<len;i++)
    {
        thisChar=in[i];
        if(thisChar=='0')
        {
            if(first)
            {
                next=0x0<<4;
                first=false;
                continue;
            }
            next|=0x0;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='1')
        {
            if(first)
            {
                next=0x1<<4;
                first=false;
                continue;
            }
            next|=0x1;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='2')
        {
            if(first)
            {
                next=0x2<<4;
                first=false;
                continue;
            }
            next|=0x2;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='3')
        {
            if(first)
            {
                next=0x3<<4;
                first=false;
                continue;
            }
            next|=0x3;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='4')
        {
            if(first)
            {
                next=0x4<<4;
                first=false;
                continue;
            }
            next|=0x4;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='5')
        {
            if(first)
            {
                next=0x5<<4;
                first=false;
                continue;
            }
            next|=0x5;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='6')
        {
            if(first)
            {
                next=0x6<<4;
                first=false;
                continue;
            }
            next|=0x6;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='7')
        {
            if(first)
            {
                next=0x7<<4;
                first=false;
                continue;
            }
            next|=0x7;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='8')
        {
            if(first)
            {
                next=0x8<<4;
                first=false;
                continue;
            }
            next|=0x8;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='9')
        {
            if(first)
            {
                next=0x9<<4;
                first=false;
                continue;
            }
            next|=0x9;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='A'||thisChar=='a')
        {
            if(first)
            {
                next=0xA<<4;
                first=false;
                continue;
            }
            next|=0xA;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='B'||thisChar=='b')
        {
            if(first)
            {
                next=0xB<<4;
                first=false;
                continue;
            }
            next|=0xB;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='C'||thisChar=='c')
        {
            if(first)
            {
                next=0xC<<4;
                first=false;
                continue;
            }
            next|=0xC;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='D'||thisChar=='d')
        {
            if(first)
            {
                next=0xD<<4;
                first=false;
                continue;
            }
            next|=0xD;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='E'||thisChar=='e')
        {
            if(first)
            {
                next=0xE<<4;
                first=false;
                continue;
            }
            next|=0xE;
            first=true;
            out[size++]=next;
        }
        else if(thisChar=='F'||thisChar=='f')
        {
            if(first)
            {
                next=0xF<<4;
                first=false;
                continue;
            }
            next|=0xF;
            first=true;
            out[size++]=next;
        }
    }
    if(!first) // Example: "1"
        out[size++]=in[len-1];
    return out;
}

int32_t text::round(double in)
{
    int32_t f=floor(in);
    return (double)in-f>=0.5?f+1:f;
}

double text::roundToPrecision(double in, int32_t precision)
{
    return round(in*pow((double)10,(double)precision))/pow((double)10,(double)precision);
}

double text::doubleFromString(char *in)
{
    // Only supports a.b
    text_t dotPos=indexOf(in,".");
    if(dotPos==pos_notFound)
        return (double)longFromString(in);
    int64_t base=longFromString(substr(in,0,dotPos));
    char *fracStr=substr(in,dotPos+1);
    int64_t fraction=longFromString(fracStr);
    int8_t n=base<0?-1:1;
    return (double)base+((double)fraction/int64Pow(10,strlen(fracStr)))*n;
}

uint32_t text::indexOf(char *haystack, char *needle)
{
    char *pos=strstr(haystack,needle);
    if(pos==0)
        return pos_notFound;
    return pos-haystack;
}

uint32_t text::indexOfFrom(char *haystack, char *needle, uint32_t startFrom)
{
    char *pos=strstr(haystack+startFrom,needle);
    if(pos==0)
        return pos_notFound;
    return pos-haystack;
}

text_t text::lastIndexOf(char *haystack, char *needle)
{
    text_t haystackLen=strlen(haystack);
    text_t needleLen=strlen(needle);
    if(needleLen>haystackLen||haystackLen==0)
        return pos_notFound;
    for(text_t i=haystackLen-needleLen+1;i>0;i--) // Use this because text_t cannot be -1.
    {
        if(strncmp(haystack+i-1/*See above.*/,needle,needleLen)==0)
            return i-1;
    }
    return pos_notFound;
}

text_t text::lastIndexOfTo(char *haystack, char *needle, text_t to)
{
    text_t haystackLen=strlen(haystack);
    text_t needleLen=strlen(needle);
    if(needleLen>haystackLen||haystackLen==0)
        return pos_notFound;
    for(text_t i=to+1;i>0;i--) // Use this because text_t cannot be -1.
    {
        if(strncmp(haystack+i-1/*See above.*/,needle,needleLen)==0)
            return i-1;
    }
    return pos_notFound;
}

char *text::substr(char *str, uint32_t start)
{
    return substr(str,start,strlen(str)-start);
}

char *text::substr(char *str, uint32_t start, uint32_t length)
{
    char *out=mkstr(length);
    strncpy(out,str+start,length);
    return out;
}

char *text::replace(char *str, char *what, char *with)
{
    text_t i=0;
    text_t whatLen=strlen(what);
    text_t withLen=strlen(with);
    text_t len;
Next:
    text_t n=indexOfFrom(str,what,i);
    if(n!=pos_notFound)
    {
        len=strlen(str);
        char *newStr=(char*)mkstr(len-whatLen+withLen);
        strcat(newStr,substr(str,0,n));
        strcat(newStr,with);
        strcat(newStr,substr(str,n+whatLen,len-n-whatLen));
        str=newStr;
        i=n+withLen+1;
        if(i==strlen(str))
            goto Ret;
        goto Next;
    }
Ret:
    return str;
}

char *text::escape(char *str)
{
    text_t length=strlen(str);
    text_t outPos=0;
    char *out=(char*)mkstr(length*2); // Max possible size
    char thisChar;
    for(text_t i=0;i<length;i++)
    {
        thisChar=str[i];
        if(thisChar=='\'')
        {
            out[outPos++]='\\';
            out[outPos++]='\'';
            continue;
        }
        else if(thisChar=='"')
        {
            out[outPos++]='\\';
            out[outPos++]='"';
            continue;
        }
        else if(thisChar=='\\')
        {
            out[outPos++]='\\';
            out[outPos++]='\\';
            continue;
        }
        out[outPos++]=thisChar;
    }
    out[outPos]=0; // Terminate the string.
    return out;
}

char *text::unescape(char *str)
{
    text_t length=strlen(str);
    text_t outPos=0;
    char *out=(char*)mkstr(length); // Max possible size.
    char thisChar;
    char prevChar;

    #define TEXT_UNESCAPE_NEXT_ITERATION \
    \
    prevChar=thisChar;\
    continue;

    for(text_t i=0;i<length;i++)
    {
        thisChar=str[i];
        if(thisChar=='\'')
        {
            if(prevChar=='\\')
            {
                out[outPos-1]=thisChar; // No ++ here.
                TEXT_UNESCAPE_NEXT_ITERATION;
            }
        }
        else if(thisChar=='"')
        {
            if(prevChar=='\\')
            {
                out[outPos-1]=thisChar; // No ++ here.
                TEXT_UNESCAPE_NEXT_ITERATION;
            }
        }
        else if(thisChar=='\\')
        {
            if(prevChar=='\\')
            {
                prevChar=0;
                continue; // Not TEXT_UNESCAPE_NEXT_ITERATION, we want to avoid errors if: \\"
            }
        }
        out[outPos++]=thisChar;
        TEXT_UNESCAPE_NEXT_ITERATION;
    }

    out[outPos]=0; // Terminate the string.

    #undef TEXT_UNESCAPE_NEXT_ITERATION

    return out;
}

char *text::escapeDoubleQuotationMarks(char *str)
{
    text_t length=strlen(str);
    text_t outPos=0;
    char *out=(char*)mkstr(length*2); // Max possible size
    char thisChar;
    for(text_t i=0;i<length;i++)
    {
        thisChar=str[i];
        if(thisChar=='"')
        {
            out[outPos++]='\\';
            out[outPos++]='"';
            continue;
        }
        else if(thisChar=='\\')
        {
            out[outPos++]='\\';
            out[outPos++]='\\';
            continue;
        }
        out[outPos++]=thisChar;
    }
    out[outPos]=0; // Terminate the string.
    return out;
}

char *text::unescapeDoubleQuotationMarks(char *str)
{
    text_t length=strlen(str);
    text_t outPos=0;
    char *out=(char*)mkstr(length); // Max possible size.
    char thisChar;
    char prevChar;

    #define TEXT_UNESCAPE_NEXT_ITERATION \
    \
    prevChar=thisChar;\
    continue;

    for(text_t i=0;i<length;i++)
    {
        thisChar=str[i];
        if(thisChar=='"')
        {
            if(prevChar=='\\')
            {
                out[outPos-1]=thisChar; // No ++ here.
                TEXT_UNESCAPE_NEXT_ITERATION;
            }
        }
        else if(thisChar=='\\')
        {
            if(prevChar=='\\')
            {
                prevChar=0;
                continue; // Not TEXT_UNESCAPE_NEXT_ITERATION, we want to avoid errors if: \\"
            }
        }
        out[outPos++]=thisChar;
        TEXT_UNESCAPE_NEXT_ITERATION;
    }

    out[outPos]=0; // Terminate the string.

    #undef TEXT_UNESCAPE_NEXT_ITERATION

    return out;
}

char *text::escapeSingleQuotationMarks(char *str)
{
    text_t length=strlen(str);
    text_t outPos=0;
    char *out=(char*)mkstr(length*2); // Max possible size
    char thisChar;
    for(text_t i=0;i<length;i++)
    {
        thisChar=str[i];
        if(thisChar=='\'')
        {
            out[outPos++]='\\';
            out[outPos++]='\'';
            continue;
        }
        else if(thisChar=='\\')
        {
            out[outPos++]='\\';
            out[outPos++]='\\';
            continue;
        }
        out[outPos++]=thisChar;
    }
    out[outPos]=0; // Terminate the string.
    return out;

}

char *text::unescapeSingleQuotationMarks(char *str)
{
    text_t length=strlen(str);
    text_t outPos=0;
    char *out=(char*)mkstr(length); // Max possible size.
    char thisChar;
    char prevChar;

    #define TEXT_UNESCAPE_NEXT_ITERATION \
    \
    prevChar=thisChar;\
    continue;

    for(text_t i=0;i<length;i++)
    {
        thisChar=str[i];
        if(thisChar=='\'')
        {
            if(prevChar=='\\')
            {
                out[outPos-1]=thisChar; // No ++ here.
                TEXT_UNESCAPE_NEXT_ITERATION;
            }
        }
        else if(thisChar=='\\')
        {
            if(prevChar=='\\')
            {
                prevChar=0;
                continue; // Not TEXT_UNESCAPE_NEXT_ITERATION, we want to avoid errors if: \\"
            }
        }
        out[outPos++]=thisChar;
        TEXT_UNESCAPE_NEXT_ITERATION;
    }

    out[outPos]=0; // Terminate the string.

    #undef TEXT_UNESCAPE_NEXT_ITERATION

    return out;
}

std::vector<char *> text::split(char *in, char *separator)
{
    std::vector<char*> out;
    text_t separatorLen=strlen(separator);
    char *from=in;
    char *next;
    char *last=0;
    while((next=strstr(from,separator))!=0)
    {
        out.push_back(substr(from,0,next-from));
        from=next+separatorLen;
        if(from[0]==0)
            return out;
        last=next;
    }
    if(last!=0) // Push last item
        out.push_back(substr(last+separatorLen,0,in+strlen(in)-last-separatorLen));
    return out;
}

text_t text::count(char *haystack, char *needle)
{
    text_t out=0;
    text_t needleLen=strlen(needle);
    char *from=haystack;
    char *next;
    while((next=strstr(from,needle))!=0)
    {
        out++;
        from=next+needleLen;
        if(from[0]==0)
            return out;
    }
    return out; // This is never reached.
}

char *text::trimStart(char *in)
{
    text_t trimStart=0;
    while(isWhitespace(in[trimStart])) // (char)0 is not a whitespace char.
        trimStart++;
    return substr(in,trimStart);
}

char *text::trimEnd(char *in)
{
    text_t trimEnd=0;
    text_t inLen=strlen(in);
    while(isWhitespace(in[inLen-trimEnd-1])) // (char)0 is not a whitespace char.
        trimEnd++;
    return substr(in,0,inLen-trimEnd);
}

char *text::trim(char *in)
{
    text_t trimStart=0;
    text_t trimEnd=0;
    text_t inLen=strlen(in);
    if(inLen==0)
        return in;
    while(isWhitespace(in[trimStart])) // (char)0 is not a whitespace char.
        trimStart++;
    while(isWhitespace(in[inLen-trimEnd-1])) // (char)0 is not a whitespace char.
        trimEnd++;
    return substr(in,trimStart,inLen-trimStart-trimEnd);
}

char *text::firstChars(char *in, uint32_t chars)
{
    char *out=(char*)mkstr(chars);
    strncpy(out,in,chars);
    return out;
}

char *text::lastChars(char *in, text_t chars)
{
    char *out=(char*)mkstr(chars);
    text_t len=strlen(in);
    strcpy(out,in+len-chars);
    return out;
}

wchar_t *text::wFirstChars(wchar_t *in, text_t chars)
{
    wchar_t *out=mkwstr(chars);
    wcsncpy(out,in,chars);
    return out;
}

wchar_t *text::wLastChars(wchar_t *in, uint32_t chars)
{
    wchar_t *out=mkwstr(chars);
    text_t len=wcslen(in);
    wcscpy(out,in+len-chars);
    return out;
}

bool text::isWhitespace(char chr)
{
    return chr==' '  ||
           chr=='\t' ||
           chr=='\r' ||
           chr=='\n'
            ;
}

bool text::startsWith(char *str, char *with)
{
    text_t strLen=strlen(str);
    text_t withLen=strlen(with);
    if(withLen>strLen||withLen==0)
        return false;
    return strcmp(substr(str,0,withLen),with)==0;
}

bool text::endsWith(char *str, char *with)
{
    text_t strLen=strlen(str);
    text_t withLen=strlen(with);
    if(withLen>strLen||withLen==0)
        return false;
    return strcmp(substr(str,strLen-withLen),with)==0;
}

bool text::iStartsWith(char *str, char *with)
{
    text_t strLen=strlen(str);
    text_t withLen=strlen(with);
    if(withLen>strLen||withLen==0)
        return false;
    return stricmp(substr(str,0,withLen),with)==0;
}

bool text::iEndsWith(char *str, char *with)
{
    text_t strLen=strlen(str);
    text_t withLen=strlen(with);
    if(withLen>strLen||withLen==0)
        return false;
    return stricmp(substr(str,strLen-withLen),with)==0;
}

char *text::charToString(char in)
{
    char *out=mkstr(1);
    out[0]=in;
    return out;
}

char *text::getDirPath(char *path)
{
    if(indexOf(path,"/")!=pos_notFound)
        path=replace(path,"/","\\");
    text_t len=strlen(path);
    if(path[len-1]=='\\')
    {
        if(len==1)
            return path;
        len--;
    }
    text_t pos=lastIndexOfTo(path,"\\",len-1);
    if(pos==pos_notFound)
        return path;
    return substr(path,0,pos+1);
}

char *text::getFileName(char *filePath)
{
    if(indexOf(filePath,"/")!=pos_notFound)
        filePath=replace(filePath,"/","\\");
    text_t pos=lastIndexOf(filePath,"\\");
    return substr(filePath,pos+1,strlen(filePath)-pos-1);
}

char *text::getFileExtension(char *filePath)
{
    if(indexOf(filePath,"/")!=pos_notFound)
        filePath=replace(filePath,"/","\\");
    text_t pos=lastIndexOf(filePath,".");
    if(pos==pos_notFound)
        return "";
    return substr(filePath,pos+1,strlen(filePath)-pos-1);
}

char *text::getFileNameWithoutExtension(char *filePath)
{
    if(indexOf(filePath,"/")!=pos_notFound)
        filePath=replace(filePath,"/","\\");
    text_t pos=lastIndexOf(filePath,"\\");
    text_t pos2=lastIndexOf(filePath,".");
    if(pos2==pos_notFound)
    {
        if(pos==pos_notFound)
            return filePath;
        return substr(filePath,pos+1,strlen(filePath)-pos-1);
    }
    if(pos==pos_notFound)
        return substr(filePath,0,pos2);
    return substr(filePath,pos+1,pos2-pos-1);
}
