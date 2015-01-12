/* CSC 473 Fall 2014
 * Ryan Guy
 */
#ifndef POINTMASS_H
#define POINTMASS_H
#include <vector>
#include <QtOpenGL>
#include "glm/glm.hpp"
#include "spring.h"

class Spring;

// A point mass is a mass at a position
// This object controls motion of the mass under the force of gravity
// and connected springs
class PointMass
{
public:
    PointMass(float mass, float x, float y, float z);

    void setPosition(float x, float y, float z);
    void setMass(float m);
    void setDistanceScale(float scale);
    void setFixedPosition(bool r);
    void addSpring(Spring *s);

    glm::vec3 calculateForces();  // calculate all forces acting upon the mass
                                  // (gravity and springs
    void update(float dt);
    void draw();

    float mass;     // in kilograms;
    float dscale;   // distance scale in units/metre
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 gravity;
    bool isFixedPosition;    // masses in fixed position do not move

    std::vector<Spring*> springs;  // holds all springs connected to a mass

};

#endif // POINTMASS_H
