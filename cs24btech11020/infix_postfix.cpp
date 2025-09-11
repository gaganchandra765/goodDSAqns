# include <iostream>
using namespace std;

struct Stack{
int top;
char arr[100];
Stack(){
top =-1;
}

void push(char c){
arr[++top] = c;
}

char pop(){
return arr[top--];
}

void displayStack(){
for (int i=0;i<top+1;i++){
cout << arr[i];
}
cout <<endl;
}

char peek(){
return arr[top];
}

bool isEmpty(){
return top==-1;
}
};

bool isChar(char c){
if (c >='a' && c<= 'z' || c>='A' && c<='Z'){
return true;
}else return false;
}

int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

bool isRightAssociative(char op) {
    return op == '^';
}

void infix2postfix(string infix) {
    Stack symbolStack;
    string output = "";
    int len = infix.size();

    for (int i = 0; i < len; i++) {
        char c = infix[i];

        if (isChar(c)) {
            output += c;
        }
        else if (c == '(') {
            symbolStack.push(c);
        }
        else if (c == ')') {
            while (!symbolStack.isEmpty() && symbolStack.peek() != '(') {
                output += symbolStack.pop();
            }
            if (!symbolStack.isEmpty() && symbolStack.peek() == '(') {
                symbolStack.pop(); // remove '('
            }
        }
        else { // operator
            while (!symbolStack.isEmpty() &&
                   precedence(symbolStack.peek()) > 0 &&
                   (precedence(symbolStack.peek()) > precedence(c) ||
                   (precedence(symbolStack.peek()) == precedence(c) && !isRightAssociative(c))) &&
                   symbolStack.peek() != '(') {
                output += symbolStack.pop();
            }
            symbolStack.push(c);
        }
    }

    while (!symbolStack.isEmpty()) {
        output += symbolStack.pop();
    }

    cout << "corresponding postfix form: "<<output << endl;
}

int main() {
    int k;
    cin >> k;
    for (int i = 0; i < k; i++) {
        string infix;
        cin >> infix;
        infix2postfix(infix);
    }
    return 0;
}

