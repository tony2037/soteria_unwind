#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <asm/stacktrace.h>

static int pid = 0;

module_param(pid, int, 0);

static int __init my_module_init(void) {
    struct task_struct *task;
    struct thread_info *ti;
    unsigned long *stack;
    int i, depth = 16;

    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (!task) {
        pr_err("Could not find task with pid %d\n", pid);
        return -EINVAL;
    }

    ti = task_thread_info(task);
    stack = (unsigned long *)ti->task->mm->start_stack;

    pr_info("Backtrace for PID %d:\n", pid);
    print_ip_sym(KERN_INFO, (unsigned long)task->thread.ip, "\n");

    for (i = 0; i < depth; i++) {
        if (!in_task_stack(ti, (unsigned long)stack))
            break;
        print_ip_sym(KERN_INFO, (unsigned long)*stack, "\n");
        stack++;
    }

    return 0;
}

static void __exit my_module_exit(void) {
    pr_info("Module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ztex");