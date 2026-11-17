# Seeed Studio XIAO RP2350 6-Segment PWM LED Bar Graph — Interactive Tutorial Series (`./tutorial/`)

An interactive, Edward Tufte-inspired hardware and embedded RTOS course teaching how to wire the 400-tie-point solderless breadboard circuit and build the out-of-tree Apache NuttX board support package (`xiao-rp2350-bargraph/board/`) and 50 Hz ADC-to-PWM application (`xiao-rp2350-bargraph/apps/bargraph/`).

## Course Landing Page

- **Interactive Course Index**: [index.html](./index.html) (compiled from [index.md](./index.md))
- **Mission Statement**: [MISSION.md](./MISSION.md)
- **Trusted Resources**: [RESOURCES.md](./RESOURCES.md)
- **Curriculum Roadmap**: [NOTES.md](./NOTES.md)

## Story Arc Lessons (`./lessons/`)

1. **Lesson 0001 — Breadboard Circuit, Pinout & 23.6 mA Current Budget**
   - Source: [lessons/0001-breadboard-circuit-and-current-budget.md](./lessons/0001-breadboard-circuit-and-current-budget.md) | Compiled HTML: [lessons/0001-breadboard-circuit-and-current-budget.html](./lessons/0001-breadboard-circuit-and-current-budget.html)
   - Companion App: `apps/0001-breadboard-circuit-and-current-budget/`
2. **Lesson 0002 — Proportional 6-Segment Bar Graph Math & 16-Bit PWM Scaling**
   - Source: [lessons/0002-proportional-bargraph-math-and-pwm-scaling.md](./lessons/0002-proportional-bargraph-math-and-pwm-scaling.md) | Compiled HTML: [lessons/0002-proportional-bargraph-math-and-pwm-scaling.html](./lessons/0002-proportional-bargraph-math-and-pwm-scaling.html)
   - Companion App: `apps/0002-proportional-bargraph-math-and-pwm-scaling/`
3. **Lesson 0003 — Fixed-Point Q4 EMA Filtering, Deadbands & ADC Sample Extraction**
   - Source: [lessons/0003-q4-ema-filtering-and-adc-extraction.md](./lessons/0003-q4-ema-filtering-and-adc-extraction.md) | Compiled HTML: [lessons/0003-q4-ema-filtering-and-adc-extraction.html](./lessons/0003-q4-ema-filtering-and-adc-extraction.html)
   - Companion App: `apps/0003-q4-ema-filtering-and-adc-extraction/`
4. **Lesson 0004 — Multi-Channel RP2350 PWM Routing & Safe Hardware Shutdown**
   - Source: [lessons/0004-multichannel-pwm-routing-and-safe-shutdown.md](./lessons/0004-multichannel-pwm-routing-and-safe-shutdown.md) | Compiled HTML: [lessons/0004-multichannel-pwm-routing-and-safe-shutdown.html](./lessons/0004-multichannel-pwm-routing-and-safe-shutdown.html)
   - Companion App: `apps/0004-multichannel-pwm-routing-and-safe-shutdown/`
5. **Lesson 0005 — Out-of-Tree NuttX Board Bringup, RP2350-E9 Pad Isolation & NVIC IRQ Gating**
   - Source: [lessons/0005-out-of-tree-bsp-and-adc-irq-gating.md](./lessons/0005-out-of-tree-bsp-and-adc-irq-gating.md) | Compiled HTML: [lessons/0005-out-of-tree-bsp-and-adc-irq-gating.html](./lessons/0005-out-of-tree-bsp-and-adc-irq-gating.html)
   - Companion App: `apps/0005-out-of-tree-bsp-and-adc-irq-gating/`
6. **Lesson 0006 — The 50 Hz `bargraphd` Daemon State Machine & Thread-Safe NSH CLI**
   - Source: [lessons/0006-bargraphd-daemon-and-nsh-cli.md](./lessons/0006-bargraphd-daemon-and-nsh-cli.md) | Compiled HTML: [lessons/0006-bargraphd-daemon-and-nsh-cli.html](./lessons/0006-bargraphd-daemon-and-nsh-cli.html)
   - Companion App: `apps/0006-bargraphd-daemon-and-nsh-cli/`

## Printable Reference Sheets (`./reference/`)

- **Glossary**: [reference/GLOSSARY.md](./reference/GLOSSARY.md) ([reference/glossary.html](./reference/glossary.html))
- **Hardware Pinout & Breadboard Wiring**: [reference/hardware-pinout-and-wiring.md](./reference/hardware-pinout-and-wiring.md) ([reference/hardware-pinout-and-wiring.html](./reference/hardware-pinout-and-wiring.html))
- **7 NuttX & RP2350 Driver Traps**: [reference/nuttx-rp2350-driver-traps.md](./reference/nuttx-rp2350-driver-traps.md) ([reference/nuttx-rp2350-driver-traps.html](./reference/nuttx-rp2350-driver-traps.html))
- **Verification & NSH CLI Cheat Sheet**: [reference/verification-and-cli-cheatsheet.md](./reference/verification-and-cli-cheatsheet.md) ([reference/verification-and-cli-cheatsheet.html](./reference/verification-and-cli-cheatsheet.html))

## Verification & Build Commands

Run from `./tutorial/`:

```bash
# 1. Verify all 6 companion C99 applications match their lesson listings byte-for-byte
.venv/bin/python ../.agents/skills/teach-code/scripts/sync_listing.py --check

# 2. Compile and validate all Markdown lessons, reference sheets, links, and quiz option word counts
.venv/bin/python build.py --out-dir _site --check && rm -rf _site
.venv/bin/python build.py --check

# 3. Run all 6 companion application unit test suites
for dir in apps/000*; do make -C "$dir" test && make -C "$dir" clean; done
```

## License

Apache License 2.0
