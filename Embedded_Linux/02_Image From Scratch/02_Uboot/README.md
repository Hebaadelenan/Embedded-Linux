# U-Boot 🚀

U-Boot, or Das U-Boot, originally developed for embedded PowerPC boards, has evolved into a versatile open-source bootloader for various architectures, including ARM, MIPS, and SH. Maintained by Denx Software Engineering, it offers extensive documentation and resources, with [Denx](http://www.denx.de/wiki/U-Boot) being a great starting point.

## Installation 🛠️
For detailed installation instructions, check the [official documentation](https://docs.u-boot.org/en/latest/build/gcc.html).

``` bash
ls configs/     # List Available pre-configured targets

# Apply your configuration
make ARCH=<target_arch> CROSS_COMPILE=<your_cross_toolchain> <pre_configuration>    

# Open menuconfig to further customize your bootloader
make ARCH=<target_arch> CROSS_COMPILE=<your_cross_toolchain> menuconfig

# Build U-Boot
make ARCH=<target_arch> CROSS_COMPILE=<your_cross_toolchain> 
```

Upon compilation, you get:
- **u-boot**: ELF object format for debugging
- **u-boot.map**: Symbol table
- **u-boot.bin**: Raw binary for device use
- **u-boot.img**: Binary with U-Boot header for uploading
- **u-boot.srec**: Motorola S-record format for serial transfer

## Usage 🖥️

U-Boot configuration relies on environment variables, including baud rate, boot delay, and default boot command, stored in the binary image. Use **envreset** to restore defaults.

### Useful Commands 🛠️

- **setenv** & **printenv**
``` bash
setenv myEnv 12345
printenv myEnv
# myEnv=12345
```
> Use ${env_var} for variable expansion
``` bash
setenv myNumber 123456
setenv var This is my number: ${myNumber}
printenv var
# var=This is my number: 123456
```

- **run** command:

``` bash
setenv cmd1 "setenv var 123; printenv var; saveenv"
run cmd1 
# var=123
# Saving Environment to FAT... OK
```

- **fatload** command:

``` bash
fatload mmc <dev>[:partition] <loadAddress> <bootfilename>

# e.g., for vexpress target
fatload mmc 0:1 $kernel_addr_r zImage
fatload mmc 0:1 $fdt_addr_r vexpress-v2p-ca9.dtb
```

- **bootz** command:

``` bash
bootz $kernel_addr_r - $fdt_addr_r
```

## Bootscript ⚙️

Automatically executes predefined U-Boot commands before OS boot. Useful for environments without a serial port.

### Bootscript Process

1. U-Boot checks loadbootsc variable; if "no", normal execution continues.
2. If loadbootsc is "yes" (factory default), U-Boot attempts bootscript download from TFTP server IP (default: 192.168.42.1).
3. If successful, executes bootscript.
4. Failure in any command cancels remaining script.
5. Upon completion, normal execution resumes.

``` bash
mkimage -T script -n "Bootscript" -C none -d <input_file> <output_file>
```

## QEMU 🖥️

Use QEMU with U-Boot by creating and mounting a virtual SD-Card.

``` bash
# Create a file with 1 GB filled with zeros
dd if=/dev/zero of=sd.img bs=1M count=1024

# Show where your SD card is mounted
lsblk 

# Partition the virtual SD card
cfdisk sd.img
```

Partition Layout:
| Partition Size | Partition Format | Bootable  |
| :------------: | :--------------: | :-------: |
|    `200 MB`    |     `FAT 16`     | ***Yes*** |
|    `300 MB`    |     `Linux`      | ***No***  |

``` bash
# Attach sd.img as block storage
sudo losetup -f --show --partscan sd.img

# Format partitions
sudo mkfs.vfat -F 16 -n boot /dev/loopxxp1
sudo mkfs.ext4 -L rootfs /dev/loopxxp2
```

Run QEMU with U-Boot and Virtual SD:

``` bash
qemu-system-arm -M vexpress-a9 -m 128M -nographic \
-kernel u-boot/u-boot \
-sd sd.img
```

### U-Boot and TFTP 🔌

Install TFTP protocol for file transfer:

``` bash
sudo apt install tftpd-hpa
```

- By default, all files in /srv/tftp are accessible to TFTP clients.
- Change configuration in /etc/default/tftpd-hpa for custom settings.
- Change ownership of /srv/tftp to tftp:tftp and set TFTP_OPTIONS="--secure --create".

#### U-Boot Environment for TFTP

``` bash
#Switch to root
sudo su
#Download tftp protocol
sudo apt-get install tftpd-hpa
#Change the configuration of tftp
nano /etc/default/tftpd-hpa
	#write inside the file
    tftf_option = “--secure –-create”
#Restart the protocol
Systemctl restart tftpd-hpa
#Change the file owner
cd /srv
chown tftp:tftp tftp 
#Move your image or file to the server
cp [File name] /srv/tftp
```

1. Create a virtual interface *tap* for QEMU:

``` bash
# Run QEMU with script for tap interface
sudo qemu-system-arm -M vexpress-a9 -m 128M -nographic \
-kernel ~/u-boot/u-boot \
-sd sd.img \
-net tap,script=./qemu-ifup -net nic
```

``` bash
# qemu-ifup script for tap interface
#!/bin/sh
ip a add 192.168.0.1/24 dev $1
ip link set $1 up
```

2. U-Boot Variables

``` bash
setenv ipaddr 192.168.0.5; setenv serverip 192.168.0.1

tftp 0x60100000 zImage; tftp 0x60000000 vexpress-v2p-ca9.dtb
```

3. Boot the Kernel

```bash
bootz $kernel_addr_r - $fdt_addr_r
```

## Useful Variables 

``` bash
LoadTFTP=echo Loading From TFTP; setenv ipaddr 192.168.0.5; setenv serverip 192.168.0.1; run FATLOAD_TFTP

FATLOAD_TFTP=tftp $kernel_addr_r zImage; tftp $fdt_addr_r vexpress-v2p-ca9.dtb

LoadSD=if mmc dev; then run FATLOAD_SD; else echo SD Not Found; fi

FATLOAD_SD=fatload mmc 0:1 $kernel_addr_r zimage; fatload mmc 0:1 $fdt_addr_r vexpress-v2p-ca9.dtb

KERNEL

_RUN=bootz $kernel_addr_r - $fdt_addr_r
```