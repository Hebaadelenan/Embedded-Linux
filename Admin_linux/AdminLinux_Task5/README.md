## Q1- Create a folder called myteam in your home directory and change its permissions to read only for the owner. 

```bash
mkdir myteam
chmod u-wx myteam
```

## Q2- Log out and log in by another user 

## Q3- Try to access (by cd command) the folder (myteam) 

> Since the owner cannot execute then it will not work

```bash
bash: cd: myteam: Permission denied
```
## Q4- Using the command Line 

### a. Change the permissions of oldpasswd file to give owner read and write permissions and for group write and execute and execute only for the others (using chmod in 2 different ways)  

```bash
chmod u=rw,g=wx,o=x /etc/passwd
chmod 631 /etc/passwd
```

### b. Change your default permissions to be as above.
> We can use the **umask** value to change the default permissions. umask just subtract from 777 UGO

```bash
umask 146
```

### c. What is the maximum permission a file can have, by default when it is just created? And what is that for directory.  

>For **files** the max is rw- rw- r--

>For **directories** the max is rwx rwx r-x


### d. Change your default permissions to be no permission to everyone then create a directory and a file to verify. 

```bash
umask 777
```

### e. State who can access a this file if any with proof. 

> The root can access any file/directory in the system without any restrictions


## Q5- Create a file with permission 444. Try to edit in it and to remove it? Note what happened.

> 444 means read-only for UGO 



## Q6- What is the difference between the “x” permission for a file and for a directory?  

### File
> Execute permission for files allow you to use commands such as *cat*, *more*, *less* etc...

### Directory
> Execute permission for directories allows you to use commands such as *cd*, *ls*, etc...

## Q7- Create a new directory. 
1st. Set the sticky bit on the newly created directory. 
2nd. set the setgui bit on the created directory 
3rd. Create multiple user accounts. 
4th. Allow these users to create files within the directory and directory. 
5th. Attempt to delete or rename each other's files. 
6th. Provide a clear output demonstrating the impact of the sticky bit on file 
deletion and renaming within the directory. 
7th. Provide a clear output for the directory created. 


## Q8- List the permission passwd command has and explain why it has S 

```bash
ll /bin/passwd
-rwsr-xr-x 
```
> it has the setuid to make any user execute this command as the owner. Providing them to change their password easly