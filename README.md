# selfscheduler
A self-created counter to overcome unreliable RTC by utilising shared memory to enable access from multiple processes in single OS.

## Building
As it was built to target PPC machines, a custom variable CROSS_COMPILE needs to be set to empty string.
```
make CROSS_COMPILE=
```
