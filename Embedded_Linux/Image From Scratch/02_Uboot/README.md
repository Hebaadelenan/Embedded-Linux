```markdown
# U-boot

U-Boot is **the most popular boot loader in** **linux** **based embedded devices**. It is released as open source under the GNU GPLv2 license. It supports a wide range of microprocessors like MIPS, ARM, PPC, Blackfin, AVR32 and x86.

## Setup U-boot

### Download U-Boot

```bash
git clone git@github.com:u-boot/u-boot.git
cd u-boot/
git checkout v2022.07
```

### Configure U-Boot Machine

In this section we will **configure** u-boot for several Machine

```bash
# In order to find the machine supported by U-Boot
ls configs/ | grep [your machine] 
```

#### Vexpress Cortex A9 (Qemu)

In **U-boot directory** Assign this value

```bash
# Set the Cross Compiler into the environment
# To be used by the u-boot
export CROSS_COMPILE=<Path To the Compiler>/arm-cortexa9_neon-linux-musleabihf-
export ARCH=arm

# load the default configuration of ARM Vexpress Cortex A9
make vexpress_ca9x4_defconfig
```

#### Raspberry Pi

First we need to download the **compiler** for the architecture set for **RPI**

```bash
# update the package manager
sudo apt-get update

# the following compiler support 32 bit architecture
sudo apt-get install gcc-arm-linux-gnueabihf-

# in case of using architecture 64 Download the following compiler
sudo apt-get install gcc-aarch64-linux-gnu
```

##### Raspberry Pi 3

In **U-boot directory** Assign this value

```bash
# Set the Cross Compiler into the environment
# To be used by the u-boot
export CROSS_COMPILE=arm-linux-gnueabihf-
export ARCH=arm

# load the default configuration of rpi 3
make rpi_3_defconfig
```

##### Raspberry Pi 4

In **U-boot directory** Assign this value

```bash
# Set the Cross Compiler into the environment
# To be used by the u-boot
export CROSS_COMPILE=arm-linux-gnueabihf-
export ARCH=arm

# load the default configuration of rpi 3
make rpi_4_32b_defconfig
```

**In case of 64 architecture**

```bash
export CROSS_COMPILE=aarch64-linux-gnu-
export ARCH=arm64

# depends what raspberry pi in used
make rpi_4_b_defconfig
```

#### Beaglebone

In **U-boot directory** Assign this value

```bash
# Set the Cross Compiler into the environment
# To be used by the u-boot
export CROSS_COMPILE=<Path To the Compiler>/arm-cortexa9_neon-linux-musleabihf-
export ARCH=arm

# load the default configuration of ARM Vexpress Cortex A9
make am335x_evm_defconfig
```

### Configure U-Boot 

In this part we need to configure some u-boot configuration for the specific board chosen up.

```bash
make menuconfig
```

**The customer requirement are like following**:

- [ ] Support **editenv**.
- [ ] Support **bootd**.
- [ ] Store the environment variable inside file call **uboot.env**.
- [ ] Unset support of **Flash**
- [ ] Support **FAT file system**
  - [ ] Configure the FAT interface to **mmc**
  - [ ] Configure the partition where the fat is store to **0:1**

## SD Card

In this section it's required to have SD card with first partition to be FAT as pre configured in **U-boot Menuconfig**.

### ! ONLY FOR WHO WORK WITH QEMU !

In order to Emulate SD card to attached to Qemu following steps will be followed: (ONLY FOR NON PHYSICAL SD):

```bash
# Change directory to the directory before U-Boot
cd ..

# Create a file with 1 GB filled with zeros
dd if=/dev/zero of=sd.img bs=1M count=1024
```

### ! ONLY FOR WHO WORK WITH HW !

Plug your SD card to the computer

```bash
# To show where your sd card is mounted
lsblk 
```

`WARNING: The following command will completely erase all data on the specified SD card.`

```bash
### WARNING ####
sudo dd if=/dev/zero of=/dev/mmblk<x> bs=1M

