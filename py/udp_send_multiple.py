# ubuntu host, usb to eth adapter
# zuboard IP 192.168.1.10
# sudo ip addr add 192.168.1.100/24 dev enxd03745fbd50e
# vitis udp perf server example is expecting specific payload

import socket
import struct
import time

DEST_IP = "192.168.1.10"         # ZUBoard IP
DEST_PORT = 5001                 # Port your UDP server is listening on
SOURCE_IP = "192.168.1.100"      # Your host IP (USB-to-Ethernet)
NUM_WORDS = 1121                  # 64-bit words (184 max non-jumbo) jumbo max = 1125 (9000bytes)
# zynqmp max 8968 bytes (1121 words). don't know why...
NUM_FRAMES = 4                    # Number of identical frames to send
FIXED_UPPER = 0xABC0DEAD7700   # Upper 54 bits

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((SOURCE_IP, 0))
sock.connect((DEST_IP, DEST_PORT))  # Optional but improves performance

# Build payload once
payload = b''
for i in range(NUM_WORDS):
  word = (FIXED_UPPER << 16) | (i & 0xFFFF)
  payload += struct.pack("<Q", word)  # Little-endian 64-bit

# Send frames in tight loop
start_time = time.time()
for _ in range(NUM_FRAMES):
  #sock.sendto(payload, (DEST_IP, DEST_PORT))
  sock.send(payload)
end_time = time.time()

print(f"Sent {NUM_FRAMES} frames of {len(payload)} bytes "
      f"({NUM_FRAMES * len(payload)} bytes total) in {end_time - start_time:.4f} seconds")

sock.close()


# > sudo ip addr add 192.168.1.100/24 dev enxd03745fbd50e
# > ip route get 192.168.1.10
# > py ../sub/sw/py/udp_send.py
# 
# > ip link show enxd03745fbd50e
# host set jumbo
# > sudo ip link set enxd03745fbd50e mtu 9000