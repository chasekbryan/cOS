#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CMD_LEN 128
#define MAX_ARGS    16

void print_banner() {
    printf("========================================\n");
    printf("        Welcome to cOS (v0.0.4)           \n");
    printf("  A Simple Text-based OS Emulator in C   \n");
    printf("========================================\n\n");
}

void print_help() {
    printf("Available commands:\n");
    printf("  help              : Show this help message\n");
    printf("  cls               : Clear the screen\n");
    printf("  echo [msg]        : Print message\n");
    printf("  time              : Show current date and time\n");
    printf("  ls                : List files and show storage info\n");
    printf("  run <script.py>   : Execute a Python script\n");
    printf("  exit              : Exit cOS\n\n");
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

void list_files() {
#ifdef _WIN32
    system("dir");
#else
    system("ls -la");
#endif
}

void show_storage() {
#ifdef _WIN32
    printf("\nStorage Information:\n");
    system("wmic logicaldisk get caption,freespace,size");
#else
    printf("\nStorage Information:\n");
    system("df -h .");
#endif
}

void run_python(char *script, char *args[]) {
    char cmd[MAX_CMD_LEN] = {0};
    snprintf(cmd, sizeof(cmd), "python3 %s", script);
    for (int i = 1; args[i] != NULL; i++) {
        strncat(cmd, " ", sizeof(cmd) - strlen(cmd) - 1);
        strncat(cmd, args[i], sizeof(cmd) - strlen(cmd) - 1);
    }
    int ret = system(cmd);
    if (ret == -1) {
        printf("Failed to run script: %s\n", script);
    }
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
        } else if (strcmp(args[0], "ls") == 0) {
            list_files();
            show_storage();
        } else if (strcmp(args[0], "run") == 0) {
            if (argc < 2) {
                printf("Usage: run <script.py> [args]\n");
            } else {
                run_python(args[1], &args[1]);
            }
        } else if (strcmp(args[0], "exit") == 0) {
            printf("Exiting cOS...\n");
            break;
        } else {
            printf("Unknown command: %s. Type 'help' for a list of commands.\n", args[0]);
        }
    }

    return 0;
}
