# add impairments for testing gardner TED only

#!/usr/bin/env python3
"""
Generate a real pass-band MSK waveform (h = 0.5) at 200 MHz,
symbol rate 10 MHz, IF 50 MHz, then resample it to emulate
timing impairments that a Gardner TED must correct.
The output (signed 16-bit decimal, one per line) is written
to adc_samples.txt.  First line contains the TX bit pattern.
"""

import numpy as np

# ───────── user-adjustable parameters ─────────
test_pattern = '5_alt'  # Choose: 'zeros', 'ones', or 'alt'

N_SYM              = 20000    # number of symbols
FS_TX              = 200e6    # nominal TX sample rate (Hz)
RSYM               = 10e6     # symbol rate          (Hz)
F_IF               = 50e6     # IF (Hz)
#OUTFILE            = "../../../msk_modem/sim/data/adc_5alt_Tp10.dat"
FULL_SCALE         = 0.9            # 0…1 of 16-bit range

# Gardner-related impairments
TIMING_OFFSET_SYM  = 0.43   # initial offset (fraction of symbol) * SHORTER WAVEFORMS, IMMEDIATE CORRECTION
    # +- 0.10 basic
    # +- 0.49 extreme
CLOCK_OFFSET_PPM   = 100 #10    # constant rate error, ppm (+ faster) *NEED LONGER WAVEFORMS
    # +-10  basic
    # +-100 extreme
JITTER_STD_SAMPLES = 0.005 #0.001   # rms sample jitter (samples) 
    # 0.001 – 0.003  realistic board level
    # 0.005   stress the loop harsher but not absurd
    # 0.01    extreme worse-case margin test
# ──────────────────────────────────────────────
# Helper: convert float to string with desired precision, stripping "0."
def fmt_float(val, scale=1000, digits=3):
  # Example: 0.001 → '001'
  return f"{int(round(abs(val)*scale)):0{digits}d}"

# Build filename elements
T_str = f"T{int(round(TIMING_OFFSET_SYM*100))}"              # E.g., 0.10 → 'T10'
C_str = f"C{int(round(abs(CLOCK_OFFSET_PPM)))}"              # E.g., 10   → 'C10'
J_str = f"J{fmt_float(JITTER_STD_SAMPLES,scale=1000,digits=3)}" # E.g., 0.001 → 'J001'

# Build the OUTFILE name
OUTFILE = f"../../../msk_modem/sim/data/adc_{test_pattern}_{T_str}_{C_str}_{J_str}.dat"
# ──────────────────────────────────────────────


# Generate bits vector of length N_SYM, and select test pattern
bits = np.zeros(N_SYM, dtype=np.int8)  # Default to zeros

# ---------- Select test pattern: 'zeros', 'ones', or 'alternating' ----------

if test_pattern == 'zeros':
  bits[:] = 0
elif test_pattern == 'ones':
  bits[:] = 1
#elif test_pattern == 'alternating':
#  bits[:] = np.arange(N_SYM) % 2
elif test_pattern == 'alt':          # 010101…
    bits[:] = np.arange(N_SYM) & 1           # % 2 faster with bit-and
else:
    # expect strings like "2_alt", "3_alt", …
    import re
    m = re.fullmatch(r'(\d+)_alt', test_pattern)
    if not m:
        raise ValueError(f"Unknown test_pattern: {test_pattern}")

    run = int(m.group(1))                    # N in "N_alt"
    # make one period: run zeros followed by run ones  → length 2*run
    period = np.r_[np.zeros(run,  dtype=np.int8),
                   np.ones (run,  dtype=np.int8)]
    # tile the period and slice to exact length
    bits[:] = np.tile(period, (N_SYM + 2*run - 1) // (2*run))[:N_SYM]


# ---------------------------------------------------------------------------

# ───────── MSK baseband generation (ideal clock) ─────────
SPS = int(FS_TX / RSYM)            # 20 samples per symbol
ak  = 2 * bits - 1                 # NRZ ±1

n_total = N_SYM * SPS
phase   = np.zeros(n_total)

dphi = np.pi * 0.5 / SPS           # per-sample phase step

idx = 0
for a in ak:
  phase[idx : idx + SPS] = phase[max(idx - 1, 0)] + dphi * a * np.arange(1, SPS + 1)
  idx += SPS

msk_bb = np.exp(1j * phase)

# Up-convert to pass-band
t_tx = np.arange(n_total) / FS_TX
x_tx = np.real(msk_bb * np.exp(1j * 2 * np.pi * F_IF * t_tx))

# ───────── apply timing impairments ─────────
# Target ADC sample times (seconds)
ppm      = CLOCK_OFFSET_PPM * 1e-6
dt_nom   = 1.0 / FS_TX
n_adc    = n_total                   # keep same length for simplicity

# Sample index accumulator with drift
k = np.arange(n_adc)
t_adc = (k * (1 + ppm) + TIMING_OFFSET_SYM * SPS) * dt_nom
t_adc += np.random.normal(0, JITTER_STD_SAMPLES * dt_nom, n_adc)

# Interpolate to obtain ADC samples
idx_adc = t_adc * FS_TX             # fractional indices into x_tx
idx_adc = np.clip(idx_adc, 0, n_total-1)
x_adc = np.interp(idx_adc, np.arange(n_total), x_tx, left=0.0, right=0.0)

# ───────── scale and save ─────────
scale = FULL_SCALE * (2**15 - 1)
samples_i16 = np.round(scale * x_adc).astype(np.int16)

with open(OUTFILE, "w") as f:
  f.write(f"#bits={''.join(str(b) for b in bits)}, len={len(bits)}\n")
  for s in samples_i16:
    f.write(f"{int(s)}\n")

print(f"Wrote {n_adc} impaired samples to {OUTFILE}")
