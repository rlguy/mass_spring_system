/* CSC 473 Fall 2014
 * Ryan Guy
 */

#include "quaternion.h"
#include <QDebug>
#include <cmath>


Quaternion::Quaternion() {
    x = 0;
    y = 0;
    z = 0;
    w = 1;
}

Quaternion::Quaternion(glm::vec3 v) {
    x = v.x, y = v.y, z = v.z;
    w = 0.0;
}

Quaternion::Quaternion(float nx, float ny, float nz, float rads)
{
    x = nx*sin(0.5*rads);
    y = ny*sin(0.5*rads);
    z = nz*sin(0.5*rads);
    w = cos(0.5*rads);
}

Quaternion Quaternion::mult(Quaternion q2) {
    float s1 = w;
    float s2 = q2.w;
    glm::vec3 v1 = glm::vec3(x, y, z);
    glm::vec3 v2 = glm::vec3(q2.x, q2.y, q2.z);

    float newScalar = s1*s2 - glm::dot(v1, v2);
    glm::vec3 newVect = s1*v2 + s2*v1 + glm::cross(v1, v2);

    Quaternion newQ = Quaternion();
    newQ.set(newVect.x, newVect.y, newVect.z, newScalar);
    return newQ;
}

bool Quaternion::isFloatZero(float f) {
    float eps = 0.00000001;
    return fabs(f) < eps;
}

Quaternion Quaternion::inverse() {
    Quaternion q = Quaternion();
    float lensqr = x*x + y*y + z*z + w*w;
    if (isFloatZero(lensqr)) {
        return q;
    }
    float inv = 1/lensqr;
    q.set(-inv*x, -inv*y, -inv*z, inv*w);
    return q;
}

float Quaternion::length() {
    float sqr = x*x + y*y + z*z + w*w;
    if (sqr < 0) {
        return -1;
    }
    return sqrt(sqr);
}

glm::vec3 Quaternion::rotateVector(glm::vec3 v) {
    Quaternion vq = Quaternion(v);
    Quaternion r = (this->mult(vq)).mult(this->inverse());

    return glm::vec3(r.x, r.y, r.z);
}

glm::mat4 Quaternion::getRotationMatrix() {
    return glm::mat4( 1 - 2*y*y - 2*z*z, 2*x*y - 2*z*w, 2*x*z + 2*y*w, 0.0,
                      2*x*y + 2*z*w, 1 - 2*x*x - 2*z*z, 2*y*z - 2*x*w, 0.0,
                      2*x*z - 2*y*w, 2*y*z + 2*x*w, 1 - 2*x*x - 2*y*y, 0.0,
                      0.0, 0.0, 0.0, 1.0 );
}

void Quaternion::set(float xx, float yy, float zz, float ww) {
    x = xx; y = yy; z = zz; w = ww;
}

void Quaternion::print()
{
    qDebug() << x << y << z << w;
}
