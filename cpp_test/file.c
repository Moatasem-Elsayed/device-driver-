#include <stdio.h>
#include <stdlib.h>
#define PATH "/dev/test_file"
void WriteFile()
{
    char str[100] = {};
    FILE *fptr;
    fptr = fopen(PATH, "w");
    if (fptr == NULL)
    {
        return;
    }
    printf("please enter Data:");
    scanf("%100[a-zA-Z ]", str);

    fprintf(fptr, "%s", str);
    fclose(fptr);
}
void ReadFile()
{
    char str[100] = {};
    FILE *fptr;
    fptr = fopen(PATH, "r");
    if (fptr == NULL)
    {
        return;
    }

    fscanf(fptr, "%[^\n]", str);
    printf("message is %s\n", str);
    fclose(fptr);
}
int main()
{
    WriteFile();
    ReadFile();
    return 0;
}