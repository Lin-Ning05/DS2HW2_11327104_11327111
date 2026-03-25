#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <algorithm>

struct GraduateInfo {
    int number;
    int graduateCount;
    std::string school_code , school_name , department_code, department_name;
    std::string program , level; //日間∕進修別 , 等級別
    int student , teacher;
    std::string city , type; //體系別
};

struct Info {
    std::vector <int> number;
    std:: string school_name;
};

struct Node2 {
    Info info;
    Node2* parent = nullptr; // 記錄父節點方便trace 保持平衡
    Node2* left = nullptr;
    Node2* right = nullptr;
};

struct Data {
    int number;
    int graduateCount;
};

/********************************************/
void PrintData(int number , std::vector<GraduateInfo>& informa);
void PrintMenu();
std::string RemoveSpace(std::string target);
std::string RemoveDotTab(std::string target);
bool IsInt(std::string num);
void GetN(std::string temp , int &n);
bool SetInfo(std::vector<GraduateInfo> &informatio);
std::string GetFileName();
bool LoadFile(std::string filename , std::vector<GraduateInfo> &information);

/*********************************************/

class AVL_Tree {
  private:
    Node2 *root = nullptr;

  public:
    void Insert(std::vector<GraduateInfo> &informa) {
        for (int i = 0; i < informa.size(); i++) {
            if (root == nullptr) {
                root = new Node2;
                root->info.number.push_back(informa[i].number);
                root->info.school_name = informa[i].school_name;
                root->left = nullptr;
                root->right = nullptr;
                root->parent = nullptr; // 只有root的parent是空
                continue;
            }
            // 開始找要放此節點的位置
            Node2* cur = root;
            while(true) {
                if (informa[i].school_name < cur -> info.school_name) { // 小於放左邊
                    if (cur -> left == nullptr) {
                        cur->left = new Node2;
                        cur->left->parent = cur;
                        cur = cur -> left;
                        cur->info.number.push_back(informa[i].number);
                        cur->info.school_name = informa[i].school_name;
                        CheckBalance(cur);
                        break;
                    }
                    cur = cur -> left;
                }

                else if (informa[i].school_name == cur -> info.school_name) {
                    cur->info.number.push_back(informa[i].number);
                    break;
                }

                else {
                    if (cur -> right == nullptr) {
                        cur->right = new Node2;
                        cur->right->parent = cur;
                        cur = cur -> right;
                        cur->info.number.push_back(informa[i].number);
                        cur->info.school_name = informa[i].school_name;
                        CheckBalance(cur);
                        break;
                    }
                    cur = cur -> right;   
                }
            }
        }
    }

    void CheckBalance(Node2* cur) {
        while (cur != nullptr) {
            int bf = CountBF(cur);
            if (bf > 1) {
                if (CountBF (cur -> left) >= 0) {
                    cur = LL_Rotate(cur);
                }

                else {
                    cur = LR_Rotate(cur);
                }
            }

            if (bf < -1) {
                if (CountBF (cur -> right) <= 0) {
                    cur = RR_Rotate(cur);
                }

                else {
                    cur = RL_Rotate(cur);
                }
            }

            cur = cur -> parent;
        }
    }

    int CountBF (Node2* cur) { // 計算平衡係數
        int left_h = CountHeight(cur -> left);
        int right_h = CountHeight(cur -> right);
        return left_h - right_h;
    }

    int CountHeight(Node2 *node) {
        if (node == nullptr) {
            return 0;
        }

        int left = CountHeight(node->left);
        int right = CountHeight(node->right);
        if (left > right) {
            return left + 1;
        } else {
            return right + 1;
        }
    }

    int CountNodeNum(Node2 *node) {
        if (node == nullptr) return 0;
        int temp = CountNodeNum(node->left);
        temp += CountNodeNum(node->right);
        return temp + 1;
    }

    Node2* LL_Rotate(Node2 *cur) {
        Node2* to_return = cur -> left; // to_return是要被旋轉上去的節點
        cur -> left = to_return -> right;
        if (to_return -> right) to_return -> right -> parent = cur;
        to_return -> parent = cur -> parent;
        to_return -> right = cur;
        cur -> parent = to_return;

        if (to_return->parent == nullptr) {
            root = to_return;
        }
        else if (to_return->parent->left == cur) {
            to_return->parent->left = to_return;
        }
        else {
            to_return->parent->right = to_return;
        }
        return to_return;
    }

    Node2* RR_Rotate(Node2 *cur) {
        Node2* to_return = cur -> right; // to_return是要被旋轉上去的節點
        cur -> right = to_return -> left;
        if (to_return -> left) to_return -> left -> parent = cur;
        to_return -> parent = cur -> parent;
        to_return -> left = cur;
        cur -> parent = to_return;
        
        if (to_return->parent == nullptr) {
            root = to_return;
        }
        else if (to_return->parent->left == cur) {
            to_return->parent->left = to_return;
        }
        else {
            to_return->parent->right = to_return;
        }
        return to_return;
    }

    Node2* LR_Rotate(Node2 *cur) {
        cur->left = RR_Rotate(cur->left);
        return LL_Rotate(cur);
    }

    Node2* RL_Rotate(Node2 *cur) {
        cur->right = LL_Rotate(cur->right);
        return RR_Rotate(cur);
    }

     void PrintHeight() {
        int height = CountHeight(root);
        std::cout << "Tree height = " << height << std::endl;
    }

    void PrintNodeNum() {
        int num = CountNodeNum(root);
        std::cout << "Number of nodes = " << num << std::endl;
    }

    void SortRoot() {
        int size = root->info.number.size();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (root->info.number[j] > root->info.number[j + 1]) {
                    std::swap(root->info.number[j], root->info.number[j + 1]);
                }
            }
        }
    }

    void PrintRoot(std::vector<GraduateInfo>& informa) {
        SortRoot();
        int num = 0;
        for (int i = 0 ; i < root->info.number.size() ; i++) {
            num++;
            std::cout << num << ": ";
            PrintData(root->info.number[i] , informa);      
        }
    }
};

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
            if (information.empty()) {
                std::cout << "### Choose 1 first. ###\n";
            }
            AVL_Tree avl;
            avl.Insert(information);
            avl.PrintHeight();
            avl.PrintNodeNum();
            avl.PrintRoot(information);

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
    std::cout << "* 1. Build 23 tree               *" << std::endl;
    std::cout << "* 2. Build AVL tree              *" << std::endl;
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

void PrintData(int number , std::vector<GraduateInfo>& informa){
    for (int i = 0 ; i < informa.size() ; i++) {
        if (informa[i].number == number) {
            std::cout << "[" << informa[i].number << "] " << informa[i].school_name << informa[i].department_name;
            std::cout << ", " << informa[i].program << ", " << informa[i].level << ", " << informa[i].student << ", " << informa[i].graduateCount;
            std::cout << std::endl;
            return;
        }
    }
}

