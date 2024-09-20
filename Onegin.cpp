#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys\stat.h>
#include <assert.h>
#include <TXlib.h>

void SortText (char** index, int str);

int Compare_lines (const char* s1, const char* s2);

void Swap(char** swap1, char** swap2);

void ScanText ();

void PrintText(char** index, int nlines);

int main()
{
    ScanText();

    return 0;
}

void ScanText ()
{
    struct stat buffer = {};
    stat("OneginText.txt", &buffer);//buffer.st_size unsigned long int

    char *text = (char*) calloc(buffer.st_size, sizeof(char));

    FILE* scan = fopen ("OneginText.txt", "rb");

    assert(scan);

    fread (text, buffer.st_size, 1, scan);

    int nlines = 0;
    for (int i = 0; i < buffer.st_size; i++)
    {
        if (text[i] =='\r')
        {
            assert(text[i]);
            text[i+1] = '\0';
            text[i] = '\n';
            nlines++;
            //printf ("%d\n", text[i]);
        }

    }

    char** index = (char**)calloc(nlines, sizeof(char*));
    int str = 1;
    index[0] = text;
    for (int j = 0; j < buffer.st_size; j++)
    {
        if (text[j] == '\0')
        {
            index[str] = &text[j] + 1;
            str++;
            //printf("%d\n", index[str]);
        }
    }

    fclose(scan);

    /*for (int ind = 0; ind < nlines; ind++)
    {
        printf ("%s\n", index[ind]);
    }*/

    SortText(index, nlines);
    PrintText(index, nlines);
}
void PrintText(char** index, int nlines)
{
    for (int ind = 0; ind < nlines; ind++)
    {
        printf ("%s\n", index[ind]);
    }
}
void SortText (char** index, int str)
{
    int nlines = str;

    while (nlines > 0)
    {
        for (int i = 0; i < nlines - 1; i++)
        {
            if (Compare_lines(index[i], index[i + 1]) > 0)
            {
                //printf ("строка 1 %s\n строка 2 %s\n", index[i], index[i+1] );
                Swap(&index[i], &index[i + 1]);
            }
        }
        /*for (int ind = 0; ind < nlines; ind++)
        {
            printf ("%s\n", index[ind]);
        }*/
        nlines--;
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
