#ifndef BST_H
#define BST_H

// Bst
template <typename T>
class Bst
{
private:


    // Bst Node
    class Node
    {
    public:
        T value;
        Node *parent;
        Node *lft;
        Node *rgt;


        Node(T v)
            : value(v), parent(NULL), lft(NULL), rgt(NULL)
        {}
    };

    // Inserts Node* to bst
    void _insert(Node **currentNode, Node *parent, Node *newNode);

    // Removes Node* from bst
    void _remove(Node *del);

    // Set newChild as new child of parent
    void _replaceChild(Node *parent, Node *oldChild, Node *newChild);

    // Deletes all the nodes from the bst.
    void _deleteAllNodes(Node *tree);

    // Bst root node
    Node *root;

    // Number of nodes in bst
    int _size;

public:

    // Bst forward only iterator.
    // Will traverse the tree using in order traversal.
    class iterator
    {
        friend class Bst <T>;

    private:
        Node *node;
        iterator(Node *node) : node(node) {}
    public:
        iterator& operator++();
        bool operator==(iterator other);
        bool operator!=(iterator other);
        T& operator*();
    };


    // Constructor
    Bst();

    // Copy constructor
    Bst(const Bst &other);

    // Destructor
    ~Bst();

    // Insert a value
    void insert(const T &value);

    // For this project, key will be student or faculty id (int)
    template <typename Key>
    iterator search(Key key) const;

    // Remove a value
    void remove(iterator it);

    // Returns the size of bst
    int size() const { return _size; }

    // Return begin and end iterators
    iterator begin() const;
    iterator end() const;
};


// Constructor
template <typename T>
Bst<T>::Bst()
    : root(NULL)
    , _size(0)
{

}


// Copy Constructor
template <typename T>
Bst<T>::Bst(const Bst &other)
    : root(NULL)
    , _size(0)
{
    iterator it = other.begin();

    // Iterate and insert all the values from other
    while ( it != other.end() ) {
        insert( *it );
        ++it;
    }
}


// Destructor
template <typename T>
Bst<T>::~Bst()
{
    _deleteAllNodes(root);
}


// Insert a value
template <typename T>
void Bst<T>::insert(const T &value)
{
    _insert( &root, NULL, new Node(value) );
    ++_size;
}


// Search for a value
template <typename T>
template <typename Key>
typename Bst<T>::iterator Bst<T>::search(Key key) const
{
    Node *node = root;

    while (node) {

        // Found
        if ( key == node->value )
            return iterator(node);

        // Go left
        if ( key < node->value )
            node = node->lft;

        // Go right
        else
            node = node->rgt;
    }

    // Not Found
    return end();
}


// Insert a value
template <typename T>
void Bst<T>::remove(Bst<T>::iterator it)
{
    _remove(it.node);
    --_size;
}


template <typename T>
typename Bst<T>::iterator Bst<T>::begin() const
{
    // Return iterator pointing to first node
    // in inorder traversal.
    Node *node = root;

    // Set node to the left most node.
    while ( node && node->lft ) {
        node = node->lft;
    }

    return iterator(node);
}


template <typename T>
typename Bst<T>::iterator Bst<T>::end() const
{
    return iterator(NULL);
}


template <typename T>
void Bst<T>::_insert(Node **currentNode, Node* parent, Node *newNode)
{
    // Insert
    if (*currentNode == NULL ) {
        newNode->parent = parent;
        *currentNode = newNode;
        return;
    }

    // Traverse left side.
    // If current node left member is NULL, we need to make new_node
    // as left child of current node.
    if (newNode->value < (*currentNode)->value )
        return _insert(&(*currentNode)->lft, *currentNode, newNode );

    // Traverse right side.
    // If current node right member is NULL, we need to make new_node
    // as right child of current node.
    else
        return _insert(&(*currentNode)->rgt, *currentNode, newNode );
}


// Deletes Node*
template <typename T>
void Bst<T>::_remove(Node *del)
{
    // Case 1. Node has both left and right children
    if ( del->lft && del->rgt ) {

        // We need to find a replacement node "succ"
        // and swap its contents with "del" and delete
        // that replacement node

        // The replacement node will be successor node
        // of "del" ie., the next node in inorder traversal

        Node *succ = del->rgt;

        // Special case, The successor node is child of
        // "del"
        if ( succ->lft == NULL ) {
            // Replace del with succ
            del->value = succ->value;

            // succ does not has any left child.
            // succ is right child of del.
            // So, succ right child will be right child of del.
            del->rgt = succ->rgt;
        }
        else {
            // Find the successor node.
            // Just traverse down the left child.
            // If a node does not have a left child, it is the
            // successor node.
            while ( succ->lft != NULL ) {
                succ = succ->lft;
            }

            // Replace del with succ
            del->value = succ->value;

            // succ does not has a lft child.
            // And succ is left child of its parent.
            // So succ right child will be the left child of
            // succ parent.
            succ->parent->lft = succ->rgt;
        }

        delete succ;
    }

    // Case 2. Node has only one child
    else if ( del->lft || del->rgt ) {

        // Make del's child as child of del's parent

        if ( del->lft )
            _replaceChild( del->parent, del, del->lft );
        else
            _replaceChild( del->parent, del, del->rgt );
        delete del;
    }

    // Case 3. Node has no children
    else {

        // Remove child from parent ie.,
        // set the parent lft or rgt pointer to NULL.
        _replaceChild( del->parent, del, NULL );
        delete del;
    }
}


template <typename T>
void Bst<T>::_replaceChild(Node *parent, Node *oldChild, Node *newChild)
{
    if ( newChild )
        newChild->parent = parent;

    // Parent is null so oldChild is the root element.
    // Set new root to newChild
    if ( parent == NULL ) {
        root = newChild;
    }
    else {

        // oldChild is left child of its parent.
        if ( parent->lft == oldChild )
            parent->lft = newChild;

        // oldChild is right child of its parent.
        else
            parent->rgt = newChild;
    }
}


template <typename T>
void Bst<T>::_deleteAllNodes(Node *tree)
{
    if (tree == NULL )
        return;

    _deleteAllNodes(tree->lft );
    _deleteAllNodes(tree->rgt );

    delete tree;
}


template <typename T>
typename Bst<T>::iterator& Bst<T>::iterator::operator++()
{
    // Set node to next node in inorder traversal.

    // No right child, go back to parent.
    if ( node->rgt == NULL ) {

        int whichChild = -1;

        // Traverse up. Stop the traversal if we come
        // from left child.
        while ( node && whichChild != 0 ) {

            if ( node->parent && node->parent->lft == node )
                whichChild = 0; // left child
            else
                whichChild = 1; // right child

            // Go back to parent
            node = node->parent;
        }
    }

    // Go right and then go to left most node.
    else {
        node = node->rgt;
        while ( node->lft )
            node = node->lft;
    }

    return *this;
}


template <typename T>
bool Bst<T>::iterator::operator==(Bst<T>::iterator other)
{
    return node==other.node;
}


template <typename T>
bool Bst<T>::iterator::operator!=(Bst<T>::iterator other)
{
    return !(*this == other);
}


template <typename T>
T& Bst<T>::iterator::operator*()
{
    return node->value;
}


#endif // BST_H
