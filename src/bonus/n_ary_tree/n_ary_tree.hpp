#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <functional>

/**
 * @brief Node structure for a binary tree.
 *
 * Each node contains data and pointers to its left and right children.
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
    std::function<TType(std::vector<TType>)>  parentFunct; // used for postorderCompute
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
 * @note Can execute preorder(for display of debug) and postorder traversals to execute all children
 * of the tree.
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

            // On exécute la fonction du parent avec les résultats des enfants

        }
        return node->parentFunct ? node->parentFunct(childResults) : TResult(node->data);
    }
};
#endif