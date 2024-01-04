import sys
import os
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_v1_5
from Crypto import Random

from time import perf_counter

def generate_rsa_keypair():
    random_generator = Random.new().read
    key = RSA.generate(2048, random_generator)
    
    private_key = key.export_key()
    public_key = key.publickey().export_key()
    
    with open("private.txt", "wb") as private_file:
        private_file.write(private_key)
    
    with open("public.txt", "wb") as public_file:
        public_file.write(public_key)
    
    print("RSA keypair generated and saved to private.txt and public.txt.")

def encrypt_file(filename):
    with open("public.txt", "rb") as public_file:
        public_key = RSA.import_key(public_file.read())
    
    cipher = PKCS1_v1_5.new(public_key)
    
    with open(filename, "rb") as input_file:
        plaintext = input_file.read()
    
    ciphertext = cipher.encrypt(plaintext)
    
    with open(f"{filename}.enc", "wb") as output_file:
        output_file.write(ciphertext)
    
    print(f"File encrypted: {filename}.enc")

def decrypt_file(filename):
    with open("private.txt", "rb") as private_file:
        private_key = RSA.import_key(private_file.read())
    
    cipher = PKCS1_v1_5.new(private_key)
    
    with open(filename, "rb") as input_file:
        ciphertext = input_file.read()
    
    plaintext = cipher.decrypt(ciphertext, None)
    
    with open(f"{filename}.dec", "wb") as output_file:
        output_file.write(plaintext)
    
    print(f"File decrypted: {filename}.dec")



def main():
    if len(sys.argv) < 2:
        print("Usage: python rsa_crypto.py <mode> [<filename>]")
        print("Modes:")
        print("1 - Generate RSA keypair (saves private.txt and public.txt)")
        print("2 - Encrypt file with RSA (requires public.txt)")
        print("3 - Decrypt file with RSA (requires private.txt)")
        return

    mode = int(sys.argv[1])
    
    t1_start = perf_counter()
    
    if mode == 1:
        generate_rsa_keypair()
    elif mode in [2, 3] and len(sys.argv) >= 3:
        filename = sys.argv[2]
        if mode == 2:
            encrypt_file(filename)
        else:
            decrypt_file(filename)
    else:
        print("Invalid mode. Use 1, 2, or 3.")

    t1_stop = perf_counter()    
    
    print("Elapsed time during the whole program in seconds:",
                                            t1_stop-t1_start)

if __name__ == "__main__":
    main()
