/* CSC 473 Fall 2014
 * Ryan Guy
 */

#include "pointmass.h"

PointMass::PointMass(float m, float x, float y, float z)
{
    mass = m;
    dscale = 0.1;
    position = glm::vec3(x, y, z);
    velocity = glm::vec3(0.0, 0.0, 0.0);
    gravity = glm::vec3(0.0, -9.81, 0.0);
    isFixedPosition = false;
}

void PointMass::setPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

// in kilograms
void PointMass::setMass(float m) {
    mass = m;
}

// in units/meter
void PointMass::setDistanceScale(float scale) {
    dscale = scale;
}

// a mass in fixed position does not move
void PointMass::setFixedPosition(bool r) {
    isFixedPosition = r;
}

// connect a spring to the mass
void PointMass::addSpring(Spring *s) {
    springs.push_back(s);
}

// force of gravity and springs
glm::vec3 PointMass::calculateForces() {
    glm::vec3 fg = gravity * mass;
    glm::vec3 fs = glm::vec3(0.0, 0.0, 0.0);

    for (uint i=0; i<springs.size(); i++) {
        fs += springs[i]->getForce(this);
    }

    glm::vec3 force = fg + fs;
    return force;
}

void PointMass::update(float dt) {
    if (isFixedPosition) { return; }

    // F = m*a -> a = F / m
    glm::vec3 acc = calculateForces() / mass;

    // vf = vi + a*dt
    velocity = velocity + acc * dt;

    // pf = pi + v*dt
    position = position + velocity * dscale * dt;
}

void PointMass::draw() {

    glColor4f(0.0, 0.5, 1.0, 1.0);
    glPointSize(2.0);
    glBegin(GL_POINTS);
    glVertex3f(position.x, position.y, position.z);
    glEnd();
}




















