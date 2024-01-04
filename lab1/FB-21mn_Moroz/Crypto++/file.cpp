#include <iostream>
#include <fstream>
#include <string>
#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>

using namespace CryptoPP;

const size_t RSA_CHUNK_SIZE = 256; // Adjust the chunk size as needed

// string RSAEncryptString (string publickeyfile, string message)
// {
//     string seed = randomStrGen(50);

//     FileSource pubFile(publickeyfile.c_str(), true, new HexDecoder);
//     RSAES_OAEP_SHA_Encryptor pub(pubFile);

//     RandomPool randPool;
//     randPool.IncorporateEntropy((byte *)seed.c_str(),
//     strlen(seed.c_str()));

//     string result;
//     StringSource( message.c_str(), true, new PK_EncryptorFilter( randPool, pub, new HexEncoder(new StringSink(result))));
//     return result;
// }

// // Decrypt string with RSA
// string RSADecryptString (string privatekeyfile, string ciphertext)
// {
//     FileSource privFile(privatekeyfile.c_str(), true, new HexDecoder);
//     RSAES_OAEP_SHA_Decryptor priv(privFile);

//     string result;
//     StringSource(ciphertext.c_str(), true, new HexDecoder( new PK_DecryptorFilter(GlobalRNG(), priv, new StringSink(result))));
//     return result;
// }

void PrintErr(char* argv[])
{
    std::cerr << "Usage: " << argv[0] << " <option>" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  1 - Generate RSA Keys" << std::endl;
    std::cerr << "  2 - Encrypt File (provide input and output file names)" << std::endl;
    std::cerr << "  3 - Decrypt File (provide input and output file names)" << std::endl;
}

// Function to generate RSA public and private keys
void GenerateRSAKeys(const std::string& publicKeyFile, const std::string& privateKeyFile)
{
    AutoSeededRandomPool rng;

    RSA::PrivateKey privateKey;
    privateKey.GenerateRandomWithKeySize(rng, 2048);

    RSA::PublicKey publicKey(privateKey);

    FileSink publicKeySink(publicKeyFile.c_str(), true);
    publicKey.Save(publicKeySink);

    FileSink privateKeySink(privateKeyFile.c_str(), true);
    privateKey.Save(privateKeySink);
}

// // Function to encrypt a file using RSA
// void EncryptFile(const std::string& inputFile, const std::string& outputFile, const RSA::PublicKey& publicKey)
// {
//     AutoSeededRandomPool rng;
//     RSAES_OAEP_SHA_Encryptor pub(publicKey);
//     FileSource file(inputFile.c_str(), true, new PK_EncryptorFilter(rng, pub, new FileSink(outputFile.c_str())));
// }

// // Function to decrypt a file using RSA
// void DecryptFile(const std::string& inputFile, const std::string& outputFile, const RSA::PrivateKey& privateKey)
// {
//     AutoSeededRandomPool rng;
//     RSAES_OAEP_SHA_Decryptor priv(privateKey);
//     FileSource file(inputFile.c_str(), true, new PK_DecryptorFilter(rng, priv, new FileSink(outputFile.c_str())));
// }

// Function to encrypt a file using RSA
void EncryptFile(const std::string& inputFile, const std::string& outputFile, const RSA::PublicKey& publicKey)
{
    AutoSeededRandomPool rng;


    // Open the input and output files
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);

    // Read and encrypt the file in chunks
    byte buffer[RSA_CHUNK_SIZE];
    std::string encryptedChunk;

    while (inFile.read(reinterpret_cast<char*>(buffer), RSA_CHUNK_SIZE))
    {
        encryptedChunk.clear();

        // Encrypt the chunk and write it to the output file
        RSAES_OAEP_SHA_Encryptor encryptor(publicKey);
        StringSource(buffer, RSA_CHUNK_SIZE, true, new PK_EncryptorFilter(rng, encryptor, new StringSink(encryptedChunk)));
        outFile.write(encryptedChunk.c_str(), encryptedChunk.size());
    }

    // Handle the remaining data
    if (inFile.gcount() > 0)
    {
        encryptedChunk.clear();
        // Encrypt the remaining data and write it to the output file
        RSAES_OAEP_SHA_Encryptor encryptor(publicKey);
        StringSource(buffer, inFile.gcount(), true, new PK_EncryptorFilter(rng, encryptor, new StringSink(encryptedChunk)));
        outFile.write(encryptedChunk.c_str(), encryptedChunk.size());
    }

    inFile.close();
    outFile.close();
}

