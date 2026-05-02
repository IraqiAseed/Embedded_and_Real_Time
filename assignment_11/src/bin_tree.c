#include "./../include/bin_tree.h"
#include <stdlib.h>
typedef struct Node Node;

struct Node
{
    void *m_data;

    Node *m_left;
    Node *m_right;
    Node *m_father;
};

struct BSTree
{

    Node m_root; // sentinel node
    int (*comparator)(void *_left, void *_right);
};

BSTree *BSTreeCreate(Comparator _less)
{
    if (_less == NULL)
    {
        return NULL;
    }

    BSTree *bst = (BSTree *)malloc(sizeof(BSTree));

    if (bst == NULL)
    {
        return NULL;
    }

    bst->m_root.m_data = NULL;
    bst->m_root.m_left = NULL;  // actual root
    bst->m_root.m_right = NULL; // always NULL

    bst->m_root.m_father = &bst->m_root;
    bst->comparator = _less;
    return bst;
}

static void destroy_subtree(Node *_node, void (*_destroyer)(void *))
{
    if (_node == NULL)
    {
        return;
    }

    destroy_subtree(_node->m_left, _destroyer);
    destroy_subtree(_node->m_right, _destroyer);

    if (_destroyer != NULL)
    {
        _destroyer(_node->m_data);
    }

    free(_node);
}
void BSTreeDestroy(BSTree **_tree, void (*_destroyer)(void *))
{
    if (_tree == NULL || *_tree == NULL)
    {
        return;
    }

    destroy_subtree((*_tree)->m_root.m_left, _destroyer);

    free(*_tree);
    *_tree = NULL;
}

BSTreeItr BSTreeInsert(BSTree *_tree, void *_item)
{
    if (_tree == NULL || _item == NULL)
    {
        return BSTreeItrEnd(_tree);
    }

    // start
    Node *current = _tree->m_root.m_left; // actual root
    Node *parent = &_tree->m_root;        // sentinel node

    while (current != NULL)
    {
        if (_tree->comparator(_item, current->m_data))
        {
            parent = current;
            current = current->m_left;
        }
        else if (_tree->comparator(current->m_data, _item))
        {
            parent = current;
            current = current->m_right;
        }
        else
        {
            return BSTreeItrEnd(_tree); // duplicate
        }
    }

    // insert the new node
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        return BSTreeItrEnd(_tree); // allocation failure
    }
    newNode->m_data = _item;
    newNode->m_left = NULL;
    newNode->m_right = NULL;
    newNode->m_father = parent;

    if (parent == &_tree->m_root) // sentinel node, tree was empty
    {
        _tree->m_root.m_left = newNode;
    }
    else if (_tree->comparator(_item, parent->m_data))
    {
        parent->m_left = newNode;
    }
    else
    {
        parent->m_right = newNode;
    }

    return (BSTreeItr)newNode;
}

BSTreeItr BSTreeInsert_verson2(BSTree *_tree, void *_item)
{
    Node **link;
    Node *parent;
    Node *newNode;

    if (_tree == NULL || _item == NULL)
    {
        return BSTreeItrEnd(_tree);
    }

    link = &_tree->m_root.m_left; /* address of actual-root pointer */
    parent = &_tree->m_root;      /* sentinel */

    while (*link != NULL)
    {
        Node *current = *link;

        if (_tree->comparator(_item, current->m_data))
        {
            parent = current;
            link = &current->m_left;
        }
        else if (_tree->comparator(current->m_data, _item))
        {
            parent = current;
            link = &current->m_right;
        }
        else
        {
            return BSTreeItrEnd(_tree); /* duplicate */
        }
    }

    newNode = malloc(sizeof(Node));
    if (newNode == NULL)
    {
        return BSTreeItrEnd(_tree);
    }

    newNode->m_data = _item;
    newNode->m_left = NULL;
    newNode->m_right = NULL;
    newNode->m_father = parent;

    *link = newNode;

    return (BSTreeItr)newNode;
}

