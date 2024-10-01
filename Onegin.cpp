#include <TXlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys\stat.h>
#include <assert.h>

static const char* TEXTFILE = "OneginText.txt";

struct LineInfo
{
    char* address;
    int lineslen;
};

struct TextInfo
{
    int nlines;
    char* text;
    LineInfo* linesdata;
};

enum ErrorCode
{
    FILE_ERROR = 2,
    PIZDEC = 1,
    ZAEBIS = 0,
    READ_FILE_ERROR = 3,
    CALLOC_ERROR = 4,
    DTOR_ERROR = 5,
    GETFILE_ERROR = 6
};

ErrorCode QuickSort (TextInfo* text);

ErrorCode BubbleSort (TextInfo* text);

int ReverseComparelines (const char* s1, const char* s2, int len1, int len2);

int StraightComparelines (const char* s1, const char* s2, int len1, int len2);

int Comparelines (const char* s1, const char* s2, int len1, int len2);

void Swap (void* swap1, void* swap2, size_t size);

ErrorCode ScanText (TextInfo* text, unsigned long size_file);

void PrintText (TextInfo* text);

ErrorCode GetFileSize (TextInfo* text, unsigned long size_file);

int CompareQSort (const void* s1, const void* s2);

ErrorCode TextInfoDtor (TextInfo* text);

ErrorCode InitTextInfo (TextInfo* text, unsigned long int elem);

ErrorCode InitAddressInfo(TextInfo* text, unsigned long int elem);

int main()
{
    TextInfo text = {};
    struct stat buffer = {};

    stat(TEXTFILE, &buffer);

    if (InitTextInfo(&text, buffer.st_size) != ZAEBIS)
    {
        printf("InitText ERROR");
        return CALLOC_ERROR;
    }

    if (ScanText(&text, buffer.st_size) != ZAEBIS)
    {
        printf("ScanText ERROR");
        return PIZDEC;
    }

    if (InitAddressInfo(&text, text.nlines) != ZAEBIS)
    {
        printf("InitAddress ERROR");
        return CALLOC_ERROR;
    }

    if (GetFileSize(&text, buffer.st_size) != ZAEBIS)
    {
        printf("GetFileSize ERROR");
        return GETFILE_ERROR;
    }

    QuickSort(&text);

    PrintText(&text);
    if (TextInfoDtor(&text) != ZAEBIS)
    {
        printf("TextInfoDtor ERROR");
        return DTOR_ERROR;
    }

    return 0;
}
ErrorCode InitTextInfo (TextInfo* text, unsigned long int elem)
{
    text->text = (char*) calloc(elem, sizeof(char*));
    return ZAEBIS;
}

ErrorCode InitAddressInfo(TextInfo* text, unsigned long int elem)
{
    text->linesdata = (LineInfo*) calloc(elem, sizeof(LineInfo));
    return ZAEBIS;
}

ErrorCode TextInfoDtor (TextInfo* text)
{
    free(text->text);
    text->text = NULL;

    free(text->linesdata);
    text->linesdata = NULL;
    return ZAEBIS;
}

ErrorCode ScanText(TextInfo* text, unsigned long size_file)
{
    FILE* textfile = fopen (TEXTFILE, "rb");

    if (textfile == NULL)
    {
        printf("Cannot open file");
        return FILE_ERROR;
    }

    if (fread (text->text, size_file, 1, textfile) == 0)
    {
        printf("Cannot read file");
        return READ_FILE_ERROR;
    }

    for (unsigned int elem = 0; elem < size_file; elem++)
    {
        if ((text->text)[elem] =='\r')
        {
            (text->text)[elem+1] = '\0';
            (text->text)[elem] = '\n';
            text->nlines++;
        }
    }
    return ZAEBIS;
}

void PrintText(TextInfo* text)
{
    for (int str = 0; str < text->nlines; str++)
    {
        printf ("%s\n", (text->linesdata)[str].address);
    }
}

ErrorCode GetFileSize(TextInfo* text, unsigned long size_file)
{
    int current_str = 1;
    int len = 0;
    (text->linesdata)[0].address = text->text;
    for (unsigned int elem = 0; elem < size_file; elem++)
    {
        if (text->text[elem] == '\0')
        {
            if (elem + 1 == size_file)
            {
                break;
            }
            text->linesdata[current_str].address = &(text->text[elem]) + 1;
            text->linesdata[len].lineslen = text->linesdata[current_str].address - text->linesdata[current_str-1].address;

            len++;
            current_str++;
        }
    }
    (text->linesdata)[text->nlines-1].lineslen = &(text->text[size_file]) - text->linesdata[text->nlines-1].address;

    return ZAEBIS;
}

ErrorCode QuickSort (TextInfo* text)
{
    qsort(text->linesdata, text->nlines, sizeof(LineInfo), CompareQSort);

    return ZAEBIS;
}

ErrorCode BubbleSort (TextInfo* text)
{
    int line = text->nlines - 1;

    while (line > 0)
    {
        for (int str = 0; str < line ; str++)
        {
            if (ReverseComparelines((text->linesdata)[str].address, (text->linesdata)[str+1].address, (text->linesdata)[str].lineslen, (text->linesdata)[str+1].lineslen) > 0)
            {
                Swap(&(text->linesdata)[str].address,  &(text->linesdata)[str+1].address, sizeof(char**));
                Swap(&(text->linesdata)[str].lineslen, &(text->linesdata)[str+1].lineslen, sizeof(int*));
            }
        }
        line--;
    }
    return ZAEBIS;
}

