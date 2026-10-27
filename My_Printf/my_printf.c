#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int malloc_length(long number, int division) {
    int length = 0;

    while (number != 0) {
        length++;
        number /= division;
    }

    return length;
}

char *my_strdup(char *str) {
    int length;
    for (length = 0; str[length] != 0; length++);

    char *copy = malloc(length + 1);
    if (copy == NULL) return NULL;
    
    for (int index = 0; str[index] != 0; index++) {
        copy[index] = str[index];
    }

    return copy;                            // Return the new string
}

int my_printf(char *string, ...) {
    va_list specifiers;
    va_start(specifiers, string);

    char *new_string = my_strdup(string);
    char *write_string = new_string;
    int memory_size = 0;
    int specifier_length = 0;
    char *arg;
    char *malarg = NULL;

    int argi;
    long long_arg;

    uintptr_t pointer_arg;

    for (memory_size = 0; write_string[memory_size] != 0; memory_size++);
    
    for (int i = 0; write_string[i] != 0; i++) {
        if (write_string[i] == '%') {
            switch (write_string[i+1]) {
                case 'd':
                    argi = va_arg(specifiers, int);

                    specifier_length = malloc_length(argi, 10);
                    
                    malarg = malloc(specifier_length);

                    if (malarg == NULL) {
                        return (-1);
                    }

                    if (argi < 0) {
                        malarg[0] = '-';

                        argi *= -1;
                        for (int itoa = 0; itoa < specifier_length; itoa++) {
                            malarg[specifier_length - itoa] = (argi % 10) + '0';
                            argi /= 10;
                        }
                        specifier_length++;
                    }
                    else {
                        for (int itoa = 0; itoa < specifier_length; itoa++) {
                            malarg[specifier_length - itoa - 1] = (argi % 10) + '0';
                            argi /= 10;
                        }
                    }

                    if (specifier_length >= 2) {
                        memory_size += specifier_length - 2;

                        if (memory_size - specifier_length > i && specifier_length >= 2) {
                            for (int move_characters = memory_size; move_characters > i; move_characters--) {
                                write_string[move_characters + specifier_length - 2] = write_string[move_characters];
                            }
                        }

                        for (int place_string = 0; place_string < specifier_length; place_string++) {
                            write_string[i+place_string] = malarg[place_string];
                        }
                    }
                    else {
                        write_string[i+1] = malarg[0];
                        for (int move_characters = i+1; move_characters < memory_size; move_characters++) {
                            write_string[move_characters-1] = write_string[move_characters];
                        }

                        memory_size--;
                    }
                    free(malarg);
                    break;
                case 'o':
                    argi = va_arg(specifiers, int);
                    long_arg = argi;

                    if (argi >= 0) {
                        long_arg = argi;
                    }
                    else {
                        long_arg = 4294967296 + argi;
                    }

                    malarg = malloc(malloc_length(long_arg, 8) + 1);

                    if (malarg == NULL) {
                        return (-1);
                    }

                    specifier_length = malloc_length(long_arg, 8);

                    for (int itoa = 0; itoa < specifier_length; itoa++) {
                        malarg[itoa] = (long_arg % 8) + '0';
                        long_arg /= 8;
                    }

                    if (specifier_length >= 2) {
                        memory_size += specifier_length - 2;

                        if (memory_size - specifier_length > i) {
                            for (int move_characters = memory_size; move_characters > i; move_characters--) {
                                write_string[move_characters + specifier_length - 2] = write_string[move_characters];
                            }
                        }

                        for (int place_string = 0; place_string < specifier_length; place_string++) {
                            write_string[i+place_string] = malarg[specifier_length - place_string - 1];
                        }
                    }
                    else {
                        write_string[i+1] = malarg[0];
                        for (int move_characters = i+1; move_characters < memory_size; move_characters++) {
                            write_string[move_characters-1] = write_string[move_characters];
                        }

                        memory_size--;
                    }
                    free(malarg);
                    break;
                case 'u':
                    argi = va_arg(specifiers, long);

                    if (argi >= 0) {
                        long_arg = argi;
                    }
                    else {
                        long_arg = 4294967296 + argi;
                    }

                    specifier_length = malloc_length(long_arg, 10);

                    malarg = malloc(malloc_length(long_arg, 10) + 1);

                    if (malarg == NULL) {
                        return (-1);
                    }

                    for (int itoa = 0; itoa < specifier_length; itoa++) {
                        malarg[specifier_length - itoa - 1] = (long_arg % 10) + '0';
                        long_arg /= 10;
                    }

                    if (specifier_length >= 2) {
                        memory_size += specifier_length - 2;

                        if (memory_size - specifier_length > i) {
                            for (int move_characters = memory_size; move_characters > i; move_characters--) {
                                write_string[move_characters + specifier_length - 2] = write_string[move_characters];
                            }
                        }

                        for (int place_string = 0; place_string < specifier_length; place_string++) {
                            write_string[i+place_string] = malarg[place_string];
                        }
                    }
                    else {
                        write_string[i+1] = malarg[0];
                        for (int move_characters = i+1; move_characters < memory_size; move_characters++) {
                            write_string[move_characters-1] = write_string[move_characters];
                        }

                        memory_size--;
                    }
                    free(malarg);
                    break;
                case 'x':
                    argi = va_arg(specifiers, int);

                    specifier_length = 0;

                    if (argi >= 0) {
                        long_arg = argi;
                    }
                    else {
                        long_arg = 4294967296 + argi;
                    }

                    malarg = malloc(malloc_length(long_arg, 16) + 1);

                    while (long_arg != 0) {
                        switch (long_arg % 16) {
                            case 10:
                                malarg[specifier_length] = 'a';
                                break;
                            case 11:
                                malarg[specifier_length] = 'b';
                                break;
                            case 12:
                                malarg[specifier_length] = 'c';
                                break;
                            case 13:
                                malarg[specifier_length] = 'd';
                                break;
                            case 14:
                                malarg[specifier_length] = 'e';
                                break;
                            case 15:
                                malarg[specifier_length] = 'f';
                                break;
                            default:
                                malarg[specifier_length] = (long_arg % 16) + '0';
                                break;
                        }

                        long_arg /= 16;
                        specifier_length++;
                    }
                    
                    if (specifier_length >= 2) {
                        memory_size += specifier_length - 2;

                        if (memory_size - specifier_length > i) {
                            for (int move_characters = memory_size; move_characters > i; move_characters--) {
                                write_string[move_characters + specifier_length - 2] = write_string[move_characters];
                            }
                        }

                        for (int place_string = 0; place_string < specifier_length; place_string++) {
                            write_string[i+place_string] = malarg[specifier_length - place_string - 1];
                        }
                    }
                    else {
                        write_string[i+1] = malarg[0];
                        for (int move_characters = i+1; move_characters < memory_size; move_characters++) {
                            write_string[move_characters-1] = write_string[move_characters];
                        }

                        memory_size--;
                    }
                    free(malarg);
                    break;
                case 'c':
                    write_string[i+1] = va_arg(specifiers, int);
                    for (int move_characters = i+1; move_characters < memory_size; move_characters++) {
                        write_string[move_characters-1] = write_string[move_characters];
                    }
                    memory_size--;
                    break;
                case 's':
                    arg = va_arg(specifiers, char*);

                    specifier_length = 0;
                    while (arg[specifier_length] != 0) {
                        specifier_length++;
                    }
                    malarg = malloc(specifier_length);
                    
                    for (int p = 0; p < specifier_length; p++) {
                        malarg[p] = arg[p];
                    }
                    
                    memory_size += specifier_length - 2;

                    if (specifier_length == 1) {
                        write_string[i+1] = malarg[0];
                        for (int move_characters = i+1; move_characters < memory_size+1; move_characters++) {
                            write_string[move_characters-1] = write_string[move_characters];
                        }
                    }
                    else {
                        if (memory_size - specifier_length > i) {
                            for (int move_characters = memory_size; move_characters > i; move_characters--) {
                                write_string[(move_characters + specifier_length - 2)] = write_string[move_characters];
                            }
                        }
                    }
                    
                    for (int place_string = 0; place_string < specifier_length; place_string++) {
                        write_string[i+place_string] = malarg[place_string];
                    }
                    free(malarg);
                    break;
                case 'p':
                    pointer_arg = va_arg(specifiers, uintptr_t);

                    int specifier_length = 0;
                    malarg = malloc(malloc_length(long_arg, 16) + 1);
                    
                    while (pointer_arg != 0) {
                        switch (pointer_arg % 16) {
                            case 10:
                                malarg[specifier_length] = 'a';
                                break;
                            case 11:
                                malarg[specifier_length] = 'b';
                                break;
                            case 12:
                                malarg[specifier_length] = 'c';
                                break;
                            case 13:
                                malarg[specifier_length] = 'd';
                                break;
                            case 14:
                                malarg[specifier_length] = 'e';
                                break;
                            case 15:
                                malarg[specifier_length] = 'f';
                                break;
                            default:
                                malarg[specifier_length] = (pointer_arg % 16) + '0';
                                break;
                        }

                        pointer_arg /= 16;
                        specifier_length++;
                    }

                    memory_size += specifier_length;
                    if (memory_size - specifier_length > i) {
                        for (int move_characters = memory_size; move_characters > i; move_characters--) {
                            write_string[move_characters + specifier_length] = write_string[move_characters];
                        }
                    }

                    write_string[i] = '0';
                    write_string[i+1] = 'x';
                    for (int place_string = 0; place_string < specifier_length; place_string++) {
                        write_string[i+place_string+2] = malarg[specifier_length - place_string - 1];
                    }
                    free(malarg);
                    break;
                case '%':
                    if (memory_size > i + 1) {
                        for (int move_characters = i; move_characters < memory_size; move_characters++) {
                            write_string[move_characters] = write_string[move_characters+1];
                        }
                    }
                    memory_size--;
                    break;
                default:
                    memory_size = 28;
                    write_string = "incomplete format specifier";
                    break;
            }
        }
    }

    va_end(specifiers);

    write(1, write_string, memory_size);
    free(write_string);
    return 0;
}

int main() {
    my_printf();

    return 0;
}