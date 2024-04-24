savedcmd_/home/heba/001-myfirstdriver/mydriver.mod := printf '%s\n'   mydriver.o | awk '!x[$$0]++ { print("/home/heba/001-myfirstdriver/"$$0) }' > /home/heba/001-myfirstdriver/mydriver.mod
