#include "Input.h"
#include <DirectXMath.h>
#include "../PreCompiled.h"
#include <windowsx.h>


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

CInput* CInput::m_cInput = nullptr;

/********************************************
* CInput(): Setup the variables of the Input class. [ Constructor ]
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 09/10/2015
* Mod. Initials: MM
********************************************/
CInput::CInput()
{
	m_eID = NUM_BUTTONS;
	m_tCurrentMousePosition = XMFLOAT2(0, 0);
	m_nWheelDelta = 0;
	m_bIsLeftMouseButtonDown = false;
	m_bIsPaused = false;
	m_bIsMenu = false;
	m_bIsCursor = false;

	m_nMaxWeaponCount = 5;
	m_nCurrentWeapon = 0;

	m_xboxPlayer = new CXBOXController(1);

	if (m_xboxPlayer->IsConnected())
		m_bIsControllerEnabled = true;
}

/********************************************
* CInput(): Deletes all memory allocated variables of the Input class. [ Destructor ]
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 09/17/2015
* Mod. Initials: MM
********************************************/
CInput::~CInput()
{
	if (m_xboxPlayer != nullptr)
		delete m_xboxPlayer;
}
/********************************************
* GetInstance(): Checks to see if there's an instance of the class already.
*				 If not, Create a new instance of the class and return it.
*
* Ins: None
*
* Outs: None
*
* Returns: CInput*
*
* Mod. Date 06/10/2015
* Mod. Initials: MM
********************************************/
CInput* CInput::GetInstance()
{
	if (m_cInput == nullptr)
		m_cInput = new CInput();
	return m_cInput;
	
}
/********************************************
* Destroy: Deletes the pointer to Input
*
*
* Ins: None
*
* Outs: None
*
* Returns: CInput*
*
* Mod. Date 06/10/2015
* Mod. Initials: Richard Lizana
********************************************/
void CInput::Destroy()
{
	delete m_cInput;
}
/********************************************
* TranslateKeys(): Uses the value of the integer passed in
*				   and converts it into a button ID and returns it.
* 
* Ins: _nVkeycode
* 
* Outs: None
*
* Returns: eButtonID
*
* Mod. Date 06/17/2015
* Mod. Initials: MM
********************************************/
eButtonID CInput::TranslateKeys(const unsigned char _nVkeycode)
{
	switch (_nVkeycode)
	{
	case 'A':
	case VK_LEFT:
		{
			return LEFT;
			break;
		}
	case 'D':
	case VK_RIGHT:
		{
			return RIGHT;
			break;
		}
	case 'W':
	case VK_UP:
		{
			return UP;
			break;
		}
	case 'S':
	case VK_DOWN:
		{
			return DOWN;
			break;
		}
	case 0x31:
		{
			return WEAPON1;
			break;
		}
	case 0x32:
		{
			return WEAPON2;
			break;
		}
	case 0x33:
		{
			return WEAPON3;
			break;
		}
	case 0x34:
		{
			return WEAPON4;
			break;
		}
	case 0x35:
		{
			return WEAPON5;
			break;
		}
	case VK_RETURN:
		{
			return SELECT;
			break;
		}
	case VK_LBUTTON:
		{
			return SHOOT;
			break;
		}
	case VK_ESCAPE:
		{
			return PAUSE;
			break;
		}
	case VK_CONTROL:
		{
			return CTRL;
			break;
		}
	case VK_SPACE:
		{
			return SPACE;
			break;
		}
	case VK_NUMPAD0:
		{
			return NUM0;
			break;
		}
	case VK_NUMPAD1:
		{
			return NUM1;
			break;
		}
	case VK_NUMPAD2:
		{
			return NUM2;
			break;
		}
	case VK_NUMPAD3:
		{
			return NUM3;
			break;
		}
	case VK_NUMPAD4:
		{
			return NUM4;
			break;
		}
	case VK_NUMPAD5:
		{
			return NUM5;
			break;
		}
	case VK_NUMPAD6:
		{
			return NUM6;
			break;
		}
	case VK_NUMPAD7:
		{
			return NUM7;
			break;
		}
	case VK_NUMPAD8:
		{
			return NUM8;
			break;
		}
	case VK_F1:
		{
			return F1;
			break;
		}
	default:
		{
			return NUM_BUTTONS;
			break;
		}
		
	}
	
}

