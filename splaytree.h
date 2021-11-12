#ifndef AVLTREE_H
#define AVLTREE_H

#include "binarytree.h"

namespace splay {
    template <typename TKey, typename TValue>
    node<TKey, TValue> *rotate_right(node<TKey, TValue> *p_node)
    {
        node<TKey, TValue> *q_node = p_node->left;
        p_node->left = q_node->right;
        q_node->right = p_node;
        return q_node;
    }

    template <typename TKey, typename TValue>
    node<TKey, TValue> *rotate_left(node<TKey, TValue> *p_node)
    {
        node<TKey, TValue> *q_node = p_node->right;
        p_node->right = q_node->left;
        q_node->left = p_node;
        return q_node;
    }

    template <typename TKey, typename TValue>
    node<TKey, TValue> *find_max_node(node<TKey, TValue>* root_node)
    //найти максимальный элемент в дереве
    {
        if(!root_node)
        //дерево пустое
        {
            return nullptr;
        }
        else if(!root_node->right)
        //дошли до конца правого поддерева (там где наибольший элемент)
        {
            return root_node;
        }
        else
        //рекурсивно идем по правому поддереву
        {
            return find_max_node(root_node->right);
        }
    }

    template <typename TKey, typename TValue>
    node<TKey, TValue> *splay(node<TKey, TValue> *root_node,
                              node<TKey, TValue> *p_node,
                              comparator<TKey> *key_comparator)
    {
        //базовые случаи: root_node равен nullptr или ключ находится в корне
        if (!root_node || (*key_comparator)(p_node->key, root_node->key) == EQUAL)
        {
            return root_node;
        }
        //ключ лежит в левом поддереве
        if ((*key_comparator)(p_node->key, root_node->key) == LESS)
        {
            //ключа нет в дереве, мы закончили
            if (!root_node->left)
            {
                return root_node;
            }
            //zig-zig (левый-левый)
            if ((*key_comparator)(p_node->key, root_node->left->key) == LESS)
            {
                //cначала рекурсивно поднимем ключ как корень left-left
                root_node->left->left = splay(root_node->left->left, p_node, key_comparator);
                //первый разворот для root_node, второй разворот выполняется после else
                root_node = rotate_right(root_node);
            }
            //zig-zag (Левый-правый)
            else if ((*key_comparator)(p_node->key, root_node->left->key) == GREAT)
            {
                //сначала рекурсивно поднимаем ключ как корень left-right
                root_node->left->right = splay(root_node->left->right, p_node, key_comparator);

                //выполняем первый разворот для root_node->left
                if (root_node->left->right)
                {
                    root_node->left = rotate_left(root_node->left);
                }
            }
            //выполняем второй разворот для корня
            return (!root_node->left)? root_node: rotate_right(root_node);
        }
        //ключ находится в правом поддереве
        else if ((*key_comparator)(p_node->key, root_node->key) == GREAT)
        {
            //ключа нет в дереве, мы закончили
            if (!root_node->right)
            {
                return root_node;
            }
            //zag-zig (правый-левый)
            if ((*key_comparator)(p_node->key, root_node->right->key) == LESS)
            {
                //поднять ключ как корень right-left
                root_node->right->left = splay(root_node->right->left, p_node, key_comparator);

                //выполняем первый поворот для root_node->right
                if (root_node->right->left)
                {
                    root_node->right = rotate_right(root_node->right);
                }
            }
            //zag-zag (правый-правый)
            else if ((*key_comparator)(p_node->key, root_node->right->key) == GREAT)
            {
                //поднимаем ключ как корень right-right и выполняем первый разворот
                root_node->right->right = splay(root_node->right->right, p_node, key_comparator);
                root_node = rotate_left(root_node);
            }
            //выполняем второй разворот для root_node
            return (!root_node->right)? root_node: rotate_left(root_node);
        }
    }

    template <typename TKey, typename TValue>
    void split(node<TKey, TValue> *root_node,
               node<TKey,TValue> *&right_node,
               node<TKey, TValue> *&left_node)
    {
        //если дерево пустое
        if(!root_node)
        {
            left_node = nullptr;
            right_node = nullptr;
            return;
        }
        right_node = root_node;
        left_node = root_node->left;
    }

