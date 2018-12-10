#include "Light.h"
#include <glm/gtx/transform.hpp>

const float Light::MOVEMENT_SPEED = 0.1f;

Light::Light() :
	viewDirection(0.108157814f, -0.629318893f, -0.769580841f),
	position(-1.02820563f, 4.88593626f, 3.66996455f),
	UP(0.0f, 1.0f, 0.0f)
{
}

glm::vec3 Light::getPosition() const
{
	return Light::position;
}

glm::mat4 Light::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);
}

void Light::moveForward()
{
	position += MOVEMENT_SPEED * viewDirection;
}
void Light::moveBackward()
{
	position += -MOVEMENT_SPEED * viewDirection;
}
void Light::strafeLeft()
{
	position += -MOVEMENT_SPEED * strafeDirection;
}
void Light::strafeRight()
{
	position += MOVEMENT_SPEED * strafeDirection;
}
void Light::moveUp()
{
	position += MOVEMENT_SPEED * UP;
}
void Light::moveDown()
{
	position += -MOVEMENT_SPEED * UP;
}