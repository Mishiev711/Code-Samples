#pragma once
#include "../PreCompiled.h"
#include <DirectXMath.h>
#include <bitset>
#include <vector>
#include "Xbox_Controller.h"

using namespace DirectX;

/************************************************
* Filename:      Input.h
* Date:          06/08/2015
* Mod. Date:     09/17/2015
* Mod. Initials: MM
* Author:        Mitchel Mishiev
* Purpose:       To handle and translate keyboard input
* Credit:		 Based off of J. Murphy's input manager 
*				 in the WWise tutorial.
*************************************************/

bool IsRealCursorHidden();

enum eButtonID { UP, DOWN, LEFT, RIGHT, SELECT, WEAPON1, WEAPON2, 
				 WEAPON3, WEAPON4, WEAPON5, SHOOT, PAUSE, CTRL, SPACE, 
				 NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, F1, NUM_BUTTONS
};

class CInput
{
private:
	eButtonID m_eID;
	std::bitset<256> m_unKeys;
	std::vector<unsigned char> m_ActiveKeys;
	bool m_bIsLeftMouseButtonDown;
	bool m_bIsPaused;
	bool m_bIsMenu;
	XMFLOAT2 m_tCurrentMousePosition;
	XMFLOAT2 m_tMouseDelta;
	HWND m_tHwnd;
	int m_nScreenWidth;
	int m_nScreenHeight;
	bool m_bIsCursor;
	int m_nWheelDelta;
	int m_nCurrentWeapon;
	int m_nMaxWeaponCount;

	bool m_bIsControllerEnabled;

	static CInput* m_cInput;

	CInput();
	~CInput();
	CInput(const CInput&) {}
	CInput& operator=(const CInput&) {}

	
	void KeyPressed(const unsigned char _nVkeycode);
	void KeyReleased(const unsigned char _nVkeycode);

	//XBOX Controller Variables
	CXBOXController* m_xboxPlayer;
	bool m_bSentMessage;

	float m_fSensitivity;

public:
	static CInput* GetInstance();
	void Destroy();

	bool HandleWindowMessages(const UINT _uMsg, const WPARAM _wParam, const LPARAM _lParam);

	eButtonID TranslateKeys(const unsigned char _nVkeycode);
	bool IsKeyPressing(const unsigned char _nVkeycode);
	bool IsMouseClick();
	void MouseMovement(LPARAM _lParam);
	void MouseWheelMovement(WPARAM _wParam);
	void KeyReleaseAll();
	void CenteredCursor();

	bool AreCheatKeysHeld(const unsigned int _nCheat);
	void CursorUpdate();

	//XBOX Controller Functions
	bool IsControllerConnected();
	CXBOXController* GetController() { return m_xboxPlayer; };
	void ControllerKeyRelease();

	/**************************
	******** ACCESSORS ********
	***************************/

	XMFLOAT2 GetMouseDelta() { return m_tMouseDelta; }
	XMFLOAT2 GetMousePosition() { return m_tCurrentMousePosition; }
	int GetCurrentWeapon() { return m_nCurrentWeapon; };
	bool GetIsPaused() { return m_bIsPaused; }
	bool GetIsMenu() { return m_bIsMenu; }
	bool GetCursor() { return m_bIsCursor; };
	bool GetControllerEnabled() { return m_bIsControllerEnabled; };

	/**************************
	******** MUTATORS ********
	***************************/
	void SetHWND(HWND _hwnd) { m_tHwnd = _hwnd; }
	void SetIsPaused(bool _IsPaused) { m_bIsPaused = _IsPaused; }
	void SetIsMenu(bool _IsMenu) { m_bIsMenu = _IsMenu; }
	void SetScreenWidth(int _nScreenWidth) { m_nScreenWidth = _nScreenWidth; }
	void SetScreenHeight(int _nScreenHeight) { m_nScreenHeight = _nScreenHeight; }
	void SetSensetivity(float _fSensitivity) { m_fSensitivity = _fSensitivity; }
	void SetCursor(bool _bIsCursor) { m_bIsCursor = _bIsCursor; };
	void SetCurrentWeapon(int _nCurrentWeapon) { m_nCurrentWeapon = _nCurrentWeapon; };
	void SetControllerEnabled(bool _bIsEnabled) { m_bIsControllerEnabled = _bIsEnabled; };
};