#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#	include <wx/wx.h>
#endif

#include "main.hpp"

IMPLEMENT_APP(MainApp) // Initializes the MainApp class...

// .. and tells our program to run it
bool MainApp::OnInit()
{
	// Create an instance of our frame, or window
	MainFrame *MainWin = new MainFrame("Hello Dialog", wxPoint(1, 1), wxSize(800, 600));
	MainWin->Show(true); // show the window
	SetTopWindow(MainWin); // and finally, set it as the main window
	return true;
}

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
: wxFrame((wxFrame*) NULL, -1, title, pos, size)
{
    // Creating a box for application controls
    _box = new wxBoxSizer(wxVERTICAL);
    
    _note_book = new wxNotebook(this, NoteBookIt);
    _box->Add(_note_book, 1, wxEXPAND | wxALL, 5);

    wxScrolledWindow* p1 = new wxScrolledWindow(_note_book);
    wxScrolledWindow* p2 = new wxScrolledWindow(_note_book);
    
    _note_book->AddPage(p1, wxT("Page1"), true);
    _note_book->AddPage(p2, wxT("Page2"), true);

    wxPanel* pan = new wxPanel(this);
    wxBoxSizer* level = new wxBoxSizer(wxHORIZONTAL);
    _button = new wxButton(pan, ButtonIt, wxT("My Button"), wxDefaultPosition, wxSize(138, -1));
    level->Add(_button, 1, wxALIGN_CENTER);
  
    pan->SetAutoLayout(true);
    pan->SetSizer(level);
    level->Fit(pan);
    _box->Add(pan, 0, wxEXPAND);
  
    SetAutoLayout(true);
    SetSizer(_box);
}

void MainFrame::ButtonOnClick(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxT("Hello You!"), wxT("Hello"), wxICON_EXCLAMATION);
}

