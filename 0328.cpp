//11327104林采寧 11327111林方晴

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
        data.push_back(d);
        for (int i = 0; i < 4; i++) {
            children[i] = nullptr;
        }
    }

    bool isFull() {return data.size() == 3;}
    void InsertKey(Data d);
    void PushChildren(Node *node);
};

class TwoThreeTree {
  private:
    Node* root = nullptr;

    void Insert(Data d);
    std::vector<int> DeleteMax();
    void split(Node *node);
    int CountHeight(Node* node);
    int CountNodeNum(Node* node);
    void DeleteTree(Node* node);
    void Fix(Node* node);

  public:
    
    void BuildTree(std::vector<GraduateInfo> information);
    void PrintHeight();
    void PrintNodeNum();
    void PrintRoot(std::vector<GraduateInfo>& informa);
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

class AVL_Tree {
  private:
    Node2 *root = nullptr;

    void CheckBalance(Node2* cur);
    int CountBF (Node2* cur);
    int CountHeight(Node2 *node);
    int CountNodeNum(Node2 *node);

    Node2* LL_Rotate(Node2 *cur);
    Node2* RR_Rotate(Node2 *cur);
    Node2* LR_Rotate(Node2 *cur);
    Node2* RL_Rotate(Node2 *cur);

    std::vector<int> DeleteMax();
    void DeleteTree(Node2* node);
  public:
    ~AVL_Tree() {
        DeleteTree(root);
        root = nullptr;
    }

    void Insert(std::vector<GraduateInfo> &informa);

    void PrintHeight();
    void PrintNodeNum();
    void SortRoot();
    void PrintRoot(std::vector<GraduateInfo>& informa);

    void RangeDelete(int range , std::vector<GraduateInfo>& informa);

    Node2* GetRoot();
    int GetSize() {return CountNodeNum(root);}
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
void PrintData(int number , std::vector<GraduateInfo>& informa);
/*********************************************/

int main() {
    std::string verb;
    std::vector<GraduateInfo> information;
    TwoThreeTree twoThreeTree;
    AVL_Tree avl;

    while(true) {
        PrintMenu();
        std::cin >> verb;
        verb = RemoveSpace(verb);
        
        if (verb == "0") break;
        else if (verb == "1") {
            information.clear();
            avl.~AVL_Tree();
            if (!SetInfo(information)) {//讀到0的情況要停止執行
                std::cout << std::endl;
                continue; 
            }
            if (information.size() == 0) continue; 

            twoThreeTree.BuildTree(information);
            twoThreeTree.PrintHeight();
            twoThreeTree.PrintNodeNum();
            twoThreeTree.PrintRoot(information);
            std::cout << std::endl;
        } else if (verb == "2") {
            if (information.size()==0) {
                std::cout << "### Choose 1 first. ###\n\n";
                continue;
            }
            
            if (avl.GetRoot()) std::cout << "### AVL tree has been built. ###\n";
            else avl.Insert(information);

            avl.PrintHeight();
            avl.PrintNodeNum();
            avl.PrintRoot(information);
            std::cout << std::endl;
        } 
        /*else if (verb == "3") {
            
        } else if (verb == "4") {
            if (!avl.GetRoot()) {
                std::cout << "### Execute command 2 first! ###\n\n";
                continue;
            }

            std::cout << "Enter the value of K in [1," << avl.GetSize() << "]: ";
            int range;

            std::cin >> range;
            if (range < 1 || range > avl.GetSize()) {
                std::cout << "\n### The value of K is out of range! ###\n\n";
                continue;
            }
            avl.RangeDelete(range , information);
            std::cout << std::endl;
        }*/
        
        else {
            std::cout << "\nCommand does not exist!\n\n";
        }
    }

    return 0;
}

/**********************************************************************/

void PrintMenu() {
    std::cout << "* Data Structures and Algorithms *" << std::endl;
    std::cout << "****** Balanced Search Tree ******" << std::endl;
    std::cout << "* 0. QUIT                        *" << std::endl;
    std::cout << "* 1. Build 23 tree               *" << std::endl;
    std::cout << "* 2. Build AVL tree              *" << std::endl;
    //std::cout << "* 3. Build a min-max heap        *" << std::endl;
    //std::cout << "* 4: Top-K max from min-max heap *" << std::endl;
    std::cout << "**********************************" << std::endl;
    std::cout << "Input a choice(0, 1, 2): ";
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
    std::cout << "\nInput a file number ([0] Quit): ";
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
        std::cout << "\n### " << filename << " does not exist! ###\n";
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
            std::cout << "[" << informa[i].number << "] " << informa[i].school_name << ", " << informa[i].department_name;
            std::cout << ", " << informa[i].program << ", " << informa[i].level << ", " << informa[i].student << ", " << informa[i].graduateCount;
            std::cout << std::endl;
            return;
        }
    }
}

