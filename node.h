#ifndef NODE_H
#define NODE_H

template <typename TKey, typename TValue>
struct node
{
    TKey key;
    TValue value;
    int height;
    int color = 0;
    node *left = nullptr;
    node *right = nullptr;
    node();
    node(TKey key, TValue value);
    node &operator = (const node &node);
};

template <typename TKey, typename TValue>
node<TKey, TValue>::node()
{

}

template <typename TKey, typename TValue>
node<TKey, TValue>::node(TKey key, TValue value)
{
    this->key = key;
    this->value = value;
}

#endif // NODE_H
