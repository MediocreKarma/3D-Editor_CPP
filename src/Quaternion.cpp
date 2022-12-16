#include "Quaternion.h"
#include <iostream>
#include <math.h>

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

Quaternion Quaternion::operator*(const Quaternion& q) {
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
    if(norm() != 0) {
        //std::cout<<"norm: "<<norm()<<"\n";
        double value = 1/norm();
        //std::cout<<"value to multiply by: "<<value<<"\n";
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
    vector*=sin(angle/2);
    setReal(cos(angle/2));
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
    abs*=abs;
    abs = 1/abs;
    Quaternion conj = conjugate();

    conj*=abs;
    return conj;
}
void Quaternion::display() {
    //for debugging
    for(size_t i = 0; i<4; i++) {
        std::cout<<m_data[i]<<" ";
    }
    std::cout<<"\n";
}
