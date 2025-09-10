#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include <cmath>

#include "../IVector2/ivector2.hpp"
#include "../random_2D_coordinate_generator/random_2D_coordinate_generator.hpp"
class PerlinNoise
{
private:
    int seedGlobal = 29874789;

public:
    IVector2<float> gradient(const int& i, const int& j)
    {
        long long                   hashSeed = i ^ j ^ seedGlobal;
        Random2DCoordinateGenerator generator(hashSeed);

        long long randVal = generator(i, j);
        float     angle   = static_cast<float>(randVal % 360) * 3.14159f / 180.f;

        return IVector2<float>(cos(angle), sin(angle)); // vecteur unitaire
    }

    float linearInterpolation(float a, float b, float t)
    {
        return a + t * (b - a);
    }

    float sample(const float& x, const float& y)
    {
        IVector2<float> cell(std::floor(x), std::floor(y));

        IVector2<float> localCoord(x, y);
        localCoord -= cell; // vecteur point→coin bas-gauche

        float u = localCoord.x; // position locale dans la cellule
        float v = localCoord.y;

        // Générer les gradients pour les 4 coins
        IVector2<float> g00 = gradient(cell.x, cell.y);
        IVector2<float> g10 = gradient(cell.x + 1, cell.y);
        IVector2<float> g01 = gradient(cell.x, cell.y + 1);
        IVector2<float> g11 = gradient(cell.x + 1, cell.y + 1);

        // Calcul des vecteurs point vers le coin et dot products
        IVector2<float> d00(u, v);
        IVector2<float> d10(u - 1, v);
        IVector2<float> d01(u, v - 1);
        IVector2<float> d11(u - 1, v - 1);

        float dot00 = d00.dot(g00);
        float dot10 = d10.dot(g10);
        float dot01 = d01.dot(g01);
        float dot11 = d11.dot(g11);

        // Partie lissage du bruit pour eviter les blocs
        auto  fade = [](float t) { return t * t * t * (t * (t * 6 - 15) + 10); };
        float fu   = fade(u);
        float fv   = fade(v);

        // Interpolation bilinéaire
        float ix0   = linearInterpolation(dot00, dot10, fu); // bas
        float ix1   = linearInterpolation(dot01, dot11, fu); // haut
        float value = linearInterpolation(ix0, ix1, fv);     // final

        return value;
    }
};

#endif
