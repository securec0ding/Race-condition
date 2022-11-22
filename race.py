from pwn import *
import subprocess
import shlex

# first run of program, overwrite aes_secret_key.bin
subprocess.check_call(shlex.split("rm -f encrypted_data.bin"))
p = process("./race")

p.recvuntil("Random IV: ")
iv = p.recvline().strip()
print "IV: ", iv

with open("new_key", "w") as f:
  f.write(iv)

subprocess.check_call(shlex.split("ln -s aes_secret_key.bin encrypted_data.bin"))

p.wait_for_close()

# second run of program, encrypt data with IV from previous run
subprocess.check_call(shlex.split("rm -f encrypted_data.bin"))
p = process("./race")
p.wait_for_close()

print subprocess.check_output(shlex.split("python aes_decrypt.py new_key encrypted_data.bin"))