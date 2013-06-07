#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <gl\glu.h>

using namespace std;

//tell compiler that we will be using some openGL libraries
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

class MeasureWin
{
#define FRAME_WIN_CLASS		"FRAME_WIN_CLASS"
#define CONTROLS_WIN_CLASS	"CONTROLS_WIN_CLASS"
#define GL_WIN_CLASS		"GL_WIN_CLASS"

	struct Measurment
	{
		int _w_f, _w_i, _l_f, _l_i;
		Measurment() : _w_f(0), _w_i(0), _l_f(0), _l_i(0) {}
		Measurment(int w_f,int w_i,int l_f,int l_i) : 
			_w_f(w_f), _w_i(w_i), _l_f(l_f), _l_i(l_i) {}
		
		void add_width(const Measurment &m)
		{
			_w_f += m._w_f;
			_w_i += m._w_i;

			bool good = false;
			while(!good)
			{
				if(_w_i > 11)
				{
					_w_f++;
					_w_i -= 12;
				}
				else
					good = true;
			}
		}

		bool is_name_length(const Measurment &m)
		{
			return (_l_f == m._l_f) && (_l_i == m._l_i);
		}

		void get_string(char *buff, size_t size)
		{
			string str;
			if(_w_f < 10)
				str.append(" %d'");
			else
				str.append("%d'");

			if(_w_i < 10)
				str.append(" %d\"");
			else
				str.append("%d\"");

			str.append(" X ");

			if(_l_f < 10)
				str.append(" %d'");
			else
				str.append("%d'");

			if(_l_i < 10)
				str.append(" %d\"");
			else
				str.append("%d\"");
			sprintf_s(buff,size,str.c_str(),_w_f,_w_i,_l_f,_l_i);
		}
	};

	HINSTANCE _hInstance;

	HWND _frame_wnd;
	HWND _control_wnd;
	HWND _gl_wnd;

	HDC _hDC;
	HGLRC _hRC;

	float z_dist;

	HWND _width_feet_edit;
	HWND _width_inch_edit;
	HWND _length_feet_edit;
	HWND _length_inch_edit;
	HWND _measurements_edit;
	HWND _standard_edit;
	HWND _needs_edit;

	HWND _add_button;

	vector<Measurment> _measurments;
	vector<Measurment> _standard;
	vector<Measurment> _needs;

	static LRESULT CALLBACK WndProcFrame (HWND frame_wnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		MeasureWin *mw = NULL;

		//set user data, in this case, a pointer to my CubeInterface class
		if(message == WM_CREATE)
		{
			//lParam contains a pointer to a CREATESTRUCT
			//(CREATESTRUCT*)lParam)->lpCreateParams contains a pointer to CubeInterface
			mw = (MeasureWin*)((CREATESTRUCT*)lParam)->lpCreateParams;

			//Actual line that stores my class in the window's user data
			SetWindowLongPtr(frame_wnd,GWLP_USERDATA,(LONG_PTR)mw);

			mw->create_child_windows(frame_wnd);

			return 0;
		}

		//Retrieve my class
		mw = (MeasureWin*)GetWindowLongPtr(frame_wnd,GWLP_USERDATA);

		switch (message)
		{
		case WM_CLOSE:
			PostQuitMessage (0);
			return 0;

		case WM_SIZE:
			mw->size_frame_window();
			return 0;
		}

		return DefWindowProc(frame_wnd, message, wParam, lParam);
	}
	
	static LRESULT CALLBACK WndProcControls (HWND control_wnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		MeasureWin *mw = NULL;

		//set user data, in this case, a pointer to my CubeInterface class
		if(message == WM_CREATE)
		{
			//lParam contains a pointer to a CREATESTRUCT
			//(CREATESTRUCT*)lParam)->lpCreateParams contains a pointer to CubeInterface
			mw = (MeasureWin*)((CREATESTRUCT*)lParam)->lpCreateParams;

			mw->create_controls(control_wnd);

			//Actual line that stores my class in the window's user data
			SetWindowLongPtr(control_wnd,GWLP_USERDATA,(LONG_PTR)mw);
			return 0;
		}

		//Retrieve my class
		mw = (MeasureWin*)GetWindowLongPtr(control_wnd,GWLP_USERDATA);

		switch (message)
		{
		case WM_SIZE:
			mw->resize_control_window();
			return 0;

		case WM_COMMAND:
			switch(HIWORD(wParam))
			{
			case BN_CLICKED:
				mw->process_button_click(lParam);
				break;
			}
			break;
		}

		return DefWindowProc(control_wnd, message, wParam, lParam);
	}

