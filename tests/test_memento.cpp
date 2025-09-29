#include <gtest/gtest.h>

#include <cstring>
#include <string>

#include "libftpp.hpp"

// ========================
// Classe Saveable de test
// ========================
class SaveableDummy : public Memento
{
public:
    SaveableDummy(int a = 0, double b = 0.0, const std::string& s = "") : _a(a), _b(b), _s(s) {}

    int getA() const
    {
        return _a;
    }
    double getB() const
    {
        return _b;
    }
    std::string getS() const
    {
        return _s;
    }

    void set(int a, double b, const std::string& s)
    {
        _a = a;
        _b = b;
        _s = s;
    }

private:
    int         _a;
    double      _b;
    std::string _s;

    void _saveToSnapshot(Memento::Snapshot& snapshot) const override
    {
        snapshot << _a << _b << _s; // Utilisation de l'opérateur
    }

    void _loadFromSnapshot(Memento::Snapshot& snapshot) override
    {
        snapshot >> _a >> _b >> _s;
        snapshot.reset();
        // Note: On remet le curseur a 0 pour eventuellement relire les memes donnees
        //  Sinon on considere que les donnees du snapshot sont consommees
        //  Aussi possible de faire une copie locale du snapshot et de lire dedans
    }
};

TEST(MementoTest, SaveAndLoadRestoresState)
{
    SaveableDummy obj(42, 3.14, "hello");
    auto          snapshot = obj.save();

    SaveableDummy copy;
    copy.load(snapshot);

    EXPECT_EQ(copy.getA(), 42);
    EXPECT_DOUBLE_EQ(copy.getB(), 3.14);
    EXPECT_EQ(copy.getS(), "hello");
}

TEST(MementoTest, LoadChangesState)
{
    SaveableDummy obj(1, 2.0, "abc");
    auto          snapshot = obj.save();

    obj.set(99, 99.9, "zzz");
    obj.load(snapshot);

    EXPECT_EQ(obj.getA(), 1);
    EXPECT_DOUBLE_EQ(obj.getB(), 2.0);
    EXPECT_EQ(obj.getS(), "abc");
}

TEST(MementoTest, SnapshotThrowsOnReadOutOfBounds)
{
    Memento::Snapshot snap;
    int               val = 0;
    // Pas de données dans le snapshot → exception attendue
    EXPECT_THROW(snap >> val, std::out_of_range);
}
