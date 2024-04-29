// SPDX-License-Identifier: GPL-2.0-only
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/list.h>

#define MAX_NAME_LEN 20

struct identity {
	char name[MAX_NAME_LEN];
	int id;
	bool hired;
	struct list_head list;
};

// list head initialized at compile time
LIST_HEAD(identity_list);

// It can be done also like:
// struct list_head identity_list = LIST_HEAD_INIT(identity_list);
// then when creating a new identity:
// we need to also initialize the list_head like:
// INIT_LIST_HEAD(&new_identity->list);
struct identity *identity_find(int id)
{
	struct identity *temp;

	// loop cursor, temp storage, member
	list_for_each_entry(temp, &identity_list, list) {
		// if id found return member
		if (temp->id == id)
			return temp;
	}

	return NULL;
}

int identity_create(char *name, int id)
{
	struct identity *new_identity;
	struct identity *temp = NULL;

	// check if name is too long
	if (strlen(name) > MAX_NAME_LEN)
		return -EINVAL;
	// check if id already exists
	temp = identity_find(id);
	if (temp)
		return -EEXIST;
	// allocate memory for new identity
	new_identity = kmalloc(sizeof(struct identity), GFP_KERNEL);
	if (!new_identity)
		return -ENOMEM;
	strscpy(new_identity->name, name, MAX_NAME_LEN);
	// default values
	new_identity->id = id;
	new_identity->hired = false;
	// add to list
	list_add_tail(&new_identity->list, &identity_list);
	return 0;
}

void identity_destroy(int id)
{
	struct identity *temp, *next;

	// loop cursor, temp storage, list head, member
	list_for_each_entry_safe(temp, next, &identity_list, list) {
		// if id found delete and free memory
		if (temp->id == id) {
			list_del(&temp->list);
			kfree(temp);
			return;
		}
	}
}

int identity_hire(int id)
{
	struct identity *node = identity_find(id);

	if (!node)
		return -EINVAL;
	node->hired = true;
	return 0;
}

static int __init identity_init(void)
{
	struct identity *temp;

	if (identity_create("valmpani", 1))
		pr_debug("Failed to create identity\n");

	if (identity_create("alice", 2))
		pr_debug("Failed to create identity\n");
	if (identity_create("alice", 2))
		pr_debug("Failed to create identity alice\n");
	temp = identity_find(1);
	if (temp)
		pr_debug("id 1 = %s\n", temp->name);
	if (identity_hire(1))
		pr_debug("identity with id 1 is not hired\n");
	if (identity_hire(10))
		pr_debug("identity with id 10 is not hired\n");
	temp = identity_find(10);
	if (temp)
		pr_debug("id 10 = %s\n", temp->name);
	else
		pr_debug("id 10 not found\n");

	return 0;
}

static void __exit identity_exit(void)
{
	struct identity *temp, *next;

	list_for_each_entry_safe(temp, next, &identity_list, list) {
		pr_info("Freeing node %s\n", temp->name);
		list_del(&temp->list);
		kfree(temp);
	}
}

module_init(identity_init);
module_exit(identity_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vasileios Almpanis");
MODULE_DESCRIPTION("Kernel module for linked list manipulation");

