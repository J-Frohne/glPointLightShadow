#pragma once

#include <glm/glm.hpp>

#include <map>


// Simple Class to handle keyboard input
class Input
{
public:
	// Stores the key-state of the last frame
	void update();

	// Updates the state of key. Called by the window.
	void setKey(int key, bool pressed);

	// Returns true if key was pushed down in this frame
	// Usage: isPushed(GLFW_KEY_A);
	bool isPushed(int key);

	// Returns true if key is currently pressed
	// Usage: isHold(GLFW_KEY_A);
	bool isHold(int key);

	// Returns true if key was released in this frame
	// Usage: isReleased(GLFW_KEY_A);
	bool isReleased(int key);

	// Returns the current position of the cursor
	glm::vec2 getCursorPos() const;

	// Returns the position the cursor had in the last frame
	glm::vec2 getCursorPosLastFrame() const;

	// Updates the position of the cursor. Called by the window.
	void setCursorPos(glm::vec2 pos);

private:
	std::map<int, bool> m_keysPressed; // stores whether a key is currently pressed
	std::map<int, bool> m_keysPressedLastFrame; // stores whether a key was pressed in the last frame

	glm::vec2 m_cursorPos; // Current position of the cursor
	glm::vec2 m_cursorPosLastFrame; // Position of the cursor in the last frame
};