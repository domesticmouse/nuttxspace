## What this proposes
This document proposes `xiao-rp2350-bargraph`, an out-of-tree Apache NuttX firmware project for the Seeed Studio XIAO RP2350 that samples a breadboard potentiometer on `D0` (`GPIO 26`) at 20 Hz and drives six active-high LEDs (`D3–D5`, `D8–D10`) as a proportional bar graph without modifying tracked files in `nuttx/` or `apps/`. A custom board layer registers `/dev/adc0`, clears digital pad input-enable (`IE=0`) across `GPIO 26–29` after `rp23xx_common_bringup()` completes, exposes the six LEDs as a bitmask via `/dev/userleds`, and embeds an `/etc/init.d/rcS` ROMFS startup script. At boot, `rcS` launches a singleton `bargraph` background daemon before USB console enumeration so the board runs autonomously at power-on while keeping NuttShell (`nsh>`) accessible over USB-C (`/dev/ttyACM0`). The daemon prevents continuous ADC interrupts by using per-tick `open`/`read`/`close` cycles on `/dev/adc0`, filters FIFO errors, applies a 7-level (`0..6`) hysteresis quantizer to update `/dev/userleds`, logs errors to RAMLOG (`dmesg`), and blinks an alternating fault pattern (`0x15`/`0x2A`) after repeated ADC failures.

## How the existing system works, per this document
Upstream Apache NuttX provides `xiao-rp2350` board support (`usbnsh/defconfig`) and a shared `rp23xx` common layer and ADC driver (`rp23xx_adc.c`) whose registration path (`adc_register()` -> `my_reset()`) resets all four ADC pins (`GPIO 26–29`) to floating inputs and re-enables their digital input buffers (`IE=1`). While `/dev/adc0` remains open, `rp23xx_adc.c` runs free-running conversions with no clock divider and rejects `ANIOC_TRIGGER` (`-ENOTTY`), continuously firing FIFO interrupts and dropping new samples once the 8-entry upper-half ring buffer (`adc.c`) fills. Upstream `xiao-rp2350` also omits `CONFIG_BOARD_LATE_INITIALIZE=y` from `usbnsh/defconfig`, hardcodes a single onboard LED (`GPIO 25`) in `board_userled_all()` (with `CONFIG_ARCH_LEDS` defaulting to `y`), and leaves `CONFIG_ETC_ROMFS` unimplemented in `CMakeLists.txt`. During NSH startup, `nsh_initialize()` executes `/etc/init.d/rcS` with boot FDs `0, 1, 2` bound to `/dev/null` before `nsh_consolemain()` connects USB CDC/ACM and blocks waiting for three Enter keystrokes on `/dev/ttyACM0`.

---

### Findings

1. **[S6-1] Severity**: Major
   **Affected Scope**: `Detailed Implementation Plan` -> `Phase 1 Checkpoint` (line 312) vs `Phase 1` (lines 260–310), `Phase 2 Checkpoint` (line 381), and `Phase 3` (lines 383–400).
   **Discrepancy Description & Rationale**: Line 312 states that at the Phase 1 checkpoint, `make -C xiao-rp2350-bargraph configure` links `apps/external` and configures `nuttx/.config` with zero errors, including verifying `CONFIG_BARGRAPH=y`. However, `xiao-rp2350-bargraph/external/` (including `external/Makefile`, `external/Make.defs`, and `external/bargraph/Kconfig` where `BARGRAPH` is defined) is not created until Phase 3 (Items 13–15), and `board/src/Make.defs` is not created until Phase 2 (Item 6). Executing `configure` at the end of Phase 1 would create a dangling `../apps/external` symlink and drop `CONFIG_BARGRAPH=y` because `BARGRAPH` is not yet in Kconfig; furthermore, the Phase 2 checkpoint (line 381) explicitly assumes building "without `bargraph` enabled (or before Phase 3 app code)", contradicting the Phase 1 checkpoint.

2. **[S6-2] Severity**: Major
   **Affected Scope**: `CUJ 3` (lines 61–63), `RAMLOG Diagnostics and Fault Loop` (line 184), and `Item 16: bargraph_main.c` (`bargraph_daemon`, line 432).
   **Discrepancy Description & Rationale**: The error-handling behavior of `bargraph_daemon` has two clarity gaps/inconsistencies between FeatureRange and DetailedRange:
   - *Logging timing*: Lines 61–63 and 184 indicate the daemon logs to `syslog(LOG_ERR, ...)` when it detects `>= 5` consecutive hard failures on `/dev/adc0`, whereas line 432 states "if `ret < 0`, logs once on error and increments `consecutive_errors`, calling `fault_blink_loop(led_fd)` when `consecutive_errors >= 5`". It is ambiguous whether "logs once on error" means logging on the first error in a streak, logging once when reaching 5 errors, or logging once per failed call.
   - *Retry sleep on hard errors*: Line 432 explicitly specifies sleeping `CONFIG_BARGRAPH_INTERVAL_US` on `ret == -EAGAIN` and at the end of the loop on `ret == 0`, but does not state whether `ret < 0` (while `consecutive_errors < 5`) sleeps `CONFIG_BARGRAPH_INTERVAL_US` before retrying or loops immediately without sleeping.

