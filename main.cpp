#include <iostream>

#include "splaytree.h"

using namespace std;

template<typename TKey, typename TValue>
void print(TKey key, TValue value, int depth)
{
    cout << "Key = " << key << "  :  Value = " << value << "  :  Depth = " << depth << endl;
}

void example_1()
{
    //пример бинарного дерева с ключем типа "int" и данными типа "string"
    cout << "Example 1:" << endl << "TKey - int, TValue - string" << endl;
    comparator<int> *comparator_int = new comparator<int>;
    //создаем экземпляр бинарного дерева
    splay_tree<int, string> *tree = new splay_tree<int, string>(comparator_int);
    try
    {
        cout << "Insert 8 : \"008\"" << endl;
        tree->insert(8, "008");
        cout << "Insert 7 : \"007\"" << endl;
        tree->insert(7, "007");
        cout << "Insert 6 : \"006\"" << endl;
        tree->insert(6, "006");
        cout << "Insert 5 : \"005\"" << endl;
        tree->insert(5, "005");
        cout << "Insert 4 : \"004\"" << endl;
        tree->insert(4, "004");
        cout << "Insert 1 : \"001\"" << endl;
        tree->insert(1, "001");
        cout << "Insert 4 : \"004\"" << endl;
        tree->insert(4, "004");
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    cout << "Result tree:" << endl;
    tree->prefix_traversal(print<int, string>);
    cout << endl;
    try
    {
        cout << "Find 4 item: " << tree->find(4) << endl;
        cout << "Find 10 item: " << tree->find(10) << endl;
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    try
    {
        cout << "Deleting 1\n";
        tree->remove(1);
        tree->prefix_traversal(print<int, string>);
        cout << endl;
        cout << "Deleting 8\n";
        tree->remove(8);
        tree->prefix_traversal(print<int, string>);
        cout << endl;
        cout << "Deleting 7\n";
        tree->remove(7);
        tree->prefix_traversal(print<int, string>);
        cout << endl;
        cout << "Deleting 6\n";
        tree->remove(6);
        tree->prefix_traversal(print<int, string>);
        cout << endl;
        cout << "Deleting 5\n";
        tree->remove(5);
        tree->prefix_traversal(print<int, string>);
        cout << endl;
        cout << "Deleting 12\n";
        tree->remove(12);
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    delete tree;
}

void example_2()
{
    //пример бинарного дерева с ключем типа "string" и данными типа "string"
    cout << "Example 1:" << endl << "TKey - string, TValue - string" << endl;
    comparator<string> *comparator_string = new comparator<string>;
    splay_tree<string, string> *tree = new splay_tree<string, string>(comparator_string);
    try
    {
        cout << "Insert \"008\" : \"008\"" << endl;
        tree->insert("008", "008");
        cout << "Insert \"007\" : \"007\"" << endl;
        tree->insert("007", "007");
        cout << "Insert \"006\" : \"006\"" << endl;
        tree->insert("006", "006");
        cout << "Insert \"005\" : \"005\"" << endl;
        tree->insert("005", "005");
        cout << "Insert \"004\" : \"004\"" << endl;
        tree->insert("004", "004");
        cout << "Insert \"001\" : \"001\"" << endl;
        tree->insert("001", "001");
        cout << "Insert \"009\" : \"009\"" << endl;
        tree->insert("009", "009");
        cout << "Insert \"010\" : \"010\"" << endl;
        tree->insert("010", "010");
        cout << "Insert \"011\" : \"011\"" << endl;
        tree->insert("011", "011");
        cout << "Insert \"004\" : \"004\"" << endl;
        tree->insert("004", "004");
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    cout << "Result tree:" << endl;
    tree->prefix_traversal(print<string, string>);
    cout << endl;
    try
    {
        cout << "Find 004 item: " << tree->find("004") << endl;
        cout << "Find 010 item: " << tree->find("010") << endl;
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    try
    {
        cout << "Deleting 005\n";
        tree->remove("005");

        tree->prefix_traversal(print<string, string>);
        cout << endl;
        cout << "Deleting 007\n";
        tree->remove("007");

        tree->prefix_traversal(print<string, string>);
        cout << endl;
        cout << "Deleting 012\n";
        tree->remove("012");
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    //delete tree;
}

void example_3()
{
    //проверка перегруженного оператора присваивания
    cout << "Testing the assignment operator:" << endl;
    comparator<int> *comparator_int = new comparator<int>;
    splay_tree<int, string> *tree = new splay_tree<int, string>(comparator_int);
    splay_tree<int, string> *tree_copy = new splay_tree<int, string>(comparator_int);
    cout << "An instance of a splay tree named \"tree\"" << endl;
    cout << "Insert into a \"tree\" \"4 : 004\"" << endl;
    tree->insert(4, "004");
    cout << "Insert into a \"tree\" \"2 : 002\"" << endl;
    tree->insert(2, "002");
    tree->prefix_traversal(print<int, string>);
    cout << endl;
    cout << "An instance of a splay tree named \"tree_copy\"" << endl;
    cout << "Insert into a \"tree_copy\" \"5 : 005\"" << endl;
    tree_copy->insert(5, "005");
    cout << "Insert into a \"tree_copy\" \"3 : 003\"" << endl;
    tree_copy->insert(3, "003");
    tree_copy->prefix_traversal(print<int, string>);
    cout << endl;
    cout << "\"tree_copy\" = \"tree\"" << endl;
    *tree_copy = *tree;
    cout << endl;
    cout << "Insert into a \"tree\" \"8 : 008\"" << endl;
    tree->insert(8, "008");
    cout << "Insert into a \"tree_copy\" \"7 : 007\"" << endl;
    tree_copy->insert(7, "007");
    cout << endl;
    cout << "The binary tree named \"tree\":" << endl;
    tree->prefix_traversal(print<int, string>);
    cout << endl;
    cout << "The binary tree named \"tree_copy\":" << endl;
    tree_copy->prefix_traversal(print<int, string>);
    cout << endl;
    delete tree;
    delete tree_copy;
}

int main()
{
    example_1();
    getchar();
    example_2();
    getchar();
    example_3();
    return 0;
}
