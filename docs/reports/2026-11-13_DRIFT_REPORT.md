# EGM Drift Report (2026-11-13)

Scope: All 33 newly created and modified EGM `README.md` contracts in `nuttxspace`, `nuttx`, and `apps`.

## README.md

No drift detected

## apps/nshlib/README.md

- Missing file coverage in `## Interface` / `## Invariants` / `## Side Effects`: 18 `.c` files in `apps/nshlib/` are not referenced (`nsh_altconsole.c`, `nsh_codeccmd.c`, `nsh_dbgcmds.c`, `nsh_dropbear.c`, `nsh_fscmds.c`, `nsh_mmcmds.c`, `nsh_mntcmds.c`, `nsh_modcmds.c`, `nsh_netcmds.c`, `nsh_passwdcmds.c`, `nsh_printf.c`, `nsh_proccmds.c`, `nsh_routecmds.c`, `nsh_test.c`, `nsh_timcmds.c`, `nsh_usbconsole.c`, `nsh_usbtrace.c`, `nsh_wait.c`).

## apps/system/README.md

- Unreferenced subdirectories: 7 subdirectories in `apps/system/` are not listed (`argtable3`, `fdt`, `flatbuffers`, `libffi`, `libuv`, `psmq`, `zlib`).

## nuttx/README.md

- Unreferenced subdirectories: 5 subdirectories in `nuttx/` are not listed (`cmake`, `dummy`, `nxtmpdir`, `openamp`, `pass1`).

## nuttx/arch/arm/src/README.md

- Subdirectory count mismatch: Overview states `95` subdirectories while `96` exist on disk (`cmake/` is not listed).
- Broken/unqualified leaf citations in `## Invariants` and `## Side Effects`: cites child-directory files without `./a1x/` relative paths (`a1x_boot.c`, `a1x_irq.c`, `a1x_lowputc.c`, `a1x_pio.c`).
- Missing `.S` assembly file count in child summaries: `arm` (`6` `.S` files), `armv7-a` (`8` `.S` files), `armv7-r` (`6` `.S` files), and `armv8-r` (`5` `.S` files) report `.c` counts only.

## nuttx/arch/arm/src/arm/README.md

- Missing `.S` assembly files in overview count: Overview states `11 source file(s)` (matching `.c` files) while 6 `.S` files also exist on disk (`arm_cache.S`, `arm_head.S`, `arm_nommuhead.S`, `arm_saveusercontext.S`, `arm_vectors.S`, `arm_vectortab.S`).

## nuttx/arch/arm/src/arm_a_r/README.md

- Unmentioned build file: `Make.defs` is present on disk and not referenced in `README.md`.

## nuttx/arch/arm/src/armv7-a/README.md

- Missing `.S` assembly files in overview count: Overview states `40 source file(s)` (matching `.c` files) while 8 `.S` files also exist on disk (`arm_cpuhead.S`, `arm_fpuconfig.S`, `arm_head.S`, `arm_pghead.S`, `arm_saveusercontext.S`, `arm_smccc.S`, `arm_vectors.S`, `arm_vectortab.S`).

## nuttx/arch/arm/src/armv7-r/README.md

- Missing `.S` assembly files in overview count: Overview states `21 source file(s)` (matching `.c` files) while 6 `.S` files also exist on disk (`arm_cpuhead.S`, `arm_fpuconfig.S`, `arm_head.S`, `arm_saveusercontext.S`, `arm_vectors.S`, `arm_vectortab.S`).

## nuttx/arch/arm/src/armv8-r/README.md

- Missing `.S` assembly files in overview count: Overview states `16 source file(s)` (matching `.c` files) while 5 `.S` files also exist on disk (`arm_fpuconfig.S`, `arm_head.S`, `arm_saveusercontext.S`, `arm_vectors.S`, `arm_vectortab.S`).

## nuttx/arch/arm/src/stm32h5/README.md

- Missing file coverage: 39 files on disk (`Make.defs`, `stm32_capture.c`, `stm32_dts.h`, `stm32_edata.c`/`.h`, `stm32_efuse.c`/`.h`, `stm32_ethernet.c`/`.h`, `stm32_fdcan.c`/`.h`, `stm32_flash.c`/`.h`, `stm32_hsi48.c`/`.h`, `stm32_icache.c`/`.h`, `stm32_lse.c`, `stm32_mdio.c`/`.h`, `stm32_pm.h`, `stm32_pminitialize.c`, `stm32_pmstandby.c`, `stm32_pmstop.c`, `stm32_pwm.c`, `stm32_pwr.c`/`.h`, `stm32_qspi.c`/`.h`, `stm32_rcc.c`, `stm32_sdmmc.c`/`.h`, `stm32_spi.c`/`.h`, `stm32_usbdrdhost.c`, `stm32_usbfs.c`/`.h`, `stm32h563xx_flash.c`, `stm32h5xx_rcc.c`) are not cited in `## Interface` / `## Invariants` / `## Side Effects`.