	static LRESULT CALLBACK WndProcGL (HWND gl_wnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		MeasureWin *mw = NULL;

		//set user data, in this case, a pointer to my CubeInterface class
		if(message == WM_CREATE)
		{
			//lParam contains a pointer to a CREATESTRUCT
			//(CREATESTRUCT*)lParam)->lpCreateParams contains a pointer to CubeInterface
			mw = (MeasureWin*)((CREATESTRUCT*)lParam)->lpCreateParams;

			//Actual line that stores my class in the window's user data
			SetWindowLongPtr(gl_wnd,GWLP_USERDATA,(LONG_PTR)mw);
			return 0;
		}

		//Retrieve my class
		mw = (MeasureWin*)GetWindowLongPtr(gl_wnd,GWLP_USERDATA);

		switch (message)
		{
		case WM_SIZE:
			mw->resize_gl_window();
			return 0;

		case WM_LBUTTONDOWN:
			SetFocus(mw->_gl_wnd);
			//ci->gl_select(LOWORD(lParam),HIWORD(lParam));
			break;
		case WM_MOUSEWHEEL:
			mw->proc_wheel(HIWORD(wParam));
			break;
		}

		return DefWindowProc(gl_wnd, message, wParam, lParam);
	}

	void clear_inputs()
	{
		SetWindowText(_width_feet_edit,"");
		SetWindowText(_width_inch_edit,"");
		SetWindowText(_length_feet_edit,"");
		SetWindowText(_length_inch_edit,"");
	}

	void consolidate_needs()
	{
		for (unsigned i = 0; i < _needs.size(); i++)
		{
			for (unsigned j = i+1; j < _needs.size(); j++)
			{
				if(_needs[i].is_name_length(_needs[j]))
				{
					_needs[i].add_width(_needs[j]);
					_needs.erase(_needs.begin()+j);
				}
			}
		}
	}

	void consolidate_standards()
	{
		int val = 0;
		for(unsigned i = 0; i < _needs.size(); i++)
		{
			if(_needs[i]._w_f > 11)
			{
				Measurment *pm = &_needs[i];
				bool good = false;
				while(!good)
				{
					if(pm->_w_f > 11)
					{
						_standard.push_back(Measurment(12,0,pm->_l_f,pm->_l_i));
						pm->_w_f -= 12;
					}
					else if(pm->_w_f == 0)
					{
						if(pm->_w_i == 0)
						{
							_needs.erase(_needs.begin()+i);	
						}
						good = true;
					}
					else
					{
						good = true;
					}
				}
			}
		}
	}

	void create_child_windows(HWND frame_wnd)
	{
		_control_wnd = CreateWindow (CONTROLS_WIN_CLASS, NULL,
			WS_CHILD | WS_VISIBLE,0, 0, 390, 200,
			frame_wnd, NULL, _hInstance, this);

		_gl_wnd = CreateWindow (GL_WIN_CLASS, NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | 
			WS_CLIPCHILDREN,390, 0, 200, 200,
			frame_wnd, NULL, _hInstance, this);

		set_up_gl_context();
	}
	
	void create_controls(HWND control_wnd)
	{
		CreateWindow ("BUTTON", "Input",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,10, 10, 265, 55,
			control_wnd, NULL, _hInstance, NULL);

		_width_feet_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_LEFT | WS_TABSTOP,
                      17, 30, 35, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "'",WS_VISIBLE | WS_CHILD,
                      54, 30, 5, 25,control_wnd,NULL, _hInstance, NULL);

