#include "Light.h"
#include <glm/gtx/transform.hpp>

const float Light::MOVEMENT_SPEED = 0.1f;

Light::Light() :
	viewDirection(-0.00f, -5.00f, -3.25f),
	position(0.00f, 5.00f, 3.25f),
	UP(0.0f, -1.0f, 0.0f)
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