#include <SDL3/SDL.h>

#include "../inc/CameraControl.h"

CameraControl::CameraControl() {}

CameraControl::~CameraControl() {}

void CameraControl::OnMouse(unsigned char button, bool bDownOrUpint, float xPos, float yPos) 
{
	if (bDownOrUpint)
	{
		m_currentX = xPos;
		m_currentY = yPos;
	}

	switch (button)
	{
		case SDL_BUTTON_LEFT:
		{
			m_leftMouseDown = bDownOrUpint;
		}
		break;
		case SDL_BUTTON_RIGHT:
		{
			m_rightMouseDown = bDownOrUpint;
		}
		break;
		case SDL_BUTTON_MIDDLE:
		{
			m_middleMouseDown = bDownOrUpint;
		}
		break;
	}
}

void CameraControl::OnCursor(float xPos, float yPos) {}

void CameraControl::OnKey(unsigned key, bool bDownOrUp, bool repeat) 
{
	// 过滤掉长按的情况
	if (repeat) 
	{
		return;
	}

	m_keyMapState[key] = bDownOrUp;
}

void CameraControl::OnScroll(float offsetY) {}

void CameraControl::Update() {}