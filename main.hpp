#pragma once

#include <wx/app.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/notebook.h>

enum {
  NoteBookIt,
  ButtonIt
};

// MainApp is the class for our application, it just acts
// as a container for the window or frame in MainFrame.
class MainApp: public wxApp
{
public:
    virtual bool OnInit();

};

// MainFrame is the class for our window, it
// contains the window and all objects in it.
class MainFrame: public wxFrame
{
public:
	MainFrame( const wxString &title, const wxPoint &pos, const wxSize &size );
	
	void ButtonOnClick( wxCommandEvent& event );
	
	DECLARE_EVENT_TABLE()
	
private:
    wxBoxSizer* _box;
    wxNotebook* _note_book;
    wxButton* _button;
};

BEGIN_EVENT_TABLE ( MainFrame, wxFrame )
	EVT_BUTTON(ButtonIt, MainFrame::ButtonOnClick)
END_EVENT_TABLE()
