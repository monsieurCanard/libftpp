#ifndef IVECTOR2_HPP
#define IVECTOR2_HPP

#include <cmath>

// Debattre sur l'utilite du fichier cpp
template <typename TType>
struct IVector2
{
    TType x;
    TType y;

    IVector2() : x(0), y(0) {}
    IVector2(TType x, TType y) : x(x), y(y) {}

    IVector2 operator+(const IVector2& other) const
    {
        return IVector2(x + other.x, y + other.y);
    }

    IVector2 operator-(const IVector2& other) const
    {
        return IVector2(x - other.x, y - other.y);
    }

    IVector2 operator*(const IVector2& other) const
    {
        return IVector2(x * other.x, y * other.y);
    }
    IVector2 operator/(const IVector2& other) const
    {
        if (other.x == 0 || other.y == 0)
            throw std::invalid_argument("Division by zero: denominator has a zero component");

        return IVector2(x / other.x, y / other.y);
    }

    bool operator==(const IVector2& other) const
    {
        return (x == other.x) && (y == other.y);
    }

    bool operator!=(const IVector2& other) const
    {
        return !(*this == other);
    }

    // On retourne une reference pour permettre le chainage
    IVector2& operator+=(const IVector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    IVector2& operator-=(const IVector2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    float length() const
    {
        // C++11 feature, plus stable sur les overflows et underflows
        return std::hypot(x, y);
    }

    IVector2<float> normalize()
    {
        float length = this->length();
        if (length == 0)
            throw std::invalid_argument("Cannot normalized zero vector");
        return IVector2(x / length, y / length);
    }

    // Ceci n'est pas un vrai dot, renvoie juste le scalaire du |v| carre
    float dot()
    {
        return (x * x + y * y);
    }

    float dot(const IVector2& other)
    {
        return (x * other.x + y * other.y);
    }

    // Pseudo Cross, le cross d'un vecteur 2d devrait renvoyer un scalaire du coup j'ai fais une
    // rotation pour renvoyer un vector perpendiculaire (plus utile dans les calculs 2D)
    IVector2 cross()
    {
        return IVector2(-y, x);
    }

    // Scalaire représentant l’aire signée du parallélogramme formé par 2 vectorA et vectorB
    // Signe positif : vectorA est à gauche de vectorB
    // Signe négatif : vectorA est à droite de vectorB
    float cross(const IVector2& other) const
    {
        return (x * other.y - y * other.x);
    }
};
#endif