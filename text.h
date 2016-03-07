#ifndef TEXT_H
#define TEXT_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include <vector>
#include <sstream>

#define text_t uint32_t
#define pos_notFound INT_MAX
#define DEFAULT_DOUBLE_TO_STRING_PRECISION 6

class text
{
public:
    static int32_t int32Pow(int32_t base,int32_t exp);
    static int64_t int64Pow(int64_t base,int64_t exp);
    static uint32_t uint32Pow(uint32_t base,uint32_t exp);
    static uint64_t uint64Pow(uint64_t base,uint64_t exp);
    static char *mkstr(text_t length);
    static wchar_t *mkwstr(text_t length);
    static char *cloneString(const char *in);
    static char *terminateFixedLengthString(const char *in,text_t length);
    static char *concat(const char *part1,const char *part2);
    static char *concat(const char *part1,const char *part2,const char *part3);
    static char *concat(const char *part1,const char *part2,const char *part3,const char *part4);
    static char *concat(const char *part1,const char *part2,const char *part3,const char *part4,const char *part5);
    static char *concat(const char *part1,const char *part2,const char *part3,const char *part4,const char *part5,const char *part6);
    static char *concat(const char *part1,const char *part2,const char *part3,const char *part4,const char *part5,const char *part6,const char *part7);
    static char *concat(const char *part1,const char *part2,const char *part3,const char *part4,const char *part5,const char *part6,const char *part7,const char *part8);
    static wchar_t *concatWideString(const wchar_t *part1,const wchar_t *part2);
    static wchar_t *concatWideString(const wchar_t *part1,const wchar_t *part2,const wchar_t *part3);
    static wchar_t *concatWideString(const wchar_t *part1,const wchar_t *part2,const wchar_t *part3,const wchar_t *part4);
    static char *concatPaths(const char *part1,const char *part2);
    static char *concatPaths(const char *part1,const char *part2,const char *part3);
    static char *concatPaths(const char *part1,const char *part2,const char *part3,const char *part4);
    static char *concatPaths(const char *part1,const char *part2,const char *part3,const char *part4,const char *part5);
    static char *concatPaths(const char *part1,const char *part2,const char *part3,const char *part4,const char *part5,const char *part6);
    static char *concatPaths(const char *part1,const char *part2,const char *part3,const char *part4,const char *part5,const char *part6,const char *part7);
    static char *concatPaths(const char *part1,const char *part2,const char *part3,const char *part4,const char *part5,const char *part6,const char *part7,const char *part8);
    static char *toString(int32_t in);
    static char *longToString(int64_t in);
    static char *doubleToString(double in); // Warning: precision loss (inavoidable)
    static char *doubleToStringWithFixedPrecision(double in,uint8_t precision); // Warning: precision loss (inavoidable)
    static char *unsignedIntToString(uint32_t in);
    static char *unsignedLongToString(uint64_t in);
    static int32_t intFromString(const char *in);
    static int64_t longFromString(const char *in);
    static char *bytesToHexString(const char *in,text_t len,bool addSpaces);
    static char *bytesFromHexString(const char *in,text_t &size);
    static int32_t round(double in);
    static double roundToPrecision(double in,int32_t precision);
    static double doubleFromString(const char *in); // Only supports a.b
    static text_t indexOf(const char *haystack,const char *needle);
    static text_t indexOfFrom(const char *haystack,const char *needle,text_t startFrom);
    static text_t lastIndexOf(const char *haystack,const char *needle);
    static text_t lastIndexOfTo(const char *haystack,const char *needle,text_t to); // "to" may not be bigger than strlen(haystack).
    static char *substr(const char *str,text_t start);
    static char *substr(const char *str,text_t start,text_t length);
    static char *replace(const char *str,const char *what,const char *with);
    static char *escape(const char *str); // Escapes all instances of \ " '
    static char *unescape(const char *str); // Unescapes all instances of \ " '
    static char *escapeDoubleQuotationMarks(const char *str); // Escapes all instances of \ "
    static char *unescapeDoubleQuotationMarks(const char *str); // Unescapes all instances of \ "
    static char *escapeSingleQuotationMarks(const char *str); // Escapes all instances of \ '
    static char *unescapeSingleQuotationMarks(const char *str); // Unescapes all instances of \ '
    static std::vector<char*> split(const char *in,const char *separator);
    static text_t count(const char *haystack,const char *needle);
    static char *trimStart(const char *str);
    static char *trimEnd(const char *str);
    static char *trim(const char *str);
    static char *firstChars(const char *in,text_t chars);
    static char *lastChars(const char *in,text_t chars);
    static wchar_t *wFirstChars(const wchar_t *in,text_t chars);
    static wchar_t *wLastChars(const wchar_t *in,text_t chars);
    static bool isWhitespace(char chr);
    static bool startsWith(const char *str,const char *with);
    static bool endsWith(const char *str,const char *with);
    static bool iStartsWith(const char *str,const char *with);
    static bool iEndsWith(const char *str,const char *with);
    static char *charToString(char in);
    static char *getDirPath(const char *path);
    static char *getFileName(const char *filePath);
    static char *getFileExtension(const char *filePath);
    static char *getFileNameWithoutExtension(const char *filePath);
};

#endif // TEXT_H
