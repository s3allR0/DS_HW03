#include <iostream>
#include <cmath>

using namespace std;

struct node {// 定義節點結構，用於存儲多項式的係數和指數
    int coef;
    int exp;
    node* link;// 指向下一個節點的指標
    node(int c, int e, node* l) {// 節點的建構函數
        coef = c;
        exp = e;
        link = l;
    }
};

class Polynomial {
private:
    node* head;// 指向循環鏈表的頭節點
    void clear() {// 清空多項式的所有節點
        if (!head) return;
        node* curr = head->link;
        while (curr != head) {
            node* temp = curr;
            curr = curr->link;
            delete temp;
        }
        delete head;
        head = nullptr;
    }

public:
    Polynomial() :head(new node(0, 0, nullptr)) {// 預設建構函數
        head->link = head;
    }
    Polynomial(const Polynomial& other) :head(new node(0, 0, nullptr)) {// 複製建構函數
        head->link = head;
        node* curr_this = head;
        node* curr_other = other.head->link;
        while (curr_other != other.head) {
            curr_this->link = new node(curr_other->coef, curr_other->exp, head);
            curr_this = curr_this->link;
            curr_other = curr_other->link;
        }
    }
    Polynomial(int coef, int exp) :Polynomial() {// 帶參數的建構函數，用於初始化單項式
        node* newNode = new node(coef, exp, head->link); // 插入到 head 之後
        head->link = newNode;
    }
    ~Polynomial() {
        clear();
    }
    Polynomial operator+(const Polynomial& other) const { // 重載加法運算符，實現多項式相加
        Polynomial result;
        node* curr_1 = head->link;
        node* curr_2 = other.head->link;
        node* tail = result.head;

        while (curr_1 != head || curr_2 != other.head) {
            if (curr_2 != other.head && (curr_1 == head || curr_2->exp > curr_1->exp)) { // Corrected condition
                tail->link = new node(curr_2->coef, curr_2->exp, result.head);
                curr_2 = curr_2->link;
            }
            else if (curr_1 != head && (curr_2 == other.head || curr_1->exp > curr_2->exp)) { // Corrected condition
                tail->link = new node(curr_1->coef, curr_1->exp, result.head);
                curr_1 = curr_1->link;
            }
            else if (curr_1 != head && curr_2 != other.head && curr_1->exp == curr_2->exp) { // added check to prevent segfault
                int sumCoef = curr_1->coef + curr_2->coef;
                if (sumCoef != 0) {
                    tail->link = new node(sumCoef, curr_1->exp, result.head);
                }
                curr_1 = curr_1->link;
                curr_2 = curr_2->link;
            }
            if (tail->link) tail = tail->link; // Important: Advance tail only if a node is added
        }
        return result;
    }
    Polynomial operator-(const Polynomial& other) const {// 重載減法運算符，實現多項式相減
        Polynomial negOther;
        node* curr = other.head->link;
        while (curr != other.head) {
            negOther = negOther + Polynomial(-curr->coef, curr->exp);
            curr = curr->link;
        }
        return *this + negOther;
    }
    Polynomial operator*(const Polynomial& other) const {// 重載乘法運算符，實現多項式相乘
        Polynomial result;
        node* curr_1 = head->link;
        while (curr_1 != head) {
            node* curr_2 = other.head->link;
            while (curr_2 != other.head) {
                result = result + Polynomial(curr_1->coef * curr_2->coef, curr_1->exp + curr_2->exp);
                curr_2 = curr_2->link;
            }
            curr_1 = curr_1->link;
        }
        return result;
    }
    Polynomial operator=(const Polynomial& other) {// 重載賦值運算符，實現多項式的深拷貝
        if (this == &other) return *this;// 防止自我賦值
        clear();
        head = new node(0, 0, nullptr); // 重新創建 head
        head->link = head;
        node* curr = other.head->link;
        node* tail = head;
        while (curr != other.head) {
            tail->link = new node(curr->coef, curr->exp, head);
            tail = tail->link;
            curr = curr->link;
        }
        return *this;
    }
    float Evaluate(float x) const {
        float result = 0;
        node* check = head->link;
        while (check != head) {
            result += check->coef * pow(x, check->exp);
            check = check->link;
        }
        return result;
    }
    friend istream& operator>>(istream& is, Polynomial& x) {// 重載輸入運算符，從輸入流中讀取多項式
        int n;
        is >> n;
        x.clear();
        x.head = new node(0, 0, nullptr);
        x.head->link = x.head;
        for (int i = 0; i < n; ++i) {
            int coef, exp;
            is >> coef >> exp;
            x = x + Polynomial(coef, exp);
        }
        return is;
    }
    friend ostream& operator<<(ostream& os, const Polynomial& x) {// 重載輸出運算符，將多項式輸出到輸出流
        node* curr = x.head->link;
        bool first = true;
        while (curr != x.head) {
            if (!first && curr->coef > 0) os << "+";
            os << curr->coef << "x^" << curr->exp;
            if (curr->link != x.head) os << " ";
            curr = curr->link;
            first = false;
        }
        return os;
    }
};

int main() {
    Polynomial p1, p2;
    cout << "Enter the first polynomial(n,c1,e1,c2,e2...): ";
    cin >> p1;
    cout << "Enter the second polynomial(n,c1,e1,c2,e2...): ";
    cin >> p2;
    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial pro = p1 * p2;
    cout << "p1:" << p1 << endl;
    cout << "p2:" << p2 << endl;
    cout << "p1+p2=" << sum << endl;
    cout << "p1-p2=" << diff << endl;
    cout << "p1*p2=" << pro << endl;
    float x = 0;
    cout << "x=";
    cin >> x;
    cout << "f1(" << x << ")=" << "p1.Evaluate(x) << '\n';
        cout << "f1(" << x << ")=" << p2.Evaluate(x);
}