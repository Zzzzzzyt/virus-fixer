// This file is encoded in GBK.

#include <dirent.h>
#include <filesystem>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>

//This is a bit hacky...
#define _MSC_VER
#include "sha1.cpp"

using namespace std;

// /c start stQQ.vbe&start
// clang-format off
unsigned char lnk_content[48] =
{
	0x2f, 0x00, 0x63, 0x00, 0x20, 0x00, 0x73, 0x00, 
	0x74, 0x00, 0x61, 0x00, 0x72, 0x00, 0x74, 0x00, 
	0x20, 0x00, 0x73, 0x00, 0x74, 0x00, 0x51, 0x00, 
	0x51, 0x00, 0x2e, 0x00, 0x76, 0x00, 0x62, 0x00, 
	0x65, 0x00, 0x26, 0x00, 0x73, 0x00, 0x74, 0x00, 
	0x61, 0x00, 0x72, 0x00, 0x74, 0x00, 0x20, 0x00, 
};
// clang-format on
const size_t lnk_content_length = 48;

bool endswith(string a, string b) {
    if (b.length() > a.length())
        return false;
    for (int i = b.length() - 1; i >= 0; i--) {
        if (a[a.size() - 1 - i] != b[b.size() - 1 - i])
            return false;
    }
    return true;
}

const size_t buffer_size = 1048576;
unsigned char file_buf[1048576];
size_t file_length;

void read_file(const string &fname) {
    FILE *ptr;
    ptr = fopen(fname.c_str(), "rb");
    file_length = fread(file_buf, sizeof(unsigned char), buffer_size, ptr);
    fclose(ptr);
}

string get_sha1() {
    SHA1 sha1;
    return sha1(file_buf, file_length);
}

bool within_file(const unsigned char *pattern, const size_t &pattern_length) {
    if (pattern_length > file_length) {
        return false;
    }
    for (size_t i = 0; i <= file_length - pattern_length; i++) {
        bool flag = true;
        for (size_t j = 0; j < pattern_length; j++) {
            if (pattern[j] != file_buf[i + j]) {
                flag = false;
                break;
            }
        }
        if (flag) {
            return true;
        }
    }
    return false;
}

void remove_file(const string &fname) {
    cout << "删除文件: " << fname << endl;
    remove(fname.c_str());
}

inline bool exists(const string &name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

int main() {
    char drive_letter = std::filesystem::current_path().string()[0];
    if(drive_letter=='C'||drive_letter=='D'){
        cout<<"注意: 本程序设计为在U盘根目录下运行!"<<endl;
        cout<<"确定要继续? [Y/n] ";
        char ans=getchar();
        if(!(ans=='\n'||ans=='y'||ans=='Y')){
            return 0;
        }
        cout<<endl;
    }

    chdir("/");

    cout << "尝试结束病毒进程:" << endl;
    system("taskkill /F /IM rundll32.exe");
    system("taskkill /F /IM wscript.exe");
    cout << endl;

    cout << "尝试恢复隐藏文件/文件夹:" << endl;
    system("attrib /D -S -H *");
    cout << endl;

    cout << "尝试删除病毒可执行文件:" << endl;
    system("attrib -A -S -H \"%APPDATA%\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\stQQ.vbe\"");
    system("attrib -A -S -H \"%APPDATA%\\stQQ.vbe\"");
    system("attrib -A -S -H \"%APPDATA%\\Microsoft\\Office\\rundll32.exe\"");
    system("del /F \"%APPDATA%\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\stQQ.vbe\"");
    system("del /F \"%APPDATA%\\stQQ.vbe\"");
    system("del /F \"%APPDATA%\\Microsoft\\Office\\rundll32.exe\"");
    cout << endl;

    DIR *dir;
    struct dirent *ptr;
    dir = opendir(".");
    while ((ptr = readdir(dir)) != NULL) {
        string fname = ptr->d_name;
        struct stat info;
        stat(fname.c_str(), &info);
        auto fsize = info.st_size;

        if (endswith(fname, ".exe") && fsize == 680511) {
            cout << "文件\"" << fname << "\": 疑似exe型病毒" << endl;
            read_file(fname);
            string hash = get_sha1();
            cout << "SHA1: " << hash << endl;
            if (hash == "06293dea80e39c7eb7ee2bdb00d60b58d932fa8a") {
                remove_file(fname);
            }
            cout << endl;
        }
        if (fname == "stQQ.vbe" && fsize == 101650) {
            cout << "文件\"" << fname << "\": 疑似vbe型病毒" << endl;
            read_file(fname);
            string hash = get_sha1();
            cout << "SHA1: " << hash << endl;
            if (hash == "17010e068d05b03651089a04ff3ebee9b3317e4c") {
                remove_file(fname);
            }
            cout << endl;
        }
        if (endswith(fname, ".lnk") && fsize <= 10000) {
            read_file(fname);
            if (within_file(lnk_content, lnk_content_length)) {
                cout << "文件\"" << fname << "\": 疑似vbe型病毒生成的快捷方式" << endl;
                remove_file(fname);
                cout << endl;
            }
        }
    }
    closedir(dir);

    system("pause");

    return 0;
}