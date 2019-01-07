# LinMod
This repo contains a variety of Linux kernel modules that are for educational purposes.

See the equivalent FreeBSD kernel module project: [BSDMod](https://github.com/ldilley/bsdmod)

These modules demonstrate:

* How kernel space differs from user space (availability of different functions)
* How device I/O works
* How /proc and /sys I/O works
* How to work with sysctl keys and values
* How to load, unload, and display loaded modules
* How to view information on a module
* How to build Linux kernel modules
* Module interdependence
* Logging to dmesg

### Instructions
1. Issue `make` to build all modules.
2. To load a module: `insmod <module_name>.ko`
3. To view loaded modules: `lsmod` or `lsmod | grep <module_pattern>`
4. To view module output after loading: `dmesg`
5. To unload a module: `rmmod -v <module_name>`
6. To view module output after unloading: `dmesg`
7. To view information about a particular module: `modinfo <module_name>.ko`

### Examples
##### Device
```
[root@lindev /home/ldilley/projects/linmod]# insmod linmod_dev.ko
[root@lindev /home/ldilley/projects/linmod]# dmesg | tail -1
[936244.931811] linmod_dev->linmod_dev_init(): Kernel module loaded using /dev/linmod with major number: 244
[root@lindev /home/ldilley/projects/linmod]# mknod /dev/linmod c 244 0
[root@lindev /home/ldilley/projects/linmod]# ls -la /dev/linmod
crw-r--r--. 1 root root 244, 0 Jan  6 22:38 /dev/linmod
[root@lindev /home/ldilley/projects/linmod]# cat /dev/linmod
Kernel module loaded on: 01/07/2019 03:37:22 UTC
[root@lindev /home/ldilley/projects/linmod]# echo "foo bar baz" > /dev/linmod
[root@lindev /home/ldilley/projects/linmod]# cat /dev/linmod
foo bar baz
[root@lindev /home/ldilley/projects/linmod]# echo "foo" >> /dev/linmod
[root@lindev /home/ldilley/projects/linmod]# cat /dev/linmod
foo
[root@lindev /home/ldilley/projects/linmod]# rmmod linmod_dev
[root@lindev /home/ldilley/projects/linmod]# cat /dev/linmod
cat: /dev/linmod: No such device or address
```

##### Module dependency
```
[root@lindev /home/ldilley/projects/linmod]# insmod linmod2.ko
insmod: ERROR: could not insert module linmod2.ko: Unknown symbol in module
[root@lindev /home/ldilley/projects/linmod]# insmod linmod.ko
[root@lindev /home/ldilley/projects/linmod]# insmod linmod2.ko
[root@lindev /home/ldilley/projects/linmod]# dmesg | tail -4
[934782.497545] linmod->linmod_init(): Kernel module loaded.
[934783.977805] linmod2->linmod2_init(): Kernel module loaded.
[934783.977815] linmod2->linmod2_init(): Calling linmod_sum(2, 3)...
[934783.977823] linmod->linmod_sum(2, 3): 2 + 3 = 5
[root@lindev /home/ldilley/projects/linmod]# lsmod | grep linmod
linmod2            12545  0
linmod             12769  1 linmod2
[root@lindev /home/ldilley/projects/linmod]# rmmod linmod
rmmod: ERROR: Module linmod is in use by: linmod2
```

##### ProcFS
```
[root@lindev /home/ldilley/projects/linmod]# insmod linmod_proc.ko
[root@lindev /home/ldilley/projects/linmod]# cat /proc/linmod
test
[root@lindev /home/ldilley/projects/linmod]# echo "foo bar baz" > /proc/linmod
[root@lindev /home/ldilley/projects/linmod]# cat /proc/linmod
foo bar baz
[root@lindev /home/ldilley/projects/linmod]# rmmod linmod_proc
[root@lindev /home/ldilley/projects/linmod]# cat /proc/linmod
cat: /proc/linmod: No such file or directory
```

##### SysFS
```
[root@lindev /home/ldilley/projects/linmod]# insmod linmod_sysfs.ko
[root@lindev /home/ldilley/projects/linmod]# ls -la /sys/kernel/linmod/
total 0
drwxr-xr-x.  2 root root    0 Jan  6 22:26 .
drwxr-xr-x. 12 root root    0 Dec 27 02:33 ..
-rw-r--r--.  1 root root 4096 Jan  6 22:26 test
[root@lindev /home/ldilley/projects/linmod]# cat /sys/kernel/linmod/test
0
[root@lindev /home/ldilley/projects/linmod]# echo 7 > /sys/kernel/linmod/test
[root@lindev /home/ldilley/projects/linmod]# cat /sys/kernel/linmod/test
7
[root@lindev /home/ldilley/projects/linmod]# dmesg | tail -4
[935587.717754] linmod_sysfs->linmod_init(): Kernel module loaded.
[935613.579523] linmod_sysfs->sysfs_read(): 0 (2 bytes) read from /sys/kernel/linmod/test.
[935643.362360] linmod_sysfs->sysfs_write(): 7 (2 bytes) written to /sys/kernel/linmod/test.
[935647.926363] linmod_sysfs->sysfs_read(): 7 (2 bytes) read from /sys/kernel/linmod/test.
[root@lindev /home/ldilley/projects/linmod]# rmmod linmod_sysfs.ko
[root@lindev /home/ldilley/projects/linmod]# cat /sys/kernel/linmod/test
cat: /sys/kernel/linmod/test: No such file or directory
```

##### Sysctl
```
[root@lindev /home/ldilley/projects/linmod]# insmod linmod_sysctl.ko
[root@lindev /home/ldilley/projects/linmod]# sysctl -a | grep linmod
linmod.test = 0
[root@lindev /home/ldilley/projects/linmod]# sysctl linmod.test=3
sysctl: setting key "linmod.test": Invalid argument
linmod.test = 3
[root@lindev /home/ldilley/projects/linmod]# sysctl linmod.test=1
linmod.test = 1
[root@lindev /home/ldilley/projects/linmod]# sysctl -a | grep linmod
linmod.test = 1
[root@lindev /home/ldilley/projects/linmod]# rmmod linmod_sysctl
[root@lindev /home/ldilley/projects/linmod]# sysctl -a | grep linmod
[root@lindev /home/ldilley/projects/linmod]#
```

#### Notes:
* The `-v` argument in the above instruction for `rmmod` is optional and attempts to generate verbose output if available.
* `make clean` can be used to remove all build data.
* `make test` can be used to quickly build, load, display details about, and unload the basic modules.
* The Linux kernel headers, a C compiler, `make`, and possibly other packages are required to build the modules contained in this project. The package names will vary depending on distribution.
* CentOS 7.6 was used for the development and testing of this project.
