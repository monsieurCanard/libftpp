// test_nary_tree.cpp
#include <numeric> // pour std::accumulate
#include <vector>

#include "gtest/gtest.h"
#include "libftpp.hpp"

TEST(NAryTreeTest, PreorderTraversal)
{
    NAryTree<int> tree;
    auto          root   = tree.setRoot(1);
    auto          child1 = tree.addChild(root, 2);
    auto          child2 = tree.addChild(root, 3);
    tree.addChild(child1, 4);
    tree.addChild(child1, 5);
    tree.addChild(child2, 6);

    std::vector<int> values;
    tree.preorder(root, values);
    std::vector<int> expected = {1, 2, 4, 5, 3, 6};
    EXPECT_EQ(values, expected);
}

TEST(NAryTreeTest, PostorderTraversal)
{
    NAryTree<int> tree;
    auto          root   = tree.setRoot(1);
    auto          child1 = tree.addChild(root, 2);
    auto          child2 = tree.addChild(root, 3);
    tree.addChild(child1, 4);
    tree.addChild(child1, 5);
    tree.addChild(child2, 6);

    std::vector<int> values;
    tree.postorder(root, values);
    std::vector<int> expected = {4, 5, 2, 6, 3, 1};
    EXPECT_EQ(values, expected);
}

TEST(NAryTreeTest, PostorderComputeSum)
{
    NAryTree<int> tree;
    auto          root = tree.setRoot(1);

    auto c1 = tree.addChild(root, 2);
    auto c2 = tree.addChild(root, 3);

    tree.addChild(c1, 4);
    tree.addChild(c1, 5);
    tree.addChild(c2, 6);

    // Définir une fonction qui additionne les résultats des enfants + la valeur du noeud
    auto sumFunc = [](const std::vector<int>& children)
    { return std::accumulate(children.begin(), children.end(), 0); };

    // On définit sumFunc pour chaque noeud interne
    root->setParentFunct(sumFunc);
    c1->setParentFunct(sumFunc);
    c2->setParentFunct(sumFunc);

    int result = tree.postorderCompute<int>(root);
    // Calcul attendu :
    // c1: 4+5=9
    // c2: 6
    // root: 9+6=15
    EXPECT_EQ(result, 15);
}

TEST(NAryTreeTest, PostorderComputeLeafFallback)
{
    NAryTree<int> tree;
    auto          root = tree.setRoot(10);
    auto          leaf = tree.addChild(root, 5);
    (void)leaf;

    // Seule le noeud root a une fonction
    root->setParentFunct(
        [](const std::vector<int>& children)
        {
            return children[0] * 2; // double la valeur de l'enfant
        });

    int result = tree.postorderCompute<int>(root);
    // leaf n'a pas de fonction donc retourne sa valeur 5, root double => 10
    EXPECT_EQ(result, 10);
}
