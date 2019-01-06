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

#### Notes:
* The `-v` argument in the above instruction for `rmmod` is optional and attempts to generate verbose output if available.
* `make clean` can be used to remove all build data.
* `make test` can be used to quickly build, load, display details about, and unload the basic modules.
* The Linux kernel headers, a C compiler, `make`, and possibly other packages are required to build the modules contained in this project. The package names will vary depending on distribution.
* CentOS 7.6 was used for the development and testing of this project.
