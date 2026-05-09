//FINAL PROJECT
#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
using namespace std;

// Node structure
struct Node {
    string data;

    Node* left;
    Node* right;
};

// Print tree sideways
void printTree(Node* root, int space = 0, int height = 5) {
    if (root == nullptr) return;

    space += height;

    printTree(root->right, space);

    cout << endl;
    for (int i = height; i < space; i++)
        cout << " ";

    cout << root->data << "\n";

    printTree(root->left, space);
}

// Create a new node
Node* creatNode(string data)
{
    Node* Node_new = new Node();

    Node_new->data = data;
    Node_new->left = nullptr;
    Node_new->right = nullptr;

    return Node_new;
}

// Check operator
bool isOperator(string x)
{
    return x == "+" || x == "-" || x == "*" || x == "/";
}

// Operator precedence
int precedence(string op)
{
    if (op == "+" || op == "-")
        return 1;

    if (op == "*" || op == "/")
        return 2;

    return 0;
}

// Convert infix to postfix
vector<string> infixToPostfix(vector<string> tokens)
{
    vector<string> postfix;
    stack<string> operators;

    for (int i = 0; i < tokens.size(); i++)
    {
        string token = tokens[i];

        if (!isOperator(token) && token != "(" && token != ")")
        {
            postfix.push_back(token);
        }
        else if (isOperator(token))
        {
            while (!operators.empty() &&
                   operators.top() != "(" &&
                   precedence(operators.top()) >= precedence(token))
            {
                postfix.push_back(operators.top());
                operators.pop();
            }

            operators.push(token);
        }
        else if (token == "(")
        {
            operators.push(token);
        }
        else if (token == ")")
        {
            while (!operators.empty() && operators.top() != "(")
            {
                postfix.push_back(operators.top());
                operators.pop();
            }

            if (!operators.empty())
                operators.pop();
        }
    }

    while (!operators.empty())
    {
        postfix.push_back(operators.top());
        operators.pop();
    }

    return postfix;
}

// Build binary tree
Node* buildTree(vector<string> postfix)
{
    stack<Node*> st;

    for (int i = 0; i < postfix.size(); i++)
    {
        Node* Node_new = creatNode(postfix[i]);

        if (isOperator(postfix[i]))
        {
            Node_new->right = st.top();
            st.pop();

            Node_new->left = st.top();
            st.pop();
        }

        st.push(Node_new);
    }

    return st.top();
}

// Postorder traversal
void postorder(Node* root)
{
    if (root == nullptr) return;

    postorder(root->left);
    postorder(root->right);

    cout << root->data << " ";
}

// Evaluate tree
int evaluate(Node* root)
{
    stack<int> values;
    stack<Node*> s1;
    stack<Node*> s2;

    s1.push(root);

    while (!s1.empty())
    {
        Node* Current = s1.top();
        s1.pop();

        s2.push(Current);

        if (Current->left != nullptr)
            s1.push(Current->left);

        if (Current->right != nullptr)
            s1.push(Current->right);
    }

    while (!s2.empty())
    {
        Node* Current = s2.top();
        s2.pop();

        if (!isOperator(Current->data))
        {
            values.push(stoi(Current->data));
        }
        else
        {
            int right = values.top();
            values.pop();

            int left = values.top();
            values.pop();

            if (Current->data == "+")
                values.push(left + right);
            else if (Current->data == "-")
                values.push(left - right);
            else if (Current->data == "*")
                values.push(left * right);
            else if (Current->data == "/")
                values.push(left / right);
        }
    }

    return values.top();
}

// Free memory
void freeTree(Node* root)
{
    if (root == nullptr) return;

    freeTree(root->left);
    freeTree(root->right);

    delete root;
}

int main() {

    char choice;

    do {
        string line;
        string token;
        vector<string> tokens;

        cout << "\nEnter infix expression: ";
        getline(cin, line);

        stringstream ss(line);

        while (ss >> token)
        {
            tokens.push_back(token);
        }

        vector<string> postfix = infixToPostfix(tokens);

        cout << "\nPostfix Expression: ";
        for (int i = 0; i < postfix.size(); i++)
        {
            cout << postfix[i] << " ";
        }

        Node* root = buildTree(postfix);

        cout << "\n\nBinary Tree:";
        printTree(root);

        cout << "\nPostorder Traversal: ";
        postorder(root);

        int answer = evaluate(root);

        cout << "\n\nResult: " << answer << endl;

        freeTree(root);

        cout << "\nContinue? (y/n): ";
        cin >> choice;
        cin.ignore();

    } while (choice == 'y' || choice == 'Y');

    return 0;
}
