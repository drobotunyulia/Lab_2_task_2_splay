#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include "comparator.h"
#include "node.h"
#include "treeexception.h"

enum status_t {
    FIND_SUCCESS,
    FIND_ERROR,
    INSERT_SUCCESS,
    INSERT_ERROR,
    REMOVE_SUCCESS,
    REMOVE_ERROR
};

namespace bst {
    template <typename TKey, typename TValue>
    node<TKey, TValue> *get_left(node<TKey, TValue> *p_node)
    //получить указатель на левого потомка
    {
        if (p_node)
        {
            return p_node->left;
        }
        return nullptr;
    }

    template <typename TKey, typename TValue>
    node<TKey, TValue> *get_right(node<TKey, TValue> *p_node)
    //получить указатель на правого потомка
    {
        if (p_node)
        {
            return p_node->right;
        }
        return nullptr;
    }


    template <typename TKey, typename TValue>
    node<TKey, TValue> *find_min_node(node<TKey, TValue>* root_node)
    //найти минимальный элемент в дереве
    {
        if(!root_node)
        //дерево пустое
        {
            return nullptr;
        }
        else if(!root_node->left)
        //дошли до конца левого поддерева (там где наименьший элемент)
        {
            return root_node;
        }
        else
        //рекурсивно идем по левому поддереву
        {
            return find_min_node(root_node->left);
        }
    }

    template <typename TKey, typename TValue>
    node<TKey, TValue> *get_parent(node<TKey, TValue> *root_node,
                                   node<TKey, TValue> *p_node,
                                   comparator<TKey> *key_comparator)
    //получить указатель на предка узла p_node
    {
        node<TKey, TValue> *parent_node = nullptr;
        if (!root_node)
        //дерево пустое
        {
            return nullptr;
        }
        if (!p_node)
        //элемент, для которого ищется предок, пустой
        {
            return nullptr;
        }
        while (root_node && root_node->key != p_node->key)
        {
            compare_t compare_result = (*key_comparator)(p_node->key, root_node->key);
            parent_node = root_node;
            if (compare_result == LESS)
            //идем по левой стороне
            {
                root_node = root_node->left;
            }
            else if (compare_result == GREAT)
            //идем по правой стороне
            {
                root_node = root_node->right;
            }
        }
        return parent_node;
    }
}

template <typename TKey, typename TValue>
class binary_tree
{
protected:
    //вложенный класс исключения "ошибка поиска"
    class find_error_exception : public tree_exception
    {
    public:
        find_error_exception(TKey key);
    };
    //вложенный класс исключения "ошибка вставки"
    class insert_error_exception : public tree_exception
    {
    public:
        insert_error_exception(TKey key);
    };
    //вложенный класс исключения "ошибка удаления"
    class remove_error_exception : public tree_exception
    {
    public:
        remove_error_exception(TKey key);
    };
    class find_template_method
    //вложенный класс шаблонного метода поиска элемента в дереве
    {
    public:
        //декорирующий интерфейсный метод (обертка) для поиска элемента в дереве
        //не может быть переопределен в наследуемом классе
        node<TKey, TValue> *invoke_find(node<TKey, TValue> *&root_node,
                                        TKey key,
                                        comparator<TKey> *key_comparator);
    protected:
        //основной метод поиска элемента в дереве
        //в find_node возвращает указатель на найденный элемент
        //в случае необходимости может быть переопределен в наследуемом классе
        virtual status_t inner_find(node<TKey, TValue> *&root_node,
                                    TKey key,
                                    comparator<TKey> *key_comparator,
                                    node<TKey, TValue> *&find_node);
        //метод-хук, вызываемый после основного метода поиска элемента в дереве
        //в случае необходимости может быть переопределен в наследуемом классе
        virtual void post_find_hook(node<TKey, TValue> *&root_node,
                                    node<TKey, TValue> *&find_node,
                                    comparator<TKey> *key_comparator);
    };

