#include "WallpaperChanger.h"
#include "resource.h"
#include <windows.h>
#include <Shlobj.h>
#include <iostream>
#include <fstream>

void WallpaperChanger::setWallpaperFromResource(int resourceId) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(resourceId), RT_BITMAP);
    if (!hRes) {
        std::cerr << "리소스 찾기 실패!" << std::endl;
        return;
    }

    HGLOBAL hData = LoadResource(hInstance, hRes);
    if (!hData) {
        std::cerr << "리소스 로드 실패!" << std::endl;
        return;
    }

    void* pData = LockResource(hData);
    DWORD dataSize = SizeofResource(hInstance, hRes);
    if (!pData || dataSize == 0) {
        std::cerr << "리소스 데이터가 유효하지 않습니다!" << std::endl;
        return;
    }

    // 바탕화면에 저장할 경로 설정
    char userPath[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_DESKTOP, NULL, 0, userPath);
    std::string bmpPath = std::string(userPath) + "\\hacked.bmp";

    // BMP 파일을 올바르게 저장하기 위해 BITMAPFILEHEADER 추가
    std::ofstream outFile(bmpPath, std::ios::binary);
    if (!outFile) {
        std::cerr << "BMP 파일 저장 실패: " << bmpPath << std::endl;
        return;
    }

    // BMP 파일 헤더 추가 (BITMAPFILEHEADER)
    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER* bih = (BITMAPINFOHEADER*)pData;

    bmfHeader.bfType = 0x4D42;  // "BM"
    bmfHeader.bfSize = dataSize + sizeof(BITMAPFILEHEADER);
    bmfHeader.bfReserved1 = 0;
    bmfHeader.bfReserved2 = 0;
    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + bih->biSize + (bih->biBitCount <= 8 ? (1 << bih->biBitCount) * 4 : 0);

    // 헤더 쓰기
    outFile.write(reinterpret_cast<const char*>(&bmfHeader), sizeof(BITMAPFILEHEADER));

    // BMP 데이터 쓰기
    outFile.write(reinterpret_cast<const char*>(pData), dataSize);

    outFile.close();

    // 바탕화면 변경
    if (SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID)bmpPath.c_str(), SPIF_SENDCHANGE)) {
        std::cout << "바탕화면이 변경되었습니다: " << bmpPath << std::endl;
    }
    else {
        std::cerr << "바탕화면 변경 실패" << std::endl;
    }
}
