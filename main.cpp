#include<iostream>
#include "DividedDifferenceInterpolator.h"
#include "integration.h"
using namespace std;

 int main()
 {
    /*divide d1;
    d1.askP();
    d1.askX();
    d1.askF();
    d1.askXX();
    d1.diffTable();
    d1.calcP();*/

    NumericalIntegrator integrator;
    integrator.run();
    return 0;
    
    

 }
