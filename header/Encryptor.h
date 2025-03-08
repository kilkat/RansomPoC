#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <iostream>
#include <vector>
#include <string>

class Encryptor {
public:
    Encryptor(const std::string& key);
    void encryptFile(const std::string& filePath);
    void decryptFile(const std::string& filePath);
    void encryptDirectory(const std::string& dirPath);
    void decryptDirectory(const std::string& dirPath);

private:
    std::string key;
    void xorEncryptDecrypt(std::vector<unsigned char>& buffer);
};

#endif
