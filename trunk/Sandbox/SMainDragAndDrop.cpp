#include "SMainDragAndDrop.h"
#include "wx/filename.h"
#include "SMeshImportor.h"


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

			SGLOBAL::AssetPanel()->Reload();
		}
	}

	return true;
}

