## README.md

No drift detected.

## apps/README.md

No drift detected.

## nuttx/README.md

- Incomplete State Progression in `Invariants`: Line 79 states that `g_nx_initstate` progresses monotonically through `OSINIT_BOOT`, `OSINIT_TASKLISTS`, `OSINIT_MEMORY`, `OSINIT_OSREADY`, and `OSINIT_IDLELOOP` during `nx_start()`, omitting the intermediate `OSINIT_HARDWARE` state assigned at `nuttx/sched/init/nx_start.c:660` (and checked by `OSINIT_HW_READY()` in `nuttx/include/nuttx/init.h:104`).
