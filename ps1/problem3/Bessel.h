#ifndef BESSEL_H
#define BESSEL_H

#include <cmath>
#include <iostream>

class Bessel { 

private:

  void   fSetR() { fR = std::sqrt((float)fX*(float)fX+(float)fY*(float)fY+(float)fZ*(float)fZ); }
    
public:

  three_vector<T>() {}

  three_vector<T>(const T x, const T y, const T z)
  {
    fX = x;
    fY = y;
    fZ = z;
    fSetR();
  }
    
  virtual ~three_vector<T>() {}

  void SetXYZ(const T x, const T y, const T z)
  {
    fX = x;
    fY = y;
    fZ = z;
    fSetR();
  }
    
  void Scale(const T& factor)
  {
    fX *= factor;
    fY *= factor;
    fZ *= factor;
  }

  T Dot(const three_vector<T>& rhs)
  {
    return fX*rhs.X() + fY*rhs.Y() + fZ*rhs.Z();
  }
    
  T     X()    const;
  T     Y()    const;
  T     Z()    const;
  float R()    const;
  float dRdx() const;
  float dRdy() const;
  float dRdz() const;

  three_vector<T> operator+(const three_vector<T>& rhs) const;
  three_vector<T> operator*(const three_vector<T>& rhs) const;
  three_vector<T> operator-(const three_vector<T>& rhs) const;

  friend std::ostream& operator<<(std::ostream& os, const three_vector<T>& v)
  {
    os << "(" << v.fX << ", " << v.fY << ", " << v.fZ << ")";
    return os;
  }
    
};



template <class T> inline std::three_vector<T> std::three_vector<T>::operator+(const std::three_vector<T>& rhs) const
{
  return three_vector(fX+rhs.fX,fY+rhs.fY,fZ+rhs.fZ);
}

template <class T> inline std::three_vector<T> std::three_vector<T>::operator-(const std::three_vector<T>& rhs) const
{
  return three_vector(fX-rhs.fX,fY-rhs.fY,fZ-rhs.fZ);
}

template <class T> inline std::three_vector<T> std::three_vector<T>::operator*(const std::three_vector<T>& rhs) const
{
  return three_vector(fY*rhs.fZ - fZ*rhs.fY, fZ*rhs.fX - fX*rhs.fZ, fX*rhs.fY - fY*rhs.fX);
}

template <class T> inline T     std::three_vector<T>::X()    const { return fX;    }
template <class T> inline T     std::three_vector<T>::Y()    const { return fY;    }
template <class T> inline T     std::three_vector<T>::Z()    const { return fZ;    }
template <class T> inline float std::three_vector<T>::R()    const { return fR;    }
template <class T> inline float std::three_vector<T>::dRdx() const { return fX/fR; }
template <class T> inline float std::three_vector<T>::dRdy() const { return fY/fR; }
template <class T> inline float std::three_vector<T>::dRdz() const { return fZ/fR; }

#endif
