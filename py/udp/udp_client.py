# ubuntu host, usb to eth adapter
# zuboard IP 192.168.1.10
# sudo ip addr add 192.168.1.100/24 dev enxd03745fbd50e
# vitis udp perf server example is expecting specific payload

import socket
import struct
import time

DEST_IP = "192.168.1.10"
DEST_PORT = 5001
SOURCE_IP = "192.168.1.100"
PACKET_SIZE = 1470
DURATION_SEC = 5

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((SOURCE_IP, 0))

payload_data = b'A' * (PACKET_SIZE - 12)

start_time = time.time()
frame_id = 0

print(f"Sending UDP traffic to {DEST_IP}:{DEST_PORT}...")

while time.time() - start_time < DURATION_SEC:
  now = time.time()
  tv_sec = int(now)
  tv_usec = int((now - tv_sec) * 1_000_000)

  header = struct.pack(">iII", frame_id, tv_sec, tv_usec)
  #header = struct.pack("iII", frame_id, tv_sec, tv_usec)
  sock.sendto(header + payload_data, (DEST_IP, DEST_PORT))

  frame_id += 1
  time.sleep(0.001)

# Final packet with id = -1 to stop and trigger summary
now = time.time()
tv_sec = int(now)
tv_usec = int((now - tv_sec) * 1_000_000)

final_header = struct.pack(">iII", -1, tv_sec, tv_usec)
#final_header = struct.pack("iII", -1, tv_sec, tv_usec)
sock.sendto(final_header + payload_data, (DEST_IP, DEST_PORT))

sock.close()