BSTreeItr BSTreeItrBegin(const BSTree *_tree)
{
    if (_tree == NULL || _tree->m_root.m_left == NULL)
    {
        return BSTreeItrEnd(_tree);
    }

    // go to the leftmost node
    Node *current = _tree->m_root.m_left;
    while (current->m_left != NULL)
    {
        current = current->m_left;
    }

    return (BSTreeItr)current;
}

BSTreeItr BSTreeItrEnd(const BSTree *_tree)
{
    if (_tree == NULL)
    {
        return NULL;
    }
    else
    {
        return (BSTreeItr)&_tree->m_root;
    }
}

static Node *FindLeftMost(Node *_node)
{
    if (_node == NULL)
    {
        return NULL;
    }
    while (_node->m_left != NULL)
    {
        _node = _node->m_left;
    }
    return _node;
}

static Node *FindFirstLeftParent(Node *_node)
{
    if (_node == NULL || _node->m_father == NULL)
    {
        return NULL;
    }

    Node *parent = _node->m_father;

    // no need to check parent != NULL because the sentinel node's father is itself
    while (_node == parent->m_right)
    {
        _node = parent;
        parent = parent->m_father;
    }
    return parent;
}
BSTreeItr BSTreeItrNext(BSTreeItr _it)
{
    if (_it == NULL)
    {
        return NULL;
    }

    Node *current = (Node *)_it;

    if (current->m_right == NULL)
    {
        return (BSTreeItr)FindFirstLeftParent(current);
    }
    else
    {
        return (BSTreeItr)FindLeftMost(current->m_right);
    }
}

static Node *FindRightMost(Node *_node)
{
    if (_node == NULL)
    {
        return NULL;
    }
    while (_node->m_right != NULL)
    {
        _node = _node->m_right;
    }
    return _node;
}
static Node *FindFirstRightParent(Node *_node)
{
    if (_node == NULL || _node->m_father == NULL)
    {
        return NULL;
    }

    Node *parent = _node->m_father;

    // no need to check parent != NULL because the sentinel node's father is itself
    while (_node == parent->m_left)
    {
        _node = parent;
        parent = parent->m_father;
    }
    return parent;
}
BSTreeItr BSTreeItrPrev(BSTreeItr _it)
{
    if (_it == NULL)
    {
        return NULL;
    }

    Node *current = (Node *)_it;
    Node *prev = NULL;

    if (current->m_left == NULL)
    {
        prev = FindFirstRightParent(current);
    }
    else
    {
        return (BSTreeItr)FindRightMost(current->m_left);
    }

    if (prev == prev->m_father) // sentinel node (!= NULL)
    {
        return (BSTreeItr)current;
    }
    else
    {
        return (BSTreeItr)prev;
    }
}

static void ReplaceNodeInParent(Node *_node, Node *_child)
{
    Node *parent = _node->m_father;

    if (parent->m_left == _node)
    {
        parent->m_left = _child;
    }
    else
    {
        parent->m_right = _child;
    }

    if (_child != NULL)
    {
        _child->m_father = parent;
    }
}
void *BSTreeItrRemove(BSTreeItr _it)
{
    if (_it == NULL || ((Node *)_it)->m_father == ((Node *)_it)) // end iterator is the sentinel node, which cannot be removed
    {
        return NULL;
    }

    Node *nodeToBeRemoved = (Node *)_it;
    void *data = nodeToBeRemoved->m_data;
    Node *successor;
    Node *child;

    // case 3 - node has two children
    if (nodeToBeRemoved->m_left != NULL && nodeToBeRemoved->m_right != NULL)
    {
        successor = FindLeftMost(nodeToBeRemoved->m_right);

        // nodeToBeRemoved data changes to successor data
        nodeToBeRemoved->m_data = successor->m_data;

        // nodeToBeRemoved updated to successor, which will be removed instead
        nodeToBeRemoved = successor;
    }

    // case 2 - node has one child at most
    if (nodeToBeRemoved->m_left != NULL)
    {
        child = nodeToBeRemoved->m_left;
    }
    else
    {
        child = nodeToBeRemoved->m_right;
        // can be NULL for case 1 (no children)
        // can also be the only child for case 2
    }

    ReplaceNodeInParent(nodeToBeRemoved, child);

    free(nodeToBeRemoved);

    return data;
}

