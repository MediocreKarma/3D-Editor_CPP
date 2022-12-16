#ifndef QUATERNION_H
#define QUATERNION_H

#include "MyArray.h"

using TVector3 = MyArray<double, 3>;
using TVector4 = MyArray<double, 4>;

class Quaternion
{
    public:
        Quaternion();
        Quaternion(const TVector3& complex, const double& real);
        Quaternion(const TVector4& data);
        Quaternion(const Quaternion& other);
        Quaternion(const double& real, const double& i, const double& j, const double& k);
    protected:

    private:
        TVector4 m_data;
};

#endif // QUATERNION_H
