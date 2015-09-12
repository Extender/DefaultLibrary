#ifndef TEXT_H
#define TEXT_H

#include <vector>
#include <stdint.h>
#include <math.h>
#include <sstream>

#define text_t uint32_t
#define pos_notFound INT32_MAX
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
    static char *cloneString(char *in);
    static char *concat(char *part1,char *part2);
    static char *concat(char *part1,char *part2,char *part3);
    static char *concat(char *part1,char *part2,char *part3,char *part4);
    static char *concat(char *part1,char *part2,char *part3,char *part4,char *part5);
    static char *concat(char *part1,char *part2,char *part3,char *part4,char *part5,char *part6);
    static char *concat(char *part1,char *part2,char *part3,char *part4,char *part5,char *part6,char *part7);
    static char *concat(char *part1,char *part2,char *part3,char *part4,char *part5,char *part6,char *part7,char *part8);
    static wchar_t *concatWideString(wchar_t *part1,wchar_t *part2);
    static wchar_t *concatWideString(wchar_t *part1,wchar_t *part2,wchar_t *part3);
    static wchar_t *concatWideString(wchar_t *part1,wchar_t *part2,wchar_t *part3,wchar_t *part4);
    static char *concatPaths(char *part1,char *part2);
    static char *concatPaths(char *part1,char *part2,char *part3);
    static char *concatPaths(char *part1,char *part2,char *part3,char *part4);
    static char *concatPaths(char *part1,char *part2,char *part3,char *part4,char *part5);
    static char *concatPaths(char *part1,char *part2,char *part3,char *part4,char *part5,char *part6);
    static char *concatPaths(char *part1,char *part2,char *part3,char *part4,char *part5,char *part6,char *part7);
    static char *concatPaths(char *part1,char *part2,char *part3,char *part4,char *part5,char *part6,char *part7,char *part8);
    static char *toString(int32_t in);
    static char *longToString(int64_t in);
    static char *doubleToString(double in); // Warning: precision loss (inavoidable)
    static char *unsignedIntToString(uint32_t in);
    static char *unsignedLongToString(uint64_t in);
    static int32_t intFromString(char *in);
    static int64_t longFromString(char *in);
    static char *bytesToHexString(char *in,text_t len,bool addSpaces);
    static char *bytesFromHexString(char *in,text_t &size);
    static int32_t round(double in);
    static double roundToPrecision(double in,int32_t precision);
    static double doubleFromString(char *in); // Only supports a.b
    static text_t indexOf(char *haystack,char *needle);
    static text_t indexOfFrom(char *haystack,char *needle,text_t startFrom);
    static text_t lastIndexOf(char *haystack,char *needle);
    static text_t lastIndexOfTo(char *haystack,char *needle,text_t to); // "to" may not be bigger than strlen(haystack).
    static char *substr(char *str,text_t start);
    static char *substr(char *str,text_t start,text_t length);
    static char *replace(char *str,char *what,char *with);
    static char *escape(char *str); // Escapes all instances of \ " '
    static char *unescape(char *str); // Unescapes all instances of \ " '
    static char *escapeDoubleQuotationMarks(char *str); // Escapes all instances of \ "
    static char *unescapeDoubleQuotationMarks(char *str); // Unescapes all instances of \ "
    static char *escapeSingleQuotationMarks(char *str); // Escapes all instances of \ '
    static char *unescapeSingleQuotationMarks(char *str); // Unescapes all instances of \ '
    static std::vector<char*> split(char *in,char *separator);
    static text_t count(char *haystack,char *needle);
    static char *trimStart(char *in);
    static char *trimEnd(char *in);
    static char *trim(char *in);
    static char *firstChars(char *in,text_t chars);
    static char *lastChars(char *in,text_t chars);
    static wchar_t *wFirstChars(wchar_t *in,text_t chars);
    static wchar_t *wLastChars(wchar_t *in,text_t chars);
    static bool isWhitespace(char chr);
    static bool startsWith(char *str,char *with);
    static bool endsWith(char *str,char *with);
    static bool iStartsWith(char *str,char *with);
    static bool iEndsWith(char *str,char *with);
    static char *charToString(char in);
    static char *getDirPath(char *path);
    static char *getFileName(char *filePath);
    static char *getFileExtension(char *filePath);
    static char *getFileNameWithoutExtension(char *filePath);
};

#endif // TEXT_H
