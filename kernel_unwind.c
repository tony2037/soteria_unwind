#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/kallsyms.h>
#include <linux/stacktrace.h>
#include <linux/kprobes.h>

#define MAX_DEPTH 10

static int pid = 0;

module_param(pid, int, 0);
MODULE_PARM_DESC(pid, "Process ID");

void (*show_stack_fn)(struct task_struct *task, unsigned long *sp, const char *loglvl);

int noop_pre(struct kprobe *p, struct pt_regs *regs) { return 0; }
void *find_kallsyms_lookup_name(char *sym) {
    int ret;
    void *p;
    struct kprobe kp = {
        .symbol_name = sym,
        .pre_handler = noop_pre
    };
    if ((ret = register_kprobe(&kp)) < 0) {
        printk(KERN_INFO "fail to register kprobe\n", ret);
        return NULL;
    }

    p = kp.addr;
    printk(KERN_INFO "%s addr: %lx\n", sym, (unsigned long)p);
    unregister_kprobe(&kp);
    return p;
}

static int __init kernel_unwind_init(void)
{
    struct task_struct *task;
    unsigned long store[1024];
    show_stack_fn = find_kallsyms_lookup_name("show_stack");
    if (!show_stack_fn)
        return -EINVAL;
    printk(KERN_INFO "kernel_unwind: starting...\n");

    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (!task) {
        printk(KERN_INFO "kernel_unwind: process with pid %d not found\n", pid);
        return -EINVAL;
    }

    printk(KERN_INFO "[ztex] kernel_unwind: backtrace for process %d (%s):\n", pid, task->comm);
    show_stack_fn(task, NULL, KERN_INFO);
    return 0;
}

static void __exit kernel_unwind_exit(void)
{
    printk(KERN_INFO "kernel_unwind: exiting...\n");
}

module_init(kernel_unwind_init);
module_exit(kernel_unwind_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ztex");
MODULE_DESCRIPTION("Get stack trace for arbitrary process by PID");

