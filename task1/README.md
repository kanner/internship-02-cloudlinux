# Task1 Kernelcare

~~~
Write a Linux kernel module, and stand-alone Makefile [0], that when loaded prints to the kernel debug log level, "Hello, KernelCare!". Be sure to make the module be able to be unloaded as well.

                                        

Bonus points if you do this like a real kernel dev:

* within Linus’s latest git tree [1], e.g. in samples/hello_world/

* using some new config option, e.g. CONFIG_SAMPLE_HELLO_WORLD

* send result as a patch, which is ok with checkpatch.pl [2]

                                        

Extra bonus points if you do it on a "real" machine and not a virtual one, or show proof how it works for the latest Linux kernel.

--

Link[0]: https://www.kernel.org/doc/html/latest/kbuild/modules.html

Link[1]: https://git.kernel.org/linus/                

Link[2]: https://www.kernel.org/doc/html/latest/process/submitting-patches.html#the-can

onical-patch-format
~~~

## How to build the module in the hello-world directory
~~~
cd hello_world
make
~~~

## Load and unload the module
~~~
sudo insmod kernel-care.ko
sudo rmmod kernel_care
sudo dmesg
~~~

### The output will look something like this
~~~
[37439.220976] Hello, KernelCare!
[37655.279522] Goodbye, KernelCare!
~~~

## We could also do:

~~~
cat /var/log/syslog
~~~

## When the module is loaded we can see it by running:
~~~
cat /proc/modules | grep kernel_care
~~~

### The output will look like this:
~~~
kernel_care 16384 0 - Live 0x0000000000000000 (OE)
~~~
When we unload the module if we run the same command there will be no output  

> To do the bonuses we have to install dependencies in advance.  
### First we clone the latest kernel tree
~~~
git clone git@github.com:torvalds/linux.git
~~~

### Then we need to install some dependencies to be able to run checkpatch.pl
>For python version 3.11^
~~~
sudo apt install python3-ply
sudo apt install python3-git
~~~

### make menuconfig dependencies
~~~
sudo apt install flex
sudo apt install bison
~~~

### To use a CONFIG_SAMPLE_KERNEL_CARE in my Makefile I introduce a new submenu in the kernel's Kconfig file and I remove the rules for "all" and "clean" from my Makefile  
#### To enable this module in menuconfig you go to:  
>Kernel hacking-> Sample Kernel code -> Build kernel care sample  
> Press space until you see the asterisk in the field

### With the new makefile to build the module run
~~~
make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
~~~

# Every screen shot provided runs on my ubuntu 23.10 and not in a VM.