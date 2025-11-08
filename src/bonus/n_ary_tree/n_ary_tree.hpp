#ifndef N_ARY_TREE_HPP
#define N_ARY_TREE_HPP

#include <functional>

/**
 * @brief Node structure for an n-ary tree.
 *
 * Each node contains data and pointers to its children.
 * @note You can change Node struct like you want for your implementation. (like add token, parent
 * pointer, etc.)
 *
 */
template <typename TType>
class Node
{
public:
    TType                                     data;
    std::vector<std::unique_ptr<Node<TType>>> _children;
    std::function<TType(std::vector<TType>)>  parentFunct;
    Node(TType value) : data(value) {}

    void addChild(const TType& child)
    {
        _children.push_back(std::make_unique<Node<TType>>(child));
    }

    Node<TType>* setParentFunct(std::function<TType(std::vector<TType>)> func)
    {
        parentFunct = func;
        return this;
    }
};

/**
 * @brief N ary Tree class.
 * @note Can execute preorder and postorder traversals to execute all children
 * of the tree.
 * @code
 *
 * Simple Usage:
 *
 * NAryTree<int> tree;
 * auto          root   = tree.setRoot(1);
 * auto          child1 = tree.addChild(root, 2);
 * auto          child2 = tree.addChild(root, 3);
 * tree.addChild(child1, 4);
 * tree.addChild(child1, 5);
 * tree.addChild(child2, 6);
 *
 * Preorder Traversal:
 *
 * std::vector<int> values;
 * tree.preorder(root, values);
 * std::vector<int> expected = {1, 2, 4, 5, 3, 6};
 *
 * Postorder Traversal:
 *
 * std::vector<int> postorderValues = tree.postorderValues();
 * std::vector<int> postorderExpected = {4, 5, 2, 6, 3, 1};
 *
 * Postorder Compute with custom function:
 *
 * NAryTree<int> tree;
 *  auto          root = tree.setRoot(1);
 *  auto c1 = tree.addChild(root, 2);
 *  auto c2 = tree.addChild(root, 3);
 *  tree.addChild(c1, 4);
 *  tree.addChild(c1, 5);
 *  tree.addChild(c2, 6);
 *
 * // Définir une fonction qui additionne les résultats des enfants + la valeur du noeud
 *  auto sumFunc = [](const std::vector<int>& children)
 *  { return std::accumulate(children.begin(), children.end(), 0); };
 *
 * // On définit sumFunc pour chaque noeud interne
 *  root->setParentFunct(sumFunc);
 *  c1->setParentFunct(sumFunc);
 *  c2->setParentFunct(sumFunc);
 *  int result = tree.postorderCompute<int>(root);
 *
 * // Calcul attendu :
 *  // c1: 4+5=9
 *  // c2: 6
 *  // root: 9+6=15
 *  EXPECT_EQ(result, 15);
 * @endcode
 */
template <typename TType>
class NAryTree
{
private:
    std::unique_ptr<Node<TType>> _root;

public:
    NAryTree() : _root(nullptr) {}
    Node<TType>* setRoot(TType value)
    {
        if (_root == nullptr)
            _root = std::make_unique<Node<TType>>(value);
        return _root.get();
    }

    Node<TType>* getRoot() const
    {
        return _root.get();
    }

    Node<TType>* addChild(Node<TType>* parent, const TType& value)
    {
        if (!parent)
            return nullptr;
        parent->_children.push_back(std::make_unique<Node<TType>>(value));
        return parent->_children.back().get();
    }

    Node<TType>* addChildToRoot(const TType& value)
    {
        return addChild(_root.get(), value);
    }

    /**
     * @brief Traverse the tree in preorder (parent → children).
     * @param funct Function applied to each node's value
     */
    void preorder(std::function<void(TType)>& funct)
    {
        if (_root != nullptr)
            preorder(_root.get(), funct);
    }

    void preorder(Node<TType>* node, std::function<void(TType)>& funct)
    {
        if (node != nullptr)
        {
            funct(node->data);
            for (const auto& child : node->_children)
            {
                preorder(child.get(), funct);
            }
        }
    }

    std::vector<TType> preorderValues()
    {
        std::vector<TType> result;
        preorder(_root.get(), result);
        return result;
    }

    void preorder(Node<TType>* node, std::vector<TType>& result)
    {
        if (!node)
            return;
        result.push_back(node->data);
        for (const auto& child : node->_children)
            preorder(child.get(), result);
    }

    /**
     * @brief Traverse the tree in postorder (children → parent).
     * @param funct Function applied to each node's value
     */
    void postorder(std::function<void(TType)>& funct)
    {
        if (_root != nullptr)
            postorder(_root, funct);
    }

    void postorder(Node<TType>* node, std::function<void(TType)>& funct)
    {
        if (node != nullptr)
        {
            for (const auto& child : node->_children)
            {
                postorder(child.get(), funct);
            }
            funct(node->data);
        }
    }

    std::vector<TType> postorderValues()
    {
        std::vector<TType> result;
        postorder(_root.get(), result);
        return result;
    }

    void postorder(Node<TType>* node, std::vector<TType>& result)
    {
        if (!node)
            return;

        for (const auto& child : node->_children)
            postorder(child.get(), result);

        result.push_back(node->data);
    }

    template <typename TResult>
    TResult postorderCompute(Node<TType>* node)
    {
        if (!node)
            return TResult();

        std::vector<TResult> childResults;

        if (node->parentFunct)
        {
            for (const auto& child : node->_children)
                childResults.push_back(postorderCompute<TResult>(child.get()));
        }
        return node->parentFunct ? node->parentFunct(childResults) : TResult(node->data);
    }
};
#endif