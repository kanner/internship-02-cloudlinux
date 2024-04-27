# SPDX-License-Identifier: GPL-2.0-only
obj-$(CONFIG_SAMPLE_KERNEL_CARE) += kernel-care.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
