#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

long number_parse(const char* bencoded_value) {
    if (bencoded_value == NULL) return 0;
    if (bencoded_value[0] != 'i') return 0;

    return strtol(bencoded_value + 1, NULL, 10);
}

/*
    [b encode]
    protocol : https://wiki.theory.org/BitTorrentSpecification#Bencoding
    
    <length>:<contents>
    "5:hello"
    i-52e
*/
char* decode_command_handler(const char* bencoded_value) {
    
    if (is_digit(bencoded_value[0])) { // 맨 앞자리가 숫자일 것으로 기대되므로
        int length = atoi(bencoded_value);
        const char* colon_index = strchr(bencoded_value, ':');
        if (colon_index != NULL) {
            const char* start = colon_index + 1;
            char* decoded_str = (char*)malloc(length + 1);
            strncpy(decoded_str, start, length);
            decoded_str[length] = '\0';
            return decoded_str;
        } else {
            fprintf(stderr, "Invalid encoded value: %s\n", bencoded_value);
            exit(1);
        }
    } else if(bencoded_value[0] == 'i') { // i-52e
        long num = number_parse(bencoded_value);
        char buff[1024];
        printf("%ld\n", num);
        exit(EXIT_SUCCESS);
    }
    else {
        fprintf(stderr, "Only strings are supported at the moment\n");
        exit(1);
    }
}

int main(int argc, char** argv) {
	// Disable output buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);

    if (argc < 3) {
        fprintf(stderr, "Usage: your_bittorrent.sh <command> <args>\n");
        return 1;
    }

    



    const char* command = argv[1];
    if (strcmp(command, "decode") == 0) {
        const char* encoded_str = argv[2];
        char* decoded_str = decode_command_handler(encoded_str);
        printf("\"%s\"\n", decoded_str);
        free(decoded_str);
    } else {
        fprintf(stderr, "Unknown command: %s\n", command);
        return 1;
    }

    return 0;
}
