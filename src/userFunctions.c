#include "userFunctions.h"

int meni()  
{
    printf("Meni:\n");
    printf("0. Exit\n");
    printf("1. Subscribe\n");
    printf("2. Unsubscribe\n");
    printf("3. Publish\n");
    printf("Your option:\n");

    int selected;
    while(1)
    {
        scanf("%d", &selected);

        if(selected<0 || selected>3)
            printf("Option must be in range [0,3]\nYour option:\n");
        else
            break;
    }

    return selected;
}

int meniCity()
{
    puts("City you want to sub/unsub:");
    puts("0. Exit");
    puts("1. Novi Sad");
    puts("2. Belgrade");
    puts("3. Kragujevac");
    puts("4. Nis");

    int selected;
    while(1)
    {
        printf("Choose city:\n");
        scanf("%d", &selected);

        if(selected<0 || selected>4)
            printf("Option must be in range [0,4]\n");
        else
            break;
    }

    return selected;
}

int meniTopic()
{
    printf("0. Exit\n");
    printf("1. Crypto\n");
    printf("2. Numbers\n");
    printf("3. Pollution\n");
    printf("4. Trump\n");
    printf("5. Weather\n");
    printf("6. Years\n");

    int selected;
    while(1)
    {
        printf("Choose topic:\n");
        scanf("%d", &selected);

        if(selected<0 || selected>6)
            printf("Option must be in range [0,6]\n");
        else
            break;
    }

    return selected;
}

void inputCityToString(int input, char* dst)
{
    switch(input)
    {
        case 1:
            sprintf(dst, "%s", "novi sad\"");
            break;

        case 2:
            sprintf(dst, "%s", "brelgrade\"");
            break;

        case 3:
            sprintf(dst, "%s", "kragujevac\"");
            break;

        case 4:
            sprintf(dst, "%s", "nis\"");
            break;

        default:
            printf("INVALID CITY INPUT\n");
            exit(1);
    }
}

void inputToString(int input, char* dst)
{
    switch(input)
    {
        case 1:
            sprintf(dst, "%s", "sub -t ");
            break;
        
        case 2:
            sprintf(dst, "%s", "unsub -t ");
            break;

        default:
            printf("INVALID INPUT\n");
            exit(1);
    }
}

void inputTopicToString(int input, char* dst)
{
    switch(input)
    {
        case 1:
            sprintf(dst, "%s", "\"crypto\"");
            break;

        case 2:
            sprintf(dst, "%s", "\"numbers\"");
            break;
        
        case 3:
            sprintf(dst, "%s", "\"pollution/");
            break;

        case 4:
            sprintf(dst, "%s", "\"trump\"");
            break;

        case 5:
            sprintf(dst, "%s", "\"weather/");
            break;

        case 6:
            sprintf(dst, "%s", "\"years\"");
            break;

        default:
            printf("INVALID TOPIC INPUT\n");
            exit(1);
    }
}