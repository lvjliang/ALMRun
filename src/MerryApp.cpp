#include "MerryApp.h"

IMPLEMENT_APP(MerryApp)

bool MerryApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;
	#ifdef __WXMSW__
	wxStandardPaths std; //<wx/stdpaths.h>
	wxFileName fname = wxFileName(std.GetExecutablePath());
	wxString volume;
	wxString pathTmp = fname.GetPathWithSep(); //<wx/filename.h>
	::wxSetEnv(wxT("ALMRUN_HOME"),pathTmp.c_str());
	::wxSetEnv(wxT("ALMRUN_ROOT"),pathTmp.c_str());
	::wxSetEnv(wxT("Desktop"),std.MSWGetShellDir(0));//CSIDL_DESKTOP 
	::wxSetEnv(wxT("Programs"),std.MSWGetShellDir(2));//CSIDL_PROGRAMS 
	::wxSetEnv(wxT("CommonDesktop"),std.MSWGetShellDir(0x19));//CSIDL_COMMON_DESKTOPDIRECTORY 
	::wxSetEnv(wxT("CommonPrograms"),std.MSWGetShellDir(0x17));//COMMON_PROGRAMS
	wxFileName::SplitVolume(pathTmp,&volume,NULL);
	if (!volume.empty())
	{
		volume.Append(':');
		::wxSetEnv(wxT("ALMRUN_DRIVE"),volume.c_str());
	}
	::wxSetWorkingDirectory(pathTmp);
	pathTmp.Clear();
	volume.Clear();
	#endif
	m_frame = NULL;
	this->NewFrame();
	assert(m_frame);
	this->Connect(wxEVT_ACTIVATE_APP,wxObjectEventFunction(&MerryApp::EvtActive));
	return true;
}

int MerryApp::OnExit()
{
	__DEBUG_BEGIN("")
	this->Disconnect(wxEVT_ACTIVATE_APP);
	if (m_frame)
		delete m_frame;
	m_frame = NULL;
	__DEBUG_END("")
	return wxApp::OnExit();
}

void MerryApp::EvtActive(wxActivateEvent &e)
{
	if (!e.GetActive())
		m_frame->Hide();
	else if (g_config && g_config->Changed())
		m_frame->NewConfig();
//	e.Skip();
}

void MerryApp::NewFrame()
{
	if (m_frame)
	{
		bool ok = m_frame->Close();
		assert(ok);
	}
	
	m_frame = new MerryFrame();
	m_frame->OnInit();
}

MerryFrame& MerryApp::GetFrame()
{
	assert(m_frame);
	return *m_frame;
}