void *BSTreeItrGet(BSTreeItr _it)
{
    if (_it == NULL || ((Node *)_it)->m_father == ((Node *)_it)) // end iterator is the sentinel node, which does not contain valid data
    {
        return NULL;
    }

    Node *node = (Node *)_it;
    return node->m_data;
}

BSTreeItr TraverseInOrder(const BSTree *_tree, ActionFunction _action, void *_context)
{
    if (_tree == NULL || _action == NULL)
    {
        return BSTreeItrEnd(_tree);
    }

    for (BSTreeItr it = BSTreeItrBegin(_tree); it != BSTreeItrEnd(_tree); it = BSTreeItrNext(it))
    {
        if (_action(BSTreeItrGet(it), _context) == 0)
        {
            return it;
        }
    }

    return BSTreeItrEnd(_tree);
}

// recursive implementation of pre-order traversals
static BSTreeItr TraversePreOrderHelper(Node *_node,ActionFunction _action,void *_context)
{
    BSTreeItr result;

    if (_node == NULL)
    {
        return NULL; // no stop happened
    }

    if (_action(_node->m_data, _context) == 0)
    {
        return (BSTreeItr)_node; // stop happened here
    }

    result = TraversePreOrderHelper(_node->m_left, _action, _context);
    
    if (result != NULL) //result != NULL means stop happened in left subtree
    {
        return result;
    }

    result = TraversePreOrderHelper(_node->m_right, _action, _context);
    
    if (result != NULL) //result != NULL means stop happened in right subtree
    {
        return result;
    }

    return NULL; // finished this subtree without stop
}

static BSTreeItr TraversePreOrder(const BSTree *_tree,ActionFunction _action,void *_context)
{
    BSTreeItr result;

    if (_tree == NULL || _action == NULL)
    {
        return BSTreeItrEnd(_tree);
    }

    result = TraversePreOrderHelper(_tree->m_root.m_left, _action, _context);

    if (result == NULL)
    {
        return BSTreeItrEnd(_tree);
    }

    return result;
}
static BSTreeItr TraversePostOrderHelper(Node *_node,ActionFunction _action,void *_context)
{
    BSTreeItr result;

    if (_node == NULL)
    {
        return NULL; // no stop happened
    }

    result = TraversePostOrderHelper(_node->m_left, _action, _context);
    
    if (result != NULL) //result != NULL means stop happened in left subtree
    {
        return result;
    }

    result = TraversePostOrderHelper(_node->m_right, _action, _context);
    
    if (result != NULL) //result != NULL means stop happened in right subtree
    {
        return result;
    }

    if (_action(_node->m_data, _context) == 0)
    {
        return (BSTreeItr)_node; // stop happened here
    }

    return NULL; // finished this subtree without stop
}

static BSTreeItr TraversePostOrder(const BSTree *_tree,ActionFunction _action,void *_context)
{
    BSTreeItr result;

    if (_tree == NULL || _action == NULL)
    {
        return BSTreeItrEnd(_tree);
    }

    result = TraversePostOrderHelper(_tree->m_root.m_left, _action, _context);

    if (result == NULL)
    {
        return BSTreeItrEnd(_tree);
    }

    return result;
}

BSTreeItr BSTreeForEach(const BSTree *_tree, BSTreeTraversalMode _mode, ActionFunction _action, void *_context)
{
    if (_tree == NULL || _action == NULL)
    {
        return BSTreeItrEnd(_tree);
    }

    BSTreeItr itr = BSTreeItrEnd(_tree);

    switch (_mode)
    {
    case BSTREE_TRAVERSAL_PREORDER:
        itr = TraversePreOrder(_tree, _action, _context);
        break;
    case BSTREE_TRAVERSAL_INORDER:
        itr = TraverseInOrder(_tree, _action, _context);
        break;
    case BSTREE_TRAVERSAL_POSTORDER:
        itr = TraversePostOrder(_tree, _action, _context);
        break;
    }

    return itr;
}
