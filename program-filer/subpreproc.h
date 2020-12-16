
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char **preprocessing(char *ori_file, int *sc)
{
    int count = 0;
    int sentence_count = 0;
    int count_holder = 0;
    int size_of_string[100];
    int character_count = 0;
    int count2 = 0;
    int max_size = 0;
    int total_character = 0;
    int size_of_ori_file = strlen(ori_file);

    for (int i = 0; i < size_of_ori_file; i++) //Indtil sætningen er slut
    {
        character_count++; //Tæller hvor mange karakterer der er i sætningen

        if (ori_file[count] == '\n') //Hvis sætningen er slut
        {
            sentence_count++;                         //Tilføjer et antal sætning
            size_of_string[count2] = character_count; //Indsætter antallet af karakterer i et nyt array
            count2++;
            total_character = total_character + character_count;
            character_count = 0; //Nulstiller karakterer, da der startes en ny sætning
        }
        count++; //Går videre til næste karakter i filen
    }
    size_of_string[count2] = size_of_ori_file - total_character;
    sentence_count++;

    //sentence_count++; // for sidste sætning - TEST
    char **sentence_arr = malloc((sentence_count) * sizeof(char *));
    *sc = sentence_count;

    if (sentence_arr == 0)
    {
        printf("Failure accured");
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < sentence_count; i++)
    {
        sentence_arr[i] = (char *)malloc(size_of_string[i] * sizeof(char));

        if (sentence_arr == 0)
        {
            printf("Failure accured");
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < sentence_count; i++)
    {
        if (max_size < size_of_string[i])
        {
            max_size = size_of_string[i];
        }
    }

    sentence_count = 0; //Nulstiller
    count = 0;
    count_holder = 0;
    //char temp_arr[max_size];
    for (int i = 0; i < size_of_ori_file; i++)
    {
        if (ori_file[count] == '\n')
        {
            ori_file[count] = '\0'; //Indsætter \0 i slutningen er hver sætning
            count++;

            for (int j = 0; j < (count - count_holder); j++)
            { //Count er placeringen af den sidste karakter og count_holder er placeringen af den første
                //printf("S: %d  j: %d  B: %c\n", sentence_count, j, ori_file[j + count_holder]);
                sentence_arr[sentence_count][j] = ori_file[j + count_holder]; //Bliver placeret i et nyt array
            }

            sentence_count++;
            count_holder = count;
        }
        count++;
    }
    for (int i = 0; i < (size_of_ori_file - count_holder); i++)
    {
        sentence_arr[sentence_count][i] = ori_file[i + count_holder];
    }

    return sentence_arr;
}
