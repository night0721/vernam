#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KEY_LENGTH 4096
void genkey(int length, char *key) {
    srand(time(NULL));
    for (int i = 0; i < length; ++i) {
        key[i] = rand() % 256;
    }
}

void vernam(char *s, char *t, int length, char *result) {
    for (int i = 0; i < length; ++i) {
        result[i] = s[i] ^ t[i];
    }
}

void writeKeyToFile(char *key, int key_count) {
    char filename[9]; // 4 digits only
    for (int i = 0; i < key_count; ++i) {
        sprintf(filename, "key-%d", i + 1);
        FILE *key_file = fopen(filename, "wb"); // b means binary
        if (key_file == NULL) {
            fprintf(stderr, "ERROR: Cannot open key file.\n");
            exit(EXIT_FAILURE);
        }
        // writes KEY_LENGTH items of data, each 1 byte long, to the
        // stream key_file pointed to by stream, obtaining them from the
        // location given by pointer key.
        fwrite(key, 1, KEY_LENGTH, key_file);
        fclose(key_file);
    }
}

void vernam_files(char *input_file, char* key_file, char *output_file) {
    FILE *input = fopen(input_file, "rb");
    FILE *key = fopen(key_file, "rb");

    if (input == NULL || key == NULL) {
        fprintf(stderr, "ERROR: Cannot open input or key file.\n");
        exit(EXIT_FAILURE);
    }

    // Get the length of the input file
    fseek(input, 0, SEEK_END);
    int length = ftell(input);
    rewind(input);

    char *message = malloc(length);
    char *keybuf = malloc(length);
    fread(message, 1, length, input);
    fread(keybuf, 1, length, key);

    fclose(input);
    fclose(key);

    char encrypted[length];
    vernam(message, keybuf, length, encrypted);
    
    FILE *out = fopen(output_file, "wb");
    if (out == NULL) {
        fprintf(stderr, "ERROR: Cannot open output file.\n");
        exit(EXIT_FAILURE);
    }

    fwrite(encrypted, 1, length, out);
    fclose(out);

    free(message);
    free(keybuf);
}

int main(int argc, char *argv[]) {
    char *usage = "Usage: %s -i <input_file> -k <key_file> -o <output_file> | -g <keys_count> | -h\n";
    if (argc < 2) {
        fprintf(stderr, usage, argv[0]);
        exit(EXIT_FAILURE);
    }
    if (strncmp(argv[1], "-i", 2) == 0 && argc == 7 && strncmp(argv[3], "-k", 2) == 0 && strncmp(argv[5], "-o", 2) == 0) {
        vernam_files(argv[2], argv[4], argv[6]);
    } else if (strncmp(argv[1], "-g", 2) == 0 && argc == 3) {
        int key_count = atoi(argv[2]);
        if (key_count <= 0 || key_count > 9999) {
            fprintf(stderr, "vernam: Key count must be more than 0 and less than 9999.\n");
            exit(EXIT_FAILURE);
        }
        char key[KEY_LENGTH];
        genkey(KEY_LENGTH, key);
        
        writeKeyToFile(key, key_count);
    } else if (strncmp(argv[1], "-h", 2) == 0 && argc == 2) {
        printf("Vernam Cipher Tool\n");
        printf(usage, "vernam");
        printf("Example: vernam -i message_to_tom -k key-1 -o encrypted_to_tom\n");
        printf("To acquire a truly random and secure key, use\nvernam -g <keys_count>\n\n");
        printf("Made with <3 by night0721. Licensed under GPL v3.\n");
    } else {
        fprintf(stderr, usage, argv[0]);
        exit(EXIT_FAILURE);
    }
    return 0;
}
