/* linmod_sysfs.c */

/*
 * An example Linux kernel module to demonstrate I/O via a sysfs file
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/vmalloc.h>

const char *MODULE_NAME = "linmod_sysfs";
const char *SYSFS_DIR = "linmod";
#define SYSFS_FILE "test"

MODULE_AUTHOR("Lloyd Dilley");
MODULE_DESCRIPTION("An example Linux kernel module to demonstrate I/O via a sysfs file");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

/* Function prototypes for device file operations */
static ssize_t sysfs_read(struct kobject *, struct kobj_attribute *, char *);
static ssize_t sysfs_write(struct kobject *, struct kobj_attribute *, const char *, size_t);

static struct kobject *linmod_kobj; /* /sys/kernel/SYSFS_DIR path */
static int test = 0; /* test file residing under /sys/kernel/SYSFS_DIR */

/* File attributes and operations for the test file */
static struct kobj_attribute test_attr =
{
  .attr.name = SYSFS_FILE,
  .attr.mode = 0644,
  .show = sysfs_read,
  .store = sysfs_write
};

/* __ATTR() macro also works for above if desired */
/*__ATTR(test, 0644, sysfs_read, sysfs_write);*/

/* Called when sysfs file is read from */
static ssize_t sysfs_read(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
  ssize_t bytes = sprintf(buf, "%d\n", test);
  printk(KERN_INFO "%s->%s(): %d (%zu bytes) read from /sys/kernel/%s/%s.\n", MODULE_NAME, __func__, test, bytes, SYSFS_DIR, SYSFS_FILE);
  return bytes;
}

/* Called when sysfs file is written to */
static ssize_t sysfs_write(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
  int len = 0;
  char *buffer = NULL;
  sscanf(buf, "%du", &test);
  len = strlen(buf);
  buffer = (char *)vmalloc(sizeof(char) * len);
  if(!buffer)
  {
    printk(KERN_ERR "%s->%s(): ENOMEM during write to /sys/kernel/%s/%s.\n", MODULE_NAME, __func__, SYSFS_DIR, SYSFS_FILE);
    return -ENOMEM;
  }
  strcpy(buffer, buf);
  buffer[strcspn(buffer, "\n")] = 0; /* omit trailing newline for printk() */
  printk(KERN_INFO "%s->%s(): %s (%zu bytes) written to /sys/kernel/%s/%s.\n", MODULE_NAME, __func__, buffer, count, SYSFS_DIR, SYSFS_FILE);
  vfree(buffer);
  return count;
}

static int __init linmod_init(void)
{
  int ret_val = 0;
  linmod_kobj = kobject_create_and_add(SYSFS_DIR, kernel_kobj);
  if(!linmod_kobj)
  {
    printk(KERN_ERR "%s->%s(): ENOMEM during write to /sys/kernel/%s.\n", MODULE_NAME, __func__, SYSFS_DIR);
    return -ENOMEM;
  }
  printk(KERN_INFO "%s->%s(): Created /sys/kernel/%s.\n", MODULE_NAME, __func__, SYSFS_DIR);
  ret_val = sysfs_create_file(linmod_kobj, &test_attr.attr);
  if(ret_val)
  {
    printk(KERN_ERR "%s->%s(): Unable to create /sys/kernel/%s/%s.\n", MODULE_NAME, __func__, SYSFS_DIR, SYSFS_FILE);
    return ret_val;
  }
  printk(KERN_INFO "%s->%s(): Created /sys/kernel/%s/%s.\n", MODULE_NAME, __func__, SYSFS_DIR, SYSFS_FILE);
  printk(KERN_INFO "%s->%s(): Kernel module loaded.\n", MODULE_NAME, __func__);
  return ret_val;
}

static void __exit linmod_exit(void)
{
  kobject_put(linmod_kobj);
  printk(KERN_INFO "%s->%s(): Removed /sys/kernel/%s/%s.\n", MODULE_NAME, __func__, SYSFS_DIR, SYSFS_FILE);
  printk(KERN_INFO "%s->%s(): Kernel module unloaded.\n", MODULE_NAME, __func__);
  return;
}

module_init(linmod_init);
module_exit(linmod_exit);
