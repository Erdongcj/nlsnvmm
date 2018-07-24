# NLNVM

### NLSPM: NUMA-aware Log-Structured Persistent Memory

NLSPM provides a simple interface for programming with persistent 
memory. 
### Authors:

### Dependencies:

* SCons: A software construction tool
* GCC 6.2.1 or above
* GLIBC 2.19 or above
* libconfig
```
	Fedora : $ dnf install libconfig-devel.x86_64 libconfig.x86_64
	Ubuntu : $ apt-get install libconfig-dev libconfig9
```
* gelf
```
	Fedora : $ dnf install elfutils-libelf-devel.x86_64 elfutils-libelf.x86_64
	Ubuntu : $ apt-get install libelf-dev elfutils
```
* libevent (For memcached)
```
	Fedora : $ dnf install libevent-devel.x86_64 
	Ubuntu : $ apt-get install libevent-dev
```
* /dev/shm or mount point backed by persistent memory
	- The heap will be placed in segments_dir defined in mnemosyne.ini
	- Please ensure you have at least 1.00 GB of space for the heap.

```

### Build NLSPM:
```
$ cd usermode
$ scons [--build-stats] [--config-ftrace] [--verbose]
 
* scons -h <For more options>
```

```
### Build bench:
```
$ scons --build-bench=hashmap/btreemap/binarytree  [--verbose]
$ ./build/bench/hashmap/hashmap
```

### Documentation:
For further information please refer to the Doxygen generated documentation.
Running doxygen will create documentation under mnemosyne/doc/html

$ cd ./usermode/..
$ doxygen

### License:

GPL-V2
See license file under each module

# 18-numa-lspm
# NLSPM
# nlnvm
