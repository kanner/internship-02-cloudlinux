// SPDX-License-Identifier: GPL-2.0-only
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/namei.h>
#include <linux/string.h>

#define DIR_NAME "kernelcare"
#define FILE_NAME1 "jiffies"
#define FILE_NAME2 "data"
#define MAX_BUFFER_SIZE (PAGE_SIZE + 1)

// PAGE_SIZE + 1 for null terminator
static char data_buffer[MAX_BUFFER_SIZE];
/*
 * JFYI: Linux kernel has different types of locks,
 * https://www.kernel.org/doc/Documentation/locking/locktypes.rst
 * one or another is better in different situations,
 * rwlock_t seems to be better here, but mutex is ok for this task
 */
static DEFINE_MUTEX(data_mutex);
static struct dentry *dir_entry;
static struct dentry *file_entry_jiffies;
static struct dentry *file_entry_data;

static ssize_t jiffies_read_ops(struct file *filp, char __user *buff,
					size_t count, loff_t *offp)
{
	unsigned long	jiffies_val = jiffies;
	char		jiffies_str[20];
	int		len;

	len = snprintf(jiffies_str, sizeof(jiffies_str), "%lu\n", jiffies_val);
	if (*offp >= len)
		return 0;

	if (count > len - *offp)
		count = len - *offp;
	if (copy_to_user(buff, jiffies_str + *offp, count) != 0)
		return -EFAULT;
	*offp += count;
	return count;
}

static ssize_t data_read_ops(struct file *filp, char __user *buff,
					size_t count, loff_t *offp)
{
	size_t ret;

	mutex_lock(&data_mutex);
	ret = simple_read_from_buffer(buff, count, offp, data_buffer, strlen(data_buffer));
	mutex_unlock(&data_mutex);
	return ret;
}

static ssize_t data_write_ops(struct file *filp, const char __user *buff,
					size_t count, loff_t *offp)
{
	size_t ret;

	mutex_lock(&data_mutex);
	memset(data_buffer, 0, MAX_BUFFER_SIZE);
	ret = simple_write_to_buffer(data_buffer, MAX_BUFFER_SIZE - 1, offp, buff, count);
	mutex_unlock(&data_mutex);
	return ret;
}

static const struct file_operations fops = {
	.read = jiffies_read_ops,
};

static const struct file_operations fops_data = {
	.read = data_read_ops,
	.write = data_write_ops,
};

static void check_if_dir_exists(const char *path)
{
	struct path p;
	int ret = kern_path(path, LOOKUP_DIRECTORY, &p);

	if (!ret)
		debugfs_remove_recursive(p.dentry);
}

static int __init jiffies_init(void)
{
	check_if_dir_exists("/sys/kernel/debug/jiffies");
	check_if_dir_exists("/sys/kernel/debug/data");
	// create directory
	dir_entry = debugfs_create_dir(DIR_NAME, NULL);
	if (!dir_entry) {
		pr_err("Failed to create debugfs directory %s\n", DIR_NAME);
		return -ENODEV;
	}

	// create jiffies file
	file_entry_jiffies = debugfs_create_file(FILE_NAME1, 0444, dir_entry, NULL, &fops);

	if (!file_entry_jiffies) {
		pr_err("Failed to create debugfs file %s\n", FILE_NAME1);
		debugfs_remove(dir_entry);
		return -ENODEV;
	}
	// create data file
	file_entry_data = debugfs_create_file(FILE_NAME2, 0644, dir_entry, NULL, &fops_data);
	if (!file_entry_data) { // same check issue here
		pr_err("Failed to create debugfs file %s\n", FILE_NAME2);
		debugfs_remove_recursive(dir_entry);
		return -ENODEV;
	}
	memset(data_buffer, 0, MAX_BUFFER_SIZE);
	pr_info("Debugfs directory and file created successfully\n");
	return 0;
}

static void __exit jiffies_exit(void)
{
	// remove files and directory
	debugfs_remove_recursive(dir_entry);
	pr_info("Debugfs directory and file removed\n");
}

module_init(jiffies_init);
module_exit(jiffies_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vasileios Almpanis");
MODULE_DESCRIPTION("Kernel module creating debugfs directory and file for data");