# is not always mmblck to make sure use the command lsblk

# Assign the Block device as global variable to be treated as MACRO
export DISK=/dev/mmblck<x>
```



### Configure the Partition Table for the SD card

```bash
# for the VIRTUAL SD card
cfdisk sd.img

# for Physical SD card
cfdisk /dev/mmblck<x>
```

| Partition Size | Partition Format | Bootable  |
| :------------: | :--------------: | :-------: |
|    `200 MB`    |     `FAT 16`     | ***Yes*** |
|    `300 MB`    |     `Linux`      | ***No***  |

**write** to apply changes

### Loop Driver FOR Virtual usage ONLY

To emulate the sd.img file as a sd card we need to attach it to **loop driver** to be as a **block storage**

```bash
# attach the sd.img to be treated as block storage
sudo losetup -f --show --partscan sd.img

# Running the upper command will show you
# Which loop device the sd.img is connected
# take it and assign it like down bellow command

# Assign the Block device as global variable to be treated as MACRO
export DISK=/dev/loop<x>
```

### Format Partition Table

As pre configured from **cfdisk command** first partition is **FAT**

```bash
# Formating the first partition as FAT
sudo mkfs.vfat -F 16 -n boot ${DISK}p1
```

 As pre configured from cfdisk Command second partition is linux

```bash
# format the created partition by ext4
sudo mkfs.ext4 -L rootfs ${DISK}p2
```

## Test U-Boot

Check the **u-boot** and the **sd card** are working

### Vexpress-a9 (QEMU)

Start Qemu with the **Emulated SD** card

```bash
qemu-system-arm -M vexpress-a9 -m 128M -nographic \
-kernel u-boot/u-boot \
-sd sd.img
```

### Raspberry Pi



Add all **close source** file provided my Raspberry depend for each version to the **SD card in FAT Partition**

```
1. Bootcode.bin 
2. Fixup.dat
3. cmdline.txt
4. config.txt
5. start.elf
6. u-boot.bin {generated from u-boot}
```

Edit file `config.txt` as following

```
kernel=u-boot.bin
enable_uart=1
```

### Beaglebone

Add following file generated by U-Boot in the SD card FAT partition

```
1. MLO
2. u-boot.bin
```

## Initialize TFTP protocol

### Ubuntu

```bash
#Switch to root
sudo su
#Make sure you are connected to internet
ping google.com
#Download tftp protocol
sudo apt-get install tftpd-hpa
#Check the tftp ip address
ip addr `will be needed`
#Change the configuration of tftp
nano /etc/default/tftpd-hpa
	#write inside the file
    tftf_option = “--secure –-create”
#Restart the protocal
Systemctl restart tftpd-hpa
#Make sure the tftp protocol is running
Systemctl status tftpd-hpa
#Change the file owner
cd /srv
chown tftp:tftp tftp 
#Move your image or file to the server
cp [File name] /srv/tftp
```

### Create Virtual Ethernet For QEMU

This section for Qemu emulator users only **no need for who using HW**

Create a script `qemu-ifup` 

```bash
#!/bin/sh
ip a add 192.168.0.1/24 dev $1
ip link set $1 up
```

#### Start Qemu

In order to start Qemu with the new virtual ethernet

```bash
sudo qemu-system-arm -M vexpress-a9 -m 128M -nographic \
-kernel u-boot/u-boot \
-sd sd.img \
-net tap,script=./qemu-ifup -net nic
```

## Setup U-Boot IP address

```bash
#Apply ip address for embedded device
setenv ipaddr [chose]
#Set the server ip address that we get from previous slide
setenv serverip [host ip address]

#### WARNING ####
#the ip address should has the same net mask

