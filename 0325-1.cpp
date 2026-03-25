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
    std::vector<int> number;
    int graduateCount;
};

struct Node {
    std::vector<Data> data;
    Node *children[4];
    Node *parent = nullptr;

    Node() {
        for (int i = 0; i < 4; i++) {
            children[i] = nullptr;
        }
    }
    Node(Data d) {
        data[0] = d;
        for (int i = 0; i < 4; i++) {
            children[i] = nullptr;
        }
    }

    void pushData(Data d) {data.push_back(d);}

    bool isFull() {
        if (data.size() == 2) {
            return true;
        }
        return false;
    }

    void InsterKey(Data d) {
        if (isFull()) {
            if (data[0].graduateCount > d.graduateCount) data.insert(data.begin() , d); //小餘第0格插最前面
            else if (data[1].graduateCount < d.graduateCount) pushData(d);//大於第1格，插最後面
            else data.insert(data.begin() + 1 , d);//界在中間放中間
        } else {
            if (data[0].graduateCount > d.graduateCount) data.insert(data.begin() , d);
            else pushData(d);
        }
    }
    void PushChildren(Node *node) {
        int pos = 0;
        while (pos < 4 && children[pos] != nullptr && children[pos]->data.back().graduateCount < node->data[0].graduateCount) {
            pos++;
        }
        // 往後移
        for (int i = 3; i > pos; i--) {
            children[i] = children[i-1];
        }
        children[pos] = node;
    }
};

class TwoThreeTree {
  private:
    Node* root = nullptr;

    void Inster(Data d) {
        if (root == nullptr) {//沒根建根
            root = new Node(d);
            return;
        }

        Node *leaf = nullptr;
        Node *temp = root;
        while (temp != nullptr) {//找葉子
            if (d.graduateCount == temp->data[0].graduateCount) {
                temp->data[0].number.push_back(d.number[0]);
                return;
            }

            leaf = temp;
            if (temp->isFull()) {//3 node
                if (d.graduateCount == temp->data[1].graduateCount) {
                    temp->data[1].number.push_back(d.number[0]);
                    return;
                }
                if (d.graduateCount > temp->data[1].graduateCount) {
                    temp = temp->children[2];
                    continue;
                }
            } 

            if (d.graduateCount < temp->data[0].graduateCount) {
                temp = temp->children[0];
            } else {
                temp = temp->children[1];
            }
        }

        if (leaf->isFull()) {//滿了，分裂
            leaf->InsterKey(d);
            split(leaf);
        } else { //沒滿，放進去
            leaf->InsterKey(d);
        }
    }

    void split(Node *node) {
        Node *parent;
        if (node == root) {
            root = new Node();
            parent = root;
            parent->PushChildren(node);
        }
        else parent = node->parent;

        //分裂
        parent->InsterKey(node->data[1]);

        Node *temp = new Node(node->data[2]);
        temp->parent = parent;
        temp->PushChildren(node->children[2]);
        temp->PushChildren(node->children[3]);

        node->parent = parent;
        node->data.pop_back();
        node->data.pop_back();
        node->children[2] = nullptr;
        node->children[3] = nullptr;

        parent->PushChildren(temp);

        if(parent->isFull()) {
            split(parent);
        }
    }

    int CountHeight(Node* node) {
        if (node == nullptr) return 0;

        int height = CountHeight(node->children[0]);
        int temp = CountHeight(node->children[1]);
        if (temp > height) height = temp;
        temp = CountHeight(node->children[2]);
        if (temp > height) height = temp;

        return height + 1;
    }

    int CountNodeNum(Node* node) {
        if (node == nullptr) return 0;

        int temp = CountNodeNum(node->children[0]);
        temp += CountNodeNum(node->children[1]);
        temp += CountNodeNum(node->children[2]);

        return temp + 1;
    }

  public:
    void BuildTree(std::vector<GraduateInfo> information) {
        for (int i = 0 ; i < information.size() ; i++) {
            Data d;
            d.number.push_back(information[i].number);
            d.graduateCount = information[i].graduateCount;
            Inster(d);
        }
    }

    void PrintHeight() {
        int height = CountHeight(root);
        std::cout << "Tree height = " << height << std::endl;
    }

    void PrintNodeNum() {
        int num = CountNodeNum(root);
        std::cout << "Number of nodes = " << num << std::endl;
    }

    void PrintRoot(std::vector<GraduateInfo>& informa) {
        int num = 0;
        for (int i = 0 ; i < root->data.size() ; i++) {
            for (int j = 0 ; i < root->data[i].number.size() ; j++) {
                num++;
                std::cout << num << ": ";
                PrintData(root->data[i].number[j] , informa);
            }
        }
    }
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

            TwoThreeTree twoThreeTree;
            twoThreeTree.BuildTree(information);
            twoThreeTree.PrintHeight();
            twoThreeTree.PrintNodeNum();
            twoThreeTree.PrintRoot(information);
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
    std::cout << "* 1. Build 23 tree               *" << std::endl;
    std::cout << "* 2. Build AVL tree              *" << std::endl;
    //std::cout << "* 3. Build a min-max heap        *" << std::endl;
    //std::cout << "* 4: Top-K max from min-max heap *" << std::endl;
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
