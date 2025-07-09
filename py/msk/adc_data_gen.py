# simple version, no impairments

#!/usr/bin/env python3
"""
Generate a real pass-band MSK waveform (continuous-phase FSK, h = 0.5)
sampled at 200 MHz, symbol rate 10 MHz, up-converted to 50 MHz IF.
The output is written to adc_samples.txt as signed 16-bit decimal values,
one value per line.
"""

import numpy as np

# ---------------- user-adjustable parameters ----------------
N_SYM        = 8*4*8          # number of MSK symbols to generate
FS           = 200e6         # sampling rate (Hz)
RSYM         = 10e6          # symbol rate   (Hz)
F_IF         = 50e6          # intermediate frequency (Hz)
OUTFILE      = "../../../msk_modem/sim/data/adc_samples.txt"
FULL_SCALE   = 0.9           # 0 … 1.0 of 16-bit range to avoid clipping
SEED         = 1234          # RNG seed for repeatability
# ------------------------------------------------------------

SPS = int(FS / RSYM)         # samples per symbol (20)
assert FS % RSYM == 0, "FS must be an integer multiple of RSYM"

np.random.seed(SEED)
#bits = np.random.randint(0, 2, N_SYM, dtype=np.int8)

# Replace random bits with fixed byte vector
bytes_sv = [
  0x90, 0x10, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00,
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x1A, 0x01, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
]

bits = np.unpackbits(np.array(bytes_sv, dtype=np.uint8), bitorder='big').astype(np.int8)


ak   = 2 * bits - 1          # NRZ mapping: 0→−1, 1→+1

# Preallocate
n_total = N_SYM * SPS
phase   = np.zeros(n_total, dtype=np.float64)

# h = 0.5 ⇒ frequency deviation = RSYM / 4
dphi = np.pi * 0.5 / SPS     # per-sample phase step magnitude

idx = 0
for a in ak:
  # per-symbol phase ramp
  phase[idx : idx + SPS] = phase[max(idx - 1, 0)] + dphi * a * np.arange(1, SPS + 1)
  idx += SPS

# Complex baseband MSK
msk_bb = np.exp(1j * phase)

# Up-convert to real IF
t = np.arange(n_total) / FS
carrier = np.exp(1j * 2 * np.pi * F_IF * t)
x_if = np.real(msk_bb * carrier)

# Scale to 16-bit signed integer
scale = FULL_SCALE * (2**15 - 1)
samples_i16 = np.round(scale * x_if).astype(np.int16)

# Write to text file
with open(OUTFILE, "w") as f:
  #f.write(f"#{(bits)}, len={len(bits)}\n")
  f.write(f"#bits={''.join(str(b) for b in bits)}, len={len(bits)}\n")
  for s in samples_i16:
    f.write(f"{int(s)}\n")

print(f"Wrote {n_total} samples to {OUTFILE}")
