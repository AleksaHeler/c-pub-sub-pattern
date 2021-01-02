void meni()
{
    printf("Meni:\n");

    //printf("0. Continue recieving messages\n\n");

    printf("1. Subscribe to weather\n");
    printf("11. Unsubscribe to weather\n\n");

    printf("2. Subscribe to pollution\n");
    printf("22. Unsubscribe to pollution\n\n");

    printf("3. Subscribe to years\n");
    printf("33. Unsubscribe to years\n\n");

    printf("4. Subscribe to numbers\n");
    printf("44. Unsbscribe to numbers\n\n");

    printf("5. Subscribe to trump\n");
    printf("55. Unsubscribe to trump\n\n");

    printf("6. Subscribe to crypto market cap\n");
    printf("66. Unsubscribe to crypto market cap\n\n");

    printf("100. Exit\n\n");
}

int checkInputMeni(int input)
{
    if(input == 100)
        return 1;
    else if(input<1 || (input>6 && input<11) || (input>11 && input<22) || (input>22 && input<33) ||
           (input>33 && input<44) || (input>44 && input<55) || (input>55 && input<66) || input>66)
        return 0;

    return 1;
}

void cityMeni()
{
    puts("City you want to be subscribed to:");
    puts("1. Novi Sad");
    puts("2. Belgrade");
    puts("3. Kragujevac");
    puts("4. Nis");
    printf("Your option: ");
}

int checkInputCityMeni(int input)
{
    return input>=1 && input<=4;
}