```

## Load File to RAM

First we need to know the ram address by running the following commend

```bash
# this commend will show all the board information and it start ram address
bdinfo
```

### Load from FAT

```bash
# addressRam is a variable knowen from bdinfo commend
fatload mmc 0:1 [addressRam] [fileName]
```

### Load from TFTP

```bash
# addressRam is a variable knowen from bdinfo commend
tftp [addressRam] [fileName]
```



## Write Script in U-boot

```bash
setenv ifcondition "if mmc dev; then echo MmcExist; elif mtd list; then echo NoDeviceExist; else echo noDeviceExist; fi"
```

`setenv` 		>>>	 **T**o set a new variable

`ifcondition`       >>> 	**V**ariable name

`if` 			 >>> 	**F**irst condition

`mmc dev` 	      >>>	**I**s command to check there's device available

`then` 		    >>>	**D**o if condition **pass**

`echo` 		    >>>	**P**rint on console

`elif` 		    >>>	**Else if**

`mtd`		       >>>	**C**heck **if** any **nor flash** supported

`then`		     >>>	**E**xecute the else if

`else`		     >>>	**If** condition not **pass** and **elif** not pass execute **else** condition 

`fi`			 >>>	**Y**ou need to end the condition with **fi** (same as linux bash script)


## 2- Using U-Boot

U-Boot uses environment variables to tailor its operation. The environment variables configure
settings such as the baud rate of the serial connection, the seconds to wait before auto boot, the
default boot command, and so on.

The factory default variables and their values also are stored in the U-Boot binary image itself. In
this way, you can recover the variables and their values at any time with the **envreset** command.

### A- Usefull Commands

- **setenv** & **printenv**
``` bash
setenv myEnv 12345
printenv myEnv
# myEnv=12345

```
> You can use ${env_var} to expand the variable
``` bash
setenv myNumber 123456
setenv var This is my number: ${myNumber}
printenv var
# var=This is my number: 123456
```

Recursive variables (or scripts) contain one or more variables within their own value. The inner
variables are not expanded in the new variable. Instead, they are expanded when the recursive
variable is run as a command

``` bash
setenv dumpaddr md.b \${addr} \${bytes}
printenv dumpaddr
# dumpaddr=md.b ${addr} ${bytes}
setenv addr 2C000
setenv bytes 5
run dumpaddr
0002c000: 00 00 00 00 00 .....
```

> You must use the back slash '\\' before '$' to prevent variables from being expanded into other
variables’ values


- **run** command:

Create an environment variable called cmd1 which

 contain multiple commands
``` bash
setenv cmd1 "setenv var 123; printenv var; saveenv"
# OR: setenv cmd1 setenv var 123\; printenv var\; saveenv
# Both is the same


run cmd1 
# var=123
# Saving Environment to FAT... OK
```

- **fatload** command:

To read an image from an MMC card formatted in FAT

``` bash
fatload mmc <dev>[:partition] <loadAddress> <bootfilename>

# In our example we used vexpress target
fatload mmc 0:1 $kernel_addr_r zImage
fatload mmc 0:1 $fdt_addr_r vexpress-v2p-ca9.dtb
```

- **bootz** command:

This command will boot the kernel and fdt after loading it using **fatload command**

``` bash
# This will load boot the kernel and Flattened Device Tree
bootz $kernel_addr_r - $fdt_addr_r
```
---

### B- Common System Variables

- **bootcmd**: Defines a command string that is automatically executed when the
initial countdown is not interrupted.
Executed only when the bootdelay variable is also defined.

- **bootdelay**: Seconds to wait before running the automatic boot process in
bootcmd.



# Bootscript
The bootscript is an script that is automatically executed when the boot loader starts, and before
the OS auto boot process.
The bootscript allows the user to execute a set of predefined U-Boot commands automatically
before proceeding with normal OS boot. This is especially useful for production environments and
targets which don’t have an available serial port for showing the U-Boot monitor


## Bootscript process
he bootscript works in the following way:
1. U-Boot checks the variable loadbootsc. If set to “no”, it continues normal execution.
2. If the variable loadbootsc is set to “yes” (factory default value) U-Boot tries to download
the bootscript file with the filename stored in variable bootscript from the TFTP server IP
address defined at variable serverip (by default 192.168.42.1).
The default value of the bootscript variable is **platformname**-bootscript.
3. If the bootscript file is successfully downloaded, it is executed.
4. If any of the commands in the bootscript fails, the rest of script is cancelled.
5. When the bootscript has been fully executed (or cancelled) U-Boot continues normal
execution.


``` bash
# mkimage is in tools directory
mkimage -T script -n "Bootscript" -C none -d <input_file> <output_file>
```

# QEMU

To use QEMU with Uboot you will need to make and mount virtual SD-Card

``` bash
# Create a file with 1 GB filled with zeros
dd if=/dev/zero of=sd.img bs=1M count=1024