// Function to decrypt a file using RSA
void DecryptFile(const std::string& inputFile, const std::string& outputFile, const RSA::PrivateKey& privateKey)
{
    AutoSeededRandomPool rng;

    // Open the input and output files
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);

    // Read and decrypt the file in chunks
    byte buffer[RSA_CHUNK_SIZE];
    std::string decryptedChunk;

    while (inFile.read(reinterpret_cast<char*>(buffer), RSA_CHUNK_SIZE))
    {
        decryptedChunk.clear();

        // Decrypt the chunk and write it to the output file
        RSAES_OAEP_SHA_Decryptor decryptor(privateKey);
        StringSource(buffer, RSA_CHUNK_SIZE, true, new PK_DecryptorFilter(rng, decryptor, new StringSink(decryptedChunk)));
        outFile.write(decryptedChunk.c_str(), decryptedChunk.size());
    }

    // Handle the remaining data
    if (inFile.gcount() > 0)
    {
        decryptedChunk.clear();

        // Decrypt the remaining data and write it to the output file
        RSAES_OAEP_SHA_Decryptor decryptor(privateKey);
        StringSource(buffer, inFile.gcount(), true, new PK_DecryptorFilter(rng, decryptor, new StringSink(decryptedChunk)));
        outFile.write(decryptedChunk.c_str(), decryptedChunk.size());
    }

    inFile.close();
    outFile.close();
}



// void EncryptFile(const std::string& inputFile, const std::string& outputFile, const RSA::PublicKey& publicKey)
// {
//     AutoSeededRandomPool rng;

//     std::ifstream inFile(inputFile, std::ios::binary);
//     std::ofstream outFile(outputFile, std::ios::binary);

//     const size_t bufferSize = publicKey.GetModulus().BitCount()   / 8;
//     byte buffer[bufferSize];
 
//     while (inFile.good())
//     {
//         inFile.read(reinterpret_cast<char*>(buffer), bufferSize);

//         if (inFile.gcount() > 0)
//         {
//             // Encrypt the block and write it to the output file
//             std::string encryptedBlock;
//             RSAES_OAEP_SHA_Encryptor encryptor(publicKey); 
//             StringSource(buffer, inFile.gcount(), true, new PK_EncryptorFilter(rng, encryptor, new StringSink(encryptedBlock)));
//             outFile.write(encryptedBlock.c_str(), encryptedBlock.size());
//         }
//     }

//     inFile.close();
//     outFile.close();
// }

// // Function to decrypt a file using RSA
// void DecryptFile(const std::string& inputFile, const std::string& outputFile, const RSA::PrivateKey& privateKey)
// {
//     AutoSeededRandomPool rng;

//     std::ifstream inFile(inputFile, std::ios::binary);
//     std::ofstream outFile(outputFile, std::ios::binary);

//     const size_t bufferSize = privateKey.GetModulus().ByteCount();
//     byte buffer[bufferSize];

//     while (inFile.good())
//     {
//         inFile.read(reinterpret_cast<char*>(buffer), bufferSize);

//         if (inFile.gcount() > 0)
//         {
//             // Decrypt the block and write it to the output file
//             std::string decryptedBlock;
//             RSAES_OAEP_SHA_Decryptor decryptor(privateKey);
//             StringSource(buffer, inFile.gcount(), true, new PK_DecryptorFilter(rng, decryptor, new StringSink(decryptedBlock)));
//             outFile.write(decryptedBlock.c_str(), decryptedBlock.size());
//         }
//     }

//     inFile.close();
//     outFile.close();
// }

int main(int argc, char *argv[])
{

    //std::cout << argc;

    if (argc<2){
        PrintErr(argv);
        return 1;
    }
    
    std::string option(argv[1]);
    
    // std::cout <<option;
    
    if( argc == 2 && option == "1")
    {
   
        std::string publicKeyFile = "public.key";
        std::string privateKeyFile = "private.key";
        GenerateRSAKeys(publicKeyFile, privateKeyFile);
        std::cout << "RSA keys generated and saved." << std::endl;
    }
    else if(argc == 4){
        if(option == "2")
        {
            std::string inputFile(argv[2]), outputFile(argv[3]);
        
            RSA::PublicKey publicKey;
            FileSource publicKeyFile("public.key", true);
            
            std::cout << "wants to load" << std::endl;
            
            publicKey.Load(publicKeyFile);

            EncryptFile(inputFile, outputFile, publicKey);
            std::cout << "File encrypted and saved as " << outputFile << std::endl;
        }
        else if (option == "3")
        {
            std::string inputFile(argv[2]), outputFile(argv[3]);

            RSA::PrivateKey privateKey;
            FileSource privateKeyFile("private.key", true);
            privateKey.Load(privateKeyFile);

            DecryptFile(inputFile, outputFile, privateKey);
            std::cout << "File decrypted and saved as " << outputFile << std::endl;
        }
        else{
            PrintErr(argv);
            return 2;
        }
    }
    else{   
        PrintErr(argv);
        return 3;
    }

    return 0;
}
