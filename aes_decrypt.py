from Crypto.Cipher import AES
import sys
import string

if len(sys.argv) < 3:
    print "Usage: aes_decrypt.py keyfile ciphertext_file"
    print "ciphertext_file contains IV+ciphertext"
    exit()

keydata = open(sys.argv[1], "rb").read().strip()
if any([c not in string.hexdigits for c in keydata]):
    key = keydata[:16]
else:
    key = keydata[:32].decode("hex")

ciphertext_data = open(sys.argv[2], "rb").read()
iv = ciphertext_data[:16]
ciphertext = ciphertext_data[16:]

print "key", key.encode("hex")
print "ciphertext", ciphertext.encode("hex")
print "iv", iv.encode("hex")

aes = AES.new(key, AES.MODE_CBC, iv)
print "Plaintext: ", aes.decrypt(ciphertext)