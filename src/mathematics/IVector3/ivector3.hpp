#ifndef IVECTOR3_HPP
#define IVECTOR3_HPP

#include <cmath>

// Debattre sur l'utilite du fichier cpp
template <typename TType>
struct IVector3
{
    TType x;
    TType y;
    TType z;

    IVector3() : x(0), y(0), z(0) {}
    IVector3(TType x, TType y, TType z) : x(x), y(y), z(z) {}

    IVector3 operator+(const IVector3& other) const
    {
        return IVector3(x + other.x, y + other.y, z + other.z);
    }

    IVector3 operator-(const IVector3& other) const
    {
        return IVector3(x - other.x, y - other.y, z - other.z);
    }

    IVector3 operator*(const IVector3& other) const
    {
        return IVector3(x * other.x, y * other.y, z * other.z);
    }
    IVector3 operator/(const IVector3& other) const
    {
        if (other.x == 0 || other.y == 0 || other.z == 0)
            throw std::invalid_argument("Division by zero: denominator has a zero component");

        return IVector3(x / other.x, y / other.y, z / other.z);
    }

    bool operator==(const IVector3& other) const
    {
        return (x == other.x) && (y == other.y) && (z == other.z);
    }

    bool operator!=(const IVector3& other) const
    {
        return !(*this == other);
    }

    // On retourne une reference pour permettre le chainage
    IVector3& operator+=(const IVector3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    IVector3& operator-=(const IVector3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    // Calcule la longueur (norme) du vecteur
    float length() const
    {
        // C++11 feature, plus stable sur les overflows et underflows
        return sqrt(x * x + y * y + z * z);
    }

    // Ramène le vecteur à longueur 1, tout en conservant sa direction
    IVector3<TType> normalize()
    {
        float length = this->length();
        if (length == 0)
            throw std::invalid_argument("Cannot normalized zero vector");
        return IVector3<TType>(x / length, y / length, z / length);
    }

    // Dot sans argument : renvoie ∣𝑣∣2, utile pour comparer des longueurs sans racine carrée.
    float dot()
    {
        return (x * x + y * y + z * z);
    }

    float dot(const IVector3& other)
    {
        return (x * other.x + y * other.y + z * other.z);
    }

    // Renvoie un vecteur orthogonal aux deux vecteurs
    //     .
    //     Utile pour :
    //     Trouver la normale d’un plan
    //     Calculer des forces
    //     perpendiculaires(comme en physique)
    //     Déterminer l’orientation dans l’espace.
    IVector3 cross(const IVector3& other) const
    {
        return IVector3(
            y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }
};
#endif