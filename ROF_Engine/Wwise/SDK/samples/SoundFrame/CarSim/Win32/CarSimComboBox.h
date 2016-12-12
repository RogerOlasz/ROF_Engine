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

#include "afxwin.h"
#include "resource.h"

template<class TObject, class TList>
class CarSimAssignmentComboBox : public CComboBox
{
public:
	void ClearCombo()
	{
		ResetContent();
	}

	AkUniqueID GetSelectedObject()
	{
		int idx = GetCurSel();

		if( idx == CB_ERR )
			return NULL;

		return (AkUniqueID) GetItemDataPtr( idx );
	}

	void AddObjectAndDisable( AkUniqueID in_Object )
	{
		if( in_Object != AK_INVALID_UNIQUE_ID )
		{
			int idx = AddString( L"Unknown" );
			SetItemDataPtr( idx, (void *) in_Object );
			SetCurSel( idx );
		}

		EnableWindow( FALSE );
	}

	int AddObject( TObject* in_pObject )
	{
		int idx = AddString( in_pObject->GetName() );
		SetItemDataPtr( idx, (void *) in_pObject->GetID() );
		return idx;
	}

	void FillFromList( TList * in_pObjectList, AkUniqueID in_ObjectToSelect )
	{
		ClearCombo();

		CString csNone;
		csNone.LoadString( IDS_ASSIGNMENT_NONE );
		
		AddString( csNone );
		SetItemDataPtr( 0, NULL );

		if( in_pObjectList )
		{
			while ( TObject * pObject = in_pObjectList->Next() )
			{
				AddObject( pObject );
			}
		}

		if( ! SelectObject( in_ObjectToSelect ) )
			SetCurSel( 0 );

		if( in_pObjectList )
			in_pObjectList->Reset();
	}

	bool SelectObject( AkUniqueID in_ObjectID )
	{
		int cObjects = GetCount();
		for ( int i = 0; i < cObjects; i++ )
		{
			AkUniqueID object = (AkUniqueID) GetItemDataPtr( i );
			if ( object == in_ObjectID )
			{
				SetCurSel( i );
				return true;
			}
		}

		return false;
	}

	// Return true if the object was in the list
	bool RemoveObject( AkUniqueID in_ObjectID )
	{
		int cObjects = GetCount();
		int iSel = GetCurSel();
		for ( int i = (cObjects - 1); i >= 0; --i )
		{
			AkUniqueID object = (AkUniqueID) GetItemDataPtr( i );
			if ( object == in_ObjectID )
			{
				DeleteString( i );

				if( iSel == i )
					SetCurSel( 0 );

				return true;
			}
		}

		return false;
	}
};