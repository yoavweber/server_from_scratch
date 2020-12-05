#!/usr/bin/env python3

import socket
import time
start = time.time()


HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 8080        # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b'''
GET /chat HTTP/1.1
Host: localhost:8080
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Sec-WebSocket-Version: 13
    ''')
    data = s.recv(1024)

finish =  time.time()

time = finish - start
formatted_float = "{:.2f}".format(time)
print('time: ',formatted_float)