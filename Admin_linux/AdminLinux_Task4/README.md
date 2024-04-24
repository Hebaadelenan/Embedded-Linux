# System Administration Commands

## Q1- Create a supplementary (Secondary) group called pgroup with group ID of 30000

```bash
groupadd -g 30000 pgroup
```

   -g option is used to set the group ID

## Q2- Lock any user created account so he can't log in

```bash
sudo passwd -l username
```

   NOTE: You need to reboot to apply changes

   -l option is for locking. -u option is for unlocking

## Q3- Delete user account

```bash
userdel username
```

   By default, it will delete the home directory. If you want to keep it, you can use -r option.

## Q4- Delete group account

```bash
groupdel groupname
```

   It is recommended to remove users from the group before removing the group.

## Q5- State the difference between adduser and useradd with example shown.

### adduser

   1. Is a high-level command that provides an interactive interface for creating new users.
   
   2. It automatically creates a home directory and sets up default environment files.
   
   3. It is a script written in Perl as a wrapper for useradd.
   
```bash
file /sbin/adduser
# /sbin/adduser: Perl script text executable
```

### useradd

   1. It is a low-level command to directly add new users without any prompts.
   
   2. Requires manual configuration such as username, home directory, and shell.
   
   3. Provides more control and flexibility.

