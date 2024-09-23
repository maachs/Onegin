#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys\stat.h>
#include <assert.h>
#include <TXlib.h>

struct Inftext
{
    int nlines;
    char* text;
    char** address;
};

void SortText (Inftext* onegin);

int Compare_lines (const char* s1, const char* s2);

void Swap(char** swap1, char** swap2);

void ScanText(Inftext* onegin, unsigned long size_f);

void PrintText(Inftext* onegin);

void GetFileSize(Inftext* onegin, unsigned long size_f);
// TODO struct with indeces, text, nlines
// TODO string struct

int main()
{
    struct Inftext onegin = {};
    struct stat buffer = {};

    stat("OneginText.txt", &buffer);//buffer.st_size unsigned long int

    onegin.text = (char*) calloc(buffer.st_size, sizeof(char));

    onegin.nlines = 0;

    ScanText(&onegin, buffer.st_size);
    onegin.address = (char**) calloc(onegin.nlines, sizeof(char*));

    GetFileSize(&onegin, buffer.st_size);

    SortText(&onegin);

    PrintText(&onegin);

    free(onegin.text);
    free(onegin.address);

    return 0;
}

void ScanText(Inftext* onegin, unsigned long size_f)
{
    FILE* scan = fopen ("OneginText.txt", "rb");

    if (scan == NULL)
    {
        printf("Error text");
        exit(1);
    }

    fread (onegin->text, size_f, 1, scan);

    if (fread (onegin->text, size_f, 1, scan) == 1)
    {
        printf("Error text");
        exit(1);
    }

    for (unsigned int i = 0; i < size_f; i++)
    {
        if (onegin->text[i] =='\r')
        {
            onegin->text[i+1] = '\0';
            onegin->text[i] = '\n';
            (onegin->nlines)++;
        }
    }
}

void PrintText(Inftext* onegin)
{
    for (int ind = 0; ind < onegin->nlines; ind++)
    {
        printf ("%s\n", onegin->address[ind]);
    }
}

void GetFileSize(Inftext* onegin, unsigned long size_f)
{
    int str = 1;
    onegin->address[0] = onegin->text;
    for (unsigned int j = 0; j < size_f; j++)
    {
        if (onegin->text[j] == '\0')
        {
            onegin->address[str] = &onegin->text[j] + 1;
            str++;
            //printf("%d\n", index[str]);
        }
    }
}
void SortText (Inftext* onegin)
{
    int str = onegin->nlines;

    while (str > 0)
    {
        for (int i = 0; i < str - 1; i++)
        {
            if (Compare_lines(onegin->address[i], onegin->address[i + 1]) > 0)
            {
                //printf ("строка 1 %s\n строка 2 %s\n", index[i], index[i+1] );
                Swap(&onegin->address[i], &onegin->address[i + 1]);
            }
        }
        /*for (int ind = 0; ind < nlines; ind++)
        {
            printf ("%s\n", index[ind]);
        }*/
        str--;
    }
}

int Compare_lines (const char* s1, const char* s2)
{
    int elem1 = 0, elem2 = 0;
    //printf ("сравнение строка 1 %s\nсравнене строка 2 %s\n\n", s1, s2);
    while (true)
    {
        while (isalpha(s1[elem1]) == 0)
        {
            elem1++;
        }
        while (isalpha(s2[elem2]) == 0)
        {
            elem2++;
        }

        //if (isupper(s1[elem1]) == 0){s1[elem1] = (char)tolower(s1[elem1]);}
        //if (isupper(s2[elem2]) == 0){s2[elem2] = (char)tolower(s2[elem2]);}

        if (tolower(s1[elem1]) != tolower(s2[elem2]))
        {
            return (tolower(s1[elem1]) - tolower(s2[elem2]));
        }
        else
        {
            elem1++;
            elem2++;

            continue;
        }
    }

    return -1;
}

void Swap(char** swap1, char** swap2)
{
    char* a = *swap1;
    *swap1 = *swap2;
    *swap2 = a;
}
