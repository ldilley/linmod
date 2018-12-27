/* linmod_proc.c */

/*
 * An example Linux kernel module to demonstrate I/O via a proc file
 */

#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>    /* kmalloc() */

const char *MODULE_NAME = "linmod_proc";
const char *PROC_FILE = "linmod";

MODULE_AUTHOR("Lloyd Dilley");
MODULE_DESCRIPTION("An example Linux kernel module to demonstrate I/O via a proc file");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

/* Function prototypes for device file operations */
static ssize_t proc_read(struct file *, char *, size_t, loff_t *);
static ssize_t proc_write(struct file *, const char *buf, size_t, loff_t *);

static char *msg_buffer = NULL;

/* Structure of proc file operations tied to functions */
struct file_operations file_ops =
{
  .read = proc_read,
  .write = proc_write
};

/* Called when proc file is read from */
static ssize_t proc_read(struct file *flip, char *buf, size_t len, loff_t *offset)
{
  int bufflen = strlen(msg_buffer);
  /* Used to determine how much data left to read */
  ssize_t bytes = len < (bufflen - (*offset)) ? len : (bufflen - (*offset));
  if(copy_to_user(buf, msg_buffer, bytes))
  {
    printk(KERN_ERR "%s->%s(): EFAULT during read from /proc/%s.\n", MODULE_NAME, __func__, PROC_FILE);
    return -EFAULT;
  }
  (*offset) += bytes;
  printk(KERN_INFO "%s->%s(): %zu bytes read from /proc/%s.\n", MODULE_NAME, __func__, bytes, PROC_FILE);
  return bytes;
}

/* Called when proc file is written to */
static ssize_t proc_write(struct file *flip, const char *buf, size_t len, loff_t *offset)
{
  if(msg_buffer)
    kfree(msg_buffer);
  msg_buffer = (char *)kmalloc((sizeof(char) + 1) * len, GFP_KERNEL);
  if(!msg_buffer)
  {
    printk(KERN_ERR "%s->%s(): ENOMEM during /proc/%s buffer allocation.\n", MODULE_NAME, __func__, PROC_FILE);
    return -ENOMEM;
  }
  if(!strncpy_from_user(msg_buffer, buf, len))
  {
    printk(KERN_ERR "%s->%s(): EFAULT during write to /proc/%s.\n", MODULE_NAME, __func__, PROC_FILE);
    return -EFAULT;
  }
  msg_buffer[len] = '\0'; /* append null terminator */
  printk(KERN_INFO "%s->%s(): %zu bytes written to /proc/%s.\n", MODULE_NAME, __func__, len, PROC_FILE);
  return len;
}

int create_new_proc_entry(void)
{
  int len = 0;
  char *initial_data = "test\n";
  proc_create(PROC_FILE, 0, NULL, &file_ops);
  len = strlen(initial_data);
  msg_buffer = (char *)kmalloc((sizeof(char) + 1) * len, GFP_KERNEL);
  if(!msg_buffer)
  {
    printk(KERN_ERR "%s->%s(): ENOMEM during /proc/%s buffer allocation.\n", MODULE_NAME, __func__, PROC_FILE);
    return -ENOMEM;
  }
  strncpy(msg_buffer, initial_data, len);
  msg_buffer[len] = '\0'; /* append null terminator */
  return 0;
}

int linmod_init(void)
{
  create_new_proc_entry();
  printk(KERN_INFO "%s->%s(): Created /proc/%s.\n", MODULE_NAME, __func__, PROC_FILE);
  printk(KERN_INFO "%s->%s(): Kernel module loaded.\n", MODULE_NAME, __func__);
  return 0;
}

void linmod_exit(void)
{
  remove_proc_entry(PROC_FILE, NULL);
  printk(KERN_INFO "%s->%s(): Removed /proc/%s.\n", MODULE_NAME, __func__, PROC_FILE);
  if(msg_buffer)
    kfree(msg_buffer);
  printk(KERN_INFO "%s->%s(): kfree(msg_buffer)\n", MODULE_NAME, __func__);
  printk(KERN_INFO "%s->%s(): Kernel module unloaded.\n", MODULE_NAME, __func__);
  return;
}

module_init(linmod_init);
module_exit(linmod_exit);