int CompareQSort (const void* elem1, const void* elem2)
{
    // ⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠛⢉⢉⠉⠉⠻⣿⣿⣿⣿⣿⣿
    // ⣿⣿⣿⣿⣿⣿⣿⠟⠠⡰⣕⣗⣷⣧⣀⣅⠘⣿⣿⣿⣿⣿
    // ⣿⣿⣿⣿⣿⣿⠃⣠⣳⣟⣿⣿⣷⣿⡿⣜⠄⣿⣿⣿⣿⣿
    // ⣿⣿⣿⣿⡿⠁⠄⣳⢷⣿⣿⣿⣿⡿⣝⠖⠄⣿⣿⣿⣿⣿
    // ⣿⣿⣿⣿⠃⠄⢢⡹⣿⢷⣯⢿⢷⡫⣗⠍⢰⣿⣿⣿⣿⣿
    // ⣿⣿⣿⡏⢀⢄⠤⣁⠋⠿⣗⣟⡯⡏⢎⠁⢸⣿⣿⣿⣿⣿
    // ⣿⣿⣿⠄⢔⢕⣯⣿⣿⡲⡤⡄⡤⠄⡀⢠⣿⣿⣿⣿⣿⣿
    // ⣿⣿⠇⠠⡳⣯⣿⣿⣾⢵⣫⢎⢎⠆⢀⣿⣿⣿⣿⣿⣿⣿
    // ⣿⣿⠄⢨⣫⣿⣿⡿⣿⣻⢎⡗⡕⡅⢸⣿⣿⣿⣿⣿⣿⣿
    // ⣿⣿⠄⢜⢾⣾⣿⣿⣟⣗⢯⡪⡳⡀⢸⣿⣿⣿⣿⣿⣿⣿
    // ⣿⣿⠄⢸⢽⣿⣷⣿⣻⡮⡧⡳⡱⡁⢸⣿⣿⣿⣿⣿⣿⣿
    // ⣿⣿⡄⢨⣻⣽⣿⣟⣿⣞⣗⡽⡸⡐⢸⣿⣿⣿⣿⣿⣿⣿
    // ⣿⣿⡇⢀⢗⣿⣿⣿⣿⡿⣞⡵⡣⣊⢸⣿⣿⣿⣿⣿⣿⣿
    // ⣿⣿⣿⡀⡣⣗⣿⣿⣿⣿⣯⡯⡺⣼⠎⣿⣿⣿⣿⣿⣿⣿
    // ⣿⣿⣿⣧⠐⡵⣻⣟⣯⣿⣷⣟⣝⢞⡿⢹⣿⣿⣿⣿⣿⣿
    // ⣿⣿⣿⣿⡆⢘⡺⣽⢿⣻⣿⣗⡷⣹⢩⢃⢿⣿⣿⣿⣿⣿
    // ⣿⣿⣿⣿⣷⠄⠪⣯⣟⣿⢯⣿⣻⣜⢎⢆⠜⣿⣿⣿⣿⣿
    // ⣿⣿⣿⣿⣿⡆⠄⢣⣻⣽⣿⣿⣟⣾⡮⡺⡸⠸⣿⣿⣿⣿
    // ⣿⣿⡿⠛⠉⠁⠄⢕⡳⣽⡾⣿⢽⣯⡿⣮⢚⣅⠹⣿⣿⣿
    // ⡿⠋⠄⠄⠄⠄⢀⠒⠝⣞⢿⡿⣿⣽⢿⡽⣧⣳⡅⠌⠻⣿
    // ⠁⠄⠄⠄⠄⠄⠐⡐⠱⡱⣻⡻⣝⣮⣟⣿⣻⣟⣻⡺⣊
    return ReverseComparelines(((const LineInfo*)elem1)->address,  ((const LineInfo*)elem2)->address,
                               ((const LineInfo*)elem1)->lineslen, ((const LineInfo*)elem2)->lineslen);
}

int StraightComparelines (const char* s1, const char* s2, int len1, int len2)
{
    int elem_str1 = 0, elem_str2 = 0;

    while (elem_str1 >= len1 && elem_str2 >= len2)
    {
        while (isalpha(s1[elem_str1]) == 0)
        {
            elem_str1++;
        }
        while (isalpha(s2[elem_str2]) == 0)
        {
            elem_str2++;
        }

        if (tolower(s1[elem_str1]) != tolower(s2[elem_str2]))
        {
            return (tolower(s1[elem_str1]) - tolower(s2[elem_str2]));
        }
        else
        {
            elem_str1++;
            elem_str2++;
        }
    }
    if (elem_str1 < 0 && elem_str2 < 0)
    {
        return 0;
    }

    return elem_str1 - elem_str2;
}
int ReverseComparelines (const char* s1, const char* s2, int len1, int len2)
{
    int elem_str1 = len1 - 1 , elem_str2 = len2 - 1;

    while (elem_str1 >= 0 && elem_str2 >= 0)
    {
        while (isalpha(s1[elem_str1]) == 0)
        {
            elem_str1--;
        }
        while (isalpha(s2[elem_str2]) == 0)
        {
            elem_str2--;
        }

        if (tolower(s1[elem_str1]) != tolower(s2[elem_str2]))
        {
            return (tolower(s1[elem_str1]) - tolower(s2[elem_str2]));
        }
        else
        {
            elem_str1--;
            elem_str2--;
        }
    }
    if (elem_str1 < 0 && elem_str2 < 0)
    {
        return 0;
    }

    return elem_str1 - elem_str2;
}

void Swap(void* swap1, void* swap2, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        char swap = *((char*) swap1 + i);
        *((char*) swap1 + i) = *((char*) swap2 + i);
        *((char*) swap2 + i) = swap;
    }
}
