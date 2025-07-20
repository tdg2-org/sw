#!/usr/bin/env python3
"""
Generate a real pass-band MSK waveform (h = 0.5) at 200 MHz, symbol-rate
10 MHz, IF 50 MHz.

Impairments supported
  • Timing: offset, ppm clock error, sample jitter
  • Carrier: CFO, phase offset, slow drift, symbol-rate phase noise
  • Payload replication: REPEAT_PAYLOAD

If an impairment is zero it is **not** encoded in the output filename.
"""

import os
import numpy as np

# ───────── directory for output file ─────────
OUTPUT_DIR = "../../../msk_modem/sim/data/"
# ─────────────────────────────────────────────

# ───────── user-adjustable parameters ─────────
REPEAT_PAYLOAD     = 60        # replicate bytes_sv this many times (multiples of 256 symbols)

# Symbol / sample parameters
FS_TX              = 200e6    # sample rate (Hz)
RSYM               = 10e6     # symbol rate (Hz)
F_IF               = 50e6     # IF (Hz)
FULL_SCALE         = 0.9      # 0…1 of 16-bit range

# Timing impairments
TIMING_OFFSET_SYM  = 0.00     # fractions of symbol
CLOCK_OFFSET_PPM   = 0        # ppm
JITTER_STD_SAMPLES = 0.000    # rms jitter (samples)

# Carrier impairments
CFO_HZ             = 1e3        # constant offset (Hz)
PHASE_OFFSET_DEG   = 0        # constellation rotation (deg)
SLOW_PPM_DRIFT     = 0        # linear drift (ppm/sec of Fs)
PHASE_NOISE_DEG_RMS= 0        # white phase-noise per symbol (deg rms)
# ──────────────────────────────────────────────


# =============== fixed payload ================= 8*8*4 = 256 symbols
bytes_sv = [
  0x90, 0x10, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00,
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00,
  0x00, 0x77, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
  0x01, 0x01, 0x00, 0x00, 0xff, 0xa5, 0x0f, 0xfe
] * REPEAT_PAYLOAD                             # replicate pattern

bits = np.unpackbits(np.array(bytes_sv, dtype=np.uint8),
                     bitorder='big').astype(np.int8)
N_SYM = len(bits)
# ===============================================


# ───────── ideal MSK baseband ─────────
SPS       = int(FS_TX / RSYM)                 # 20
n_total   = N_SYM * SPS
ak        = 2*bits - 1                        # NRZ ±1

phase     = np.zeros(n_total)
dphi      = np.pi*0.5 / SPS
idx = 0
for a in ak:
  phase[idx:idx+SPS] = phase[max(idx-1, 0)] + dphi*a*np.arange(1, SPS+1)
  idx += SPS
msk_bb = np.exp(1j*phase)
t_tx   = np.arange(n_total) / FS_TX
# ───────────────────────────────────────


# ───────── carrier impairments ─────────
phi_carrier = (
  2*np.pi*CFO_HZ*t_tx +
  np.deg2rad(PHASE_OFFSET_DEG)
)
if SLOW_PPM_DRIFT != 0:
  drift_hz = SLOW_PPM_DRIFT*1e-6*FS_TX
  phi_carrier += np.pi*drift_hz*t_tx**2

if PHASE_NOISE_DEG_RMS != 0:
  sigma = np.deg2rad(PHASE_NOISE_DEG_RMS)/np.sqrt(SPS)
  phi_carrier += np.random.randn(n_total)*sigma

msk_bb *= np.exp(1j*phi_carrier)
# ───────────────────────────────────────


# ───────── timing impairments ─────────
dt_nom   = 1.0/FS_TX
ppm      = CLOCK_OFFSET_PPM*1e-6
n_adc    = n_total

k        = np.arange(n_adc)
t_adc    = (k*(1+ppm) + TIMING_OFFSET_SYM*SPS)*dt_nom
if JITTER_STD_SAMPLES != 0:
  t_adc += np.random.normal(0, JITTER_STD_SAMPLES*dt_nom, n_adc)

idx_adc  = np.clip(t_adc*FS_TX, 0, n_total-1)
x_tx     = np.real(msk_bb*np.exp(1j*2*np.pi*F_IF*t_tx))
x_adc    = np.interp(idx_adc, np.arange(n_total), x_tx, left=0.0, right=0.0)
# ───────────────────────────────────────


# ───────── scale and save ─────────
scale = FULL_SCALE*(2**15-1)
samples_i16 = np.round(scale*x_adc).astype(np.int16)

# ───────── build filename ─────────
tags = []

def tag(name, val, fmt="{}", cond=lambda v: v != 0):
  if cond(val):
    tags.append(f"{name}{fmt.format(val)}")

tag("T", TIMING_OFFSET_SYM, "{:.2f}")
tag("C", CLOCK_OFFSET_PPM, "{}")
tag("J", JITTER_STD_SAMPLES, "{:.3f}", lambda v: v > 0)

tag("F", CFO_HZ, "{}")
tag("P", PHASE_OFFSET_DEG, "{}")
tag("D", SLOW_PPM_DRIFT, "{}")
tag("N", PHASE_NOISE_DEG_RMS, "{}")

base = "adc_data_" + "_".join(tags) if tags else "adc_data_nominal"
outfile = os.path.join(OUTPUT_DIR, f"{base}.dat")
os.makedirs(OUTPUT_DIR, exist_ok=True)
# ────────────────────────────────────


with open(outfile, "w") as f:
  f.write(f"#bits={''.join(str(b) for b in bits)}, len={len(bits)}\n")
  for s in samples_i16:
    f.write(f"{int(s)}\n")

print(f"Wrote {len(samples_i16)} samples to {outfile}")
