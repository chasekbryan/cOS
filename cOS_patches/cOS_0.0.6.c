#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_CMD_LEN 128
#define MAX_ARGS    16
#define HASH_FILE   ".cos_hash"

void print_banner() {
    printf("========================================\n");
    printf("        Welcome to cOS (v0.0.6)           \n");
    printf("========================================\n\n");
    printf("Type 'help' to list available commands.\n\n");
}

void print_help() {
    printf("Available commands:\n");
    printf("  help              : Show this help message\n");
    printf("  cls               : Clear the screen\n");
    printf("  echo [msg]        : Print message\n");
    printf("  time              : Show current date and time\n");
    printf("  ls                : List files and show storage info\n");
    printf("  run <script.py>   : Execute a Python script\n");
    printf("  calc              : Launch calculator\n");
    printf("  hash [regen]      : Generate or regenerate persistent hash\n");
    printf("  exit              : Exit cOS\n");
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
    if (system(cmd) == -1) {
        printf("Failed to run script: %s\n", script);
    }
}

char *generate_random_hash() {
    static char hash[33]; // 32 hex chars + null
    const char *hex = "0123456789abcdef";
    srand((unsigned)time(NULL));
    for (int i = 0; i < 32; i++) {
        hash[i] = hex[rand() % 16];
    }
    hash[32] = '\0';
    return hash;
}

bool file_exists(const char *path) {
    FILE *f = fopen(path, "r");
    if (f) { fclose(f); return true; }
    return false;
}

void save_hash(const char *hash) {
    FILE *f = fopen(HASH_FILE, "w");
    if (!f) {
        printf("Error: Unable to write hash file.\n");
        return;
    }
    fprintf(f, "%s", hash);
    fclose(f);
}

char *load_hash() {
    static char hash[33];
    FILE *f = fopen(HASH_FILE, "r");
    if (!f) return NULL;
    if (fgets(hash, sizeof(hash), f) == NULL) {
        fclose(f);
        return NULL;
    }
    fclose(f);
    return hash;
}

void handle_hash(bool regen) {
    char *hash;
    if (!file_exists(HASH_FILE) || regen) {
        hash = generate_random_hash();
        save_hash(hash);
        printf("New hash generated: %s\n", hash);
    } else {
        hash = load_hash();
        if (hash) printf("Existing hash: %s\n", hash);
        else printf("Error: Failed to load existing hash.\n");
    }
}

void calculator() {
    char expr[MAX_CMD_LEN];
    double a, b;
    char op;
    printf("Entering calculator mode. Type 'exit' to return.\n");
    while (1) {
        printf("calc> ");
        if (!fgets(expr, sizeof(expr), stdin)) break;
        if (strncmp(expr, "exit", 4) == 0) break;
        if (sscanf(expr, "%lf %c %lf", &a, &op, &b) == 3) {
            switch (op) {
                case '+': printf("= %lf\n", a + b); break;
                case '-': printf("= %lf\n", a - b); break;
                case '*': printf("= %lf\n", a * b); break;
                case '/':
                    if (b != 0) printf("= %lf\n", a / b);
                    else printf("Error: Division by zero.\n");
                    break;
                default: printf("Unknown operator: %c\n", op);
            }
        } else {
            printf("Usage: <num> <op> <num> (e.g., 2 + 3)\n");
        }
    }
}

int main() {
    char input[MAX_CMD_LEN];
    char *args[MAX_ARGS];
    int argc;

    print_banner();

    while (1) {
        printf("cOS> ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n");
            break;
        }
        input[strcspn(input, "\n")] = '\0';

        argc = 0;
        char *token = strtok(input, " ");
        while (token && argc < MAX_ARGS - 1) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        if (argc == 0) continue;

        if (strcmp(args[0], "help") == 0) {
            print_help();
        } else if (strcmp(args[0], "cls") == 0) {
            clear_screen();
        } else if (strcmp(args[0], "echo") == 0) {
            for (int i = 1; i < argc; i++) printf("%s ", args[i]);
            printf("\n");
        } else if (strcmp(args[0], "time") == 0) {
            show_time();
        } else if (strcmp(args[0], "ls") == 0) {
            list_files();
            show_storage();
        } else if (strcmp(args[0], "run") == 0) {
            if (argc < 2) printf("Usage: run <script.py> [args]\n");
            else run_python(args[1], &args[1]);
        } else if (strcmp(args[0], "calc") == 0) {
            calculator();
        } else if (strcmp(args[0], "hash") == 0) {
            bool regen = (argc > 1 && strcmp(args[1], "regen") == 0);
            handle_hash(regen);
        } else if (strcmp(args[0], "exit") == 0) {
            break;
        } else {
            printf("Unknown command: %s. Type 'help' for available commands.\n", args[0]);
        }
    }

    return 0;
}
