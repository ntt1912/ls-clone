# myls

## Description
`myls` is a C program that mimics the `ls` command on Linux. It lists detailed information about files and directories in a specified folder. The program displays permissions, link count, owner, group, size, modification time, and file/directory name. If the entry is a symbolic link, it also shows the link target.

## Required Libraries
This program uses standard libraries available on most Linux systems:
- `stdio.h`
- `stdlib.h`
- `dirent.h`
- `sys/stat.h`
- `sys/types.h`
- `pwd.h`
- `grp.h`
- `time.h`
- `string.h`
- `unistd.h`

No additional installation is needed for these libraries on Linux.

## Build Instructions
To compile the program, use:

```bash
make
```

## Install
To install (copy the executable to /usr/local/bin):

```bash
make install
```

## Uninstall
To remove the installed executable:

```bash
make uninstall
```

## Usage
Run the program with the following syntax:

```bash
./myls [directory]
```

- If no argument is provided, the program lists the current directory.
- If a path is provided, it lists the contents of that directory.

Example:
```bash
./myls
./myls /etc
```

## Author
- github: ntt1912
- email: ntt30121912@gmail.com
