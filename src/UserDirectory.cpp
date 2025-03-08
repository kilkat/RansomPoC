#include "UserDirectory.h"
#include <windows.h>
#include <shlobj.h>

std::string UserDirectory::getUserDirectory() {
    char userPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, userPath))) {
        return std::string(userPath);
    }
    return "";
}