## nuttx/arch/arm/src/stm32h5/hardware/README.md

No drift detected

## nuttx/boards/arm/rtl8730e/rtl8730e_evb/prebuilt/README.md

No drift detected

## nuttx/boards/arm/stm32g4/README.md

- Subdirectory count mismatch: Overview states `(6 subdirectories)` while `7` subdirectories exist on disk (`common/` is present on disk and not listed).

## nuttx/boards/arm/stm32g4/nucleo-g491re/README.md

- Unreferenced subdirectories: `configs` and `scripts` exist on disk (not documented as `README.md` packages because they hold defconfig/linker assets rather than source files).

## nuttx/boards/arm/stm32g4/nucleo-g491re/include/README.md

No drift detected

## nuttx/boards/arm/stm32g4/nucleo-g491re/src/README.md

No drift detected

## nuttx/drivers/can/README.md

- Missing file coverage: `Make.defs`, `mcp2515.h`, and `sja1000.h` are not referenced in `README.md`.

## nuttx/drivers/ioexpander/README.md

- Missing file coverage: `Make.defs` and 13 internal chip headers (`aw9523b.h`, `icjx.h`, `iso1h812g.h`, `iso1i813t.h`, `mcp23x08.h`, `mcp23x17.h`, `pca9538.h`, `pca9555.h`, `pca9557.h`, `pcf8574.h`, `pcf8575.h`, `sx1509.h`, `tca64xx.h`) are not referenced in `README.md`.

## nuttx/drivers/serial/README.md

- Missing file coverage: `Make.defs` and `pty.h` are not referenced in `README.md`.

## nuttx/include/netpacket/README.md

No drift detected

## nuttx/include/nuttx/README.md

- Unreferenced subdirectory: `linker` exists under `nuttx/include/nuttx/` and is not listed in `## Interface`.

## nuttx/include/nuttx/ioexpander/README.md

No drift detected

## nuttx/include/nuttx/serial/README.md

No drift detected

## nuttx/include/nuttx/virtio/README.md

- Non-existent symbol citation (doc-to-code hallucination): `virtio.h:virtio_free_buf` is cited in `## Interface`, but `virtio_free_buf` does not exist in `nuttx/include/nuttx/virtio/virtio.h` (only `virtio_alloc_buf` / `virtio_malloc_buf` / `virtio_zalloc_buf` appear).

## nuttx/include/sys/README.md

- Non-existent symbol citations: `boardctl.h:BOARDIOC_INIT` and `queue.h:CIRCLEQ_HEAD` are cited in `## Interface`, but neither symbol exists in `nuttx/include/sys/boardctl.h` or `nuttx/include/sys/queue.h`.
- Missing file coverage: `signal.h` and `ttydefaults.h` are present in `nuttx/include/sys/` and not referenced in `README.md`.

## nuttx/libs/libc/misc/README.md

- Non-existent symbol citation: `lib_idr.c:idr_alloc_u32` is cited in `## Interface`, but `idr_alloc_u32` does not exist in `nuttx/libs/libc/misc/lib_idr.c`.

## nuttx/libs/libxx/README.md

- Unreferenced subdirectories: `etl`, `libcxx`, and `libcxxabi` exist in `nuttx/libs/libxx/` and are not listed in `## Subdirectories & Boundaries`.

## nuttx/libs/libxx/libstdc++/README.md

No drift detected

## nuttx/mm/kasan/README.md

- Unmentioned build file: `Make.defs` is present on disk and not referenced in `README.md`.

## nuttx/net/README.md

No drift detected

## nuttx/net/vsock/README.md

- Unmentioned build file: `Make.defs` is present on disk and not referenced in `README.md`.

## nuttx/tools/README.md

- Non-existent symbol citation: `gdbserver.py:GDBServer` is cited in `## Interface`, whereas `nuttx/tools/gdbserver.py` defines `DumpELFFile`, `DumpLogFile`, `RawMemoryFile`, `CoreDumpFile`, and `GDBStub` (not `GDBServer`).
- Unreferenced subdirectories: 9 subdirectories (`bl602`, `esp32`, `esp32c3`, `esp32s2`, `esp32s3`, `imx9`, `mpfs`, `nix`, `rp23xx`) exist in `nuttx/tools/` and are not listed in `## Interface`.
