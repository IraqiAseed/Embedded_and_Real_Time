
#include "../include/string_utils.h"
#include "../tests/test_string_utils.h"

#include <stdlib.h> //TODO delete
#include <stdio.h>  //TODO delete
int main()
{

    run_reverse_tests();
    run_palindrome_tests();
    run_atoi_tests();
    run_itoa_tests();
    run_reverse_words_tests();
    run_count_words_tests();
    return 0;
}