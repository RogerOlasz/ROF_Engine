/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

  Version: v2016.2.0  Build: 5972
  Copyright (c) 2006-2016 Audiokinetic Inc.
*******************************************************************************/

#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <vector>

class IJoystickAware
{
public:
	virtual void OnButtonGearUp() = 0;
	virtual void OnButtonGearDown() = 0;

	// Axis return double value from -1.0 to 1.0. with a neutral value of 0
	virtual void OnAxisThrottle( const double & in_rValue ) = 0;
	virtual void OnAxisAuxBusWet( const double & in_rValue ) = 0;
};

typedef std::vector<DIDEVICEINSTANCE>	DeviceVector;
typedef std::vector<DIDEVICEOBJECTINSTANCE>	DeviceObjectVector;

struct JoystickAssignment
{
	GUID m_joystickProductGuid;

	ULONG m_gearUpButtonIndex;
	ULONG m_gearDownButtonIndex;
	GUID m_throttleAxisGuid;
	GUID m_AuxBusAxisGuid;
};

class CarSimJoystickMgr
{
public:
	CarSimJoystickMgr();
	~CarSimJoystickMgr();

	void Init( HWND in_hMainWnd );
	void Term();

	void OnActivate();

	void Poll( IJoystickAware* in_pJoystickAware );

	bool GetAllJoystick( DeviceVector& out_rDeviceVector );
	bool GetAllJoystickButtons( const DIDEVICEINSTANCE& in_rJoystick, DeviceObjectVector& out_rButtonVector );
	bool GetAllJoystickAxis( const DIDEVICEINSTANCE& in_rJoystick, DeviceObjectVector& out_rAxisVector );

	// Return if the joystick is in use or not.
	bool GetJoystickAssignment( JoystickAssignment& out_rAssignement ) const;
	void SetJoystickAssignement( bool in_bUseJoystick, const JoystickAssignment& in_rAssignement );

private:

	void Load();
	void Save();

	void InitJoystick();
	void ClearJoystick();

	// if in_bAxis is true it return the joystick axis, else it return the buttons
	bool GetAllJoystickObjects( const DIDEVICEINSTANCE& in_rJoystick, DeviceObjectVector& out_rObjectVector, bool in_bAxis );

	static BOOL CALLBACK    FindAndCreateJoystickCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
	static BOOL CALLBACK    InitAxisCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
	
	static BOOL CALLBACK    GetAllJoystickCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
	static BOOL CALLBACK    GetAllObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidInstance, VOID* pContext );

	LPDIRECTINPUT8       m_pDI;         
	LPDIRECTINPUTDEVICE8 m_pJoystick;

	HWND m_hMainWnd;

	bool m_bUseJoytick;
	JoystickAssignment m_assignement;

	bool m_bGearUpPushed;
	bool m_bGearDownPushed;
	bool m_bIsThrottleAxisIdle;
	bool m_bIsAuxBusAxisIdle;
};