#include "wx/wx.h"
#include "wx/sizer.h"
#include "main.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include "wx/filedlg.h"
#include <sstream>
#include <string>
//#include "gl/glew.h"
#include "wx/glcanvas.h"

// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif
 
template<typename T>
void ZeroIt(T* t) {
  memset(t, 0, sizeof(T));
}

nds_t::nds_t()
{
    ZeroIt(this);
}

fs_elem::fs_elem()
{
    ZeroIt(this);
}
 
class MyApp: public wxApp
{
    virtual bool OnInit();   
    wxFrame *frame;
    BasicGLPane * glPane;


public:
    
};


void BasicGLPane::paint_buffer() {
    std::vector<finite_el_t> buff = buf[current_frame];
    for (auto it = buff.cbegin(), end = buff.cend(); it != end; ++it) {
        glBegin(GL_POLYGON);
        {
            finite_el_t el = *it;
            fp_t average = 0;
            for (int i = 0; i < KUZLOV; ++i) average += el.nds[i].sigma0;
            average /= 4;

            auto draw_vertex = [&](size_t i){
                --i;
                //fp_t koeff = 1 - ((el.nds[i].sigma0 - min_sigma0) / (max_sigma0 - min_sigma0));
                fp_t koeff = 1 - ((average - min_sigma0) / (max_sigma0 - min_sigma0));
                glColor3f(1, koeff, koeff);
                glVertex2d(el.el.x[i], el.el.y[i]);
            };
            draw_vertex(1);
            draw_vertex(2);
            draw_vertex(4);
            draw_vertex(3);
        }
        glEnd();
        //std::cout << std::endl;
    }

}
 
IMPLEMENT_APP(MyApp)
 
 
bool MyApp::OnInit()
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Hello GL World"), wxPoint(50,50), wxSize(400,200));
    
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    
    glPane = new BasicGLPane( (wxFrame*) frame, args);
    sizer->Add(glPane, 1, wxEXPAND);
    
    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);
    
    

    frame->Show();
    return true;
} 
 
BEGIN_EVENT_TABLE(BasicGLPane, wxGLCanvas)
EVT_MOTION(BasicGLPane::mouseMoved)
EVT_LEFT_DOWN(BasicGLPane::mouseDown)
EVT_LEFT_UP(BasicGLPane::mouseReleased)
EVT_RIGHT_DOWN(BasicGLPane::rightClick)
EVT_LEAVE_WINDOW(BasicGLPane::mouseLeftWindow)
EVT_SIZE(BasicGLPane::resized)
EVT_KEY_DOWN(BasicGLPane::keyPressed)
EVT_KEY_UP(BasicGLPane::keyReleased)
EVT_MOUSEWHEEL(BasicGLPane::mouseWheelMoved)
EVT_PAINT(BasicGLPane::render)
END_EVENT_TABLE()
 
 
// some useful events to use
void BasicGLPane::mouseMoved(wxMouseEvent& event) {}
void BasicGLPane::mouseDown(wxMouseEvent& event) { }
void BasicGLPane::mouseWheelMoved(wxMouseEvent& event) {}
void BasicGLPane::mouseReleased(wxMouseEvent& event) {
    if (current_frame < max_frames - 1) current_frame++;
    this->Refresh();
}
void BasicGLPane::rightClick(wxMouseEvent& event) {
if (current_frame > 1) current_frame--;
    this->Refresh();

}
void BasicGLPane::mouseLeftWindow(wxMouseEvent& event) {}
void BasicGLPane::keyPressed(wxKeyEvent& event) {}
void BasicGLPane::keyReleased(wxKeyEvent& event) {}
 
BasicGLPane::BasicGLPane(wxFrame* parent, int* args) :
    wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);
    
//    GLenum err = glewInit();
//    if (GLEW_OK != err) exit(2);
  
    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    
    current_frame = 0;
    max_frames = 51;

    /*
    wxString caption = wxT("Choose a file");
    wxString wildcard = wxT("Binary Dump (*.bin)|*.bin");
    wxString defaultDir = wxT("C:\\dev\\1\\run_20121006_20-06-21.996857_5x5_SBCGDIAG");
    wxString defaultFilename = wxEmptyString;

    wxFileDialog dialog(NULL, caption, defaultDir, defaultFilename, wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST );
    dialog.CentreOnParent();
    std::string filename("");
    if(dialog.ShowModal() == wxID_OK) {     
        auto a = dialog.GetPath();
        filename.append(a.c_str());        
    }
    else exit(1);
    */

    #define MINMAXDECL(ARG) max_##ARG = -DBL_MAX; min_##ARG = DBL_MAX;
    MINMAXDECL(sigma0); 
    MINMAXDECL(x);
    MINMAXDECL(y);
    

    const char* filename_start("C:\\dev\\1\\run_20121006_20-06-21.996857_5x5_SBCGDIAG\\00"); 
    const char* filename_end(".00.dump.bin");
    for (int i = 0; i < max_frames; ++i) {   

    std::stringstream filename;
    filename << filename_start;
    if (i < 10) filename << 0;
    filename << i;
    filename << filename_end;

    std::string act_filename(filename.str());

    std::vector<finite_el_t> buff;

    std::fstream ifs(act_filename, std::ios_base::in | std::ios_base::binary);
    //std::cout << filename << std::endl;
    u64 filesize;
    ifs.read(reinterpret_cast<char*>(&filesize),sizeof(u64));
    std::cout << "Read " << std::hex << filesize << std::endl;
    ifs.seekg(filesize, std::ios_base::beg);
    u64 element_number;
    ifs.read(reinterpret_cast<char*>(&element_number),sizeof(u64));
    std::cout << "Read " << std::hex << element_number << std::endl;
    buff.reserve(element_number);
    ifs.seekg(sizeof(u64), std::ios_base::beg);

#define FINDMINMAX(ARG, PATH) if (PATH > max_##ARG) max_##ARG = PATH; if (PATH < min_##ARG) min_##ARG = PATH;

    for (size_t i = 0; i < element_number; ++i) {
        finite_el_t el;
        ifs.read(reinterpret_cast<char*>(&el),sizeof(finite_el_t));
        for (size_t i = 0; i < KUZLOV; ++i) {
            FINDMINMAX(sigma0, el.nds[i].sigma0)
            FINDMINMAX(x, el.el.x[i]);
            FINDMINMAX(y, el.el.y[i]);
        }
        buff.push_back(el);
    }
    ifs.close();
    buf.push_back(buff);
    }
}
 
BasicGLPane::~BasicGLPane()
{
    delete m_context;
}
 
void BasicGLPane::resized(wxSizeEvent& evt)
{
//	wxGLCanvas::OnSize(evt);
    
    Refresh();
}
 
 
void BasicGLPane::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    
  //  glEnable(GL_COLOR_MATERIAL);
  //  glEnable(GL_BLEND);
  //  glDisable(GL_DEPTH_TEST);
  //  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //gluOrtho2D(topleft_x, bottomrigth_x, bottomrigth_y, topleft_y);
    glOrtho(min_x - 5, max_x + 5, min_y - 5, max_y + 5,1,-1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
 
int BasicGLPane::getWidth() { return GetSize().x; }
int BasicGLPane::getHeight() { return GetSize().y; }
 
 
void BasicGLPane::render( wxPaintEvent& evt )
{
    if(!IsShown()) return;
    
    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    prepare2DViewport(0,0,getWidth(), getHeight());
    glLoadIdentity();
    
    if (!buf.empty()) paint_buffer();


    glFlush();
    SwapBuffers();
}