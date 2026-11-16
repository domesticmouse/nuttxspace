# `xiao-rp2350-bargraph/apps` Out-of-Tree Application Root (`apps/external`)

## Overview

Out-of-tree application container linked at build time to `apps/external` (ignored by `apps/.gitignore`). Integrates external user-space applications into the upstream `apps/` build and Kconfig discovery hierarchy without modifying any tracked files in `apps/`.

## Subdirectories & Boundaries

- `bargraph`: [./bargraph/README.md](./bargraph/README.md) - Built-in user-space application (`bargraph`) and autonomous 50 Hz potentiometer-to-PWM LED bar graph background daemon (`bargraphd`).

## Invariants

- `[Kconfig:External Applications]` Tracked directly in git and sources `$APPSDIR/external/bargraph/Kconfig` so `apps/tools/mkkconfig.sh` includes the `External Applications` menu when synthesizing `apps/Kconfig`.
- `[Makefile:distclean]` Leaves `MENUDESC` unset before including `$(APPDIR)/Directory.mk` so `make distclean` never deletes the tracked `xiao-rp2350-bargraph/apps/Kconfig`, and removes the `$(APPDIR)/external` symlink via its `distclean::` rule.
- `[Make.defs:external]` Includes `$(wildcard $(APPDIR)/external/*/Make.defs)` so child applications append themselves to `CONFIGURED_APPS`.

## Side Effects

- `[Makefile:distclean]` Removes the `$(APPDIR)/external` symlink during `make -C nuttx distclean`.

## Verification

- Run: `make -C xiao-rp2350-bargraph`
