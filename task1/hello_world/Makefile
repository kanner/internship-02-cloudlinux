# SPDX-License-Identifier: GPL-2.0-only
obj-m += kernel-care.o
CFLAGS_kernel-care.o := -DDEBUG

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
