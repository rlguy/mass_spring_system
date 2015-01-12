/* CSC 473 Fall 2014
 * Ryan Guy
 */

#ifndef SPRING_H
#define SPRING_H
#include "glm/glm.hpp"
#include "pointmass.h"

class PointMass;

// A spring modelled after Hooke's law and includes damping
// A spring is connected to PointMass objects on each of it's ends
class Spring
{
public:
    // spring constant in newtons/meter
    // rest length in meters
    Spring(float springConstant, float restLength,
           PointMass *mass1, PointMass *mass2);

    void setSpringConstant(float springConstant);
    void setDampingConstant(float dampingConstant);  // in newtons/(meters/s)
    void setRestLength(float restLength);
    void setDistanceScale(float scale);  // in units/meter
    glm::vec3 getForce(PointMass *refMass);
    void draw();

private:
    float k;         // spring constant
    float kd;        // damping constants
    float rl;
    PointMass *m1;   // masses on spring's ends
    PointMass *m2;
    float dscale;

};

#endif // SPRING_H
