// SPDX-License-Identifier: GPL-2.0-only
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vasileios Almpanis");
MODULE_DESCRIPTION("A simple kernel module that prints a message");

static int __init hello_init(void)
{
	pr_debug("Hello, KernelCare!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_debug("Goodbye, KernelCare!\n");
}

module_init(hello_init);
module_exit(hello_exit);
