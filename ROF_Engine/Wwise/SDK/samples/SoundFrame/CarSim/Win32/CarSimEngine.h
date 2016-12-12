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

#include <AK/SoundFrame/SF.h>

#include "Car.h"
#include "CarSimAssignment.h"
#include "CarSimSimulationSerializer.h"
#include "CarParamSerializer.h"
#include "CarSimJoystickMgr.h"

using namespace AK;
using namespace SoundFrame;

class ICarSimUI;

class CarSimEngine : public CWnd,
					 public IClient,
					 public IJoystickAware
{
public:
	CarSimEngine();
	virtual ~CarSimEngine();

	// IClient
	virtual void OnConnect( bool in_bConnect );
	virtual void OnEventNotif( Notif in_eNotif, AkUniqueID in_eventID );
	virtual void OnDialogueEventNotif( Notif in_eNotif, AkUniqueID in_dialogueEventID );
	virtual void OnSoundObjectNotif( Notif in_eNotif, AkUniqueID in_soundObjectID );
	virtual void OnStatesNotif( Notif in_eNotif, AkUniqueID in_stateGroupID );
	virtual void OnSwitchesNotif( Notif in_eNotif, AkUniqueID in_switchGroupID );
	virtual void OnGameObjectsNotif( Notif in_eNotif, AkGameObjectID in_gameObjectID );
	virtual void OnGameParametersNotif( Notif in_eNotif, AkUniqueID in_gameParameterID );
	virtual void OnTriggersNotif( Notif in_eNotif, AkUniqueID in_triggerID );
	virtual void OnArgumentsNotif( Notif in_eNotif, AkUniqueID in_argumentID );
	virtual void OnAuxBusNotif( Notif in_eNotif, AkUniqueID in_AuxBusID );
	virtual void OnSoundBankNotif( Notif in_eNotif, AkUniqueID in_SoundBankID );

	// IJoystickAware
	virtual void OnButtonGearUp();
	virtual void OnButtonGearDown();
	virtual void OnAxisThrottle( const double & in_rValue );
	virtual void OnAxisAuxBusWet( const double & in_rValue );

	// CarSimEngine

	void Init( ICarSimUI* in_pCarmSimUI );
	void Term();

	void ResetToDefault();

	const ISoundFrame* GetSoundFrame() const;
	const Car& GetCar() const;
	Car& GetCar();
	CarSimSimulationSerializer& GetSimulationSerializer();
	CarParamSerializer& GetCarParamSerializer();

	CarSimJoystickMgr& GetJoystickMgr();

	const CString& GetGameObjectName() const;
	void ChangeGameObject( const CString& in_rNewGameObjectName );
	
	const CarSimAssignment& GetAssignement() const;
	void SetAssignement( const CarSimAssignment& in_rAssignment );

	// return true if the AuxBus is used and false if not.
	bool GetAuxBusSettings( double& out_rDryVolume, double& out_rWetVolume ) const;
	void SetAuxBusSettings( bool in_bUseAuxBus, const double& in_rDryVolume, const double& in_rWetVolume );

	void SetFrameInterval( ULONG in_ulInterval );
	ULONG GetFrameInterval() const;

	void Play();
	void Stop();
	void GearUp();
	void GearDown();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:

	void OnFrame();

	void CreateAndUpdateGameObject( bool in_bNewID = false );

	void OnGearChanged();
	void OnSpeedChanged();
	void OnRPMChanged();
	void OnLoadChanged();
	void OnAuxBusChanged();

	ISoundFrame * m_pSoundFrame;
	ICarSimUI* m_pCarmSimUI;

	CarSimSimulationSerializer m_simulationSerializer;
	CarParamSerializer m_carParamSerializer;

	CarSimJoystickMgr m_joystickMgr;

	Car m_car;

	ULONG m_timerInterval; // in ms
	DWORD m_dwLastFrameTime; // in ms

	// Sound Frame Objects
	AkGameObjectID m_gameObjectID;
	CString m_csGameObjectName;

	CarSimAssignment m_assignment;
	
	bool m_bUseAuxBus;
	double m_dblDryVolume;
	double m_dblWetVolume;
};