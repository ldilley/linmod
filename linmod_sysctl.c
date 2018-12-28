/* linmod_sysctl.c */

/*
 * An example Linux kernel module to demonstrate a sysctl key/value
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysctl.h>

const char *MODULE_NAME = "linmod_sysctl";
#define SYSCTL_DIR "linmod" /* directory under /proc/sys */
#define SYSCTL_LEAF "test"  /* file under /proc/sys/SYSCTL_DIR */

MODULE_AUTHOR("Lloyd Dilley");
MODULE_DESCRIPTION("An example Linux kernel module to demonstrate a sysctl key/value");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

static int initial_value = 0; /* initial leaf value */
static int min_value = 0;     /* minimum leaf value */
static int max_value = 1;     /* maximum leaf value */
static struct ctl_table_header *linmod_table_header;

static ctl_table state_table[] =
{
  {
    /*.ctl_name = CTL_UNNUMBERED,*/        /* allows registration without a unique ID */
    .procname = SYSCTL_LEAF,               /* file name under /proc/sys/SYSCTL_DIR */
    .mode = 0644,                          /* /proc/sys/SYSCTL_DIR/SYSCTL_LEAF with perms -rw-r--r-- */
    .proc_handler = &proc_dointvec_minmax, /* ensure written data falls within range of min/max */
    .data = &initial_value,                /* initial leaf value */
    .maxlen = sizeof(int),                 /* max data size in bytes */
    .extra1 = &min_value,                  /* minimum leaf value */
    .extra2 = &max_value                   /* maximum leaf value */
  },
  { 0 }
};

static ctl_table parent_table[] =
{
  {
    /*.ctl_name = CTL_UNNUMBERED,*/        /* allows registration without unique ID */
    .procname = SYSCTL_DIR,                /* directory under /proc/sys */
    .mode = 0555,                          /* /proc/sys/SYSCTL_DIR with perms -r-xr-xr-x */
    .child = state_table                   /* contains leaf */
  },
  { 0 }
};

int linmod_init(void)
{
  linmod_table_header = register_sysctl_table(parent_table);
  if(!linmod_table_header)
  {
    printk(KERN_ERR "%s->%s(): EFAULT during sysctl table registration.\n", MODULE_NAME, __func__);
    return -EFAULT;
  }
  printk(KERN_INFO "%s->%s(): Created sysctl key %s.%s.\n", MODULE_NAME, __func__, SYSCTL_DIR, SYSCTL_LEAF);
  printk(KERN_INFO "%s->%s(): Kernel module loaded.\n", MODULE_NAME, __func__);
  return 0;
}

void linmod_exit(void)
{
  unregister_sysctl_table(linmod_table_header);
  printk(KERN_INFO "%s->%s(): Removed sysctl key %s.%s.\n", MODULE_NAME, __func__, SYSCTL_DIR, SYSCTL_LEAF);
  printk(KERN_INFO "%s->%s(): Kernel module unloaded.\n", MODULE_NAME, __func__);
  return;
}

module_init(linmod_init);
module_exit(linmod_exit);