		_width_inch_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_LEFT | WS_TABSTOP,
                      61, 30, 30, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "\"   X",WS_VISIBLE | WS_CHILD,
                      93, 30, 30, 25,control_wnd,NULL, _hInstance, NULL);

		int buff = 110;

		_length_feet_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_LEFT | WS_TABSTOP,
                      17+buff, 30, 35, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "'",WS_VISIBLE | WS_CHILD,
                      54+buff, 30, 5, 25,control_wnd,NULL, _hInstance, NULL);

		_length_inch_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_LEFT | WS_TABSTOP,
                      61+buff, 30, 30, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "\"",WS_VISIBLE | WS_CHILD,
                      93+buff, 30, 8, 25,control_wnd,NULL, _hInstance, NULL);

		_add_button = CreateWindow("BUTTON", "Add",WS_VISIBLE | WS_CHILD | WS_TABSTOP,
                      105+buff, 30, 50, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "Measurments",WS_VISIBLE | WS_CHILD,
                      10, 70, 95, 18,control_wnd,NULL, _hInstance, NULL);

		_measurements_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE | 
					  WS_VSCROLL | ES_LEFT, 10, 90, 120, 265,
					  control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "Standard",WS_VISIBLE | WS_CHILD,
                      135, 70, 65, 18,control_wnd,NULL, _hInstance, NULL);

		_standard_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE | 
					  WS_VSCROLL | ES_LEFT, 135, 90, 120, 265,
					  control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "Needs",WS_VISIBLE | WS_CHILD,
                      260, 70, 50, 18,control_wnd,NULL, _hInstance, NULL);

		_needs_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE | 
					  WS_VSCROLL | ES_LEFT, 260, 90, 120, 265,
					  control_wnd,NULL, _hInstance, NULL);

		HFONT hFont=CreateFont(0,6,0,0,0,0,0,0,0,0,0,0,0,TEXT("Courier New"));
		SendMessage(_measurements_edit,WM_SETFONT,(WPARAM)hFont,0);
		SendMessage(_standard_edit,WM_SETFONT,(WPARAM)hFont,0);
		SendMessage(_needs_edit,WM_SETFONT,(WPARAM)hFont,0);

	}
	
	void draw_gl_scene()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();									// Reset The Current Modelview Matrix
		

		glPushMatrix();

		glTranslatef(0.0f,0.0f,z_dist);

		glBegin(GL_QUADS);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glVertex3f( 1.0f, 1.0f, 0.0f);
			glVertex3f( 1.0f,-1.0f, 0.0f);
			glVertex3f(-1.0f,-1.0f, 0.0f);
		glEnd();

		glPopMatrix();
	

		SwapBuffers(_hDC);
	}

	void init_gl()							// All Setup For OpenGL Goes Here
	{
		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	}

	void process_button_click(LPARAM lParam)
	{
		if((HWND)lParam == _add_button)
		{
			if(validate_input())
			{
				retrieve_measurment();
				clear_inputs();
				split_needs();
				consolidate_needs();
				consolidate_standards();
				update_display();
				SetFocus(_width_feet_edit);
			}
		}
	}

	void proc_wheel(WORD w)
	{
		if(w > WHEEL_DELTA)
		{
			z_dist += 1.0f;
		}
		else
		{
			z_dist -= 1.0f;
		}
	}

	void reg_win_class()
	{
		WNDCLASS wc;
		ZeroMemory(&wc, sizeof (wc));

		wc.style = CS_PARENTDC;
		wc.lpfnWndProc = WndProcFrame;
		wc.hInstance = _hInstance;
		wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor (NULL, IDC_ARROW);
		//wc.hbrBackground = (HBRUSH) COLOR_CAPTIONTEXT;
		wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
		wc.lpszClassName = FRAME_WIN_CLASS;
		RegisterClass (&wc);

		wc.style = CS_CLASSDC;
		wc.lpfnWndProc = WndProcControls;
		//wc.hbrBackground = (HBRUSH) COLOR_CAPTIONTEXT;
		wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
		wc.lpszClassName = CONTROLS_WIN_CLASS;
		RegisterClass (&wc);

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProcGL;
		wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
		//wc.hbrBackground = (HBRUSH) COLOR_CAPTIONTEXT;
		wc.lpszClassName = GL_WIN_CLASS;
		RegisterClass (&wc);

	}
	
	void resize_control_window()
	{
		RECT c_frame_rect, c_control_rect;
		
		GetClientRect(_frame_wnd,&c_frame_rect);
		GetClientRect(_control_wnd,&c_control_rect);

		MoveWindow(_control_wnd,0,0,c_control_rect.right,c_frame_rect.bottom,TRUE);

		int text_bottom = c_frame_rect.bottom - 100;

		//get the values from creation
		MoveWindow(_measurements_edit,10,90,120,text_bottom,TRUE);
		MoveWindow(_standard_edit,135, 90, 120,text_bottom,TRUE);
		MoveWindow(_needs_edit,260, 90, 120,text_bottom,TRUE);
	}
	
	void resize_gl_scene(RECT frame_rect, RECT control_rect)		// Resize And Initialize The GL Window
	{
		GLsizei width = frame_rect.right-control_rect.right;
		GLsizei height = frame_rect.bottom;

		glViewport(0,0,width,height);						// Reset The Current Viewport

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window
		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix
	}

	void resize_gl_window()
	{
		RECT c_frame_rect, c_control_rect, c_gl_rect;
		
		GetClientRect(_frame_wnd,&c_frame_rect);
		GetClientRect(_control_wnd,&c_control_rect);
		GetClientRect(_gl_wnd,&c_gl_rect);

		MoveWindow(_gl_wnd,c_control_rect.right,c_gl_rect.top,c_frame_rect.right-c_control_rect.right,c_frame_rect.bottom,TRUE);
		resize_gl_scene(c_frame_rect,c_control_rect);
	}

	void retrieve_measurment()
	{
		TCHAR buff[10];
		Measurment m;
		GetWindowText(_width_feet_edit,buff,10);
		m._w_f = atoi(buff);

		GetWindowText(_width_inch_edit,buff,10);
		m._w_i = atoi(buff);

		GetWindowText(_length_feet_edit,buff,10);
		m._l_f = atoi(buff);

		GetWindowText(_length_inch_edit,buff,10);
		m._l_i = atoi(buff);
		
		_measurments.push_back(m);
	}
	
	void set_up_gl_context()
	{
		int		PixelFormat;				// Holds The Results After Searching For A Match
		static	PIXELFORMATDESCRIPTOR pfd=	// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
			1,								// Version Number
			PFD_DRAW_TO_WINDOW |			// Format Must Support Window
			PFD_SUPPORT_OPENGL |			// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,				// Must Support Double Buffering
			PFD_TYPE_RGBA,					// Request An RGBA Format
			16,								// Select Our Color Depth
			0, 0, 0, 0, 0, 0,				// Color Bits Ignored
			0,								// No Alpha Buffer
			0,								// Shift Bit Ignored
			0,								// No Accumulation Buffer
			0, 0, 0, 0,						// Accumulation Bits Ignored
			16,								// 16Bit Z-Buffer (Depth Buffer)  
			0,								// No Stencil Buffer
			0,								// No Auxiliary Buffer
			PFD_MAIN_PLANE,					// Main Drawing Layer
			0,								// Reserved
			0, 0, 0							// Layer Masks Ignored
		};

		_hDC = GetDC(_gl_wnd);
		PixelFormat = ChoosePixelFormat(_hDC,&pfd);
		SetPixelFormat(_hDC,PixelFormat,&pfd);
		_hRC=wglCreateContext(_hDC);
		wglMakeCurrent(_hDC,_hRC);

		init_gl();
	}

	void size_frame_window()
	{
		SendMessage(_control_wnd,WM_SIZE,(WPARAM)NULL,(WPARAM)NULL);
		SendMessage(_gl_wnd,WM_SIZE,(WPARAM)NULL,(WPARAM)NULL);
	}

	void split_needs()
	{
		Measurment *pm = &_measurments.back();
		int w_f = pm->_w_f;
		bool good = false;

		while(!good)
		{
			if(w_f > 11)
			{
				_standard.push_back(Measurment(12,0,pm->_l_f,pm->_l_i));
				w_f -= 12;
			}
			else if(w_f > 0)
			{
				_needs.push_back(Measurment(w_f,pm->_w_i,pm->_l_f,pm->_l_i));
				good = true;
			}
			else if(pm->_w_i > 0)
			{
				_needs.push_back(Measurment(w_f,pm->_w_i,pm->_l_f,pm->_l_i));
				good = true;
			}
			else
			{
				good = true;
			}
		}
	}

	void update_display()
	{
		update_measurment_display();
		update_standard_display();
		update_needs_display();
	}
	
	void update_needs_display()
	{
		if(_needs.size() > 0)
		{
			string str;
			char buff[50];
			for(unsigned i = 0; i < _needs.size()-1; i++)
			{
				_needs[i].get_string(buff,50);
				str.append(buff);
				str.append("\r\n");
			}

			_needs.back().get_string(buff,50);
			str.append(buff);

			SetWindowText(_needs_edit,str.c_str());
		}
	}

	void update_measurment_display()
	{
		if(_measurments.size() > 0)
		{
			string str;
			char buff[50];
			for(unsigned i = 0; i < _measurments.size()-1; i++)
			{
				_measurments[i].get_string(buff,50);
				str.append(buff);
				str.append("\r\n");
			}

			_measurments.back().get_string(buff,50);
			str.append(buff);

			SetWindowText(_measurements_edit,str.c_str());
		}
	}

	void update_standard_display()
	{
		if(_standard.size() > 0)
		{
			string str;
			char buff[50];
		
			for(unsigned i = 0; i < _standard.size()-1; i++)
			{
				_standard[i].get_string(buff,50);
				str.append(buff);
				str.append("\r\n");
			}

			_standard.back().get_string(buff,50);
			str.append(buff);

			SetWindowText(_standard_edit,str.c_str());
		}
	}

	bool validate_input()
	{
		TCHAR buff[30];
		int val = 0;

		GetWindowText(_width_feet_edit,buff,30);
		val = atoi (buff);
		if(val == 0)
		{
			sprintf_s(buff,"Invalid feet value: %d",val);
			MessageBox(_frame_wnd,buff,"Warning",MB_OK);
			SetFocus(_width_feet_edit);
			SendMessage(_width_feet_edit, EM_SETSEL, 0, -1);
			return false;
		}

		GetWindowText(_width_inch_edit,buff,30);
		val = atoi (buff);
		if(val > 11)
		{
			sprintf_s(buff,"Invalid inch value: %d",val);
			MessageBox(_frame_wnd,buff,"Warning",MB_OK);
			SetFocus(_width_inch_edit);
			SendMessage(_width_inch_edit, EM_SETSEL, 0, -1);
			return false;
		}

		GetWindowText(_length_feet_edit,buff,30);
		val = atoi (buff);
		if(val == 0)
		{
			sprintf_s(buff,"Invalid feet value: %d",val);
			MessageBox(_frame_wnd,buff,"Warning",MB_OK);
			SetFocus(_length_feet_edit);
			SendMessage(_length_feet_edit, EM_SETSEL, 0, -1);
			return false;
		}

		GetWindowText(_length_inch_edit,buff,30);
		val = atoi (buff);
		if(val > 11)
		{
			sprintf_s(buff,"Invalid inch value: %d",val);
			MessageBox(_frame_wnd,buff,"Warning",MB_OK);
			SetFocus(_length_inch_edit);
			SendMessage(_length_inch_edit, EM_SETSEL, 0, -1);
			return false;
		}

		return true;
	}

public:

	MeasureWin(HINSTANCE hInstance) : _hInstance(hInstance)
	{
		z_dist = -10.0f;
		reg_win_class();
	}

	int msg_loop()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof (msg));

		_frame_wnd = CreateWindow (FRAME_WIN_CLASS, "Measure", 
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,0, 0, 1000, 600,
			NULL, NULL, _hInstance, this);

		size_frame_window();

		SetFocus(_width_feet_edit);

		while(GetMessage(&msg, NULL, 0, 0) > 0)
		{
			//this is for tab key to work
			if(!IsDialogMessage(_control_wnd,&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			draw_gl_scene();
		}

		return (int)msg.message;
	}
};

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	
	MeasureWin mw (hInstance);

	return mw.msg_loop();
}