/********************************************
* HandleWindowMessages: Uses the message variable that is passed in and
*			            checks through selected Windows Messages and executes the case
*			            that matches the message passed in
*
* Ins: _uMsg, _wParam, _lParam
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 09/09/2015
* Mod. Initials: MM
********************************************/
bool CInput::HandleWindowMessages(const UINT _uMsg, const WPARAM _wParam, const LPARAM _lParam)
{
	//If there is a controller connected, return out of this function
	if (m_bIsControllerEnabled/* && m_xboxPlayer->IsConnected()*/)
	{
		m_bSentMessage = false;
		return true;
	}
	else
	{
		m_bIsControllerEnabled = false;
	}

	switch (_uMsg)
	{
		case WM_KEYDOWN:
		{
			KeyPressed(unsigned char(_wParam & 0xFF));
			break;
		}
		case WM_KEYUP:
		{
			KeyReleased(unsigned char(_wParam & 0xFF));
			break;
		}
		case WM_MOUSEMOVE:
		{
			MouseMovement(_lParam);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			MouseWheelMovement(_wParam);

			if (m_nWheelDelta > 0)
			{
				m_nCurrentWeapon--;

				if (m_nCurrentWeapon == -1)
				{
					m_nCurrentWeapon = 4;
				}
			}
			else
			{
				m_nCurrentWeapon++;

				if (m_nCurrentWeapon == m_nMaxWeaponCount)
				{
					m_nCurrentWeapon = 0;
				}
			}

			break;
		}
		case WM_LBUTTONDOWN:
		{
			KeyPressed(unsigned char(VK_LBUTTON));
			break;
		}
		case WM_LBUTTONUP:
		{
			KeyReleased(unsigned char(VK_LBUTTON));
			break;
		}
		default:
		{
			return true;
			break;
		}
	}

	return false;
}

/********************************************
* KeyPressed(): Checks to see WM_KEYDOWN is true, if so
*			  Get the ButtonID of the char passed in and
*			  set the bitset to true.
*			  Make sure the ButtonID is not NUM_BUTTONS
*			  Also Make sure to Store the Keycode in the Activate key list
*
* Ins: _nVkeycode
*
* Outs: None
*
* Returns: none
*
* Mod. Date 06/17/2015
* Mod. Initials: MM
********************************************/
void CInput::KeyPressed(const unsigned char _nVkeycode)
{
	m_eID = TranslateKeys(_nVkeycode);

	if (m_eID != NUM_BUTTONS && m_eID != SHOOT)
	{
		m_ActiveKeys.push_back( _nVkeycode );
		m_unKeys.set(_nVkeycode, true);
	}
	else if (m_eID == SHOOT)
	{
		m_bIsLeftMouseButtonDown = true;
	}
}

/********************************************
* KeyReleased(): Get the ButtonID of the Last button pressed and
*			     set the bitset to false.
*			     Make sure the ButtonID is not NUM_BUTTONS
*			  Also Make sure to Remove the Keycode in the Activate key list
*
* Ins: _nVkeycode
*
* Outs: None
*
* Returns: None
*
* Mod. Date 06/17/2015
* Mod. Initials: MM
********************************************/
void CInput::KeyReleased(const unsigned char _nVkeycode)
{
	m_eID = TranslateKeys(_nVkeycode);

	if (m_eID != NUM_BUTTONS)
	{
		if (m_eID != SHOOT)
		{
			m_unKeys.set(_nVkeycode, false);
		}
		else
		{
			m_bIsLeftMouseButtonDown = false;
		}

		for (unsigned int i = 0; i < m_ActiveKeys.size(); i++)
		{
			if (m_ActiveKeys[i] == _nVkeycode)
			{
				m_ActiveKeys.erase(m_ActiveKeys.begin() + i);
				return;
			}
		}
	}
}

/********************************************
* IsKeyPressing(): Get the ButtonID of the integer passed in and
*				   return the results of bitset.at
*			       Make sure the ButtonID is not NUM_BUTTONS
*
* Ins: _nVkeycode
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 06/10/2015
* Mod. Initials: MM
********************************************/
bool CInput::IsKeyPressing(const unsigned char _nVkeycode)
{
	for (unsigned int i = 0; i < m_ActiveKeys.size(); i++)
	{
		if (m_ActiveKeys[i] == _nVkeycode)
		{
			m_eID = TranslateKeys(_nVkeycode);

			if (m_eID != NUM_BUTTONS)
			{
				return m_unKeys.at(_nVkeycode);
			}
			else
				return false;
		}
	}

	return false;
}
/********************************************
* AreCheatKeysHeld(): Checks to see if ctrl + numpad number is held down.
*					  If it is, return true, else return false.
*
* Ins: _nCheat
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 08/24/2015
* Mod. Initials: BA
********************************************/
bool CInput::AreCheatKeysHeld(const unsigned int _nCheat)
{
	bool ctrlHeld = false;
	bool numKeyHeld = false;

	unsigned int vkeycode = _nCheat + VK_NUMPAD0;

	for (unsigned int i = 0; i < m_ActiveKeys.size(); i++)
	{
		if (m_ActiveKeys[i] == vkeycode)
		{
			m_eID = TranslateKeys(vkeycode);

			if (m_eID != NUM_BUTTONS)
			{
				numKeyHeld = m_unKeys.at(vkeycode);
			}
		}
		else if (m_ActiveKeys[i] == VK_CONTROL)
		{
			m_eID = TranslateKeys(VK_CONTROL);

			if (m_eID != NUM_BUTTONS)
			{
				ctrlHeld = m_unKeys.at(VK_CONTROL);
			}
		}
	}

	return ctrlHeld && numKeyHeld;
}

