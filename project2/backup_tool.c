#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int perform_backup(const char *src_path, const char *dest_dir) {
    if (mkdir(dest_dir, 0755) == -1 && errno != EEXIST) return -1;
    int log_fd = open("backup_activity.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (log_fd != -1) {
        write(log_fd, "Backup initiated.\n", 18);
        close(log_fd);
    }
    int src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1) return -1;
    char dest_path[512];
    snprintf(dest_path, sizeof(dest_path), "%s/backup_copy.txt", dest_dir);
    int dest_fd = open(dest_path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (dest_fd == -1) {
        close(src_fd);
        return -1;
    }
    char buffer[BUFFER_SIZE];
    ssize_t bytes;
    while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {
        write(dest_fd, buffer, bytes);
    }
    close(src_fd);
    close(dest_fd);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    pid_t pid = fork();
    if (pid < 0) return 1;
    if (pid == 0) {
        int res = perform_backup(argv[1], "./backup_vault");
        exit(res == 0 ? 0 : 1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}