# To show where your sd card is mounted
lsblk 

# for the VIRTUAL SD card
cfdisk sd.img
```

| Partition Size | Partition Format | Bootable  |
| :------------: | :--------------: | :-------: |
|    `200 MB`    |     `FAT 16`     | ***Yes*** |
|    `300 MB`    |     `Linux`      | ***No***  |

``` bash
# attach the sd.img to be treated as block storage
sudo losetup -f --show --partscan sd.img

# Formating the first partition as FAT (This is only done single time)
sudo mkfs.vfat -F 16 -n boot /dev/loopxxp1

# format the created partition by ext4 (This is only done single time)
sudo mkfs.ext4 -L rootfs /dev/loopxxp2

```
---
Now to start QEMU with your Uboot and Virtual SD

``` bash
qemu-system-arm -M vexpress-a9 -m 128M -nographic \
-kernel u-boot/u-boot \
-sd sd.img
```

## U-boot and TFTP

- Trivial File Transfer Protocol
-  Somewhat similar to FTP, but without authentication and over UDP

``` bash
sudo apt install tftpd-hpa
```

> All files in /srv/tftp is exposed by default to TFTP

- You can edit this default location in the config file of the tftp which is in **/etc/default/tftpd-hpa**

- You will need also to change the ownership of the **/srv/tftp** directory to **tftp:tftp** to make this service user able to access it

- Also change TFTP_OPTIONS="--secure --create"

### Uboot environment for TFTP

#### Prerequisit
``` bash
#Switch to root
sudo su
#Make sure you are connected to internet
ping google.com
#Download tftp protocol
sudo apt-get install tftpd-hpa
#Check the tftp ip address
ip addr `will be needed`
#Change the configuration of tftp
nano /etc/default/tftpd-hpa
	#write inside the file
    tftf_option = “--secure –-create”
#Restart the protocal
Systemctl restart tftpd-hpa
#Make sure the tftp protocol is running
Systemctl status tftpd-hpa
#Change the file owner
cd /srv
chown tftp:tftp tftp 
#Move your image or file to the server
cp [File name] /srv/tftp
```

1. First when running in QEMU we will need to create a virtual interface *tap* to be able to use the physicalinterface of the machine.

``` bash
# This command will run the next script to make virtual interface to be used in QEMU

sudo qemu-system-arm -M vexpress-a9 -m 128M -nographic \
-kernel ~/u-boot/u-boot \
-sd sd.img \
-net tap,script=./qemu-ifup -net nic'   
```


``` bash
# This script will make a tap and assign it IP
#!/bin/sh
ip a add 192.168.0.1/24 dev $1
ip link set $1 up

```

2. Uboot variables

``` bash
# Assign IP for the QEMU (UBOOT) and assign the server (tap)
setenv ipaddr 192.168.0.5; setenv serverip 192.168.0.1