    class insert_template_method
    //вложенный класс шаблонного метода вставки элемента в дерево
    {
    public:
        //декорирующий интерфейсный метод (обертка) для вставки элемента в дерево
        //не может быть переопределен в наследуемом классе
        void invoke_insert(node<TKey, TValue> *&root_node, TKey key,
                           TValue value,
                           comparator<TKey> *key_comparator);
    protected:
        //основной метод вставки элемента в дерево
        //в insert_node возвращает указатель на вставленный элемент
        //в случае необходимости может быть переопределен в наследуемом классе
        virtual status_t inner_insert(node<TKey, TValue> *&root_node,
                                      TKey key,
                                      TValue value,
                                      comparator<TKey> *key_comparator,
                                      node<TKey, TValue> *&insert_node);
        //метод-хук, вызываемый после основного метода вставки элемента в дерево
        //в случае необходимости может быть переопределен в наследуемом классе
        virtual void post_insert_hook(node<TKey, TValue> *&root_node,
                                      node<TKey, TValue> *&insert_node,
                                      comparator<TKey> *key_comparator);
    };

    class remove_template_method
    //вложенный класс шаблонного метода удаления элемента из дерева
    {
    public:
        //декорирующий интерфейсный метод (обертка) для удаления элемента из дерева
        //не может быть переопределен в наследуемом классе
        void invoke_remove(node<TKey, TValue> *&root_node,
                           TKey key,
                           comparator<TKey> *key_comparator);
    protected:
        //основной метод удаления элемента из дерева
        //в случае необходимости может быть переопределен в наследуемом классе
        virtual status_t inner_remove(node<TKey, TValue> *&root_node,
                                      TKey key,
                                      comparator<TKey> *key_comparator);
        //метод-хук, вызываемый после основного метода удаления элемента из дерева
        //в случае необходимости может быть переопределен в наследуемом классе
        virtual void post_remove_hook(node<TKey, TValue> *&root_node,
                                 comparator<TKey> *key_comparator);
    };

public:
    //функция обратного вызова
    typedef std::function<void(TKey key, TValue value, int depth)> callback_function;

    binary_tree(comparator<TKey> *key_comparator);
    binary_tree(binary_tree<TKey, TValue> &tree);
    virtual ~binary_tree();
    binary_tree& operator = (const binary_tree &tree_object);

    TValue find(TKey key);
    void insert(TKey key, TValue value);
    void remove(TKey key);

