#include "input.hpp"

void Input::update()
{
	m_keysPressedLastFrame = m_keysPressed;
	m_cursorPosLastFrame = m_cursorPos;
}

void Input::setKey(int key, bool pressed)
{
	m_keysPressed[key] = pressed;
}

bool Input::isPushed(int key)
{
	return m_keysPressed[key] && !m_keysPressedLastFrame[key];
}

bool Input::isHold(int key)
{
	return m_keysPressedLastFrame[key];
}

bool Input::isReleased(int key)
{
	return !m_keysPressed[key] && m_keysPressedLastFrame[key];
}

glm::vec2 Input::getCursorPos() const
{
	return m_cursorPos;
}

glm::vec2 Input::getCursorPosLastFrame() const
{
	return m_cursorPosLastFrame;
}

void Input::setCursorPos(glm::vec2 pos)
{
	m_cursorPos = pos;
}