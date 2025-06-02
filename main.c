#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PAGE_SIZE 256
#define MAX_FILENAME_LEN 64

int is_filename(const unsigned char *buf, int i) {
    return buf[i] == '/' &&
           isprint(buf[i+1]) &&
           (strstr((char *)&buf[i], ".txt") || strstr((char *)&buf[i], ".json") || strstr((char *)&buf[i], ".py"));
}

int is_deleted(const unsigned char *buf) {
    for (int i = 0; i < 16; i++) {
        if (buf[i] != 0x00 && buf[i] != 0xFF) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <spiffs.img>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("Failed to open image");
        return 1;
    }

    unsigned char page[PAGE_SIZE];
    long offset = 0;
    int total = 0;

    printf("🔍 Scanning SPIFFS image: %s\n\n", argv[1]);

    while (fread(page, 1, PAGE_SIZE, f) == PAGE_SIZE) {
        for (int i = 0; i < PAGE_SIZE - MAX_FILENAME_LEN; i++) {
            if (is_filename(page, i)) {
                total++;
                printf("📄 File:      %s\n", &page[i]);
                printf("📍 Offset:    0x%06lx\n", offset + i);
                printf("🗂️  Status:    %s\n", is_deleted(page) ? "❌ Deleted (likely)" : "✅ Active");
                printf("----------------------------------------\n");
                break; // one per page max
            }
        }
        offset += PAGE_SIZE;
    }

    if (total == 0) {
        printf("❌ No file names found in image.\n");
    } else {
        printf("🔎 Summary: %d entries (some may be deleted)\n", total);
    }

    fclose(f);
    return 0;
}