    void prefix_traversal(callback_function function) const;
    void postfix_traversal(callback_function function) const;
    void infix_traversal(callback_function function) const;
protected:
    //пустой конструктор
    //вызывается только в конструкторе производного класса
    binary_tree();
    //инициализация шаблонных методов
    void init_template_methods(find_template_method *finder,
                               insert_template_method *inserter,
                               remove_template_method *remover);
    void prefix_traversal_base(node<TKey, TValue> *root_node,
                               callback_function function,
                               int depth) const;
    void postfix_traversal_base(node<TKey, TValue> *root_node,
                                callback_function function,
                                int depth) const;
    void infix_traversal_base(node<TKey, TValue> *roott_node,
                              callback_function function,
                              int depth) const;
    node<TKey, TValue> *root_node = nullptr;
    comparator<TKey> *key_comparator;
private:
    //указатели на классы шаблонных методов
    find_template_method *finder;
    insert_template_method *inserter;
    remove_template_method *remover;

};

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::find_error_exception::find_error_exception(TKey key)
{
    std::stringstream key_string;
    key_string << key;
    std::string exception_message = "Find error. Element with key \"" + key_string.str() + "\" not found.";
    set_exception_message(exception_message);
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::insert_error_exception::insert_error_exception(TKey key)
{
    std::stringstream key_string;
    key_string << key;
    set_exception_message("Insert error. Element with key \"" + key_string.str() + "\" already exists.");
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::remove_error_exception::remove_error_exception(TKey key)
{
    std::stringstream key_string;
    key_string << key;
    std::string exception_message = "Remove error. Element with key \"" + key_string.str() + "\" not found.";
    set_exception_message(exception_message);
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree()
{

}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree(comparator<TKey> *key_comparator)
{
    this->finder = new find_template_method;
    this->inserter = new insert_template_method;
    this->remover = new remove_template_method;
    this->key_comparator = key_comparator;
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::init_template_methods(find_template_method *finder,
                                                insert_template_method *inserter,
                                                remove_template_method *remover)
{
    this->finder = finder;
    this->inserter = inserter;
    this->remover = remover;
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree(binary_tree<TKey, TValue> &tree) : binary_tree(tree.key_comparator)
//конструктор копирования
{
    std::vector<node<TKey, TValue>> nodes;
    tree.prefix_traversal([&nodes](TKey key, TValue value, int depth) { nodes.push_back({ key, value }); });
    for (size_t i = 0; i < nodes.size(); i++)
    {
        this->inserter->invoke_insert(this->root_node, nodes[i].key, nodes[i].value, this->key_comparator);
    }
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::~binary_tree()
{
    std::vector<node<TKey, TValue>> nodes;
    postfix_traversal([&nodes](TKey key, TValue value, int depth) { nodes.push_back({ key, value }); });
    for (size_t i = 0; i < nodes.size(); i++)
    {
        this->remover->invoke_remove(this->root_node, nodes[i].key, this->key_comparator);
    }
    delete finder;
    delete inserter;
    delete remover;
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>& binary_tree<TKey, TValue>::operator = (const binary_tree &tree)
//переопределение оператора присваивания
{
    std::vector<node<TKey, TValue>> nodes;
    postfix_traversal([&nodes](TKey key, TValue value, int depth) { nodes.push_back({ key, value }); });
    for (size_t i = 0; i < nodes.size(); i++)
    {
        this->remover->invoke_remove(this->root_node, nodes[i].key, this->key_comparator);
    }
    *(this->finder) = *(tree.finder);
    *(this->inserter) = *(tree.inserter);
    *(this->remover) = *(tree.remover);
    *(this->key_comparator) = *(tree.key_comparator);
    nodes.clear();
    tree.prefix_traversal([&nodes](TKey key, TValue value, int depth) { nodes.push_back({ key, value }); });
    for (size_t i = 0; i < nodes.size(); i++)
    {
        this->inserter->invoke_insert(this->root_node, nodes[i].key, nodes[i].value, this->key_comparator);
    }
    return *this;
}

template <typename TKey, typename TValue>
TValue binary_tree<TKey, TValue>::find(TKey key)
//метод поиска элемента в дереве
//в нем вызывается декорирующий интерфейсный метод из класса шаблонного метода поиска
{
    node<TKey, TValue> *find_node = finder->invoke_find(this->root_node, key, this->key_comparator);
    return find_node->value;
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::insert(TKey key, TValue value)
//метод вставки элемента в дерево
//в нем вызывается декорирующий интерфейсный метод из класса шаблонного метода вставки
{
    inserter->invoke_insert(this->root_node, key, value, this->key_comparator);
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::remove(TKey key)
//метод удаления элемента в дереве
//в нем вызывается декорирующий интерфейсный метод из класса шаблонного метода удаления
{
    remover->invoke_remove(this->root_node, key, this->key_comparator);
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::prefix_traversal(callback_function function) const
{
    prefix_traversal_base(root_node, function, 0);
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::postfix_traversal(callback_function function) const
{
    postfix_traversal_base(root_node, function, 0);
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::infix_traversal(callback_function function) const
{
    infix_traversal_base(root_node, function, 0);
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::prefix_traversal_base(node<TKey, TValue> *root_node,
                                                      callback_function function,
                                                      int depth) const
{
    if (root_node)
    {
        function(root_node->key, root_node->value, depth);
        if (root_node->left)
        {
            prefix_traversal_base(root_node->left, function, depth + 1);
        }
        if (root_node->right)
        {
            prefix_traversal_base(root_node->right, function, depth + 1);
        }
    }
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::postfix_traversal_base(node<TKey, TValue> *root_node,
                                                       callback_function function,
                                                       int depth) const
{
    if (root_node)
    {
        if (root_node->left)
        {
            postfix_traversal_base(root_node->left, function, depth + 1);
        }
        if (root_node->right)
        {
            postfix_traversal_base(root_node->right, function, depth + 1);
        }
        function(root_node->key, root_node->value, depth);
    }
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::infix_traversal_base(node<TKey, TValue> *roott_node,
                                                     callback_function function,
                                                     int depth) const
{
    if (roott_node)
    {
        if (roott_node->left)
        {
            infix_traversal_base(roott_node->left, function, depth + 1);
        }
        function(roott_node->key, roott_node->value, depth);
        if (roott_node->right)
        {
            infix_traversal_base(roott_node->right, function, depth + 1);
        }
    }
}

template <typename TKey, typename TValue>
node<TKey, TValue>* binary_tree<TKey, TValue>::find_template_method::invoke_find(
        node<TKey, TValue> *&root_node,
        TKey key,
        comparator<TKey> *key_comparator)
{
    node<TKey, TValue> *find_node = nullptr;
    status_t status = inner_find(root_node, key, key_comparator, find_node);
    if (status == FIND_ERROR)
    {
        throw find_error_exception(key);
    }
    post_find_hook(root_node, find_node, key_comparator);
    return find_node;
}

template <typename TKey, typename TValue>
status_t binary_tree<TKey, TValue>::find_template_method::inner_find(
        node<TKey, TValue> *&root_node,
        TKey key,
        comparator<TKey> *key_comparator,
        node<TKey, TValue> *&find_node)
{
    node<TKey, TValue> *current_node = root_node;
    while(current_node)
    {
        switch ((*key_comparator)(key, current_node->key)) {
        case LESS:
            //идем по левой стороне
            current_node = current_node->left;
            break;
        case GREAT:
            //идем по правой стороне
            current_node = current_node->right;
            break;
        case EQUAL:
            //нужный элемент найден
            find_node = current_node;
            return FIND_SUCCESS;
            break;
        }
    }
    //нужный элемент отсутствует
    return FIND_ERROR;
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::find_template_method::post_find_hook(
        node<TKey, TValue> *&root_node,
        node<TKey, TValue> *&find_node,
        comparator<TKey> *key_comparator)
{
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::insert_template_method::invoke_insert(
        node<TKey, TValue> *&root_node,
        TKey key,
        TValue value,
        comparator<TKey> *key_comparator)
{
    node<TKey, TValue> *insert_node = new node<TKey, TValue>(key, value);
    status_t status = inner_insert(root_node, key, value, key_comparator, insert_node);
    if (status == INSERT_ERROR)
    {
        throw insert_error_exception(key);
    }
    post_insert_hook(root_node, insert_node, key_comparator);
}

template <typename TKey, typename TValue>
status_t binary_tree<TKey, TValue>::insert_template_method::inner_insert(
        node<TKey, TValue> *&root_node,
        TKey key,
        TValue value,
        comparator<TKey> *key_comparator,
        node<TKey, TValue> *&insert_node)
{
    if (!root_node)
    {
        root_node = insert_node;
    }
    else
    {
        node<TKey, TValue> *current_node = root_node;
        node<TKey, TValue> *parent_node = nullptr;
        compare_t compare_result;
        while (current_node)
        {
            compare_result = (*key_comparator)(insert_node->key, current_node->key);
            parent_node = current_node;
            if (compare_result == LESS)
            //идем по левой стороне
            {
                current_node = current_node->left;
            }
            else if (compare_result == GREAT)
            //идем по правой стороне
            {
                current_node = current_node->right;
            }
            else if (compare_result == EQUAL)
            //элемент с таким ключем уже существует
            {
                return INSERT_ERROR;
            }
        }
        compare_result = (*key_comparator)(insert_node->key, parent_node->key);
        if (compare_result == LESS)
        //если ключ вставляемого элемента меньше ключа предка, вставляем слева
        {
            parent_node->left = insert_node;
        }
        else if (compare_result == GREAT)
        //если ключ вставляемого элемента больше ключа предка, вставляем справа
        {
            parent_node->right = insert_node;
        }
    }
    return INSERT_SUCCESS;
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::insert_template_method::post_insert_hook(
        node<TKey, TValue> *&root_node,
        node<TKey, TValue> *&insert_node,
        comparator<TKey> *key_comparator)
{
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::remove_template_method::invoke_remove(
        node<TKey, TValue> *&root_node,
        TKey key,
        comparator<TKey> *key_comparator)
{
    status_t status = inner_remove(root_node, key, key_comparator);
    if (status == REMOVE_ERROR)
    {
        throw remove_error_exception(key);
    }
    post_remove_hook(root_node, key_comparator);
}

template <typename TKey, typename TValue>
status_t binary_tree<TKey, TValue>::remove_template_method::inner_remove(
        node<TKey, TValue> *&root_node,
        TKey key,
        comparator<TKey> *key_comparator)
{
    node<TKey, TValue> *replace_node = nullptr;
    node<TKey, TValue> *replace_parent_node = nullptr;
    node<TKey, TValue> *current_node = root_node;
    node<TKey, TValue> *remove_node = nullptr;
    while(current_node)
    //ищем удаляемый элемент
    {
        compare_t compare_result = (*key_comparator)(key, current_node->key);
        if (compare_result == LESS)
        //идем по левой стороне
        {
            current_node = current_node->left;
        }
        else if (compare_result == GREAT)
        //идем по правой стороне
        {
            current_node = current_node->right;
        }
        else if (compare_result == EQUAL)
        //элемент найден
        {
            remove_node = current_node;
            break;
        }
    }
    if (!remove_node)
    //удаляемый элемент отсутствует
    {
        return REMOVE_ERROR;
    }
    if (remove_node->left && remove_node->right)
    //удаляемый элемент имеет двоих потомков
    {
        replace_node = bst::find_min_node(remove_node->right);
        replace_parent_node = bst::get_parent(root_node, replace_node, key_comparator);
        //переносим значения из заменяющего элемента в удаляемый элемент
        remove_node->key = replace_node->key;
        remove_node->value = replace_node->value;
        //теперь заменяющий элемент становится удаляемым
        remove_node = replace_node;
        //и его нужно удалить
        if (remove_node->right)
        //заменяющий (удаляемый) элемент имеет правого потомка
        {
            replace_node = remove_node->right;
            if (remove_node == replace_parent_node->right)
            {
                replace_parent_node->right = replace_node;
            }
            else
            {
                replace_parent_node->left = replace_node;
            }
        }
        else
        //заменяющий (удаляемый) элемент не имеет потомков
        {
            replace_node = nullptr;
            if (remove_node == replace_parent_node->left)
            {
                replace_parent_node->left = replace_node;
            }
            else
            {
                replace_parent_node->right = replace_node;
            }
        }
    }
    else if (remove_node->left)
    //удаляемый элемент имеет только левого потомка
    {
        replace_node = remove_node->left;
        if (bst::get_parent(root_node, remove_node, key_comparator))
        //удаляемый элемент не корневой (имеет предка)
        {
            if (remove_node == bst::get_left(bst::get_parent(root_node, remove_node, key_comparator)))
            {
                bst::get_parent(root_node, remove_node, key_comparator)->left = replace_node;
            }
            else
            {
                bst::get_parent(root_node, remove_node, key_comparator)->right = replace_node;
            }
        }
        else
        //удаляемый элемент корневой
        {
            root_node = replace_node;
        }
    }
    else if (remove_node->right)
    //удаляемый элемент имеет только правого потомка
    {
        replace_node = remove_node->right;
        if (bst::get_parent(root_node, remove_node, key_comparator))
        //удаляемый элемент не корневой (имеет предка)
        {
            if (remove_node == bst::get_right(bst::get_parent(root_node, remove_node, key_comparator)))
            {
                bst::get_parent(root_node, remove_node, key_comparator)->right = replace_node;
            }
            else
            {
                bst::get_parent(root_node, remove_node, key_comparator)->left = replace_node;
            }
        }
        else
        //удаляемый элемент корневой
        {
            root_node = replace_node;
        }
    }
    else
    //удаляемый элемент не имеет потомков
    {
        if (bst::get_parent(root_node, remove_node, key_comparator))
        //удаляемый элемент не корневой (имеет предка)
        {
            if (remove_node == bst::get_left(bst::get_parent(root_node, remove_node, key_comparator)))
            {
                bst::get_parent(root_node, remove_node, key_comparator)->left = replace_node;
            }
            else
            {
                bst::get_parent(root_node, remove_node, key_comparator)->right = replace_node;
            }
        }
        else
        //удаляемый элемент корневой
        {
            root_node = replace_node;
        }
    }
    delete remove_node;
    return REMOVE_SUCCESS;
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::remove_template_method::post_remove_hook(
        node<TKey, TValue> *&root_node,
        comparator<TKey> *key_comparator)
{
}

#endif // BINARYTREE_H