    template <typename TKey, typename TValue>
    node<TKey, TValue>* merge(node<TKey,TValue> *right_node,
                              node<TKey, TValue> *left_node,
                              comparator<TKey> *key_comparator)
    {
        //ищем максимальный элемент в левом дереве и поднимаем его в корень
        node<TKey,TValue> *max_node = splay::find_max_node(left_node);
        left_node = splay(left_node, max_node, key_comparator);
        //соединем правое и левое деревья (при этом корнем получившегося дерева будет left_node)
        if (left_node)
        //если левое дерево не пустое
        {
            left_node->right = right_node;
        }
        else
        //если левое дерево пустое
        {
            left_node = right_node;
        }
        return left_node;
    }
}

template <typename TKey, typename TValue>
class splay_tree : public binary_tree<TKey, TValue>
{
protected:
    class splay_find_template_method : public binary_tree<TKey, TValue>::find_template_method
    {
    protected:
        void post_find_hook(node<TKey, TValue> *&root_node,
                            node<TKey, TValue> *&find_node,
                            comparator<TKey> *key_comparator);

    };
    class splay_insert_template_method : public binary_tree<TKey, TValue>::insert_template_method
    {
    protected:
        void post_insert_hook(node<TKey, TValue> *&root_node,
                              node<TKey, TValue> *&insert_node,
                              comparator<TKey> *key_comparator);

    };
    class splay_remove_template_method : public binary_tree<TKey, TValue>::remove_template_method
    {
    protected:
        status_t inner_remove(node<TKey, TValue> *&root_node,
                              TKey key,
                              comparator<TKey> *key_comparator);

    };
public:
    splay_tree(comparator<TKey> *key_comparator);
    splay_tree(binary_tree<TKey, TValue> &tree);
    ~splay_tree();
};

template <typename TKey, typename TValue>
splay_tree<TKey, TValue>::splay_tree(comparator<TKey> *key_comparator) : binary_tree<TKey, TValue>::binary_tree()
{
    splay_find_template_method *splay_finder = new splay_find_template_method;
    splay_insert_template_method *splay_inserter = new splay_insert_template_method;
    splay_remove_template_method *splay_remover = new splay_remove_template_method;
    splay_tree<TKey, TValue>::init_template_methods(splay_finder, splay_inserter, splay_remover);
    this->key_comparator = key_comparator;
}

template <typename TKey, typename TValue>
splay_tree<TKey, TValue>::splay_tree(binary_tree<TKey, TValue> &tree) : binary_tree<TKey, TValue>::binary_tree(tree)
{

}

template <typename TKey, typename TValue>
splay_tree<TKey, TValue>::~splay_tree()
{
}

template <typename TKey, typename TValue>
void splay_tree<TKey, TValue>::splay_find_template_method::post_find_hook(node<TKey, TValue> *&root_node,
                                                                        node<TKey, TValue> *&find_node,
                                                                        comparator<TKey> *key_comparator)
{
    root_node = splay::splay(root_node, find_node, key_comparator);
}

template <typename TKey, typename TValue>
void splay_tree<TKey, TValue>::splay_insert_template_method::post_insert_hook(node<TKey, TValue> *&root_node,
                                                                        node<TKey, TValue> *&insert_node,
                                                                        comparator<TKey> *key_comparator)
{
    root_node = splay::splay(root_node, insert_node, key_comparator);
}

template <typename TKey, typename TValue>
status_t splay_tree<TKey, TValue>::splay_remove_template_method::inner_remove(
        node<TKey, TValue> *&root_node,
        TKey key,
        comparator<TKey> *key_comparator)
{
    node<TKey, TValue> *current_node = root_node;
    node<TKey, TValue> *remove_node = nullptr;
    node<TKey, TValue> *right_node = nullptr;
    node<TKey, TValue> *left_node = nullptr;
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
    //подымаем удаляемый элемент в корень
    root_node = splay::splay(root_node, remove_node, key_comparator);
    //делим на два дерева
    splay::split(root_node, right_node, left_node);
    //удаляем корневой элемент (в корне и находится элемент, который нужно удалить)
    remove_node = right_node;
    right_node = right_node->right;
    delete remove_node;
    //соединяем два дерева в одно (в получившемся дереве уже не будет элемента, который нужно удалить)
    root_node = splay::merge(right_node, left_node, key_comparator);
    return REMOVE_SUCCESS;
}

#endif // AVLTREE_H