/******************** Node **************************/
void Node::PushChildren(Node *node) {
    if (!node) return;
    int index = 0;
    while (index < 4 && children[index] != nullptr && children[index]->data[0].graduateCount < node->data[0].graduateCount) {
        index++;
    }
    // 往後移
    for (int i = 3; i > index; i--) {
        children[i] = children[i-1];
    }
    children[index] = node;
    node->parent = this;
}

void Node::InsertKey(Data d) {
    int index = 0;
    while (index < data.size() && data[index].graduateCount < d.graduateCount) {
        index++;
    }
    data.insert(data.begin() + index, d);
}

/******************** TwoThreeTree **************************/
//private function
void TwoThreeTree::Insert(Data d) {
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
        if (leaf->data.size() == 2) {//3 node
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

    leaf->InsertKey(d);
    if (leaf->isFull()) {//滿了，分裂
        split(leaf);
    }
    return;
}

std::vector<int> TwoThreeTree::DeleteMax() {
    Node* temp = root;
    while (temp->children[0]) {
        if (temp->children[2])  temp = temp->children[2]; // 3node 第三個小孩一定最大
  
        else temp = temp->children[1]; // 2node 第二個小孩一定最大
    }

    int max_in_node = temp->data.size() - 1; // 節點中最後一個key是最大值
    std::vector<int> result = temp->data[max_in_node].number;

    temp->data.pop_back();

    if (temp->data.empty()) Fix(temp); // delete造成節點空掉 需要維護

    return result;
}

void TwoThreeTree::split(Node *node) {
    Node *parent;
    if (node == root) {
        root = new Node();
        parent = root;
        parent->PushChildren(node);
    }
    else parent = node->parent;

    //分裂
    parent->InsertKey(node->data[1]);

    Node *temp = new Node(node->data[2]);
    if (node->children[2]) {
        temp->PushChildren(node->children[2]);
        node->children[2]->parent = temp;
    }
    if (node->children[3]) {
        temp->PushChildren(node->children[3]);
        node->children[3]->parent = temp;
    }
    parent->PushChildren(temp);
    temp->parent = parent;

    node->data.pop_back();
    node->data.pop_back();
    node->children[2] = nullptr;
    node->children[3] = nullptr;
    node->parent = parent;

    if(parent->isFull()) {
        split(parent);
    }
}

void TwoThreeTree::Fix(Node *node) {
    if (node == root) {
        if (root->children[0]) {
            root = root->children[0];
            root->parent = nullptr;
        } 
        else {
            delete root;
            root = nullptr;
        }
        return;
    }

    Node* parent = node->parent; // 要透過parent找sibling
    int index = 0; // 是第幾個小孩
    while (parent->children[index] != node) {
        index++;
    }

    Node* left = nullptr;
    Node* right = nullptr;

    if (index >= 1) { // 左邊有sibling
        left = node->children[index - 1];
    }

    if (index < 2) { // 右邊有sibling
        right = node->children[index + 1];
    }
    // 優先跟左邊借
    if (left && left->data.size() > 1) {
        node->data.push_back(parent->data[index - 1]); // 上面的搬下來
        parent->data[index - 1] = left->data.back(); // sibling 補一個上去 
        left->data.pop_back(); // sibling 失去一個
    }
    // 左邊沒得借 跟右邊借
    else if (right && right->data.size() > 1) {
        node->data.push_back(parent->data[index - 1]); // 上面的搬下來
        parent->data[index - 1] = right->data.back(); // sibling 補一個上去 
        right->data.pop_back(); // sibling 失去一個
    }

    //都沒得借 合併
    if (left) {
        // children[1] 的左是 children[0]區分兩者的key(parent->data[0])已不需要)
        // children[2] 的左是 children[1]區分兩者的key(parent->data[1])已不需要)
        left->data.push_back(parent->data[index - 1]);
        delete node;
        node = nullptr;
        parent->data.erase(parent->data.begin() + index - 1);

        // 小孩位置調整(左移)
        for (int i = index; i < 3; i++) {
            parent->children[i] = parent->children[i + 1];
        }
    }

    else if (right) {
        // children[0] 的右是 children[1]區分兩者的key(parent->data[0])已不需要)
        // children[1] 的右是 children[2]區分兩者的key(parent->data[1])已不需要) 不會到這裡
        right->data.push_back(parent->data[index]);
        delete node;
        node = nullptr;
        parent->data.erase(parent->data.begin() + index);

        // 小孩位置調整(左移)
        for (int i = index + 1; i < 3; i++) {
            parent->children[i] = parent->children[i + 1];
        }
    }

    if (parent->data.empty()) { // 遞迴
        Fix(parent);
    }
}

int TwoThreeTree::CountHeight(Node* node) {
    if (node == nullptr) return 0;

    int height = CountHeight(node->children[0]);
    int temp = CountHeight(node->children[1]);
    if (temp > height) height = temp;
    temp = CountHeight(node->children[2]);
    if (temp > height) height = temp;

    return height + 1;
}

int TwoThreeTree::CountNodeNum(Node* node) {
    if (node == nullptr) return 0;

    int temp = CountNodeNum(node->children[0]);
    temp += CountNodeNum(node->children[1]);
    temp += CountNodeNum(node->children[2]);

    return temp + 1;
}

void TwoThreeTree::DeleteTree(Node* node) {
    if (node == nullptr) return;

    for (int i = 0; i < 4; i++) {
        DeleteTree(node->children[i]);
    }

    delete node;
}

//public function
void TwoThreeTree::BuildTree(std::vector<GraduateInfo> information) {
    DeleteTree(root);
    root = nullptr;
    for (int i = 0 ; i < information.size() ; i++) {
        Data d;
        d.number.push_back(information[i].number);
        d.graduateCount = information[i].graduateCount;
        Insert(d);
    }
}

void TwoThreeTree::PrintHeight() {
    int height = CountHeight(root);
    std::cout << "Tree height = " << height << std::endl;
}

void TwoThreeTree::PrintNodeNum() {
    int num = CountNodeNum(root);
    std::cout << "Number of nodes = " << num << std::endl;
}

void TwoThreeTree::PrintRoot(std::vector<GraduateInfo>& informa) {
    int num = 0;
    for (int i = 0 ; i < root->data.size() ; i++) {
        for (int j = 0 ; j < root->data[i].number.size() ; j++) {
            num++;
            std::cout << num << ": ";
            PrintData(root->data[i].number[j] , informa);
        }
    }
    std::cout << std::endl;
}

/******************** AVL **************************/
//private function
void AVL_Tree::CheckBalance(Node2* cur) {
    while (cur != nullptr) {
        int bf = CountBF(cur);
        if (bf > 1) {
            if (CountBF (cur -> left) >= 0) {
                cur = LL_Rotate(cur);
            }

            else {
                cur = LR_Rotate(cur);
            }

            if (cur->parent == nullptr) root = cur;
            break;
        }

        if (bf < -1) {
            if (CountBF (cur -> right) <= 0) {
                cur = RR_Rotate(cur);
            }

            else {
                cur = RL_Rotate(cur);
            }

            if (cur->parent == nullptr) root = cur;
            break;
        }

        cur = cur -> parent;
    }
}

int AVL_Tree::CountBF (Node2* cur) { // 計算平衡係數
    int left_h = CountHeight(cur -> left);
    int right_h = CountHeight(cur -> right);
    return left_h - right_h;
}

int AVL_Tree::CountHeight(Node2 *node) {
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

int AVL_Tree::CountNodeNum(Node2 *node) {
    if (node == nullptr) return 0;
    int temp = CountNodeNum(node->left);
    temp += CountNodeNum(node->right);
    return temp + 1;
}

Node2* AVL_Tree::LL_Rotate(Node2 *cur) {
    Node2* to_return = cur -> left; // to_return是要被旋轉上去的節點
    cur -> left = to_return -> right;
    if (to_return -> right) to_return -> right -> parent = cur;
    to_return -> parent = cur -> parent; // 修改父母
    to_return -> right = cur;
    cur -> parent = to_return;
    // 修改父母的小孩
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

Node2* AVL_Tree::RR_Rotate(Node2 *cur) {
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

Node2* AVL_Tree::LR_Rotate(Node2 *cur) {
    cur->left = RR_Rotate(cur->left);
    return LL_Rotate(cur);
}

Node2* AVL_Tree::RL_Rotate(Node2 *cur) {
    cur->right = LL_Rotate(cur->right);
    return RR_Rotate(cur);
}

std::vector<int> AVL_Tree::DeleteMax() {
    Node2* temp = root;
    while (temp->right) {//找到沒有右邊，temp是最大
        temp = temp->right;
    }

    std::vector<int> number = temp->info.number; //回傳的
    if (temp == root) {
        root = temp->left;
        if (root) root->parent = nullptr;
        delete temp;
        temp = nullptr;
        return number;
    }

    temp->parent->right = temp->left;
    if (temp->left) {
        temp->left->parent = temp->parent;
    }
    Node2 *falseNode = temp->parent;
    delete temp;
    temp = nullptr;

    CheckBalance(falseNode);
    return number;
}

void AVL_Tree::DeleteTree(Node2* node) {
    if (node == nullptr) return;

    DeleteTree(node->left);
    DeleteTree(node->right);

    delete node;
}

//public function
void AVL_Tree::Insert(std::vector<GraduateInfo> &informa) {
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

void AVL_Tree::PrintHeight() {
    int height = CountHeight(root);
    std::cout << "Tree height = " << height << std::endl;
}

void AVL_Tree::PrintNodeNum() {
    int num = CountNodeNum(root);
    std::cout << "Number of nodes = " << num << std::endl;
}

void AVL_Tree::SortRoot() {
    int size = root->info.number.size();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (root->info.number[j] > root->info.number[j + 1]) {
                std::swap(root->info.number[j], root->info.number[j + 1]);
            }
        }
    }
}

void AVL_Tree::PrintRoot(std::vector<GraduateInfo>& informa) {
    SortRoot();
    int num = 0;
    for (int i = 0 ; i < root->info.number.size() ; i++) {
        num++;
        std::cout << num << ": ";
        PrintData(root->info.number[i] , informa);      
    }
    std::cout << std::endl;
}

Node2* AVL_Tree::GetRoot() {
    if (root) return root;
    else return nullptr;
}

void AVL_Tree::RangeDelete(int range , std::vector<GraduateInfo>& informa) {
    for (int i = 0 ; i < range ; i++) {
        std::cout << "Top " << std::right << std::setw(3) << i+1 << ": ";
        std::vector<int> n = DeleteMax();
        for (int j = 0 ; j < n.size() ; j++) {
            PrintData(n[j] , informa);
        }
    }
}
