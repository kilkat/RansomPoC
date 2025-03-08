#include <iostream>
#include "UserDirectory.h"
#include "Encryptor.h"

int main() {
    std::string userDir = UserDirectory::getUserDirectory();
    std::string targetDir = userDir + "\\Desktop\\ransom";

    Encryptor encryptor("MySecretXORKey!!");

    std::cout << "1. 파일 암호화" << std::endl;
    std::cout << "2. 파일 복호화" << std::endl;
    std::cout << "선택: ";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        encryptor.encryptDirectory(targetDir);
        std::cout << "암호화 성공" << std::endl;
    }
    else if (choice == 2) {
        encryptor.decryptDirectory(targetDir);
        std::cout << "복호화 성공" << std::endl;
    }
    else {
        std::cout << "잘못된 선택입니다." << std::endl;
    }

    return 0;
}
