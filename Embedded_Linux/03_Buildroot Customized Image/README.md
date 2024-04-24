# Building with BuildRoot 🛠️

Dive into the depths of embedded systems with BuildRoot, the gateway to crafting custom Linux distributions tailored to your needs.

## Library Quest

Before embarking on your journey, ensure you have the necessary tools at your disposal.

```bash
sudo apt install sed make binutils gcc g++ bash patch \
gzip bzip2 perl tar cpio python unzip rsync wget libncurses-dev
```

## The Call of BuildRoot

Heed the call of BuildRoot, summoning you to its realm of customization.

```bash
git clone https://github.com/buildroot/buildroot.git
cd buildroot

# Seek guidance
make help
```

## Configuration Chronicles

Embark on a quest to configure your chosen board within the halls of BuildRoot.

### Unveiling the BeagleBone

Unleash the power of configuration for the mighty BeagleBone.

```bash
make beaglebone_defconfig
```

### Unraveling the Raspberry Pi

Navigate the labyrinth of configuration options to tame the Raspberry Pi.

```bash
make raspberrypi3_defconfig
```

### Embracing the QEMU

Harness the virtual realm of QEMU through configuration.

```bash
make qemu_arm_vexpress_defconfig
```

### Customize Your Destiny

Forge your destiny with BuildRoot's menuconfig, where dreams take shape and configurations manifest.

```bash
make menuconfig
```

In the realm of configuration, heed the following mandates:

- [ ] Choose your Init process: System V, System D, or Busybox Init.
- [ ] Alter the system host.
- [ ] Personalize the system banner.
- [ ] Fortify with a root password.

#### SSH Configuration

Venture into the realm of SSH, weaving its essence into the fabric of your BuildRoot creation.

- [ ] Enable Network application, configure Dropbear.
- [ ] Set the Root password in system configuration.

## Crafting the System

Forge your creation with the alchemy of BuildRoot.

```bash
make 2>&1 | tee build.log

# Beware, for it shall consume hours of your time ⚠️
```

## Configuring Embedded Components

Delve deeper into the essence of your system by configuring its core components.

### Mastering Linux

Tinker with the configurations of Linux itself.

```bash
make linux-menuconfig
```

### Uboot Unleashed

Unleash the power of Uboot through its configurations.

```bash
make uboot-menuconfig
```

### Busybox Mastery

Master the art of Busybox configuration.

```bash
make busybox-menuconfig
```

## The Dawn of Booting

Witness the birth of your creation as it boots into existence.

### QEMU Quest

Embark on a journey into the virtual realm with QEMU.

```bash
qemu-system-arm -M vexpress-a9 -m 128M -sd output/image/sdcard.img -kernel output/image/u-boot.img -nographic
```

### BeagleBone Odyssey

Navigate the seas of configuration to prepare your creation for the BeagleBone.

```bash
dd if=/dev/zero of=/dev/mmcblk0 bs=1M
dd if=output/images/sdcard.img of=/dev/mmcblk0 bs=1M
```

### Raspberry Pi Saga

Embark on an epic quest to prepare your creation for the Raspberry Pi.

```bash
dd if=/dev/zero of=/dev/mmcblk0 bs=1M
dd if=output/images/sdcard.img of=/dev/mmcblk0 bs=1M
```

# Crafting BuildRoot Packages 📦

Forge your personalized package and infuse it into the very fabric of your BuildRoot creation.

1. Embark on the journey by creating a folder for your package.

```bash
mkdir -p ./package/simpleapp/src
```

2. Craft your source file, the essence of your creation.

```bash
touch simpleapp.c && vim simpleapp.c
```

```c
#include <stdio.h>

int main(void) {
	printf("Hi People\r\n");
}
```

3. Forge the Makefile, the arcane script that breathes life into your creation.

```bash
touch Makefile && vim Makefile
```

```makefile
.PHONY: clean
.PHONY: simpleapp

simpleapp: simpleapp.c
        $(CC) -g -Wall $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
        -rm simpleapp
```

4. Craft a package definition to usher your creation into the realm of BuildRoot.

```bash
touch simpleapp.mk && vim simpleapp.mk
```

```makefile
################################################################################
#
# simpleapp package
#
################################################################################

SIMPLEAPP_VERSION = 1.0
SIMPLEAPP_SITE = package/simpleapp/src
SIMPLEAPP_SITE_METHOD = local

define SIMPLEAPP_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define SIMPLEAPP_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/simpleapp  $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))
```

5. Engrave your package's name in the annals of BuildRoot's configuration.

```bash
touch Config.in && vim Config.in
```

```bash
config BR2_PACKAGE_SIMPLEAPP
    bool "simpleapp"
    help
        simpleapp package.
```

6. Link your package to the grand tapestry of BuildRoot's package configuration.

```bash
vim ./package/Config.in
```

```makefile
menu "SIMPLEAPP Packages"
    source "package/simpleapp/Config.in"
endmenu
```

7. Infuse your package into the very fabric of BuildRoot's being.

```bash
cd buildroot
make menuconfig
# Enable your package under target package configuration
make -j8
```

# Sage Advice and Mystical Insights 🧙‍♂️

Unravel the secrets of U-Boot through the creation of configuration files in the boot partition:

```bash
mkdir extlinux && touch extlinux/extlinux.conf
```

```bash
label buildroot
        kernel /zImage
        devicetree /am335x-boneblack.dtb
        append console=ttyO0,115200 root=/dev/mmcblk0p2 rootwait
```

Venture forth, brave souls, into the realm of BuildRoot, where every line of code is a step towards enlightenment.