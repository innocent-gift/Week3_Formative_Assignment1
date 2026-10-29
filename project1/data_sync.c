#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main(void) {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir failed");
        return 1;
    }
    printf("[DataSync] Scanning current directory...\n");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        printf("Found file: %s\n", entry->d_name);
    }
    closedir(dir);
    return 0;
}
