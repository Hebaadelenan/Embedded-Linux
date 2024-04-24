#!/bin/bash

USERNAME="YourName"
GROUPNAME="FamilyName"

if [ id "$USERNAME" 2>/dev/null ]; then
	echo "user $USERNAME already exist"
else
	sudo useradd -m "$USERNAME"
	echo "user $USERNAME created"
fi

if [ getent group "$GROUPNAME" 2>/dev/null ]; then
        echo "group $GROUPNAME already exist"
else
        sudo groupadd "$GROUPNAME"
        echo "group $GROUPNAME created"
fi

sudo usermod -aG "$GROUPNAME" "$USERNAME"
echo "User $USERNAME added to group $GROUPNAME"

id "$USERNAME"
getent group "$GROUPNAME"


