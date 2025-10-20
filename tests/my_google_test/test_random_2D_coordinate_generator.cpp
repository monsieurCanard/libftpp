#include <gtest/gtest.h>

#include <unordered_set>

#include "libftpp.hpp"

// Test que la seed est différente à chaque instance (en pratique)
TEST(Random2DCoordinateGeneratorTest, SeedIsGenerated)
{
    Random2DCoordinateGenerator gen1;
    Random2DCoordinateGenerator gen2;

    // Normalement les seeds sont différentes
    EXPECT_NE(gen1.seed(), gen2.seed());
}

// Test que l'opérateur() produit des valeurs différentes pour différentes coordonnées
TEST(Random2DCoordinateGeneratorTest, DifferentCoordinatesProduceDifferentValues)
{
    Random2DCoordinateGenerator gen;

    long long val1 = gen(1, 2);
    long long val2 = gen(2, 1);
    long long val3 = gen(1, 2); // même coordonnées que val1

    EXPECT_NE(val1, val2); // val1 et val2 doivent être différents
    EXPECT_EQ(val1, val3); // même coordonnées → même valeur
}

// Test distribution simple (valeurs uniques pour plusieurs coordonnées)
TEST(Random2DCoordinateGeneratorTest, UniqueValuesForMultipleCoordinates)
{
    Random2DCoordinateGenerator   gen;
    std::unordered_set<long long> values;

    for (long long x = 0; x < 10; ++x)
    {
        for (long long y = 0; y < 10; ++y)
        {
            values.insert(gen(x, y));
        }
    }

    EXPECT_EQ(values.size(), 100); // toutes les valeurs doivent être uniques
}

// Optionnel : tester la reproductibilité si tu modifies le constructeur pour accepter une seed fixe
TEST(Random2DCoordinateGeneratorTest, ReproducibilityWithFixedSeed)
{
    long long                   fixedSeed = 123456789;
    Random2DCoordinateGenerator gen1(fixedSeed);
    Random2DCoordinateGenerator gen2(fixedSeed);

    EXPECT_EQ(gen1(5, 7), gen2(5, 7));
    EXPECT_EQ(gen1(0, 0), gen2(0, 0));
}
