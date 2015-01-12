/* CSC 473 Fall 2014
 * Ryan Guy
 */

#include "camera3d.h"
#include <QDebug>

camera3d::camera3d() {
}

// Camera is initialized with a position and direction
camera3d::camera3d(float posX, float posY, float posZ,
                   float dirX, float dirY, float dirZ)
{
    position = glm::vec3(posX, posY, posZ);
    direction = glm::normalize(glm::vec3(dirX, dirY, dirZ));

    // find up direction
    glm::vec3 right = glm::cross(direction, glm::vec3(0.0, 1.0, 0.0));
    up = glm::normalize(glm::cross(right, direction));
}

void camera3d::moveForward(float val) {
    position = position + val * direction;
}

void camera3d::moveBackward(float val) {
    position = position - val * direction;
}

void camera3d::moveRight(float val) {
    glm::vec3 right = glm::cross(direction, up);
    position = position + val * right;
}

void camera3d::moveLeft(float val) {
    glm::vec3 left = -glm::cross(direction, up);
    position = position + val * left;
}

void camera3d::moveUp(float val) {
    position = position + val * up;
}

void camera3d::moveDown(float val) {
    position = position - val * up;
}

void camera3d::rotateRight(float rad) {
    direction = glm::rotate(direction, rad, -up);
}

void camera3d::rotateLeft(float rad) {
    direction = glm::rotate(direction, -rad, -up);
}

void camera3d::rotateUp(float rad) {
    glm::vec3 right = glm::cross(direction, up);
    up = glm::rotate(up, rad, right);
    direction = glm::rotate(direction, rad, right);
}

void camera3d::rotateDown(float rad) {
    glm::vec3 right = glm::cross(direction, up);
    up = glm::rotate(up, -rad, right);
    direction = glm::rotate(direction, -rad, right);
}

void camera3d::rollRight(float rad) {
    up = glm::rotate(up, rad, -direction);
}

void camera3d::rollLeft(float rad) {
    up = glm::rotate(up, -rad, -direction);
}









