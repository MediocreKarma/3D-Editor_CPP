#ifndef QUATERNION_H
#define QUATERNION_H

#include "MyArray.h"

using TVector3 = MyArray<double, 3>;
using TVector4 = MyArray<double, 4>;

class Quaternion
{
    public:
        Quaternion();
        Quaternion(const double& real, const TVector3& complex);
        Quaternion(const TVector4& data);
        Quaternion(const Quaternion& other);
        Quaternion(const double& real, const double& i, const double& j, const double& k);
        //Quaternion(const double& yaw, const double& pitch, const double& roll);
        Quaternion& operator=(const Quaternion& rhs);
        MyArray<double, 3> toEuler() const;
        void display(); //for debugging
        double real() const;
        TVector3 complex() const;
        TVector4 data() const;
        double& operator[](const size_t& index);
        void operator+=(const Quaternion& q);
        void operator-=(const Quaternion& q);
        Quaternion multiply(const Quaternion& q) const;
        void operator*=(const Quaternion& q);
        void operator*=(const double& scalar);
        Quaternion operator*(const Quaternion& q) const;
        void setReal(const double& real);
        void setComplex(const TVector3& complex);
        void setCoord(const size_t& index, const double& value);
        double norm() const;
        void normalize();
        void convertToUnitQ();
        Quaternion conjugate();
        Quaternion inverse();

    protected:

    private:
        TVector4 m_data;
};

#endif // QUATERNION_H
