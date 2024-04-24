# Linux Command Line README

```bash
# Q1
mkdir myteam
chmod u-wx myteam

# Q2-4
cd myteam
# Output: bash: cd: myteam: Permission denied

# Change permissions of oldpasswd file
chmod u=rw,g=wx,o=x /etc/passwd
chmod 631 /etc/passwd

# Change default permissions
umask 146

# Maximum permissions for files and directories
# File: rw- rw- r--, Directory: rwx rwx r-x

# Change default permissions to no permission for everyone
umask 777

# Proof that root can access any file/directory without restrictions

# Q5
# Create a file with permission 444, try to edit/remove it
# Outcome: File is read-only for UGO

# Q6
# File execute permission allows commands like cat, more, less
# Directory execute permission allows commands like cd, ls

# Q7
# - Create a new directory
# - Set sticky bit and setgui bit
# - Create multiple user accounts
# - Allow users to create files, attempt deletion/rename of each other's files
# - Demonstrate sticky bit impact on file deletion and renaming
# - Provide directory output

# Q8
# List permissions of passwd command, explain the S
ll /bin/passwd
# Output: -rwsr-xr-x
# Explanation: passwd has setuid permission allowing users to change their password easily
