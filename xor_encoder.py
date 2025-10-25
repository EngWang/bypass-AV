import sys

def rolling_xor(data: bytes, key: int = 0xAA) -> bytes:
    encrypted = bytearray()
    for byte in data:
        xor_byte = byte ^ key
        encrypted.append(xor_byte)
        key = xor_byte  # rolling key
    return bytes(encrypted)

def main():
    input_file = "shellcode.bin"
    output_file = "shellcode.enc"
    key = 0xAA  

    try:
        with open(input_file, "rb") as f:
            shellcode = f.read()
            print(f"[+] Read {len(shellcode)} bytes from {input_file}")
    except FileNotFoundError:
        print(f"[-] Can't found the file {input_file}")
        sys.exit(1)

    encrypted = rolling_xor(shellcode, key)

    with open(output_file, "wb") as f:
        f.write(encrypted)
        print(f"[+] Write {len(encrypted)} encrypted byte to  {output_file}")

if __name__ == "__main__":
    main()

