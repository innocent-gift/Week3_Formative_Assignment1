#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int main(void) {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir failed");
        return 1;
    }
    FILE *log = fopen("sync_log.txt", "w");
    if (log == NULL) {
        perror("fopen failed");
        closedir(dir);
        return 1;
    }
    printf("[DataSync v2] Scanning directory with metadata check...\n");
    fprintf(log, "Starting scan session\n");
    struct dirent *entry;
    struct stat file_stat;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        if (stat(entry->d_name, &file_stat) == 0) {
            printf("File: %s | Size: %ld bytes\n", entry->d_name, file_stat.st_size);
            fprintf(log, "File: %s, Size: %ld\n", entry->d_name, file_stat.st_size);
        }
    }
    fclose(log);
    closedir(dir);
    return 0;
}
