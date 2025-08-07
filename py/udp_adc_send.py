# ubuntu host, usb to eth adapter
# zuboard IP 192.168.1.10
# sudo ip addr add 192.168.1.100/24 dev enxd03745fbd50e
# vitis udp perf server example is expecting specific payload

import socket
import struct
import time
import os

# --- User Variables ---
DEST_IP = "192.168.1.10"        # ZUBoard IP
DEST_PORT = 5001                # UDP port
SOURCE_IP = "192.168.1.100"     # Host IP
NUM_WORDS = 1121                # 64-bit words per frame
DATA_DIR = "../../msk_modem/sim/data" # Directory of data file (no trailing /)
DATA_FILENAME = "adc_data_nominal.dat"  # File with 16b signed ints, line 1 comment

# --- Read data file ---
data_path = os.path.join(DATA_DIR, DATA_FILENAME)
adc_data = []
with open(data_path, 'r') as f:
  lines = f.readlines()
  for line in lines[1:]:  # Skip first line
    line = line.strip()
    if not line: continue
    adc_data.append(int(line))

# --- Pad data to multiple of 4 for packing ---
if len(adc_data) % 4 != 0:
  pad = 4 - (len(adc_data) % 4)
  adc_data += [0]*pad

# --- Frame/word calculations ---
WORDS_PER_FRAME = NUM_WORDS
SAMPLES_PER_FRAME = WORDS_PER_FRAME * 4
total_frames = (len(adc_data) + SAMPLES_PER_FRAME - 1) // SAMPLES_PER_FRAME

print(f"Data samples: {len(adc_data)}, sending {total_frames} frames "
      f"({WORDS_PER_FRAME*8} bytes/frame)")

# --- Setup socket ---
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((SOURCE_IP, 0))
sock.connect((DEST_IP, DEST_PORT))

# --- Frame send loop ---
start_time = time.time()
for frame in range(total_frames):
  frame_start = frame * SAMPLES_PER_FRAME
  frame_end = frame_start + SAMPLES_PER_FRAME
  frame_samples = adc_data[frame_start:frame_end]

  # Pad last frame if needed
  if len(frame_samples) < SAMPLES_PER_FRAME:
    frame_samples += [0] * (SAMPLES_PER_FRAME - len(frame_samples))

  payload = bytearray()
  for i in range(0, len(frame_samples), 4):
    w0 = frame_samples[i]
    w1 = frame_samples[i+1]
    w2 = frame_samples[i+2]
    w3 = frame_samples[i+3]
    # Little endian: lower indices in lower bits
    word = ((w3 & 0xFFFF) << 48) | ((w2 & 0xFFFF) << 32) | ((w1 & 0xFFFF) << 16) | (w0 & 0xFFFF)
    payload += struct.pack("<Q", word)
  # Send frame
  sock.send(payload)

end_time = time.time()
print(f"Sent {total_frames} frames of {WORDS_PER_FRAME*8} bytes "
      f"({total_frames*WORDS_PER_FRAME*8} bytes total) in {end_time - start_time:.4f} seconds")

sock.close()


# > sudo ip addr add 192.168.1.100/24 dev enxd03745fbd50e
# > ip route get 192.168.1.10
# > py ../sub/sw/py/udp_send.py
# 
# > ip link show enxd03745fbd50e
# host set jumbo
# > sudo ip link set enxd03745fbd50e mtu 9000