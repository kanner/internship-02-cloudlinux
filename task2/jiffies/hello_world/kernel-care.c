// SPDX-License-Identifier: GPL-2.0-only
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/namei.h>

#define DIR_NAME "kernelcare"
#define FILE_NAME "jiffies"

static struct dentry *dir_entry;
static struct dentry *file_entry;

static ssize_t jiffies_read_ops(struct file *filp, char __user *buff,
					size_t count, loff_t *offp)
{
	unsigned long	jiffies_val = jiffies;
	char		jiffies_str[20]; // 20+1 in case we always want \n, right?
	int		len;

	len = snprintf(jiffies_str, sizeof(jiffies_str), "%lu\n", jiffies_val);
	if (*offp >= len)
		return 0;

	if (count > len - *offp)
		count = len - *offp;
	if (copy_to_user(buff, jiffies_str + *offp, count) != 0)
		return -EFAULT;
	*offp += count;
	// easier way, which you found in the next subtask, something like:
	// return simple_read_from_buffer(buff, count, offp, jiffies_str, sizeof(jiffies_str));
	return count;
}

static const struct file_operations fops = {
	.read = jiffies_read_ops,
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
	check_if_dir_exists("/sys/kernel/debug/jiffies"); /* wrong name 'jiffies', right? do we need it at all? */
	dir_entry = debugfs_create_dir(DIR_NAME, NULL);
	if (!dir_entry) {
        /*
         * wrong error check
         * such functions have a tricky return values in case of errors.
         * like ERR_PTR(-ERROR)
         *
         * see - https://elixir.bootlin.com/linux/latest/source/fs/debugfs/inode.c#L584
         */
		pr_err("Failed to create debugfs directory %s\n", DIR_NAME);
		return -ENODEV;
	}

        /*
         * this case can be done even without fops and error-prone magic, using:
         * debugfs_create_u64(..., (u64 *)&jiffies);
         */
	file_entry = debugfs_create_file(FILE_NAME, 0444, dir_entry, NULL, &fops);

	if (!file_entry) {
        /*
         * same here
         *
         * see - https://elixir.bootlin.com/linux/latest/source/fs/debugfs/inode.c#L478
         */
		pr_err("Failed to create debugfs file %s\n", FILE_NAME);
		debugfs_remove(dir_entry);
		return -ENODEV;
	}

	pr_info("Debugfs directory and file created successfully\n");
	return 0;
}

static void __exit jiffies_exit(void)
{
	debugfs_remove_recursive(dir_entry);
	pr_info("Debugfs directory and file removed\n");
}

module_init(jiffies_init);
module_exit(jiffies_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vasileios Almpanis");
MODULE_DESCRIPTION("Kernel module creating debugfs directory and file for jiffies");

