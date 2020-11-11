/*
    Virus Fixer v2.0
    By Zzzyt,HellHoleStudios 2018
*/

#include <bits/stdc++.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int main() {
	cout<<"=========================================="<<endl;
	cout<<"          Virus Fixer v2.0"<<endl;
	cout<<"              By Zzzyt"<<endl;
	cout<<"         HellHoleStudios 2018"<<endl;
	cout<<"=========================================="<<endl;
	
	system("del MSDOS*");
	DIR    *dir;
	struct    dirent    *ptr;
	dir = opendir(".");

	while((ptr = readdir(dir)) != NULL) {
		string ss=ptr->d_name;
		if(ss=="VirusFixer v2.exe")continue;
		if(ss.size()<=4) {
			//folder
			system(("attrib /s /d -h -s \""+ss+"\"").c_str());
		} else {
			if(ss.substr(ss.size()-4,4)==".exe") {
				//.exe
				system(("del \""+ss+"\"").c_str());
			}
			else{
				//folder
				system(("attrib /s /d -h -s \""+ss+"\"").c_str());
			}
		}
	}
	closedir(dir);
	return 0;
}