3. **[S6-3] Severity**: Major
   **Affected Scope**: `Item 5: board/configs/bargraph/defconfig` (`CONFIG_RP23XX_ROMFS_ROMDISK_DEVNAME`, line 310).
   **Discrepancy Description & Rationale**: Line 310 notes `(`CONFIG_RP23XX_ROMFS_ROMDISK_DEVNAME` unset)` without defining what this Kconfig symbol does, where it originates, or why leaving it unset is required for `CONFIG_ETC_ROMFS=y` / ROMFS startup.

4. **[S6-4] Severity**: Major
   **Affected Scope**: `Alternatives Considered` -> `Alternative C` (`ANIOC_RESET_FIFO`, lines 198–201).
   **Discrepancy Description & Rationale**: Alternative C's heading proposes keeping `/dev/adc0` continuously open with `ANIOC_RESET_FIFO`, but the body never defines `ANIOC_RESET_FIFO` or explains why resetting the upper-half FIFO via ioctl fails to solve the problem (the rationale only mentions `my_ioctl()` returning `-ENOTTY` for `ANIOC_TRIGGER` and FIFO overflow during `usleep(50000)`).

5. **[S6-5] Severity**: Nit
   **Affected Scope**: Undefined/unlinked NuttX symbols, build flags, and host tools (`lines 20, 21, 104, 106, 145, 166, 181, 206, 215, 268, 298, 369, 381, 388, 442, 447`).
   **Discrepancy Description & Rationale**: Several project/NuttX-specific identifiers, internal files, and tools are referenced without brief inline definitions or links in `Additional Resources` (lines 230–245):
   - `ANIOC_TRIGGER` (line 20), `group_setupidlefiles()` (line 21), `BOARDIOC_USBDEV_CONNECT` (line 145), and `GPIOC_WRITE` (line 206) lack brief inline definitions or links.
   - Referenced files `rp23xx_reset.c` and `rp23xx_boardctl.c` (line 104), `memmap_default.ld` (line 106), `arm_internal.h` (line 166), `apps/Application.mk` (line 181), `nuttx/fs/inode/fs_files.c` (line 206), `xiao-rp2350/scripts/Make.defs` (line 298), `apps/nshlib/nsh_init.c` (line 369), and `apps/Directory.mk` (line 388) are not linked in `Additional Resources`.
   - `genromfs` and `xxd` (lines 215, 442) are not briefly explained in terms of how `Board.mk` uses them to produce `nuttx/boards/arm/rp23xx/common/etctmp.c` (lines 381, 447), nor is it stated why generated `etctmp.c` inside `nuttx/` does not dirty `git status` (presumably gitignored).
   - `./tools/configure.sh -e -l` flags (line 268) are not explained.

6. **[S6-6] Severity**: Nit
   **Affected Scope**: `The Problem` (line 21), `CUJ 2` (line 56), `Rollback and Recovery` (line 222), `Item 4` (lines 297–298), `Item 11` (line 369), `Item 16` (line 432), and `Verification Plan` (lines 443, 449).
   **Discrepancy Description & Rationale**: Minor clarity and consistency gaps:
   - *Three Enter keystrokes* (lines 21, 56, 145, 210, 451): Repeatedly mentioned without briefly noting why `NSH_USBCONSOLE` requires three Enter keystrokes to detect a connected terminal.
   - *`nsh_main.c` vs `nsh_init.c`* (lines 21, 369): Line 21 places `nsh_initialize()`'s `rcS` execution under `apps/system/nsh/nsh_main.c`, whereas line 369 cites `apps/nshlib/nsh_init.c` for executing `rc.sysinit` before `rcS`.
   - *`BOOT` vs `BOOTSEL`* (lines 222, 443, 449): Line 222 refers to holding `BOOT`, while lines 443 and 449 refer to `BOOTSEL` mode without noting they are the same button/mode.
   - *Duplicate `ARCHSCRIPT` mention* (lines 297–298): `ARCHSCRIPT` is configured in bullet 2 and listed again in bullet 3.
   - *Implicit loop state initialization* (line 432): `current_level` and `consecutive_errors` are used in `bargraph_daemon` without explicitly stating both initialize to `0`.
