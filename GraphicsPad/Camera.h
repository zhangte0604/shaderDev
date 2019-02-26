#pragma once
#include <glm/glm.hpp>

class Camera
{
	
public:
	Camera();

	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 UP;
	glm::vec2 oldMousePosition;
	static const float MOVEMENT_SPEED;
	glm::vec3 strafeDirection;

	void mouseUpdate(const glm::vec2& newMousePosition);
	glm::mat4 getWorldToViewMatrix() const;

	glm::vec3 getPosition() const;

	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();
	void moveUp();
	void moveDown();
};

