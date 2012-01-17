#pragma once

#include "SGlobal.h"
#include "wx/dnd.h"
#include "wx/filename.h"
#include "SMeshImportor.h"
#include "SAssetPanel.h"


class SMainDragAndDrop : public wxFileDropTarget
{
public:
	bool SMainDragAndDrop::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
	{
		size_t nFiles = filenames.GetCount();

		for( UINT i=0; i < filenames.Count(); ++i)
		{
			wxFileName fn = filenames[i];
			if( fn.GetExt() == "mesh" )
			{
				TRAW_MESH rawMesh;
				LoadRawMesh(  fn.GetFullPath().char_str() , rawMesh);

				rawMesh.ChangeCoordsys( COODSYS_DIRECTX );
				ImportRawMesh( &rawMesh , fn.GetName().char_str() );

				GLOBAL::AssetPanel()->Reload();
			}
		}

		return true;
	}
};