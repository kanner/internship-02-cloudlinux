# Task 2

~~~
* task 2 */

Make sure debugfs is enabled in your kernel (CONFIG_DEBUG_FS) and mount debugfs for the distro you use [0].

- Take the kernel module you wrote for task 1, and modify it to create a debugfs subdirectory called "kernelcare".

- In that directory, create a new debugfs file called "jiffies".

- This "jiffies" file is to be read-only by any user, and when read, should return the current value of the jiffies kernel timer.

- Submit this result as a patch against task 1.

- Create one more debugfs file called "data".

- The file "data" needs to be writable only by root, but readable by  anyone. When writing to it, the value must be stored, up to one page of data. When read, which can be done by any user, the value that is stored in it must be returned. Properly handle the fact that someone could be reading from the file while someone else is writing to it (oh, a locking hint!)

- Submit this result as a patch against previous one with the "jiffies".                                        

- When the module is unloaded, all of the debugfs files are cleaned up, and any memory allocated is freed.

                                        

Bonus points if you provide some "proof" this all works.

--

Link[0]: https://www.kernel.org/doc/html/latest/filesystems/debugfs.html
~~~


## To mount debugfs for one time:
~~~
mount -t debugfs none /sys/kernel/debug
~~~

## To mount it by default on system boot  
> Edit /etc/fstab with sudo
~~~
debugfs    /sys/kernel/debug      debugfs  defaults  0 0
~~~

## Jiffies
### Enter the directory where the module is and load the module
~~~
export "CONFIG_SAMPLE_KERNEL_CARE=m"
make
sudo insmod kernel-care.ko
~~~

### Read the jiffies file
~~~
sudo cat /sys/kernel/debug/kernelcare/jiffies
~~~

>output will look similar to: 4295623222

### Unload the module
~~~
sudo rmmod kernel_care
~~~
