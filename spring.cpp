/* CSC 473 Fall 2014
 * Ryan Guy
 */

#include "spring.h"

Spring::Spring(float springConstant, float restLength,
               PointMass *mass1, PointMass *mass2)
{
    k = springConstant;
    kd = 0.1;
    rl = restLength;
    m1 = mass1;
    m2 = mass2;
    dscale = 10;
}

void Spring::setSpringConstant(float springConstant) {
    k = springConstant;
}

void Spring::setDampingConstant(float dampingConstant) {
    kd = dampingConstant;
}

void Spring::setRestLength(float restLength) {
    rl = restLength;
}

void Spring::setDistanceScale(float scale) {
    dscale = scale;
}

// calculates force vector acting upon refMass
// Force is calculated according to Hooke's law and includes damping
glm::vec3 Spring::getForce(PointMass *refMass) {
    if (!(refMass == m1 or refMass == m2)) {
        return glm::vec3(0, 0, 0);
    }

    // spring force
    float dist = glm::distance(m1->position, m2->position);
    float scalar = dscale*k*(dist - rl);
    glm::vec3 dir = glm::normalize(m2->position - m1->position);

    // find speed of contraction/expansion for damping force
    float s1 = glm::dot(m1->velocity, dir);
    float s2 = glm::dot(m2->velocity, dir);
    float dampingScalar = -kd * (s1 + s2);

    if (refMass == m1) {
        return (scalar + dampingScalar) * dir;
    } else{
        return (-scalar + dampingScalar) * dir;
    }

}

void Spring::draw() {
    glLineWidth(1.0);
    glColor4f(0.0, 0.3, 1.0, 0.3);

    glBegin(GL_LINES);
    glVertex3f(m1->position.x, m1->position.y, m1->position.z);
    glVertex3f(m2->position.x, m2->position.y, m2->position.z);
    glEnd();

}
