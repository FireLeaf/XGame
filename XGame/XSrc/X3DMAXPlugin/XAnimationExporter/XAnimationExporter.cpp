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

#include "XAnimationExporter.h"

#define XAnimationExporter_CLASS_ID	Class_ID(0x98fcf83f, 0x91fac871)

class XAnimationExporter : public SceneExport {
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
		XAnimationExporter();
		~XAnimationExporter();		

};



class XAnimationExporterClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new XAnimationExporter(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return XAnimationExporter_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("XAnimationExporter"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetXAnimationExporterDesc() { 
	static XAnimationExporterClassDesc XAnimationExporterDesc;
	return &XAnimationExporterDesc; 
}





INT_PTR CALLBACK XAnimationExporterOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static XAnimationExporter *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (XAnimationExporter *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- XAnimationExporter -------------------------------------------------------
XAnimationExporter::XAnimationExporter()
{

}

XAnimationExporter::~XAnimationExporter() 
{

}

int XAnimationExporter::ExtCount()
{
	#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR *XAnimationExporter::Ext(int n)
{		
	#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("ax");
}

const TCHAR *XAnimationExporter::LongDesc()
{
	#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("xgame file");
}
	
const TCHAR *XAnimationExporter::ShortDesc() 
{			
	#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("xgame");
}

const TCHAR *XAnimationExporter::AuthorName()
{			
	#pragma message(TODO("Return ASCII Author name"))
	return _T("YangCao");
}

const TCHAR *XAnimationExporter::CopyrightMessage() 
{	
	#pragma message(TODO("Return ASCII Copyright message"))
	return _T("copyright to yc");
}

const TCHAR *XAnimationExporter::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *XAnimationExporter::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int XAnimationExporter::Version()
{				
	#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void XAnimationExporter::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL XAnimationExporter::SupportsOptions(int ext, DWORD options)
{
	#pragma message(TODO("Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports."))
	return TRUE;
}

typedef int	(*pfnDoExport)(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);

int	XAnimationExporter::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	#pragma message(TODO("Implement the actual file Export here and"))
	int iRet = FALSE;
	HMODULE hModule = LoadLibrary("plugins/XAnimationExporterDll.dll");
	if (hModule)
	{
		pfnDoExport func = (pfnDoExport)(GetProcAddress(hModule, "DoExport"));
		if (func)
		{
			iRet = func(name, ei, i, suppressPrompts, options);
		}
		FreeLibrary(hModule);
	}
	
	#pragma message(TODO("return TRUE If the file is exported properly"))
	return iRet;
}


