# System Configuration and Commands

## Q1-List the user commands and redirect the output to /tmp/commands.list

```bash
ls /usr/bin > /tmp/commands.list
```

## Q2- Edit in your profile to display date at login and change your prompt permanently.

In `.profile` add:

```bash
date
```

## Q3- What is the command to count the word in a file or number of files in a directory.

```bash
wc
```

a. Count the number of user commands

```bash
wc -l /etc/passwd
```

## Q4 What happens if you execute:

a.

```bash
cat filename1 | cat filename2  
```

Will only display the content of `filename2`.

b.

```bash
ls | rm
```

Will not run as `rm` needs an operand.

c.

```bash
ls /etc/passwd | wc –l 
```

1 as `ls /etc/passwd` will print `/etc/passwd` and `wc -l` will count the line which results in 1.

## Q5- Write a command to search for all files on the system that, its name is “.profile”.

```bash
find / -name ".profile"
```

## Q6- List the inode numbers of /, /etc, /etc/hosts.

```bash
ls -id / /etc /etc/hosts
```

## Q7- Create a symbolic link of /etc/passwd in /boot

```bash
ln -s /etc/passwd /boot
```

## Q8- Create a hard link of /etc/passwd in /boot. Could you? Why?

```bash
ln /etc/passwd /boot
```

It may not work as these two directories are formatted differently resulting in errors if hard linked.

## Q9- `echo \` it will jump to the next line, and will print `>`

Notice the prompt `>`, what is that? and how can you change it from `>` to `:`. NOTE: search the usage of PS1

To change from `>` to `:`, you need to edit the `PS1` environment variable.
