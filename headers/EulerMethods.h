#ifndef EULER_METHODS_H
#define EULER_METHODS_H

#include <string>

class EulerMethod
{
public:
    virtual void solve(const std::string& equation, double x0, double y0, double h, int steps) = 0;
    virtual ~EulerMethod() {}
};

class BasicEuler : public EulerMethod
{
public:
    void solve(const std::string& equation, double x0, double y0, double h, int steps) override;
};

class ModifiedEuler : public EulerMethod
{
public:
    void solve(const std::string& equation, double x0, double y0, double h, int steps) override;
};

#endif // EULER_METHODS_H
