#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys\stat.h>
#include <assert.h>
#include <TXlib.h>

struct Sort
{
    char* address;
    int lineslen;
};
struct Inftext
{
    int nlines;
    char* text;
    Sort* addr;
};

void SortText (Inftext* onegin);

int Compare_lines (const char* s1, const char* s2, int len1, int len2);

//void Swap_addr(char** swap1, char** swap2);

void Swap(void* swap1, void* swap2, size_t size);

void ScanText(Inftext* onegin, unsigned long size_f);

void PrintText(Inftext* onegin);

void GetFileSize(Inftext* onegin, unsigned long size_f);

int comp (const void* s1, const void* s2);

int main()
{
    struct Inftext onegin = {};
    struct stat buffer = {};

    stat("OneginText.txt", &buffer);//buffer.st_size unsigned long int

    onegin.text = (char*) calloc(buffer.st_size, sizeof(char));

    onegin.nlines = 0;

    ScanText(&onegin, buffer.st_size);
    onegin.addr = (Sort*) calloc(onegin.nlines, sizeof(Sort));

    GetFileSize(&onegin, buffer.st_size);

    SortText(&onegin);

    PrintText(&onegin);

    free(onegin.text);
    free(onegin.addr);

    return 0;
}

void ScanText(Inftext* onegin, unsigned long size_f)
{
    FILE* scan = fopen ("OneginText.txt", "rb");

    if (scan == NULL)
    {
        printf("Error text1");
        exit(1);
    }

    if (fread (onegin->text, size_f, 1, scan) == 0)
    {
        printf("Error text2");
        exit(1);
    }

    for (unsigned int i = 0; i < size_f; i++)
    {
        if ((onegin->text)[i] =='\r')
        {
            (onegin->text)[i+1] = '\0';
            (onegin->text)[i] = '\n';
            onegin->nlines++;
        }
    }
}

void PrintText(Inftext* onegin)
{
    for (int ind = 0; ind < onegin->nlines; ind++)
    {
        printf ("%s\n", (onegin->addr)[ind].address);
    }
}

void GetFileSize(Inftext* onegin, unsigned long size_f)
{
    int str = 1;
    int len = 0;
    (onegin->addr)[0].address = onegin->text;
    for (unsigned int j = 0; j < size_f; j++)
    {
        if (onegin->text[j] == '\0')
        {
            if (j + 1 == size_f)
            {
                break;
            }
            (onegin->addr)[str].address = &(onegin->text)[j] + 1;
            (onegin->addr)[len].lineslen = (onegin->addr)[str].address - (onegin->addr)[str-1].address;


            //printf("%d\n", onegin->lineslen[len]);
            len++;
            str++;
            //printf("%d\n", index[str]);
        }
    }
    (onegin->addr)[onegin->nlines-1].lineslen = &(onegin->text)[size_f] - (onegin->addr)[onegin->nlines-1].address;

    /*for (int i = 0; i < onegin->nlines; i++)
    {
        printf("numb line %d len %d\n", i, (onegin->addr)[i].lineslen);
    }*/
}
void SortText (Inftext* onegin)
{
    //printf("1");
    qsort(onegin->addr, onegin->nlines, sizeof(Sort), comp);//sort lines len to address

    /*int str = onegin->nlines - 1;

    while (str > 0)
    {
        for (int i = 0; i < str ; i++)
        {
            if (Compare_lines(addr, (addr->address)[i], (addr->address)[i + 1], i) > 0)
            {
                //printf("swap 1 %s\nswap 2 %s\n", (addr->address)[i], (addr->address)[i + 1]);
                Swap(&(addr->address)[i], &(addr->address)[i + 1], sizeof(char**));
                Swap(&(addr->lineslen)[i], &(addr->lineslen)[i + 1], sizeof(int*));
            }
        }
        for (int ind = 0; ind < onegin->nlines; ind++)
        {
            printf ("%s\n", onegin->address[ind]);
        }
        str--;
    }*/

}

int comp (const void* elem1, const void* elem2)
{
    return Compare_lines((((const Sort*)elem1)->address), (((const Sort*)elem2)->address),
                         (((const Sort*)elem1)->lineslen), (((const Sort*)elem2)->lineslen));
}

int Compare_lines (const char* s1, const char* s2, int len1, int len2)
{
    int elem1 = len1 - 1 , elem2 = len2 - 1;
    //printf ("len1 %d\nlen2 %d\n", len1, len2);

    //printf ("compare line 1 %s\ncompare line 2 %s\n\n", s1, s2);

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
            //printf("delta %d\n",tolower(s1[elem1]) - tolower(s2[elem2]) );
            //printf("%c %d\n%c %d\n", s1[elem1], len1 - elem1 - 2, s2[elem2], len2-elem2 - 2);
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

/*void Swap_len(int* swap1, int* swap2)
{
    int a = *swap1;
    *swap1 = *swap2;
    *swap2 = a;
}*/
