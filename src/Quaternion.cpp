#include "Quaternion.h"
#include <iostream>
#include <math.h>

const double pi = 3.1415926535897;

Quaternion::Quaternion() :
    m_data() {
        m_data.fill(0);
        m_data[0] = 1;
    }

Quaternion::Quaternion(const double& real, const TVector3& complex) :
    m_data({real, complex[0], complex[1], complex[2]}){}

Quaternion::Quaternion(const TVector4& data) :
    m_data(data) {}

Quaternion::Quaternion(const Quaternion& other) :
    m_data(other.m_data) {}

Quaternion::Quaternion(const double& real, const double& i, const double& j, const double& k) :
    m_data({real, i, j, k}) {}

Quaternion::Quaternion(const double& heading, const double& attitude, const double& bank) :
    m_data() {
    //disclaimer: nu stiu ce i asta
    double w,x,y,z;
    double c1 = cos(heading/2);
    double s1 = sin(heading/2);
    double c2 = cos(attitude/2);
    double s2 = sin(attitude/2);
    double c3 = cos(bank/2);
    double s3 = sin(bank/2);
    double c1c2 = c1*c2;
    double s1s2 = s1*s2;
    w =c1c2*c3 - s1s2*s3;
  	x =c1c2*s3 + s1s2*c3;
	y =s1*c2*c3 + c1*s2*s3;
	z =c1*s2*c3 - s1*c2*s3;
	m_data[0] = w;
	m_data[1] = x;
	m_data[2] = y;
	m_data[3] = z;
}

double Quaternion::real() const {
    return m_data[0];
}

Quaternion& Quaternion::operator=(const Quaternion& rhs) {
    m_data = rhs.m_data;
    return *this;
}

TVector3 Quaternion::complex() const {
    return TVector3({m_data[1],m_data[2], m_data[3]});
}

TVector4 Quaternion::data() const {
    return m_data;
}

void Quaternion::operator+=(const Quaternion& q) {
    m_data[0] += q.m_data[0];
    m_data[1] += q.m_data[1];
    m_data[2] += q.m_data[2];
    m_data[3] += q.m_data[3];
}

void Quaternion::operator-=(const Quaternion& q) {
    m_data[0] -= q.m_data[0];
    m_data[1] -= q.m_data[1];
    m_data[2] -= q.m_data[2];
    m_data[3] -= q.m_data[3];
}

//TODO: overriding for -, + (if ever necessary)

double& Quaternion::operator[](const size_t& index) {
    return m_data[index];
}

Quaternion Quaternion::multiply(const Quaternion& q) const {
    TVector4 result= TVector4();
    result[0] = m_data[0] * q.m_data[0] - m_data[1] * q.m_data[1] - m_data[2] * q.m_data[2] - m_data[3] * q.m_data[3];
    result[1] = m_data[1] * q.m_data[0] + m_data[0] * q.m_data[1] + m_data[2] * q.m_data[3] - m_data[3] * q.m_data[2];
    result[2] = m_data[0] * q.m_data[2] - m_data[1] * q.m_data[3] + m_data[2] * q.m_data[0] + m_data[3] * q.m_data[1];
    result[3] = m_data[0] * q.m_data[3] + m_data[1] * q.m_data[2] - m_data[2] * q.m_data[1] + m_data[3] * q.m_data[0];
    return Quaternion(result);
}

void Quaternion::operator*=(const Quaternion& q) {
    (*this) = multiply(q);
}

Quaternion Quaternion::operator*(const Quaternion& q) const {
    Quaternion aux((*this));
    aux*=q;
    return aux;
}

void Quaternion::operator*=(const double& scalar) {
    for(size_t i = 0; i < 4; i++) {
        m_data[i] *= scalar;
    }
}

double Quaternion::norm() const {
    double norm = m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2] + m_data[3] * m_data[3];
    norm = sqrt(norm);
    return norm;
}

void Quaternion::normalize() {
    if(fabs(norm()) >= 0.0000000000001) {
        double value = 1/norm();
        (*this)*=value;
    }
}

void Quaternion::setReal(const double& real) {
    m_data[0] = real;
}

void Quaternion::setComplex(const TVector3& complex) {
    m_data[1] = complex[0];
    m_data[2] = complex[1];
    m_data[3] = complex[2];
}

void Quaternion::setCoord(const size_t& index, const double& value) {
    m_data[index] = value;
}

void Quaternion::convertToUnitQ() {
    //realu va reprezenta defapt gradele, in radiani.
    //daca vrei sa stii de ce s unghiurile impartite la 2
    //este pentru ca noi practic facem 2 rotatii consecutive
    //in 4d
    //care rezulta intr o singura rotatie in 3d
    double angle = m_data[0];
    Quaternion vector(0, complex());
    vector.normalize();
    vector *= sin(angle / 2);
    setReal(cos(angle / 2));
    setComplex(vector.complex());
}

Quaternion Quaternion::conjugate() {
    Quaternion aux((*this));
    aux *= -1;
    aux.setReal(m_data[0]);
    return aux;
}

Quaternion Quaternion::inverse() {
    double abs = norm();
    abs *= abs;
    abs = 1 / abs;
    Quaternion conj = conjugate();
    conj *= abs;
    return conj;
}
void Quaternion::display() {
    //for debugging
    for(size_t i = 0; i < 4; i++) {
        std::cout<<m_data[i]<<" ";
    }
    std::cout<<"\n";
}

MyArray<double, 3> Quaternion::toEuler() const {
    //disclaimer: nici asta nu stiu ce e
    double qw = m_data[0], qx = m_data[1], qy = m_data[2], qz = m_data[3];
    double yaw = atan2(2.0*(qy*qz + qw*qx), qw*qw - qx*qx - qy*qy + qz*qz);
    double pitch = asin(-2.0*(qx*qz - qw*qy));
    double roll = atan2(2.0*(qx*qy + qw*qz), qw*qw + qx*qx - qy*qy - qz*qz);
    return MyArray<double, 3>({yaw, pitch, roll});
}
