#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

/*
* Generic AES encryption function. Not in scope of vulnerability.
*/
int aes_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
                 unsigned char *iv, unsigned char *ciphertext) {
  EVP_CIPHER_CTX *ctx;

  int len;
  int ciphertext_len;

  if (!(ctx = EVP_CIPHER_CTX_new())) {
    ERR_print_errors_fp(stderr);
    abort();
  };

  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) {
    ERR_print_errors_fp(stderr);
    abort();
  }

  if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
    ERR_print_errors_fp(stderr);
    abort();
  }

  ciphertext_len = len;

  if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
    ERR_print_errors_fp(stderr);
    abort();
  }

  ciphertext_len += len;

  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}

int main() {
  cout << "AES encryption library" << endl;
  cout << "Reads AES key from aes_secret_key.bin and plaintext from secret_plaintext.txt" << endl;
  cout << "Writes encrypted output to encrypted_data.bin" << endl;

  // Read AES key
  char key[16];
  ifstream keyfile("aes_secret_key.bin", ios::binary);
  if (!keyfile.is_open()) {
    cout << "Could not open aes_secret_key.bin!" << endl;
    abort();
  }

  keyfile.read(key, sizeof(key));

  // Read secret plaintext
  char plaintext[48];
  ifstream plaintext_file("secret_plaintext.txt", ios::binary);
  if (!plaintext_file.is_open()) {
    cout << "Could not open secret_plaintext.txt!" << endl;
    abort();
  }

  plaintext_file.read(plaintext, sizeof(plaintext));

  // Generate random IV
  unsigned char iv[16];
  int retcode = RAND_bytes(iv, sizeof(iv));
  if (retcode != 1) {
    ERR_print_errors_fp(stderr);
    abort();
  }

  cout << "Random IV: ";
  for (int i = 0; i < 16; i++) {
    printf("%02x", iv[i]);
  }
  cout << endl;

  // Encrypt plaintext
  unsigned char ciphertext[128];
  int ciphertext_len = aes_encrypt((unsigned char*)plaintext, strlen(plaintext),
                                    (unsigned char*)key, iv, ciphertext);

  string output_filename = "encrypted_data.bin";
  ifstream fi(output_filename);

  if (!fi) {
    // file does not exist, create file and write encrypted data to it
    sleep(5);

    ofstream of(output_filename, ios::binary);
    of.write((char *)&iv[0], sizeof(iv));
    of.write((char *)&ciphertext[0], ciphertext_len);
    of.close();

    cout << "Successfully wrote encrypted data" << endl;
  } else {
    cout << "encrypted_data.bin already exists!" << endl;
    abort();
  }
}