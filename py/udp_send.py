# ubuntu host, usb to eth adapter
# zuboard IP 192.168.1.10
# sudo ip addr add 192.168.1.100/24 dev enxd03745fbd50e
# vitis udp perf server example is expecting specific payload

import socket
import struct

DEST_IP = "192.168.1.10"         # ZUBoard IP
DEST_PORT = 5001                 # Port your UDP server is listening on
SOURCE_IP = "192.168.1.100"      # Your host IP (USB-to-Ethernet)
NUM_WORDS = 190                  # 64-bit words (184 max non-jumbo)
FIXED_UPPER = 0xABC0DEAD776600   # Upper 54 bits

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((SOURCE_IP, 0))

payload = b''

for i in range(NUM_WORDS):
  # Mask i to 8 bits and combine with upper bits
  word = (FIXED_UPPER << 8) | (i & 0xFF)
  payload += struct.pack("<Q", word)  # Pack as little-endian 64-bit

#assert len(payload) == 1472, f"Payload length is {len(payload)} bytes, expected 1472"

sock.sendto(payload, (DEST_IP, DEST_PORT))

print(f"Sent {NUM_WORDS} 64-bit words ({len(payload)} bytes) to {DEST_IP}:{DEST_PORT}")

sock.close()


# > sudo ip addr add 192.168.1.100/24 dev enxd03745fbd50e
# > ip route get 192.168.1.10
# > py ../sub/sw/py/udp_send.py
# 
# > ip link show enxd03745fbd50e
# host set jumbo
# > sudo ip link set enxd03745fbd50e mtu 9000