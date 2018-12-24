/* linmod.c */

/*
 * An example Linux kernel module
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

const char *MODULE_NAME = "linmod";

MODULE_AUTHOR("Lloyd Dilley");
MODULE_DESCRIPTION("A demonstrational kernel module");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

static void linmod_sum(int x, int y)
{
  printk("%s->%s(%d, %d): %d + %d = %d\n", MODULE_NAME, __func__, x, y, x, y, x + y);
  return;
}
EXPORT_SYMBOL(linmod_sum); /* allows function to be called by other modules */

static int __init linmod_init(void)
{
  printk(KERN_INFO "%s->%s(): Kernel module loaded.\n", MODULE_NAME, __func__);
  return 0;
}

static void __exit linmod_exit(void)
{
  printk(KERN_INFO "%s->%s(): Kernel module unloaded.\n", MODULE_NAME, __func__);
  return;
}

module_init(linmod_init);
module_exit(linmod_exit);
