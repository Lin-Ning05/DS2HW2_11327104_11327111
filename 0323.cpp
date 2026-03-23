#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>

struct GraduateInfo {
    int number;
    int graduateCount;
    std::string school_code , school_name , department_code, department_name;
    std::string program , level; //日間∕進修別 , 等級別
    int student , teacher;
    std::string city , type; //體系別
};

struct Data {
    int number;
    int graduateCount;
};
/********************************************/
void PrintMenu();
std::string RemoveSpace(std::string target);
std::string RemoveDotTab(std::string target);
bool IsInt(std::string num);
void GetN(std::string temp , int &n);
bool SetInfo(std::vector<GraduateInfo> &informatio);
std::string GetFileName();
bool LoadFile(std::string filename , std::vector<GraduateInfo> &information);
/*********************************************/

int main() {
    int verb;
    std::vector<GraduateInfo> information;

    while(true) {
        PrintMenu();
        std::cin >> verb;
        
        if (verb == 0) break;
        else if (verb == 1) {
            information.clear();
            if (!SetInfo(information)) {//讀到0的情況要停止執行
                std::cout << std::endl;
                continue; 
            }
            if (information.size() == 0) continue; 

        } else if (verb == 2) {

        } 
        /*else if (verb == 3) {
            
        } else if (verb == 4) {
            
        }*/
        
        else {
            std::cout << "Command does not exist!\n\n";
        }
    }

    return 0;
}

/**********************************************************************/

void PrintMenu() {
    std::cout << "* Data Structures and Algorithms *" << std::endl;
    std::cout << "*** Heap Construction and Use ****" << std::endl;
    std::cout << "* 0. QUIT                        *" << std::endl;
    std::cout << "* 1. *" << std::endl;
    std::cout << "* 2. *" << std::endl;
    //std::cout << "* 3.*" << std::endl;
    //std::cout << "* 4*" << std::endl;
    std::cout << "**********************************" << std::endl;
    std::cout << "Input a choice(0, 1, 2): " << std::endl;
    return;
}

bool IsInt(std::string num) {
    if(num.size() == 0) {
        return false;
    }
    for (int i = 0; i < num.size(); i++) {
        if (i == 0 && (num[i] == '+' || num[i] == '-')) {
        continue;
    }
    if(num[i] > '9' || num[i] < '0') {
        return false;
    }
}
return true;
}

std::string RemoveSpace(std::string target) {
    std::string to_return;
    for (int i = 0; i < target.size() ;i++) {
        if (target[i] != ' ' && target[i] != '\t' && target[i] != '\n') {
            to_return = to_return + target[i];
        }
    }
    return to_return;
}

std::string RemoveDotTab(std::string target) {
    std::string to_return;
    for (int i = 0 ; i < target.size() ; i++) {
        if (target[i] == '.' || target[i] == '\t' || target[i] == ' ' || (target[i] == '\"' && i != 0)) {
            break;
        }
        if (target[i] != ',' && target[i] != '\"')
            to_return = to_return + target[i];
    }
    return to_return;
}

void GetN(std::string temp , int &n) {
    temp = RemoveDotTab(temp);
    temp = RemoveSpace(temp);
    if (IsInt(temp)) {
        n = stoi(temp);
    }
}

std::string GetFileName() {
    std::cout << "Input a file number ([0] Quit): ";
    std::string num;
    std::cin >> num;
    num = RemoveSpace(num);
    return "input" + num + ".txt";
}

bool SetInfo(std::vector<GraduateInfo> &informa) {
    std::string fileName;
    do {
        fileName = GetFileName();
        if (fileName == "input0.txt") return false;
    }
    while (!LoadFile(fileName , informa));
    return true;
}

bool LoadFile(std::string filename , std::vector<GraduateInfo> &informa) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "\n### " << filename << " does not exist! ###\n\n";
        return false;
    }

    std::string header;
    for (int i = 0 ; i < 3 ; i++) { //讀掉前三行標頭
        if (!getline(fin, header)) {
            fin.close();
            return true;
        }
    }

    GraduateInfo info;
    int number = 0; //計算唯一序號用
    std::string temp;

    while (true) {
        if (!getline(fin , info.school_code , '\t')) {
            break;
        }

        number++;
        info.number = number;
    
        getline(fin , info.school_name , '\t');
        getline(fin , info.department_code , '\t');
        getline(fin , info.department_name , '\t');

        getline(fin , info.program , '\t');
        getline(fin , info.level , '\t');

        getline(fin , temp , '\t'); //這個還要處理引號問題
        GetN(temp , info.student);
        getline(fin , temp , '\t');
        GetN(temp , info.teacher);
        getline(fin , temp , '\t');
        GetN(temp , info.graduateCount);

        getline(fin , info.city , '\t');

        getline(fin , info.type);

        informa.push_back(info);
    }
    return true;
}
