/* linmod2.c */

/*
 * An example Linux kernel module to demonstrate module dependency
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

const char *MODULE_NAME = "linmod2";

MODULE_AUTHOR("Lloyd Dilley");
MODULE_DESCRIPTION("A demonstrational kernel module");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
MODULE_SOFTDEP("pre: linmod");

extern int linmod_sum(int x, int y);

static int __init linmod2_init(void)
{
  printk(KERN_INFO "%s->%s(): Kernel module loaded.\n", MODULE_NAME, __func__);
  printk(KERN_INFO "%s->%s(): Calling linmod_sum(2, 3)...\n", MODULE_NAME, __func__);
  linmod_sum(2, 3); /* creates a dependency on module exporting this function */
  return 0;
}

static void __exit linmod2_exit(void)
{
  printk(KERN_INFO "%s->%s(): Kernel module unloaded.\n", MODULE_NAME, __func__);
  return;
}

module_init(linmod2_init);
module_exit(linmod2_exit);
