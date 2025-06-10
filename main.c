#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MIN_LEN 3

void print_directory(const char *path) {
    if (path[0] != '/') return;
    const char *slash = strrchr(path, '/');
    if (slash && slash != path) {
        char dir[256];
        size_t len = slash - path + 1;
        strncpy(dir, path, len);
        dir[len] = 0;
        printf("  Directory : %s\n", dir);
    } else {
        printf("  Directory : /\n");
    }
}

void print_separator() {
    printf("------------------------------------------------------------\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <spiffs.img>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    int c, len = 0;
    char buf[4096];
    long pos = 0, start = 0;
    int in_string = 0;

    printf("==== SPIFFS IMAGE STRING/FILE SCAN REPORT ====\n\n");

    while ((c = fgetc(fp)) != EOF) {
        if (isprint(c)) {
            if (!in_string) {
                start = pos;
                in_string = 1;
            }
            buf[len++] = c;
            if (len >= sizeof(buf) - 1) len = sizeof(buf) - 2;
        } else {
            if (in_string && len >= MIN_LEN) {
                buf[len] = 0;
                if (buf[0] == '/') {
                    print_directory(buf);
                    printf("  File      : %s\n", buf);
                    printf("  Offset    : 0x%06lx\n", start);
                } else {
                    printf("  String    : \"%s\"\n", buf);
                    printf("  Offset    : 0x%06lx\n", start);
                }
                print_separator();
            }
            len = 0;
            in_string = 0;
        }
        pos++;
    }

    if (in_string && len >= MIN_LEN) {
        buf[len] = 0;
        if (buf[0] == '/') {
            print_directory(buf);
            printf("  File      : %s\n", buf);
            printf("  Offset    : 0x%06lx\n", start);
        } else {
            printf("  String    : \"%s\"\n", buf);
            printf("  Offset    : 0x%06lx\n", start);
        }
        print_separator();
    }

    printf("\n============= END OF REPORT =============\n");
    fclose(fp);
    return 0;
}
