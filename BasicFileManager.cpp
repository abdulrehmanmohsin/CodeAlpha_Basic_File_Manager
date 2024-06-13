#include <iostream>
#include <windows.h> 
#include <fstream>
#include <string>
using namespace std;
wstring stringToWstring(const string& str) {
    // Calculate the length of the wide string
    int len;
    int strLen = static_cast<int>(str.length()) + 1;
    len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), strLen, 0, 0);

    // Allocate memory for the wide string
    wstring wstr(len, L'\0');

    // Perform the conversion
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), strLen, &wstr[0], len);

    return wstr;
}
void viewfiles() {
    WIN32_FIND_DATA findFileData;
    string a = ".", b = "..";
    wstring a1 = stringToWstring(a), b1 = stringToWstring(b);
    LPCWSTR a2 = a1.c_str(), b2 = b1.c_str();
    HANDLE hFind = FindFirstFile(L"*", &findFileData); // Use L"*" to list all files and directories in the current directory
    do {
        if (wcscmp(findFileData.cFileName, a2) != 0 && wcscmp(findFileData.cFileName, b2) != 0) {
            wcout << findFileData.cFileName << endl;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
}

bool changeDirectory(const string& path) {
    wstring wPath = stringToWstring(path);
    if (SetCurrentDirectory(wPath.c_str())) {
        return true;
    }
    else {
        DWORD error = GetLastError();
        cerr << "Error changing directory: " << error << endl;
        return false;
    }
}

void viewfile(string destination) {
    ifstream file(destination);
    string str;
    while (!file.eof()) {
        getline(file, str);
        cout << str << endl;
    }
    file.close();
}
void copyfile(string source, string destination) {
    ofstream copyfile(destination);
    ifstream file(source);
    string str;
    while (!file.eof()) {
        getline(file, str);
        copyfile << str << endl;
    }
    file.close();
    copyfile.close();
}
void movefile(string source, string destination) {
    ofstream copyfile(destination);
    ifstream file(source);
    string str;
    while (!file.eof()) {
        getline(file, str);
        copyfile << str << endl;
    }
    file.close();
    copyfile.close();
    const char* filename = source.c_str();
    if (remove(filename) == 0) {
        cout << "File deleted successfully." << endl;
    }
    else {
        perror("Error deleting file");
    }
}


void createfile(string name) {
    wstring wstr = stringToWstring(name);
    LPCWSTR str = wstr.c_str();
    if (CreateDirectory(str, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
        // Directory created successfully or already exists
        cout << "Directory created successfully or already exists." << endl;
    }
    else {
        // Directory creation failed
        cerr << "Error creating directory." << endl;
    }
}
void menu() {
    cout << "Wellcome to File manager\n";
    cout << "ALL commands are listed below \n";
    cout << "mv (MOVE)\tcp (COPY)\topen (VIEW TXT FILE)\n";
    cout << "cd (CHANGE DIRECTORY)\tls (VIEW LIST OF FILES)\tnew (CREATE NEW FILE)\nexit for exiting program\n";
}
int main() {
    string curr = "D:\\Basicfilemanager\\Basicfilemanager";
    string choice = "";
    while (choice != "exit") {
        system("cls");
        menu();
        cout << "Current directory -> " << curr << endl;
        cout << "CMD->";
        getline(cin, choice);
        if (choice == "mv") {
            string dest, source;
            cout << "Enter source file : ";
            getline(cin, source);
            cout << "Enter destination file : ";
            getline(cin, dest);
            movefile(source, dest);
        }
        else if (choice == "cp") {
            string dest, source;
            cout << "Enter source file : ";
            getline(cin, source);
            cout << "Enter destination file : ";
            getline(cin, dest);
            copyfile(source, dest);
        }
        else if (choice == "open") {
            string source;
            cout << "Enter source file : ";
            getline(cin, source);
            viewfile(source);
        }
        else if (choice == "cd") {
            cout << "Enter directory loctaion ";
            getline(cin, curr);
            changeDirectory(curr);
        }
        else if (choice == "ls") {
            viewfiles();
        }
        else if (choice == "new") {
            cout << "Enter name of new directory ";
            string dir;
            getline(cin, dir);
            createfile(dir);
        }
        else if (choice == "exit") {
            break;
        }
        else {
            cout << "Wrong command\n";
        }
        system("pause");
    }
    return 0;
}