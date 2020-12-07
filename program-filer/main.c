/*
* Titel: Automatisk Plagiat Kontrol
* Software - 1. semester (09/10/2020 - 18/12/2020)
* Aalborg universitet CPH
* 
* Gruppe: SW0001
* - Markus Hye-Knudsen, Oscar Maxwell
* - Sara Granquist, Tommy Grenaae
* - Frederik Bode Thorbensen
*/

// System Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Gruppens Headers
#include "subtools.h"
#include "subcryptic.h"
#include "subloadfile.h"
#include "subpreproc.h"
#include "subverbatim.h"
#include "prototypes.h"

// Main Function
int main(void) {
    run_checks();
    return EXIT_SUCCESS;
}

void run_checks() {
    char *fp_one = "./test-files/lotr-org.txt";
    char *fp_two = "./test-files/lotr-plag.txt";

    //param[in] : File path to txt-file.
    //param[out]: Array with all text in txt.
    char *arr_txt1 = load_file(fp_one);
    char *arr_txt2 = load_file(fp_two);

    //Preprocessing
    int sc_one = 0;
    int sc_two = 0;
    char **pre_arr = preprocessing(arr_txt1, &sc_one);
    char **pre_arr2 = preprocessing(arr_txt2, &sc_two);

    // free dynamic allocated file arrays
    free(arr_txt1);
    free(arr_txt2);

    // List of PlagMatch Struct elements
    PlagMatch vMatches[1];
    PlagMatch cMatches[1];
    
    // Verbatim
    nverbatim(pre_arr, pre_arr2, sc_one, sc_two, vMatches);

    // TEST ONLY
    char test_str1[] = "The quick brown fox jumps over the lazy dog";
    char test_str2[] = "The quick browп fox jumps over the lazy dog";

    // check strings for potential matches.
    int check_count = 1;
    for (int i = 0; i < sc_one; i ++) {
        for (int j = 0; j < sc_two; j++) {
            printf("CHECK %d:\n", check_count);
            check_string(pre_arr[i], pre_arr2[j]);
            printf("String i = %s \n", pre_arr[i]);
            printf("String j - %s\n\n", pre_arr2[j]);
            check_count++;
        }
    }

    //param[in] : the two strings to be compared
    //param[out]: match or non-match;
    locate_cryptic(test_str1, test_str2, cMatches);

    // param[in] : all the PlagMatch arrays and their sizes
    int vmSize = (int)(sizeof(vMatches) / sizeof(PlagMatch));
    int cmSize = (int)(sizeof(cMatches) / sizeof(PlagMatch));
    eval_results(vMatches, vmSize, cMatches, cmSize, fp_one, fp_two);

    free(pre_arr);
    free(pre_arr2);
}

//param[in] : text file of type .txt
//param[out]: char array (pointer) with text from file in array
char *load_file(char fp[]) {
    FILE *file = fopen(fp, "r");

    //param[in] : opened file in read mode
    //param[out]: returns if file is empty or not
    check_file(file);

    //param[in] : opened file in read mode
    //param[out]: number of char in file as size_t
    size_t size_of_arr = calc_sarray(file);
    char *txt_arr = malloc(size_of_arr * sizeof(char));

    //param[in] : content of file, array, size of array
    //param[out]: array with content of file
    write_array(file, txt_arr, size_of_arr);

    if (txt_arr != NULL) {
        printf("File: %s load success\n", fp);
    } else {
        printf("File: %s load unsuccesfull\n", fp);
    }

    fclose(file);
    return txt_arr;
}

void locate_cryptic(char str_one[], char str_two[], PlagMatch *cMatches) {
    int wc_one = count_words(str_one); // get number of words in string 1
    int wc_two = count_words(str_two); // get number of words in string 2

    // Create wordlist, split sentences into words and fill wordlist
    char **wordlist_one = malloc(wc_one * sizeof(char *));
    sentence_splliter(str_one, wordlist_one, wc_one);

    // Create wordlist, split sentences into words and fill wordlist
    char **wordlist_two = malloc(wc_two * sizeof(char *));
    sentence_splliter(str_two, wordlist_two, wc_two);

    // checking for cryptic characters
    bool result = check_crypt(wordlist_one, wc_one, wordlist_two, wc_two);
    printf("Cryptic check result: %s \n", result ? "true" : "false");

    if (result)
    {
        PlagMatch found_match;
        strcpy(found_match.text, str_one);
        found_match.word_num = 0;
        found_match.line_num = 0;
        strcpy(found_match.match_text, str_two);
        found_match.match_word_num = 0;
        found_match.match_line_num = 0;

        cMatches[0] = found_match;
    }

    free(wordlist_one);
    free(wordlist_two);
}

//param[in] : array of PlagMatches, sizes of arrays and file paths
void eval_results(PlagMatch *vMatches, int vmSize, PlagMatch *cMatches, int cmSize, char fp_one[], char fp_two[])
{
    printf("\n-----------------------------------------------\n");
    printf("RESULTS FROM EVALUATION:\n");
    printf("-----------------------------------------------\n");
    printf("FILE: %s\n", fp_one);
    printf("CHECKED AGAINST: %s\n", fp_two);
    printf("-----------------------------------------------\n\n");

    printf("VERBATIM - (found %d matches):\n\n", vmSize);
    for (int i = 0; i < vmSize; i++)
    {
        printf("- File 1 [line %2d, match]: '%s'\n", vMatches[i].line_num, vMatches[i].text);
        printf("- File 2 [line %2d, match]: '%s'\n\n", vMatches[i].match_line_num, vMatches[i].match_text);
    }
    printf("\n");

    int plagSize = sizeof(vMatches[0]);
    printf("Size of plag obj: %d\n", plagSize);

    printf("CRYPTIC (found %d matches):\n\n", cmSize);
    for (int i = 0; i < cmSize; i++)
    {
        printf("- File 1 [line %2d, word %2d]: %s\n", cMatches[i].line_num, cMatches[i].word_num, cMatches[i].text);
        printf("- File 2 [line %2d, word %2d]: %s\n\n", cMatches[i].match_line_num, cMatches[i].match_word_num, cMatches[i].match_text);
    }
}
