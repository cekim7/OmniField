#ifndef VECTOR3_HPP
#define VECTOR3_HPP

struct Vec3 {
    double x, y, z;

    Vec3(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator*(double scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
    Vec3 operator/(double scalar) const { return Vec3(x / scalar, y / scalar, z / scalar); }

    Vec3& operator+=(const Vec3& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }
};

#endif // VECTOR3_HPP
