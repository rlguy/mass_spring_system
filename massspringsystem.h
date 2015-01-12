/* CSC 473 Fall 2014
 * Ryan Guy
 */
#ifndef MASSSPRINGSYSTEM_H
#define MASSSPRINGSYSTEM_H

#include <vector>
#include "pointmass.h"
#include "spring.h"

// An interface to control a mass spring simulation
// Use this object to add point masses and connect springs
// between them
class MassSpringSystem
{
public:
    MassSpringSystem();
    ~MassSpringSystem();
    PointMass* addMass(float mass, float x, float y, float z);
    Spring* addSpring(float springConstant, float restLength,
                      PointMass *mass1, PointMass *mass2);

    void setDistanceScale(float scale);  // scale in units/meter

    void update(float dt);
    void draw();

private:
    std::vector<PointMass*> masses;
    std::vector<Spring*> springs;
    float dscale;  // distance scale

};

#endif // MASSSPRINGSYSTEM_H
