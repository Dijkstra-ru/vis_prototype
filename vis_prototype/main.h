#ifndef _glpane_
#define _glpane_
 
#include "wx/wx.h"
#include "wx/glcanvas.h"
#include <vector>
#include "interop.h"

class BasicGLPane : public wxGLCanvas
{
    wxGLContext*	m_context;
 
    std::vector<std::vector<finite_el_t>> buf;
    fp_t max_sigma0;
    fp_t min_sigma0;

    fp_t min_x;
    fp_t max_x;

    fp_t min_y;
    fp_t max_y;

    int current_frame;
    int max_frames;

    void paint_buffer();

public:
	BasicGLPane(wxFrame* parent, int* args);
	virtual ~BasicGLPane();
    
	void resized(wxSizeEvent& evt);
    
	int getWidth();
	int getHeight();
    
	void render(wxPaintEvent& evt);
	void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
    
	// events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
    
	DECLARE_EVENT_TABLE()
};
#endif 