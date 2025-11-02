import numpy as np

def rrc_taps(alpha, L, N):
    # Root-raised-cosine (same as your original)
    t = np.arange(-N/2, N/2 + 1/L, 1/L)
    h = np.zeros_like(t)
    for i, ti in enumerate(t):
        if abs(ti) < 1e-8:
            h[i] = 1.0 - alpha + 4*alpha/np.pi
        elif abs(abs(4*alpha*ti) - 1) < 1e-8:
            h[i] = (alpha/np.sqrt(2))*((1+2/np.pi)*np.sin(np.pi/(4*alpha))
                    +(1-2/np.pi)*np.cos(np.pi/(4*alpha)))
        else:
            num = np.sin(np.pi*ti*(1-alpha)) + 4*alpha*ti*np.cos(np.pi*ti*(1+alpha))
            den = np.pi*ti*(1-(4*alpha*ti)**2)
            h[i] = num/den
    # keep your original energy normalization (optional, but fine)
    h /= np.sqrt(np.sum(h**2))
    return h

# ---- parameters ----
alpha = 0.35
L     = 10
N     = 8
BITS  = 16  # coefficient width (Q1.15 when BITS=16)

# >> Set this from your measurements (linear, not dB).
# For your numbers: 7431/8191 ≈ 0.907  (and 2476/2730 ≈ 0.907)
G_MEAS = 0.9070874716   # <-- put None to skip measured-gain scaling
#HEADROOM = 1.0          # e.g., set 0.707 for ~3 dB headroom if desired
HEADROOM = 8189 / 8191.0
# --------------------

# Generate float taps
h = rrc_taps(alpha, L, N)

# Scale taps to cancel measured gain (if provided)
K = (1.0 / G_MEAS) if (G_MEAS is not None) else 1.0
K *= HEADROOM
h_scaled = h * K

# Quantize to signed Q1.(BITS-1)
max_q = (1 << (BITS - 1)) - 1
min_q = -(1 << (BITS - 1))
# clip to representable float range before rounding
h_clip = np.clip(h_scaled, -(1 - 2**-(BITS-1)), (1 - 2**-(BITS-1)))
c = np.round(h_clip * (1 << (BITS - 1))).astype(int)
c = np.clip(c, min_q, max_q)

# Report effective fixed-point "DC"/sum gain (good quick proxy)
G_eff = c.sum() / float(1 << (BITS - 1))
print(f"K used: {K:.8f}")
print(f"Effective fixed-point gain (sum/2^(BITS-1)): {G_eff:.6f}")

# Write float and int taps
np.savetxt("taps_scaled_float2.txt", h_scaled, fmt="%.8f")
np.savetxt("taps_scaled_int2.txt", c, fmt="%d")

# FIR Compiler COE files (your preferred header)
with open("taps_scaled_float2.coe", "w") as f:
    f.write("radix=10;\ncoefdata=\n")
    f.write(",\n".join(f"{v:.8f}" for v in h_scaled) + ";\n")

with open("taps_scaled_int2.coe", "w") as f:
    f.write("radix=10;\ncoefdata=\n")
    f.write(",\n".join(map(str, c)) + ";\n")
