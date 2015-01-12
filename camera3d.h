/* CSC 473 Fall 2014
 * Ryan Guy
 */

#ifndef CAMERA3D_H
#define CAMERA3D_H
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"

class camera3d
{
public:
    camera3d();
    camera3d(float posX, float posY, float PosZ,
             float dirX, float dirY, float dirZ);

    void moveForward(float val);    // move along direction vector
    void moveBackward(float val);
    void moveRight(float val);      // move side to side
    void moveLeft(float val);
    void moveUp(float val);         // move along up vector
    void moveDown(float val);
    void rotateRight(float rad);    // rotaties about up vector
    void rotateLeft(float rad);
    void rotateUp(float rad);       // rotate about right/left vector
    void rotateDown(float rad);
    void rollRight(float rad);      // roll about direction vector
    void rollLeft(float rad);

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;          // assume <0,1,0> on initialization
};

#endif // CAMERA3D_H
