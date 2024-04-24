# Initialization Mastery 🚀

Welcome to the world of initialization! 🎉 Here, we delve deep into the heart of the boot process, exploring everything from basic app setups to advanced system management techniques.

## The Sacred Init Process

At the core of every system lies the **init process**, the sentinel of the boot realm. Armed with the mighty PID of 1, it orchestrates the grand symphony of system startups.

```bash
# To unveil the process tree
pstree -gn 
```

## Crafting the App 🛠️

Let's kick things off by birthing a humble **application** and witnessing its maiden voyage into the realms of initialization.

1. **Forge the App**: Create a simple application, a mere whisper in the winds of initialization.
   
2. **Arm with Compilers**: Equip the App with board-specific compilers, ensuring its readiness for the journey ahead.

   *Behold the InitApp folder in the Repository.*

## Unveiling BusyBox Init 🛤️

Watch as the **BusyBox init** takes the stage, parsing configuration files and summoning system services, ushering the system into a realm of functionality.

### Crafting the Rootfs 📦

With Buildroot or BusyBox, we mold the filesystem to embrace the essence of BusyBox init. Then, we tweak the bootargs in U-Boot, leading the charge towards a new dawn.

```bash
# Set sail with bootargs for BusyBox init
setenv bootargs_busybox "setenv bootargs console=ttyS0,115200 root=/dev/nfs ip=192.168.7.100:::::eth0 nfsroot=192.168.7.1:/home/fady/rfs_bb,nfsvers=3,tcp rw init=/sbin/init"
```

## Adding the App Essence 🌟

Embrace the app into the initiation script, ensuring its presence at the dawn of each system awakening.

Modify the sacred scripts:

- `/etc/init.d/rcS1`
- `/etc/init.d/rcK1`

And infuse the essence into inittab:

```bash
# Enchant with the mystic arts of the init script
null::sysinit:/bin/mount -t proc proc /proc
null::sysinit:/bin/mount -t sysfs sysfs /sys

::sysinit:/etc/init.d/rcS1

console::adkfirst:-/bin/sh

::shutdown:/etc/init.d/rcK1
```

## The Genesis of SystemV Init 🌌

Behold the legacy of **SystemV init**, a grand tapestry woven with the threads of boot scripts and runlevels.

### Forging the Setup 🔨

In the depths of `/etc/init.d/deamonapp`, a script of power lies dormant, waiting to be awakened:

```sh
# Invoke the daemonapp script
/etc/init.d/deamonapp --help
/etc/init.d/deamonapp stop
/etc/init.d/deamonapp start
```

Harness its essence with symbolic links in the sacred `rc<runlevel>` folders:

```sh
# Link the essence to the runlevel
ln -s ../init.d/deamonapp ./etc/rc5.d/S40deamonapp.sh
```

## Embracing the Era of System D 🌐

Venture forth into the era of **System D**, where services are but pawns in the grand game of dependencies.

Craft a service manifest in the hallowed halls of `/etc/systemd/system/deamonapp.service`:

```bash
[Unit]
Description=App Server

[Service]
Type=simple
ExecStart=/bin/deamonapp

[Install]
WantedBy=multi-user.target
```

Embrace persistence with a single command:

```bash
systemctl enable deamonapp
```

## Basking in the Light of Mastery 🌞

As the system awakens, bask in the glory of your mastery, for you have traversed the realms of initialization and emerged victorious.

```bash
reboot
ps # behold, the application reigns supreme
```

Witness the power of services, intertwined in the dance of dependencies, orchestrating the symphony of system startups.

**Note**: Behold the symbolic links in `/etc/systemd/system/<target_name>.target.wants`, for they hold the keys to service dependencies.