#include <stdio.h>
#include <stdlib.h>

const int lines = 14, lines_len = 100;

void SortText (char text[lines][lines_len]);

int Compare_lines (const char* s1, const char* s2);

void Swap(int* swap1, int* swap2);

int main()
{
    FILE* scan = fopen ("OneginText.txt", "r");
    char text [lines][lines_len] = {};
    for (int i = 0; i < lines; ++i) {
        fgets(text[i],lines_len,scan);
    }
    //  printf ("1");
    fclose (scan);
    SortText(text);

    return 0;
}

void SortText (char text[lines][lines_len])
{
    int index[lines] = {};

    for (int n_lines = 0; n_lines < lines; n_lines++)
    {
        index[n_lines] = n_lines;
            //printf ("2");
    }

    int nlines = lines - 1;

    while (nlines > 0)
    {
        for (int i = 0; i < nlines; i++)
        {
            if (Compare_lines(text[index[i]], text[index[i + 1]]) > 0)
            {
                Swap(&index[i], &index[i + 1]);
            }
        }
        nlines--;
    }


    for (int ind = 0; ind < lines; ind++)
    {
        printf ("%s", text[index[ind]]);
    }
}

int Compare_lines (const char* s1, const char* s2)
{
    int i = 0;

    while (i < lines_len && ((int) s1[i] == (int) s2[i]))
    {
        i++;
    }

    if (i == lines_len)
    {
        return 0;
    }
    else
    {
        return ((int) s1[i] - (int) s2[i]);
    }
}

void Swap(int* swap1, int* swap2)
{
    int a = *swap1;
    *swap1 = *swap2;
    *swap2 = a;
}
