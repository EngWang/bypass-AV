import sys

def rolling_xor(data: bytes, key: int = 0xAA) -> bytes:
    encrypted = bytearray()
    for byte in data:
        xor_byte = byte ^ key
        encrypted.append(xor_byte)
        key = xor_byte  # cập nhật key theo kiểu rolling
    return bytes(encrypted)

def main():
    input_file = "shellcode.bin"
    output_file = "shellcode.enc"
    key = 0xAA  # khóa khởi đầu

    try:
        with open(input_file, "rb") as f:
            shellcode = f.read()
            print(f"[+] Đọc {len(shellcode)} byte từ {input_file}")
    except FileNotFoundError:
        print(f"[-] Không tìm thấy file {input_file}")
        sys.exit(1)

    encrypted = rolling_xor(shellcode, key)

    with open(output_file, "wb") as f:
        f.write(encrypted)
        print(f"[+] Ghi {len(encrypted)} byte mã hóa ra {output_file}")

if __name__ == "__main__":
    main()
