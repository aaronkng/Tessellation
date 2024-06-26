#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Info about camera view matrix: https://www.3dgep.com/understanding-the-view-matrix/

enum Camera_Movement
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	FOREWARD,
	BACKWARD,
	LOOKLEFT,
	LOOKRIGHT,
	LOOKUP,
	LOOKDOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float LOOKSPEED = 50.0f;

class Camera
{
public:
	// Camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler angles
	// - Yaw: Camera angle moves left/right
	// - Pitch: Camera angle moves up/down
	float Yaw;
	float Pitch;

	// Camera options
	float MovementSpeed;

	// Constructor w/ vectors 
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}

	// Constructor w/ scalar vectors
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FOREWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == UP)
			Position += WorldUp * velocity;
		if (direction == DOWN)
			Position -= WorldUp * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
		if (direction == LOOKLEFT)
			Yaw -= LOOKSPEED * velocity;
		if (direction == LOOKRIGHT)
			Yaw += LOOKSPEED * velocity;
		if (direction == LOOKUP)
			Pitch += LOOKSPEED * velocity;
		if (direction == LOOKDOWN)
			Pitch -= LOOKSPEED * velocity;

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;

		UpdateCameraVectors();
	}

private:
	// Calculate the front vector from the Camera's (updated) Euler angles
	void UpdateCameraVectors()
	{
		// Calculate the new Front Vector
		// - Front vector is what camera points up to
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		// Also, recalculate the right and up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif
