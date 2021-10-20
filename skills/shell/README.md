
# Linux Shell Commands

## Shell Commands

**Web Source**
- [Linux Shell Commands](https://docs.cs.cf.ac.uk/notes/linux-shell-commands/)

### Files and Directories

These commands allow you to create directories and handle files.

| Command | Description | Example |
| --- | --- | ---|
| `$ cat`                  | concatenate and print data                               |  `$ cat <file>` |
| `$ cd`                    | change current directory                                   |  `$ cd <directory>` |
| `$ cp`                    | copy file data                                                     |  `$ cp file <directory>/<file> ` |
|                               | copy all file called <name_file>                         |  `$ cp <name_file>.* <directory>/ ` |
|                               | copy all file with extension                                 |  `$ cp *.<extension> <directory>/ ` |
| `$ file`                | determine file type                                             |  `$ file <file>` |
| `$ ls`                    | concatenate and print data                                |  `$ ls <directory>` |
| `$ more`                | display file data at your terminal|                       | `$ more <file>` |
| `$ mkdir`              | make a new directory                                         |  `$ mkdir <directory>`  |
| `$ mv`                    | move or rename files                                          |  `$ mv file <directory>`,  `$ mv <file> <file_new>` |
| `$ rm, rmdir`      | remove (unlink) files or directories                      |  `$ rm -r <directory>`,  `$ rm <file>` |
| `$ head`                | give first few lines                                               |  `$ head <file>` |
| `$ tail`                | print last lines from file                                        |  `$ tail <file>` |
| `$ touch`              | update access and modification times of a file  |  `$ touch <file>` |
| `$ tree`                | see directory tree structure                                 |  `$ tree <directory>` |
|                               | to see colorized output                                       |  `$ tree -C <directory>` |
|                               | to list directories only                                          |  `$ tree -d <directory>` |
| `$ pwd`                  | print working directory                                        |  `$ pwd ` |



### File Editors

Editors are used to create files.

| Command | Description | Example |
| --- | --- | ---|
| `$ emacs`                      | GNU project Emacs      |  `$ emacs <file>` |
| `$ gedit`                      | GNOME text editor       |  `$ gedit <file>` |
| `$ vi, vim`                  | standard text editor      |  `$ vim <file>` |


### Manipulating data

The contents of files can be compared and altered with the following commands.

| Command | Description | Example |
| --- | --- | ---|
| `$ cmp`                  | compare the contents of two files          |  `$ cmp <file1> <file2>` |
| `$ paste`              | merge file data                                        |  `$ paste <file1> <file2>` |
| `$ comm`                | compare sorted data                              |  `$ $comm <file1> <file2>` |
| `$ diff -c`          | differential file comparator                     |  `$ diff -c <file1> <file2>` |
| `$ wc`                    | count words, lines, and characters        |  `$ wc<file>` |


### Compressed files

Files may be compressed to save space. Compressed files can be created and examined.

| Command | Description | Example |
| --- | --- | ---|
| `$ gzip`                 | compress files                    |  `$ gzip <file>` |
| `$ uncompress`     | uncompress files                |  `$ uncompress <file>` |
| `$ zcat`                 | cat a compressed file         |  `$ zcat <file>` |
| `$ gunzip`             | uncompress gzipped files  |  `$ gunzip <file>` |


### Status

These commands list or alter information about the system.

| Command | Description | Example |
| --- | --- | ---|
| `$ date`                | print the date                               |  `$ date` |
| `$ time`                | time a command                         |  `$ time` | 
| `$ kill`                | send a signal to a process          |  `$ kill` |
| `$ last`                | show last logins of users             |  `$ last` |
| `$ users`              | print names of logged in users    |  `$ users` |
| `$ who`                  | list logged in users                       |  `$ who` |


### Networking

These commands are used to send and receive files from Campus Linux hosts and from other hosts  that permit such connections, around the world.

| Command | Description | Example |
| --- | --- | ---|
| `$ scp`                  | secure shell remote file copy                                      |  `$ scp <file> localhost@host:[path]` |
| `$ ssh`                  | secure shell terminal, accessing a remote server       |  `$ ssh sername@hostname_or_ip` |
| `$ firefox`          | web browser                                                               |  `$ firefox` |