/********************************************
* KeyReleaseAll(): Loops through all keys in the bitset AND the
*			       Activate Key List and sets them to false/Remove them.
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 06/17/2015
* Mod. Initials: MM
********************************************/
void CInput::KeyReleaseAll()
{
	for (unsigned int i = 0; i < m_unKeys.count(); i++)
	{
		m_unKeys.set( i, false);
	}

	for (unsigned int i = 0; i < m_ActiveKeys.size(); i++)
	{
		KeyReleased(m_ActiveKeys[i]);
	}

	m_bIsLeftMouseButtonDown = false;
	m_bIsPaused = false;
}

/********************************************
* IsMouseClick(): Check to see if LeftMouseButton is Down, if so
*				  return true.
*
* Ins: None
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 06/10/2015
* Mod. Initials: MM
********************************************/
bool CInput::IsMouseClick()
{
	if (m_bIsLeftMouseButtonDown)
	{
		return true;
	}
	else
		return false;
}

/********************************************
* MouseMovement(): Gets the current position of the mouse and
*				   returns it.
*
* Ins: _tMousePoint
*
* Outs: None
*
* Returns: None
*
* Mod. Date 06/18/2015
* Mod. Initials: MM
********************************************/
void CInput::MouseMovement(LPARAM _lParam)
{
	m_tCurrentMousePosition.x = (float)GET_X_LPARAM(_lParam);
	m_tCurrentMousePosition.y = (float)GET_Y_LPARAM(_lParam);

	XMFLOAT2 tScreenSize = XMFLOAT2(((float)m_nScreenWidth) / 2, ((float)m_nScreenHeight) / 2);

	XMVECTOR tCurrentMousePos = XMLoadFloat2(&m_tCurrentMousePosition);
	XMVECTOR tScreen = XMLoadFloat2(&tScreenSize);

	DirectX::XMStoreFloat2(&m_tMouseDelta, (tCurrentMousePos - tScreen) * m_fSensitivity);
}

/********************************************
* MouseWheelMovement(): Gets the current delta of the wheel and
*				        returns it.
*
* Ins: _tMousePoint
*
* Outs: None
*
* Returns: None
*
* Mod. Date 09/09/2015
* Mod. Initials: MM
********************************************/
void CInput::MouseWheelMovement(WPARAM _wParam)
{
	m_nWheelDelta = GET_WHEEL_DELTA_WPARAM(_wParam);
}

/********************************************
* CenteredCursor(): Updates the cursor position of the mouse.
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 09/09/2015
* Mod. Initials: MM
********************************************/
void CInput::CenteredCursor()
{
	RECT tRect;

	if (GetClientRect(m_tHwnd, &tRect))
	{
		if (!m_bIsPaused && !m_bIsMenu)
		{
			POINT tPoint;
			tPoint.x = (m_nScreenWidth) / 2;
			tPoint.y = (m_nScreenHeight) / 2;

			ClientToScreen(m_tHwnd, &tPoint);
			SetCursorPos(tPoint.x, tPoint.y);
		}
		/*else
		{
			float oldX = static_cast<float> (m_tMouseDelta.x);
			float oldY = static_cast<float> (m_tMouseDelta.y);

			float x = static_cast<float>(m_tMouseDelta.x);
			float y = static_cast<float>(m_tMouseDelta.y);

			float newX = oldX + (x - oldX) * m_fSensitivity;
			float newY = oldY + (y - oldY) * m_fSensitivity;

			SetCursorPos(int(newX), int(newY));

			oldX = newX;
			oldY = newY;
		}*/
	}
}

/********************************************
* IsControllerConnected(): Check to see if XboxController is Connected, if so
*						   return true.
*
* Ins: None
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 09/10/2015
* Mod. Initials: MM
********************************************/
bool CInput::IsControllerConnected()
{
	if (m_xboxPlayer->IsConnected())
	{
		return true;
	}
	
	return false;
}

/********************************************
* ControllerKeyRelease(): Releases all keys currently active
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 09/11/2015
* Mod. Initials: MM
********************************************/
void ControllerKeyRelease()
{

}

/********************************************
* IsRealCursorHidden(): Hides the cursor.
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/24/2015
* Mod. Initials: MM
********************************************/
void CInput::CursorUpdate()
{
	if (m_bIsCursor)
		while (ShowCursor(TRUE) < 0);
	else
		while (ShowCursor(FALSE) >= 0);
}

