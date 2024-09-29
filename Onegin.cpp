#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys\stat.h>
#include <assert.h>
#include <TXlib.h>

#define TEXT "OneginText.txt"

struct LineInfo
{
    char* address;
    int lineslen;
};

struct TextInfo
{
    int nlines;
    char* text;
    LineInfo* addr;
};

enum ErrorCode
{
    Error = 1,
    Ok = 0
};

ErrorCode QuickSort (TextInfo* text);

ErrorCode BubbleSort (TextInfo* text);

int ReverseComparelines (const char* s1, const char* s2, int len1, int len2);

int StraightComparelines (const char* s1, const char* s2, int len1, int len2);

int Comparelines (const char* s1, const char* s2, int len1, int len2);

void Swap (void* swap1, void* swap2, size_t size);

ErrorCode ScanText (TextInfo* text, unsigned long size_f);

void PrintText (TextInfo* text);

ErrorCode GetFileSize (TextInfo* text, unsigned long size_f);

int CompareQSort (const void* s1, const void* s2);

ErrorCode TextInfoDtor (void* remove);

ErrorCode InitTextInfo (TextInfo* text, unsigned long int elem, int size_elem );

ErrorCode InitAddressInfo(TextInfo* text, unsigned long int elem, int size_elem);

int main()
{
    struct TextInfo text = {};
    struct stat buffer = {};

    stat(TEXT, &buffer);//buffer.st_size unsigned long int

    InitTextInfo(&text, buffer.st_size, sizeof(char*));

    ScanText(&text, buffer.st_size);

    InitAddressInfo(&text, text.nlines, sizeof(LineInfo));

    GetFileSize(&text, buffer.st_size);

    QuickSort(&text);

    PrintText(&text);

    TextInfoDtor(text.text);
    TextInfoDtor(text.addr);

    return 0;
}
ErrorCode InitTextInfo (TextInfo* text, unsigned long int elem, int size_elem)
{
    text->text = (char*) calloc(elem, size_elem);
    return Ok;
}

ErrorCode InitAddressInfo(TextInfo* text, unsigned long int elem, int size_elem)
{
    text->addr = (LineInfo*) calloc(elem, size_elem);
    return Ok;
}

ErrorCode TextInfoDtor (void* remove)
{
    free(remove);
    remove = NULL;
    return Ok;
}

ErrorCode ScanText(TextInfo* text, unsigned long size_f)
{
    FILE* textfile = fopen (TEXT, "rb");

    if (textfile== NULL)
    {
        printf("Cannot open file");
        return Error;
    }

    if (fread (text->text, size_f, 1, textfile) == 0)
    {
        printf("Cannot read file");
        return Error;
    }

    for (unsigned int i = 0; i < size_f; i++)
    {
        if ((text->text)[i] =='\r')
        {
            (text->text)[i+1] = '\0';
            (text->text)[i] = '\n';
            text->nlines++;
        }
    }
    return Ok;
}

void PrintText(TextInfo* text)
{
    for (int ind = 0; ind < text->nlines; ind++)
    {
        printf ("%s\n", (text->addr)[ind].address);
    }
}

ErrorCode GetFileSize(TextInfo* text, unsigned long size_f)
{
    int str = 1;
    int len = 0;
    (text->addr)[0].address = text->text;
    for (unsigned int elem = 0; elem < size_f; elem++)
    {
        if (text->text[elem] == '\0')
        {
            if (elem + 1 == size_f)
            {
                break;
            }
            text->addr[str].address = &(text->text[elem]) + 1;
            text->addr[len].lineslen = text->addr[str].address - text->addr[str-1].address;

            len++;
            str++;
        }
    }
    (text->addr)[text->nlines-1].lineslen = &(text->text[size_f]) - text->addr[text->nlines-1].address;
    return Ok;
}

ErrorCode QuickSort (TextInfo* text)
{
    qsort(text->addr, text->nlines, sizeof(LineInfo), CompareQSort);

    return Ok;
}
ErrorCode BubbleSort (TextInfo* text)
{
    int str = text->nlines - 1;

    while (str > 0)
    {
        for (int i = 0; i < str ; i++)
        {
            if (ReverseComparelines((text->addr)[i].address, (text->addr)[i+1].address, (text->addr)[i].lineslen, (text->addr)[i+1].lineslen) > 0)
            {
                Swap(&(text->addr)[i].address, &(text->addr)[i+1].address, sizeof(char**));
                Swap(&(text->addr)[i].lineslen, &(text->addr)[i+1].lineslen, sizeof(int*));
            }
        }
        str--;
    }
    return Ok;
}
int CompareQSort (const void* elem1, const void* elem2)
{
    return ReverseComparelines((((const LineInfo*)elem1)->address), (((const LineInfo*)elem2)->address),
                         (((const LineInfo*)elem1)->lineslen), (((const LineInfo*)elem2)->lineslen));
}
int StraightComparelines (const char* s1, const char* s2, int len1, int len2)
{
     int elem1 = 0 , elem2 = 0;

    while (elem1 >= len1 && elem2 >= len2)
    {
        while (isalpha(s1[elem1]) == 0)
        {
            elem1++;
        }
        while (isalpha(s2[elem2]) == 0)
        {
            elem2++;
        }

        if (tolower(s1[elem1]) != tolower(s2[elem2]))
        {
            return (tolower(s1[elem1]) - tolower(s2[elem2]));
        }
        else
        {
            elem1++;
            elem2++;
        }
    }
    if (elem1 < 0 && elem2 < 0)
    {
        return 0;
    }
    return elem1 - elem2;
}
int ReverseComparelines (const char* s1, const char* s2, int len1, int len2)
{
    int elem1 = len1 - 1 , elem2 = len2 - 1;

    while (elem1 >= 0 && elem2 >= 0)
    {
        while (isalpha(s1[elem1]) == 0)
        {
            elem1--;
        }
        while (isalpha(s2[elem2]) == 0)
        {
            elem2--;
        }

        if (tolower(s1[elem1]) != tolower(s2[elem2]))
        {
            return (tolower(s1[elem1]) - tolower(s2[elem2]));
        }
        else
        {
            elem1--;
            elem2--;
        }
    }
    if (elem1 < 0 && elem2 < 0)
    {
        return 0;
    }
    return elem1 - elem2;
}

void Swap(void* swap1, void* swap2, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        char a = *((char*) swap1 + i);
        *((char*) swap1 + i) = *((char*) swap2 + i);
        *((char*) swap2 + i) = a;
    }
}

