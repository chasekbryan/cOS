#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CMD_LEN 128
#define MAX_ARGS    16

void print_banner() {
    printf("========================================\n");
    printf("        Welcome to cOS (v0.0.1)           \n");
    printf("  A Simple Text-based OS Emulator in C   \n");
    printf("========================================\n\n");
}

void print_help() {
    printf("Available commands:\n");
    printf("  help      : Show this help message\n");
    printf("  cls       : Clear the screen\n");
    printf("  echo [msg]: Print message\n");
    printf("  time      : Show current date and time\n");
    printf("  exit      : Exit cOS\n\n");
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void show_time() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Current time: %s\n", buffer);
}

int main() {
    char input[MAX_CMD_LEN];
    char *args[MAX_ARGS];

    print_banner();
    print_help();

    while (1) {
        printf("cOS> ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n");
            break;
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Tokenize input
        int argc = 0;
        char *token = strtok(input, " ");
        while (token != NULL && argc < MAX_ARGS - 1) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        if (argc == 0) continue;

        // Command handling
        if (strcmp(args[0], "help") == 0) {
            print_help();
        } else if (strcmp(args[0], "cls") == 0) {
            clear_screen();
        } else if (strcmp(args[0], "echo") == 0) {
            for (int i = 1; i < argc; i++) {
                printf("%s ", args[i]);
            }
            printf("\n");
        } else if (strcmp(args[0], "time") == 0) {
            show_time();
        } else if (strcmp(args[0], "exit") == 0) {
            printf("Exiting cOS...\n");
            break;
        } else {
            printf("Unknown command: %s. Type 'help' for a list of commands.\n", args[0]);
        }
    }

    return 0;
}
