```markdown
# My Team's Security Guide

🔒 Welcome to My Team's Security Guide! Here you'll find instructions and explanations on managing file and directory permissions.

## Section 1: Setting Up Permissions

📁 **Q1: Create a folder called myteam in your home directory and change its permissions to read-only for the owner.**

```bash
mkdir myteam
chmod u-wx myteam
```

🔒 **Q2: Log out and log in by another user**

🔒 **Q3: Try to access (by cd command) the folder (myteam)**

> Since the owner cannot execute, it will not work

```bash
bash: cd: myteam: Permission denied
```

## Section 2: Advanced Permission Management

🔑 **Q4: Using the Command Line**

### a. Change the permissions of the oldpasswd file.

```bash
chmod u=rw,g=wx,o=x /etc/passwd
chmod 631 /etc/passwd
```

### b. Change your default permissions.

> We can use the **umask** value to change the default permissions. umask just subtracts from 777 UGO

```bash
umask 146
```

### c. Maximum permissions for file and directory.

> For **files** the max is rw- rw- r--

> For **directories** the max is rwx rwx r-x

### d. Change your default permissions and verify.

```bash
umask 777
```

### e. State who can access this file if any with proof.

> The root can access any file/directory in the system without any restrictions

## Section 3: File Permissions

📄 **Q5: Create a file with permission 444. Try to edit or remove it. Note what happened.**

> 444 means read-only for UGO 

## Section 4: Understanding Permissions

🔍 **Q6: The difference between the “x” permission for a file and for a directory.** 

### File
> Execute permission for files allows you to use commands such as *cat*, *more*, *less* etc...

### Directory
> Execute permission for directories allows you to use commands such as *cd*, *ls*, etc...

## Section 5: Advanced Directory Permissions

📂 **Q7: Creating and Managing Directories**

1. Set the sticky bit on the newly created directory. 
2. Set the setgui bit on the created directory.
3. Create multiple user accounts. 
4. Allow these users to create files within the directory and directory. 
5. Attempt to delete or rename each other's files. 
6. Provide a clear output demonstrating the impact of the sticky bit on file deletion and renaming within the directory. 
7. Provide a clear output for the directory created. 

## Section 6: Special Permissions

🔐 **Q8: List the permission passwd command has and explain why it has S**

```bash
ll /bin/passwd
-rwsr-xr-x 
```
> It has the setuid to make any user execute this command as the owner. Providing them to change their password easily.
