// This program is a simple implementation of the `ls` command in C. It lists the contents of a directory in a detailed format.

#include <stdio.h> // For printf, perror
#include <stdlib.h> // For EXIT_SUCCESS, EXIT_FAILURE
#include <dirent.h> // For opendir, readdir, closedir
#include <sys/stat.h> // For lstat, struct stat
#include <sys/types.h> // For mode_t
#include <pwd.h> // For getpwuid, struct passwd
#include <grp.h> // For getgrgid, struct group
#include <time.h> // For strftime, localtime
#include <string.h> // For strcpy, snprintf, strcmp
#include <unistd.h> // For readlink

// Converts the file mode to a human-readable permissions string (e.g., drwxr-xr-x)
void format_permissions(mode_t mode, char *str)
{
    strcpy(str, "----------");

    // Set file type character
    if (S_ISDIR(mode))
        str[0] = 'd'; // Directory
    else if (S_ISLNK(mode))
        str[0] = 'l'; // Symbolic link
    else if (S_ISCHR(mode))
        str[0] = 'c'; // Character device
    else if (S_ISBLK(mode))
        str[0] = 'b'; // Block device
    else if (S_ISFIFO(mode))
        str[0] = 'p'; // FIFO/pipe
    else if (S_ISSOCK(mode))
        str[0] = 's'; // Socket
    // Otherwise, '-' for regular file

    // Set user permissions
    if (mode & S_IRUSR)
        str[1] = 'r';
    if (mode & S_IWUSR)
        str[2] = 'w';
    if (mode & S_IXUSR)
        str[3] = 'x';

    // Set group permissions
    if (mode & S_IRGRP)
        str[4] = 'r';
    if (mode & S_IWGRP)
        str[5] = 'w';
    if (mode & S_IXGRP)
        str[6] = 'x';

    // Set other permissions
    if (mode & S_IROTH)
        str[7] = 'r';
    if (mode & S_IWOTH)
        str[8] = 'w';
    if (mode & S_IXOTH)
        str[9] = 'x';

    // Set special bits (Setuid, Setgid, Sticky bit)
    if (mode & S_ISUID)
        str[3] = (mode & S_IXUSR) ? 's' : 'S';
    if (mode & S_ISGID)
        str[6] = (mode & S_IXGRP) ? 's' : 'S';
    if (mode & S_ISVTX)
        str[9] = (mode & S_IXOTH) ? 't' : 'T';
}

// Lists the contents of the specified directory in a detailed format
void list_directory(const char *path)
{
    DIR *dir;
    struct dirent *entry; // Directory entry structure
    struct stat statbuf; // File status structure
    char permissions_str[11]; // 10 characters + null terminator
    char time_str[20];        // For formatted modification time
    struct passwd *pwd; // User information structure
    struct group *grp; // Group information structure

    dir = opendir(path);
    if (dir == NULL)
    {
        perror("Error opening directory");
        fprintf(stderr, "Could not open: %s\n", path);
        return;
    }

    // Print column headers, aligned for readability
    printf("%-10s %5s %-14s %-12s %8s %-12s %s\n",
           "Type&Perms", "Links", "Owner", "Group", "Size", "Modified", "Name");
    printf("---------- ----- -------------- ------------ -------- ------------ --------\n");

    // Iterate through directory entries
    while ((entry = readdir(dir)) != NULL)
    {
        // Skip current and parent directory entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // Get file status information
        if (lstat(full_path, &statbuf) == -1)
        {
            // Remain silent if file info cannot be retrieved, similar to real ls
            continue;
        }

        format_permissions(statbuf.st_mode, permissions_str);

        // Get owner name
        pwd = getpwuid(statbuf.st_uid);
        const char *owner_name = (pwd != NULL) ? pwd->pw_name : "unknown";

        // Get group name
        grp = getgrgid(statbuf.st_gid);
        const char *group_name = (grp != NULL) ? grp->gr_name : "unknown";

        // Format modification time as "Jun 04 16:46"
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&statbuf.st_mtime));

        // Print aligned file information
        printf("%-10s %5lu %-14s %-12s %8ld %-12s %s",
               permissions_str,                 // 10 chars, left-aligned
               (unsigned long)statbuf.st_nlink, // 5 chars, right-aligned
               owner_name,                      // 14 chars, left-aligned
               group_name,                      // 12 chars, left-aligned
               (long)statbuf.st_size,           // 8 chars, right-aligned
               time_str,                        // 12 chars, left-aligned
               entry->d_name);

        // If entry is a symbolic link, print its target
        if (S_ISLNK(statbuf.st_mode))
        {
            char link_target[1024];
            ssize_t len = readlink(full_path, link_target, sizeof(link_target) - 1);
            if (len != -1)
            {
                link_target[len] = '\0';
                printf(" -> %s", link_target);
            }
        }
        printf("\n");
    }

    // Close the directory
    if (closedir(dir) == -1)
    {
        perror("Error closing directory");
    }
}

// Entry point: parses arguments and calls list_directory
int main(int argc, char *argv[])
{
    const char *path_to_list;

    if (argc > 2)
    {
        fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
        return EXIT_FAILURE;
    }
    else if (argc == 2)
    {
        path_to_list = argv[1]; // List specified directory
    }
    else
    {
        path_to_list = "."; // List current directory if no argument is given
    }

    list_directory(path_to_list);

    return EXIT_SUCCESS;
}