# Load using TFTP
tftp 0x60100000 zImage; tftp 0x60000000 vexpress-v2p-ca9.dtb
```

3. Boot the KERNEL

```bash
bootz $kernel_addr_r - $fdt_addr_r
```


## Useful variables 

``` bash
LoadTFTP=echo Loading From TFTP; setenv ipaddr 192.168.0.5; setenv serverip 192.168.0.1; run FATLOAD_TFTP

FATLOAD_TFTP=tftp $kernel_addr_r zImage; tftp $fdt_addr_r vexpress-v2p-ca9.dtb


LoadSD=if mmc dev; then run FATLOAD_SD;

 elif mtd list; then run FATLOAD_NOR; else echo No Memory Devices; fi

FATLOAD_SD=mmc rescan; fatload mmc 0:1 $kernel_addr_r zImage; fatload mmc 0:1 $fdt_addr_r vexpress-v2p-ca9.dtb

FATLOAD_NOR=run norboot


```
---
# NOR  Flash

## 1. NOR flash Operation

The NOR flash are read by default, so there's no need to set it in read mode before reading the data.

1.1 **Read Mode**
``` bash
# in order to get the flash information
mtd list

# to read a single byte from the NOR flash
mtd read 0x00100000 0x1000 0x100
```

1.2 **Erase Operation**
``` bash
# to erase a block in the NOR flash
mtd erase 0x100000 0x10000

# to erase the entire NOR flash
mtd erase 0x0 0x0
```

1.3 **Write Operation**

``` bash
# Write a hex file to the NOR flash 
mtd write hexFile.hex 0x100000
```

## 2. Build your Kernel with the Uboot
1. First we need to clone the Linux Kernel
``` bash
git clone https://github.com/torvalds/linux.git
cd linux/
git checkout v5.10
```

2. Cross-compile the kernel
``` bash
# Set your cross compiler to the PATH environment variable 
export PATH=$PATH:/usr/bin/gcc-arm-linux-gnueabihf-
# Set the cross compiler
export CROSS_COMPILE=arm-linux-gnueabihf-
# Set your machine
export ARCH=arm

# Load the pre configured configuration of the machine
make vexpress_defconfig

# Open menuconfig
make menuconfig
```

3. Make the Kernel
``` bash
make -j8
```

4. Copy the generated kernel files to the u-boot directory
``` bash
cp arch/arm/boot/zImage ../u-boot/
cp arch/arm/boot/dts/vexpress-v2p-ca9.dtb ../u-boot/
```

5. Make the U-boot again after adding the kernel files
``` bash
cd ../u-boot/
make -j8
```

6. Run the QEMU again with the new u-boot image
``` bash
qemu-system-arm -M vexpress-a9 -m 128M -nographic \
-kernel u-boot \
-sd sd.img \
-net tap,script=./qemu-ifup -net nic 
```
---

## U-boot with QEMU

To run the QEMU with U-boot, you will need to first make and mount a virtual SD-Card.

1. Create a file with 1 GB filled with zeros:
``` bash
dd if=/dev/zero of=sd.img bs=1M count=1024
```

2. Check where your SD card is mounted:
``` bash
lsblk 
```

3. Use cfdisk to set up the partitions for the SD card. Run cfdisk on the mounted SD card:
``` bash
cfdisk /dev/mmblck<x>
```

4. Format the partitions: 
   - First partition as FAT16 
   - Second partition as Linux

5. Create the loop device for the virtual SD card:
``` bash
sudo losetup -f --show --partscan sd.img
```

6. Format the first partition as FAT:
``` bash
sudo mkfs.vfat -F 16 -n boot /dev/loopxxp1
```

7. Format the second partition as ext4:
``` bash
sudo mkfs.ext4 -L rootfs /dev/loopxxp2
```

8. Start QEMU with U-boot and the virtual SD card:
``` bash
qemu-system-arm -M vexpress-a9 -m 128M -nographic \
-kernel u-boot/u-boot \
-sd sd.img
```

Now, you're ready to use U-boot with QEMU! You can customize your U-boot environment and experiment with different configurations.