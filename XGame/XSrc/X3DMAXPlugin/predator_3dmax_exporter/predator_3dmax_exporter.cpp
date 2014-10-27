//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "predator_3dmax_exporter.h"

#define predator_3dmax_exporter_CLASS_ID	Class_ID(0xb231533e, 0x9532c4a3)

class predator_3dmax_exporter : public SceneExport {
	public:
		
		static HWND hParams;
		
		int				ExtCount();					// Number of extensions supported
		const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
		const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const TCHAR *	AuthorName();				// ASCII Author name
		const TCHAR *	CopyrightMessage();			// ASCII Copyright message
		const TCHAR *	OtherMessage1();			// Other message #1
		const TCHAR *	OtherMessage2();			// Other message #2
		unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

		BOOL SupportsOptions(int ext, DWORD options);
		int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

		//Constructor/Destructor
		predator_3dmax_exporter();
		~predator_3dmax_exporter();		

};



class predator_3dmax_exporterClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new predator_3dmax_exporter(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return predator_3dmax_exporter_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("predator_3dmax_exporter"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* Getpredator_3dmax_exporterDesc() { 
	static predator_3dmax_exporterClassDesc predator_3dmax_exporterDesc;
	return &predator_3dmax_exporterDesc; 
}





INT_PTR CALLBACK predator_3dmax_exporterOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static predator_3dmax_exporter *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (predator_3dmax_exporter *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- predator_3dmax_exporter -------------------------------------------------------
predator_3dmax_exporter::predator_3dmax_exporter()
{

}

predator_3dmax_exporter::~predator_3dmax_exporter() 
{

}

int predator_3dmax_exporter::ExtCount()
{
	#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR *predator_3dmax_exporter::Ext(int n)
{		
	#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("psm");
}

const TCHAR *predator_3dmax_exporter::LongDesc()
{
	#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("predator 1.0 skin model file");
}
	
const TCHAR *predator_3dmax_exporter::ShortDesc() 
{			
	#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("predator skin model");
}

const TCHAR *predator_3dmax_exporter::AuthorName()
{			
	#pragma message(TODO("Return ASCII Author name"))
	return _T("summon");
}

const TCHAR *predator_3dmax_exporter::CopyrightMessage() 
{	
	#pragma message(TODO("Return ASCII Copyright message"))
	return _T("copyright ---> summon");
}

const TCHAR *predator_3dmax_exporter::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *predator_3dmax_exporter::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int predator_3dmax_exporter::Version()
{				
	#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void predator_3dmax_exporter::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL predator_3dmax_exporter::SupportsOptions(int ext, DWORD options)
{
	#pragma message(TODO("Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports."))
	return TRUE;
}

typedef int (*pfnDoExport)(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);
int	predator_3dmax_exporter::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	#pragma message(TODO("Implement the actual file Export here and"))
	
	HMODULE hMoudle = ::LoadLibrary("predator_exporter.dll");
	if (!hMoudle)
	{
		MessageBox(NULL, "º”‘ÿpredator_exporter.dll ß∞‹", "..", MB_ICONERROR | MB_OK);
		return FALSE;
	}
	pfnDoExport export_func = (pfnDoExport)::GetProcAddress(hMoudle, "predator_do_export");
	if (export_func)
	{
		return export_func(name, ei, i, suppressPrompts, options);
	}
	
	#pragma message(TODO("return TRUE If the file is exported properly"))
	return FALSE;
}


