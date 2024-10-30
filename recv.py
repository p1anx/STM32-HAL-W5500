import socket
import time

def tcp_client(host, port):
    # 创建一个 TCP/IP 套接字
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # 连接到服务器
        client_socket.connect((host, port))
        print(f"Connected to {host}:{port}")

        # 发送数据（可选）
        # message = "Hello, Server!"
        # client_socket.sendall(message.encode())

        # 接收数据
        while True:
            
            time.sleep(1)
            client_socket.send(b"a")
            data = client_socket.recv(1024)  # 接收 1024 字节
            if not data:
                break
            print(f"Received: {data.decode()}")

    except Exception as e:
        print(f"An error occurred: {e}")

    finally:
        # 关闭连接
        client_socket.close()
        print("Connection closed.")

def tcp_server(host, port):
    # 创建一个 TCP/IP 套接字
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 绑定套接字到地址和端口
    server_socket.bind((host, port))

    # 监听连接
    server_socket.listen(5)
    print(f"Server listening on {host}:{port}")

    try:
        while True:
            # 接受客户端连接
            client_socket, client_address = server_socket.accept()
            print(f"Connection from {client_address}")

            # 处理客户端数据
            while True:
                data = client_socket.recv(1024)
                if not data:
                    break
                print(f"Received: {data.decode()}")

                # 回应客户端（可选）
                response = "Echo: " + data.decode()
                client_socket.sendall(response.encode())

            # 关闭客户端连接
            client_socket.close()
            print(f"Connection with {client_address} closed.")

    except Exception as e:
        print(f"An error occurred: {e}")

    finally:
        # 关闭服务器套接字
        server_socket.close()
        print("Server shut down.")


# 使用示例
host = "192.168.1.2"
port = 5001

local_host = "192.168.1.22"
local_port = 5002
# tcp_client(host, port)
tcp_server(local_host,local_port)
