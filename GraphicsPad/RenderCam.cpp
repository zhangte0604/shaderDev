#include "RenderCam.h"

#include <glm/gtx/transform.hpp>

const float RenderCam::MOVEMENT_SPEED = 0.1f;

RenderCam::RenderCam() :
	viewDirection(0.108157814f, -0.129318893f, -0.769580841f),
	position(0.0f, 0.0f, 10.0f),
	UP(0.0f, 1.0f, 0.0f)
{
}

void RenderCam::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	//Fix camemra jump when the mouse enters the screen
	if (glm::length(mouseDelta) > 50.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
	const float ROTATIONAL_SPEED = 0.5f;
	strafeDirection = glm::cross(viewDirection, UP);

	//Mouse horizontal rotation * Mouse vertical rotation
	glm::mat4 rotator = glm::rotate(-mouseDelta.x * ROTATIONAL_SPEED, UP) *
		glm::rotate(-mouseDelta.y * ROTATIONAL_SPEED, strafeDirection);


	viewDirection = glm::mat3(rotator) * viewDirection;

	oldMousePosition = newMousePosition;
}

glm::vec3 RenderCam::getPosition() const
{
	return RenderCam::position;
}

glm::mat4 RenderCam::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);
}

void RenderCam::moveForward()
{
	position += MOVEMENT_SPEED * viewDirection;
}
void RenderCam::moveBackward()
{
	position += -MOVEMENT_SPEED * viewDirection;
}
void RenderCam::strafeLeft()
{
	position += -MOVEMENT_SPEED * strafeDirection;
}
void RenderCam::strafeRight()
{
	position += MOVEMENT_SPEED * strafeDirection;
}
void RenderCam::moveUp()
{
	position += MOVEMENT_SPEED * UP;
}
void RenderCam::moveDown()
{
	position += -MOVEMENT_SPEED * UP;
}
