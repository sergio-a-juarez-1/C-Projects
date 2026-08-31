#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    // Seed the random number generator using system time
    srand(time(NULL));

    printf("Welcome to the CLI-Password Generator!\n");

    int letters_1 = 0, symbols_1 = 0, numbers_count = 0;

    printf("How many letters would you like in your password?\n");
    if (scanf("%d", &letters_1) != 1) letters_1 = 0;

    printf("How many symbols would you like?\n");
    if (scanf("%d", &symbols_1) != 1) symbols_1 = 0;

    printf("How many numbers would you like?\n");
    if (scanf("%d", &numbers_count) != 1) numbers_count = 0;

    // Define pool lookup configurations
    const char letters[] = "abcdefghijklmnopqrstuvwxyz";
    const char symbols[] = "!@#$%^&*";
    const char numbers[] = "0123456789";

    int total_length = letters_1 + symbols_1 + numbers_count;
    
    // Allocate space for password string (+1 buffer for null-terminator)
    char *password = (char *)malloc((total_length + 1) * sizeof(char));
    if (password == NULL) {
        printf("Memory allocation error!\n");
        return 1;
    }

    int index = 0;

    // 1. Process and load randomized letters (with coin-flip capitalization)
    for (int s = 0; s < letters_1; s++) {
        int selector = rand() % 26;
        int cap = rand() % 2;
        if (cap == 1) {
            password[index++] = letters[selector];
        } else {
            // Convert to uppercase by subtracting ASCII difference
            password[index++] = letters[selector] - 32;
        }
    }

    // 2. Process and load randomized symbols 
    for (int s = 0; s < symbols_1; s++) {
        int selector = rand() % 8;
        password[index++] = symbols[selector];
    }

    // 3. Process and load randomized numbers
    for (int nums = 0; nums < numbers_count; nums++) {
        int rand_num = rand() % 10;
        password[index++] = numbers[rand_num];
    }

    // Null-terminate the raw text stream array 
    password[index] = '\0';

    // 4. Shuffle the character array using the Fisher-Yates Algorithm
    for (int i = total_length - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Temporary swap variable mechanics
        char temp = password[i];
        password[i] = password[j];
        password[j] = temp;
    }

    // Print resulting generated password configuration
    printf("Here is your password: %s\n", password);

    // Free dynamic heap memory allocations cleanly
    free(password);

    return 0;
}
