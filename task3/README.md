# Task3

~~~
/* task 3 */

Let’s do some stuff you would learn in your "intro to data structures" class, if you happened to take one.

Yes, linked lists. The kernel has a unique way of creating and handling linked lists, that is quite different than the "textbook" way of doing so. But, it turns out to be faster, and simpler, than a "textbook" would describe, so that’s a good thing.


- Base your work on the kernel module you wrote for task 1 or task 2.

- You have a structure that has 3 fields:                                

char  name[20];

int   id;

bool  hired;                                        

 name this structure "identity".

- Your module has a static variable that points to a list of these "identity" structures.

- Write a function that looks like:

int identity_create(char *name, int id);                                        

 that creates the structure "identity", copies in the ’name’ and ’id’

  fields and sets ’hired’ to false. Proper error checking for out of memory issues is required. Return 0 if everything went ok; an error value if something went wrong.        

- Write a function that looks like:

struct identity *identity_find(int id);                                

that takes a given id, iterates over the list of all ids, and returns the proper ’struct identity’ associated with it. If the identity is not found, return NULL.                                

- Write a function that looks like:

void identity_destroy(int id);

that given an id, finds the proper ’struct identity’ and removes it from the system.

- Write a function that looks like:                                

int identity_hire(int id);

that given an id, finds the proper ’struct identity’ and sets the ’hired’ field to true. Return 0 if everything went ok; an error value if something went wrong.                                

- Your module_init() function will look much like the following:


struct identity *temp;

                                

identity_create("YOUR_NAME", 1);

identity_create("YOUR_FRIEND_NAME", 2);

...

                                        

temp = identity_find(1);

pr_debug("id 1 = %s\n", temp->name);

                                        

identity_hire(1);

                                        

temp = identity_find(10);

if (temp == NULL)                                        

   pr_debug("id 10 not found\n");

                                        

identity_destroy(2);

identity_destroy(1);

...


                                        

Bonus points for properly checking return values of the above functions.   
~~~