# Soteria Unwind

## Usage
```
$> make
$> sudo insmod kernel_unwind.ko
$> sudo dmesg
```

clean

```
$> make clean
$> sudo rmmod kernel_unwind
```
## Example
```
[16866.662287] stack_trace_save_tsk addr: ffff80000834e324
[16866.662295] kernel_unwind: starting...
[16866.662297] [ztex] kernel_unwind: backtrace for process 3061 (bash):
[16866.662300]  __switch_to+0xf8/0x150
[16866.662312]  do_wait+0x154/0x340
[16866.662314]  kernel_wait4+0x98/0x1b4
[16866.662316]  __do_sys_wait4+0xb0/0x10c
[16866.662317]  __arm64_sys_wait4+0x2c/0x40
[16866.662319]  invoke_syscall+0x78/0x100
[16866.662323]  el0_svc_common.constprop.0+0x54/0x184
[16866.662326]  do_el0_svc+0x30/0x9c
[16866.662329]  el0_svc+0x48/0x160
[16866.662366]  el0t_64_sync_handler+0xa4/0x130
[16866.662369]  el0t_64_sync+0x1a4/0x1a8
[16908.462634] kernel_unwind: exiting...
```