#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <gl\glu.h>
#include <algorithm>

using namespace std;

//tell compiler that we will be using some openGL libraries
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

class MeasureWin
{
#define FRAME_WIN_CLASS					"FRAME_WIN_CLASS"
#define CONTROLS_WIN_CLASS				"CONTROLS_WIN_CLASS"
#define GL_WIN_CLASS					"GL_WIN_CLASS"
#define MEASURMENT_DIPLAY_WIN_CLASS		"MEASURMENT_DIPLAY_WIN_CLASS"
#define TOP_INPUT_WIN_CLASS				"TOP_INPUT_WIN_CLASS"
#define LOWER_CONTROL_WIN_CLASS			"LOWER_CONTROL_WIN_CLASS"

	struct Measurment
	{
		int _w_f, _w_i, _l_f, _l_i;
		float _x, _y;
		bool _print_text, _is_clicked;
		Measurment() : _w_f(0), _w_i(0), _l_f(0), _l_i(0), _x(0.0f), _y(0.0f), _print_text(false),_is_clicked(false) {}
		Measurment(int w_f,int w_i,int l_f,int l_i) : 
			_w_f(w_f), _w_i(w_i), _l_f(l_f), _l_i(l_i), _x(0.0f), _y(0.0f), _print_text(false),_is_clicked(false) {}
		
		void add_width(const Measurment &m)
		{
			_w_f += m._w_f;
			_w_i += m._w_i;
			normalize_width();	
		}
		
		void draw(int name)
		{
			float w_fraction = width_fraction();
			float l_fraction = length_fraction();
			glLoadName(name);
			glPushMatrix();

				if(_is_clicked)
					glTranslatef(0,0,.001f);



				glColor3f(0.0f,0.0f,1.0f);
				glBegin(GL_QUADS);
					glVertex3f(_x, _y*-1, 0);
					glVertex3f(_x+w_fraction, _y*-1, 0);
					glVertex3f(_x+w_fraction, (l_fraction+_y)*-1, 0);
					glVertex3f(_x, (l_fraction+_y)*-1, 0);
				glEnd();

				glColor3f(0.0f,0.0f,0.0f);
				glBegin(GL_LINE_LOOP);
					glVertex3f(_x, _y*-1, 0);
					glVertex3f(_x+w_fraction, _y*-1, 0);
					glVertex3f(_x+w_fraction, (l_fraction+_y)*-1, 0);
					glVertex3f(_x, (l_fraction+_y)*-1, 0);
				glEnd();
			glPopMatrix();
		}

		float get_length_plus_y()
		{
			return _y + length_fraction();
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
		
		bool is_same_length(const Measurment &m)
		{
			return (_l_f == m._l_f) && (_l_i == m._l_i);
		}

		float length_fraction() const
		{
			float len = (float)_l_f;
			switch(_l_i)
			{
			case 1: len += 0.07f; break;
			case 2: len += 0.17f; break;
			case 3: len += 0.25f; break;
			case 4: len += 0.33f; break;
			case 5: len += 0.42f; break;
			case 6: len += 0.50f; break;
			case 7: len += 0.58f; break;
			case 8: len += 0.67f; break;
			case 9: len += 0.75f; break;
			case 10: len += 0.83f; break;
			case 11: len += 0.92f; break;
			}
			return len;
		}

		bool less_or_equal_then(const Measurment &m) const
		{
			int mywidth = (_w_f*12) + _w_i;
			int comparewidth = (m._w_f*12) + m._w_i;

			return mywidth <= comparewidth;
		}
		
		void normalize_length()
		{
			bool good = false;
			while(!good)
			{
				if(_l_i > 11)
				{
					_l_f++;
					_l_i -= 12;
				}
				else
					good = true;
			}
		}
		
		void normalize_width()
		{
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

		void subtract_width(const Measurment &m)
		{
			int myinchwidth = (_w_f*12) + _w_i;
			int compareinchwidth = (m._w_f*12) + m._w_i;

			myinchwidth -= compareinchwidth;

			_w_f = 0;

			bool good = false;

			while(!good)
			{
				if(myinchwidth > 11)
				{
					_w_f++;
					myinchwidth -= 12;
				}
				else
				{
					_w_i = myinchwidth;
					good = true;
				}
			}
		}

		float width_fraction() const
		{
			float wid = (float)_w_f;
			switch(_w_i)
			{
			case 1: wid += 0.07f; break;
			case 2: wid += 0.17f; break;
			case 3: wid += 0.25f; break;
			case 4: wid += 0.33f; break;
			case 5: wid += 0.42f; break;
			case 6: wid += 0.50f; break;
			case 7: wid += 0.58f; break;
			case 8: wid += 0.67f; break;
			case 9: wid += 0.75f; break;
			case 10: wid += 0.83f; break;
			case 11: wid += 0.92f; break;
			}
			return wid;
		}	
	};

	HINSTANCE _hInstance;

	HWND _frame_wnd;
	HWND _control_wnd;
	HWND _measure_display_wnd;
	HWND _gl_wnd;
	HWND _topInput_wnd;
	HWND _lower_control_wnd;

	HDC _hDC;
	HGLRC _hRC;

	GLuint	_base;	

	float x_dist;
	float y_dist;
	float z_dist;

	WORD last_xPos;
	WORD last_yPos;

	int motion_part;

	HWND _width_feet_edit;
	HWND _width_inch_edit;
	HWND _length_feet_edit;
	HWND _length_inch_edit;
	HWND _measurements_edit;
	HWND _standard_edit;
	HWND _needs_edit;

	HWND _add_button;
	HWND _invert_button;
	HWND _clear_button;

	vector<Measurment> _measurments;
	vector<Measurment> _standard;
	vector<Measurment> _needs;
	vector<Measurment> _accounted;

	bool _done_with_input;

	
	static bool sort_function(const Measurment &m1, const Measurment &m2)
	{
		return m1.length_fraction() > m2.length_fraction();
	}

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

		case WM_ERASEBKGND:
			return TRUE;

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
			mw->size_control_window();
			return 0;

		case WM_ERASEBKGND:
			return TRUE;

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
	
	static LRESULT CALLBACK WndProcLowerControl (HWND low_wnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		MeasureWin *mw = NULL;

		//set user data, in this case, a pointer to my CubeInterface class
		if(message == WM_CREATE)
		{
			//lParam contains a pointer to a CREATESTRUCT
			//(CREATESTRUCT*)lParam)->lpCreateParams contains a pointer to CubeInterface
			mw = (MeasureWin*)((CREATESTRUCT*)lParam)->lpCreateParams;

			mw->create_lower_control(low_wnd);

			//Actual line that stores my class in the window's user data
			SetWindowLongPtr(low_wnd,GWLP_USERDATA,(LONG_PTR)mw);
			return 0;
		}

		//Retrieve my class
		mw = (MeasureWin*)GetWindowLongPtr(low_wnd,GWLP_USERDATA);

		switch (message)
		{
		case WM_SIZE:
			mw->size_lower_control_window();
			return 0;

		case WM_ERASEBKGND:
			return TRUE;

		case WM_COMMAND:
			switch(HIWORD(wParam))
			{
			case BN_CLICKED:
				mw->process_button_click(lParam);
				break;
			}
			break;
		}

		return DefWindowProc(low_wnd, message, wParam, lParam);
	}
	
	static LRESULT CALLBACK WndProcTopInput (HWND top_wnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		MeasureWin *mw = NULL;

		//set user data, in this case, a pointer to my CubeInterface class
		if(message == WM_CREATE)
		{
			//lParam contains a pointer to a CREATESTRUCT
			//(CREATESTRUCT*)lParam)->lpCreateParams contains a pointer to CubeInterface
			mw = (MeasureWin*)((CREATESTRUCT*)lParam)->lpCreateParams;

			mw->create_top_inputs(top_wnd);

			//Actual line that stores my class in the window's user data
			SetWindowLongPtr(top_wnd,GWLP_USERDATA,(LONG_PTR)mw);
			return 0;
		}

		//Retrieve my class
		mw = (MeasureWin*)GetWindowLongPtr(top_wnd,GWLP_USERDATA);

		switch (message)
		{
		case WM_SIZE:

			return 0;

		case WM_ERASEBKGND:
			return TRUE;

		case WM_COMMAND:
			switch(HIWORD(wParam))
			{
			case BN_CLICKED:
				mw->process_button_click(lParam);
				break;
			}
			break;
		}

		return DefWindowProc(top_wnd, message, wParam, lParam);
	}

	static LRESULT CALLBACK WndProcMeasurmentDisplay (HWND meas_wnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		MeasureWin *mw = NULL;

		//set user data, in this case, a pointer to my CubeInterface class
		if(message == WM_CREATE)
		{
			//lParam contains a pointer to a CREATESTRUCT
			//(CREATESTRUCT*)lParam)->lpCreateParams contains a pointer to CubeInterface
			mw = (MeasureWin*)((CREATESTRUCT*)lParam)->lpCreateParams;

			mw->create_MeasurmentDisplay(meas_wnd);

			//Actual line that stores my class in the window's user data
			SetWindowLongPtr(meas_wnd,GWLP_USERDATA,(LONG_PTR)mw);
			return 0;
		}

		//Retrieve my class
		mw = (MeasureWin*)GetWindowLongPtr(meas_wnd,GWLP_USERDATA);

		switch (message)
		{
		case WM_SIZE:
			mw->size_MeasurmentDisplay_window();
			return 0;
		case WM_ERASEBKGND:
			return TRUE;
		}

		return DefWindowProc(meas_wnd, message, wParam, lParam);
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
			mw->size_gl_window();
			return 0;
		case WM_LBUTTONDOWN:
			SetFocus(mw->_gl_wnd);
			mw->gl_select(LOWORD(lParam),HIWORD(lParam),true);
			break;
		case WM_LBUTTONUP:
			mw->process_left_mouse_up();
			break;
		case WM_RBUTTONDOWN:
			SetFocus(mw->_gl_wnd);
			break;
		case WM_MOUSEMOVE:
			mw->process_mouse_motion(wParam,lParam);
			break;
		case WM_MOUSEWHEEL:
			mw->process_wheel(HIWORD(wParam));
			break;
		case WM_KEYDOWN:
			mw->process_keydown(wParam);
			break;

		case WM_ERASEBKGND:
			return TRUE;
		}

		return DefWindowProc(gl_wnd, message, wParam, lParam);
	}

	void build_font()								// Build Our Bitmap Font
	{
		HFONT	font;										// Windows Font ID
		HFONT	oldfont;									// Used For Good House Keeping

		_base = glGenLists(96);								// Storage For 96 Characters

		font = CreateFont(	-20,							// Height Of Font
							0,								// Width Of Font
							0,								// Angle Of Escapement
							0,								// Orientation Angle
							FW_BOLD,						// Font Weight
							FALSE,							// Italic
							FALSE,							// Underline
							FALSE,							// Strikeout
							ANSI_CHARSET,					// Character Set Identifier
							OUT_TT_PRECIS,					// Output Precision
							CLIP_DEFAULT_PRECIS,			// Clipping Precision
							ANTIALIASED_QUALITY,			// Output Quality
							FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
							"Courier New");					// Font Name

		oldfont = (HFONT)SelectObject(_hDC, font);           // Selects The Font We Want
		wglUseFontBitmaps(_hDC, 32, 96, _base);				// Builds 96 Characters Starting At Character 32
		SelectObject(_hDC, oldfont);							// Selects The Font We Want
		DeleteObject(font);									// Delete The Font
	}

	void calculate_totals()
	{
		Measurment remaining;
		remaining._w_f = 12;

		vector<Measurment> ordered_needs(_needs);
		_accounted.clear();

		sort(ordered_needs.begin(), ordered_needs.end(),sort_function);

		bool good = false;

		while(!good)
		{
			for(unsigned i = 0; i < ordered_needs.size(); i++)
			{
				if(ordered_needs[i].less_or_equal_then(remaining))
				{
					ordered_needs[i]._x = remaining._x;
					ordered_needs[i]._y = remaining._y;

					remaining.subtract_width(ordered_needs[i]);

					remaining._x += ordered_needs[i].width_fraction();

					_accounted.push_back(ordered_needs[i]);

					ordered_needs.erase(ordered_needs.begin()+i);

					i--;
				}
			}

			if(ordered_needs.size() > 0)
			{
				remaining._w_f = 12;
				remaining._x = 0;
				remaining._y = get_biggest_y_from_accounted();
			}
			else
				good = true;
		}
	}

	void clear_all()
	{
		_measurments.clear();
		_standard.clear();
		_needs.clear();
		_accounted.clear();
		clear_inputs();
		update_display();
		set_default_dists();
		_done_with_input = false;
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
				if(_needs[i].is_same_length(_needs[j]))
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
			WS_CLIPCHILDREN | WS_BORDER,390, 0, 200, 200,
			frame_wnd, NULL, _hInstance, this);

		set_up_gl_context();
	}
	
	void create_controls(HWND control_wnd)
	{

		RECT c_rect;
		GetClientRect(control_wnd,&c_rect);

		_topInput_wnd = CreateWindow (TOP_INPUT_WIN_CLASS, NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER,0, 0, c_rect.right, 80,
			control_wnd, NULL, _hInstance, this);

		_measure_display_wnd = CreateWindow (MEASURMENT_DIPLAY_WIN_CLASS, NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER,0, 400, 50, 50,
			control_wnd, NULL, _hInstance, this);

		_lower_control_wnd = CreateWindow (LOWER_CONTROL_WIN_CLASS, NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER,0, 600, 200, 50,
			control_wnd, NULL, _hInstance, this);

	}
	
	void create_lower_control(HWND low_wnd)
	{

		_clear_button = CreateWindow("BUTTON", "Clear",WS_VISIBLE | WS_CHILD,
			0, 0, 83, 35,low_wnd,NULL, _hInstance, NULL);
	}

	void create_MeasurmentDisplay(HWND meas_wnd)
	{
		CreateWindow("STATIC", "Measurments",WS_VISIBLE | WS_CHILD,
                      10, 0, 95, 18,meas_wnd,NULL, _hInstance, NULL);

		_measurements_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE | 
					  WS_VSCROLL | ES_LEFT, 10, 20, 120, 265,
					  meas_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "Standard",WS_VISIBLE | WS_CHILD,
                      135, 0, 65, 18,meas_wnd,NULL, _hInstance, NULL);

		_standard_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE | 
					  WS_VSCROLL | ES_LEFT, 135, 20, 120, 265,
					  meas_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "Needs",WS_VISIBLE | WS_CHILD,
                      260, 0, 50, 18,meas_wnd,NULL, _hInstance, NULL);

		_needs_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE | 
					  WS_VSCROLL | ES_LEFT, 260, 20, 120, 265,
					  meas_wnd,NULL, _hInstance, NULL);

		HFONT hFont=CreateFont(0,6,0,0,0,0,0,0,0,0,0,0,0,TEXT("Courier New"));
		SendMessage(_measurements_edit,WM_SETFONT,(WPARAM)hFont,0);
		SendMessage(_standard_edit,WM_SETFONT,(WPARAM)hFont,0);
		SendMessage(_needs_edit,WM_SETFONT,(WPARAM)hFont,0);

	}
	
	void create_top_inputs(HWND top_wnd)
	{

		CreateWindow ("BUTTON", "Input",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,10, 10, 265, 55,
			top_wnd, NULL, _hInstance, NULL);

		_width_feet_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_LEFT | WS_TABSTOP,
                      17, 30, 35, 25,top_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "'",WS_VISIBLE | WS_CHILD,
                      54, 30, 5, 25,top_wnd,NULL, _hInstance, NULL);

		_width_inch_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_LEFT | WS_TABSTOP,
                      61, 30, 30, 25,top_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "\"  X",WS_VISIBLE | WS_CHILD,
                      93, 30, 30, 25,top_wnd,NULL, _hInstance, NULL);

		int buff = 110;

		_length_feet_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_LEFT | WS_TABSTOP,
                      17+buff, 30, 35, 25,top_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "'",WS_VISIBLE | WS_CHILD,
                      54+buff, 30, 5, 25,top_wnd,NULL, _hInstance, NULL);

		_length_inch_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_LEFT | WS_TABSTOP,
                      61+buff, 30, 30, 25,top_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "\"",WS_VISIBLE | WS_CHILD,
                      93+buff, 30, 8, 25,top_wnd,NULL, _hInstance, NULL);

		_add_button = CreateWindow("BUTTON", "Add",WS_VISIBLE | WS_CHILD | WS_TABSTOP,
                      105+buff, 30, 50, 25,top_wnd,NULL, _hInstance, NULL);

		_invert_button = CreateWindow("BUTTON", "Invert",WS_VISIBLE | WS_CHILD,
                      180+buff, 25, 83, 35,top_wnd,NULL, _hInstance, NULL);
	}
	
	void draw_gl_scene()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();									// Reset The Current Modelview Matrix
		
		if(!(_accounted.size() > 0))
		{
			SwapBuffers(_hDC);
			return;
		}

		glPushMatrix();

		glTranslatef(x_dist,y_dist,z_dist);

		draw_red_backgroud();

		char buff[20];
		for(unsigned i = 0; i < _accounted.size(); i++)
		{
			_accounted[i].draw(i+1);

			if(_accounted[i]._print_text)
			{
				//print dimentions
				glPushMatrix();
					glColor3f(0.0f,0.0f,0.0f);
					_accounted[i].get_string(buff,20);
					glRasterPos3f(_accounted[i]._x+0.3f, (_accounted[i]._y*-1)-1.0f,0.01f);
 					gl_print(buff);
				glPopMatrix();
			}
			
		}

		glPopMatrix();

		SwapBuffers(_hDC);
	}

	void draw_red_backgroud()
	{
		float y = get_biggest_y_from_accounted()*-1;
		float myzdist = z_dist;
		myzdist -= 0.1f;

		glPushMatrix();
			

			glTranslatef(0,0,-0.001f);
			
			glColor3f(1.0f,0.0f,0.0f);
			glBegin(GL_QUADS);
				glVertex3f(0, 0, 0);
				glVertex3f(12, 0, 0);
				glVertex3f(12, y, 0);
				glVertex3f(0, y, 0);
			glEnd();

			float temp = y*-1;

			float myvar = 0;

			glColor3f(0.0f,0.0f,0.0f);

			while(true)
			{
				glBegin(GL_LINES);
					glVertex3f(0, myvar, 0);
					glVertex3f(12, myvar, 0);
				glEnd();
				temp -= 1;
				myvar -= 1;
				if(temp < 0)
					break;
			}


			myvar = 0;
			while(true)
			{
				glBegin(GL_LINES);
					glVertex3f(myvar, 0, 0);
					glVertex3f(myvar, y, 0);
				glEnd();
				myvar += 1;
				if(myvar > 12)
					break;
			}

		glPopMatrix();
	}

	float get_biggest_y_from_accounted()
	{
		float len = 0;
		float temp = 0;

		for(unsigned i = 0; i < _accounted.size(); i++)
		{
			temp = _accounted[i].get_length_plus_y();
			if(temp > len)
				len = temp;
		}

		return len;
	}
	
	float get_motion_dist()
	{
		RECT gl_rect;
		GetClientRect(_gl_wnd, &gl_rect);
		if(gl_rect.right > gl_rect.bottom)
			return (z_dist*-1)/gl_rect.right;
		
		return (z_dist*-1)/gl_rect.bottom;

	}

	void gl_print(const char *fmt, ...)					// Custom GL "Print" Routine
	{
		char		text[256];								// Holds Our String
		va_list		ap;										// Pointer To List Of Arguments

		if (fmt == NULL)									// If There's No Text
			return;											// Do Nothing

		va_start(ap, fmt);									// Parses The String For Variables
			vsprintf_s(text, fmt, ap);						// And Converts Symbols To Actual Numbers
		va_end(ap);											// Results Are Stored In Text

		glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
		glListBase(_base - 32);								// Sets The Base Character to 32
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
		glPopAttrib();										// Pops The Display List Bits
	}

	void gl_select(int x, int y, bool leftclick)
	{
		if(_accounted.size() == 0)
			return;

		GLuint buff[256] = {0};
		GLint hits, view[4];

		RECT gl_rect;
		GetClientRect(_gl_wnd,&gl_rect);

		//This choose the buffer where store the values for the selection data
		glSelectBuffer(256, buff);
 
		//This retrieve info about the viewport
		glGetIntegerv(GL_VIEWPORT, view);
 
		//Switching in selecton mode
		glRenderMode(GL_SELECT);
 
		//Clearing the name's stack
		//This stack contains all the info about the objects
		glInitNames();
 
		//Now fill the stack with one element (or glLoadName will generate an error)
		glPushName(0);
 
		//Now modify the vieving volume, restricting selection area around the cursor
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
 
		//restrict the draw to an area around the cursor
		gluPickMatrix(x, gl_rect.bottom-y, .1f, .1f, view);
		gluPerspective(45.0f,(GLfloat)gl_rect.right/(GLfloat)gl_rect.bottom,0.1f,100.0f);
 
		//Draw the objects onto the screen
		glMatrixMode(GL_MODELVIEW);
 
		//draw only the names in the stack, and fill the array
		SwapBuffers(_hDC);
		draw_gl_scene();
 
		//Do you remeber? We do pushMatrix in PROJECTION mode
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
 
		//get number of objects drawed in that area
		//and return to render mode
		hits = glRenderMode(GL_RENDER);
 
		//process hit
		if(hits > 0)
			process_hits(hits,buff,leftclick);
		else
		{
			for(unsigned i = 0; i < _accounted.size(); i++)
				_accounted[i]._print_text = false;
		}


		glMatrixMode(GL_MODELVIEW);
	}

	void init_gl()							// All Setup For OpenGL Goes Here
	{
		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		
		glClearColor(.94f, .94f, .94f, 0.5f);				// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

		build_font();
	}

	void invert_all()
	{
		vector<Measurment> temp(_measurments);
		_measurments.clear();
		_standard.clear();
		_needs.clear();
		for(unsigned i = 0; i < temp.size(); i++)
		{
			_measurments.push_back(
				Measurment(
					temp[i]._l_f,
					temp[i]._l_i,
					temp[i]._w_f,
					temp[i]._w_i
				)
			);
			process_input();
		}
	}

	bool is_numeric(char *buff, int size)
	{
		for(int i = 0; i < size; i++)
		{
			if(buff[i] < '0' || buff[i] > '9')
				return false;
		}
		return true;
	}

	void kill_font()									// Delete The Font List
	{
		glDeleteLists(_base, 96);					// Delete All 96 Characters
	}

	void kill_gl_window()								// Properly Kill The Window
	{

		if (_hRC)											// Do We Have A Rendering Context?
		{
			if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			{
				MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}

			if (!wglDeleteContext(_hRC))						// Are We Able To Delete The RC?
			{
				MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}
			_hRC=NULL;										// Set RC To NULL
		}

		if (_hDC && !ReleaseDC(_gl_wnd,_hDC))					// Are We Able To Release The DC
		{
			MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			_hDC=NULL;										// Set DC To NULL
		}

		if (_gl_wnd && !DestroyWindow(_gl_wnd))					// Are We Able To Destroy The Window?
		{
			MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			_gl_wnd=NULL;										// Set hWnd To NULL
		}

		if (!UnregisterClass(GL_WIN_CLASS,_hInstance))			// Are We Able To Unregister Class
		{
			MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			_hInstance=NULL;									// Set hInstance To NULL
		}

		kill_font();
	}

	void process_button_click(LPARAM lParam)
	{
		if((HWND)lParam == _add_button)
		{
			if(validate_input())
			{
				retrieve_measurment();
				process_input();
				clear_inputs();	
			}
		}
		else if((HWND)lParam == _invert_button)
		{
			if(!_done_with_input)
			{
				int res = MessageBox(_control_wnd,"Are you done inputting measurments?", "?", MB_YESNO);
				if(res == IDYES)
				{
					invert_all();
					_done_with_input = true;
				}
			}
			else
			{
				invert_all();
			}
		}
		else if((HWND)lParam == _clear_button)
		{
			clear_all();
		}
		SetFocus(_width_feet_edit);
		SetActiveWindow(_width_feet_edit);
	}
	
	void process_hits(GLint hits, GLuint buff[], bool leftclick)
	{
		GLuint *ptr = (GLuint *) buff;
		GLuint minZ = 0xffffffff;
		GLuint names,numberOfNames, *ptrNames = 0;
		for (int i = 0; i < hits; i++)
		{	
			names = *ptr;
			ptr++;
			if (*ptr < minZ)
			{
				numberOfNames = names;
				minZ = *ptr;
				ptrNames = ptr+2;
			}
			ptr += names+2;
		}

		for(unsigned i = 0; i < _accounted.size(); i++)
		{
			if(i+1 == *ptrNames)
			{
				_accounted[i]._print_text = true;
				if(leftclick)
				{
					_accounted[i]._is_clicked = true;
					motion_part = *ptrNames;
				}
			}
			else
			{
				if(leftclick)
				{
					_accounted[i]._is_clicked = false;
				}
				_accounted[i]._print_text = false;
			}
		}
	}
	
	void process_input()
	{
		split_needs();
		consolidate_needs();
		consolidate_standards();
		update_display();
		calculate_totals();
	}

	void process_keydown(WPARAM wParam)
	{
		switch(wParam)
		{
		case VK_ADD: z_dist += 1.0f; break;
		case VK_SUBTRACT: z_dist -= 1.0f; break;
		case VK_UP: y_dist += 1.0f; break;
		case VK_DOWN: y_dist -= 1.0f; break;
		case VK_LEFT: x_dist -= 1.0f; break;
		case VK_RIGHT: x_dist += 1.0f; break;
		}

	}

	void process_left_mouse_up()
	{
		/*if(motion_part != -1)
			_accounted[motion_part-1]._is_clicked = false;*/
		motion_part = -1;
	}

	void process_mouse_motion(WPARAM wParam,LPARAM lParam)
	{
		WORD xPos = LOWORD(lParam); 
		WORD yPos = HIWORD(lParam);
		if(wParam == MK_RBUTTON)
		{
			float dist = get_motion_dist();

			if(xPos < last_xPos)
				x_dist -= dist;
			else if(xPos > last_xPos)
				x_dist += dist;

			if(yPos < last_yPos)
				y_dist += dist;
			else if(yPos > last_yPos)
				y_dist -= dist;

			last_xPos = xPos;
			last_yPos = yPos;
		}
		else if(wParam == MK_LBUTTON)
		{
			if(_accounted.size() > 0 && motion_part > -1)
			{
				float dist = get_motion_dist();

				if(xPos < last_xPos)
					_accounted[motion_part-1]._x -= dist;
				else if(xPos > last_xPos)
					_accounted[motion_part-1]._x += dist;

				if(yPos < last_yPos)
					_accounted[motion_part-1]._y -= dist;
				else if(yPos > last_yPos)
					_accounted[motion_part-1]._y += dist;

				last_xPos = xPos;
				last_yPos = yPos;
			}
		}
		gl_select(xPos,yPos,false);
	}

	void process_wheel(WORD w)
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

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProcGL;
		wc.lpszClassName = GL_WIN_CLASS;
		RegisterClass (&wc);

		wc.style = CS_CLASSDC;
		wc.lpfnWndProc = WndProcControls;
		wc.lpszClassName = CONTROLS_WIN_CLASS;
		RegisterClass (&wc);

		wc.lpfnWndProc = WndProcTopInput;
		wc.lpszClassName = TOP_INPUT_WIN_CLASS;
		RegisterClass (&wc);

		wc.lpfnWndProc = WndProcLowerControl;
		wc.lpszClassName = LOWER_CONTROL_WIN_CLASS;
		RegisterClass (&wc);

		wc.lpfnWndProc = WndProcMeasurmentDisplay;
		wc.lpszClassName = MEASURMENT_DIPLAY_WIN_CLASS;
		RegisterClass (&wc);
	
	}
	
	void resize_gl_scene(RECT frame_rect, RECT control_rect)		// Resize And Initialize The GL Window
	{
		GLsizei width = frame_rect.right-control_rect.right;
		GLsizei height = frame_rect.bottom;

		glViewport(0,0,width,height);						// Reset The Current Viewport

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window
		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,500.0f);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix
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

		m.normalize_width();
		m.normalize_length();
		
		_measurments.push_back(m);
	}
	
	void set_default_dists()
	{
		z_dist = -50.0f;
		x_dist = -6.0f;
		y_dist = 20.0f;
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

	void size_control_window()
	{
		RECT c_frame_rect, c_control_rect;
		GetClientRect(_frame_wnd,&c_frame_rect);
		GetClientRect(_control_wnd,&c_control_rect);
		
		MoveWindow(_control_wnd,0,0,c_control_rect.right,c_frame_rect.bottom,TRUE); //good so far

		SendMessage(_measure_display_wnd,WM_SIZE,(WPARAM)NULL,(WPARAM)NULL);
		SendMessage(_lower_control_wnd,WM_SIZE,(WPARAM)NULL,(WPARAM)NULL);

	}
	
	void size_frame_window()
	{
		SendMessage(_control_wnd,WM_SIZE,(WPARAM)NULL,(WPARAM)NULL);
		SendMessage(_gl_wnd,WM_SIZE,(WPARAM)NULL,(WPARAM)NULL);
	}
	
	void size_gl_window()
	{
		RECT c_frame_rect, c_control_rect, c_gl_rect;
		
		GetClientRect(_frame_wnd,&c_frame_rect);
		GetClientRect(_control_wnd,&c_control_rect);
		GetClientRect(_gl_wnd,&c_gl_rect);

		MoveWindow(_gl_wnd,c_control_rect.right,c_gl_rect.top,c_frame_rect.right-c_control_rect.right,c_frame_rect.bottom,TRUE);
		resize_gl_scene(c_frame_rect,c_control_rect);
	}

	void size_lower_control_window()
	{
		RECT c_control_rect, display_rect,top_input_rect;
		
		GetClientRect(_topInput_wnd,&top_input_rect);
		GetClientRect(_control_wnd,&c_control_rect);
		GetClientRect(_measure_display_wnd,&display_rect);

		int top = top_input_rect.bottom + display_rect.bottom;
		int bottom = c_control_rect.bottom - top;

		MoveWindow(_lower_control_wnd,0,top,c_control_rect.right,bottom,TRUE);

	}

	void size_MeasurmentDisplay_window()
	{
		RECT measure_display_rect;
		RECT c_control_rect, top_input_rect;

		GetClientRect(_control_wnd,&c_control_rect);
		GetClientRect(_topInput_wnd,&top_input_rect);
		
		int lower_control_height = c_control_rect.bottom-top_input_rect.bottom-200;
		MoveWindow(_measure_display_wnd,0,top_input_rect.bottom,
			c_control_rect.right,lower_control_height,TRUE);

		GetClientRect(_measure_display_wnd,&measure_display_rect);

		int bottom = measure_display_rect.bottom - 25;

		MoveWindow(_measurements_edit,10,20,120,bottom,TRUE);
		MoveWindow(_standard_edit,135, 20, 120,bottom,TRUE);
		MoveWindow(_needs_edit,260, 20, 120,bottom,TRUE);

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
		else
			SetWindowText(_measurements_edit,"");
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
		else
			SetWindowText(_needs_edit,"");
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
		else
			SetWindowText(_standard_edit,"");
	}

	bool validate_input()
	{
		TCHAR buff[30];
		TCHAR wf[10];
		TCHAR wi[10];
		TCHAR lf[10];
		TCHAR li[10];

		GetWindowText(_width_feet_edit,wf,10);
		GetWindowText(_width_inch_edit,wi,10);
		GetWindowText(_length_feet_edit,lf,10);
		GetWindowText(_length_inch_edit,li,10);

		if(strlen(wf) == 0)
		{
			wf[0] = '0';
			wf[1] = '\0';
		}
		if(strlen(wi) == 0)
		{
			wi[0] = '0';
			wi[1] = '\0';
		}
		if(strlen(lf) == 0)
		{
			lf[0] = '0';
			lf[1] = '\0';
		}
		if(strlen(li) == 0)
		{
			li[0] = '0';
			li[1] = '\0';
		}
		if(!isdigit(wf[0]))
		{
			sprintf_s(buff,"Invalid feet value: %s",wf);
			MessageBox(_topInput_wnd,buff,"Warning",MB_OK);
			SetFocus(_width_feet_edit);
			SendMessage(_width_feet_edit, EM_SETSEL, 0, -1);
			return false;
		}
		if(!isdigit(wi[0]))
		{
			sprintf_s(buff,"Invalid inch value: %s",wi);
			MessageBox(_topInput_wnd,buff,"Warning",MB_OK);
			SetFocus(_width_inch_edit);
			SendMessage(_width_inch_edit, EM_SETSEL, 0, -1);
			return false;
		}
		if(!isdigit(lf[0]))
		{
			sprintf_s(buff,"Invalid feet value: %s",lf);
			MessageBox(_topInput_wnd,buff,"Warning",MB_OK);
			SetFocus(_length_feet_edit);
			SendMessage(_length_feet_edit, EM_SETSEL, 0, -1);
			return false;
		}
		if(!isdigit(li[0]))
		{
			sprintf_s(buff,"Invalid inch value: %s",li);
			MessageBox(_topInput_wnd,buff,"Warning",MB_OK);
			SetFocus(_length_inch_edit);
			SendMessage(_length_inch_edit, EM_SETSEL, 0, -1);
			return false;
		}


		int intwf = atoi (wf);
		int intwi = atoi (wi);
		int intlf = atoi (lf);
		int intli = atoi (li);

		if(intwf == 0 && intwi == 0)
		{
			MessageBox(_topInput_wnd,"Invalid Width:","Warning",MB_OK);
			SetFocus(_width_feet_edit);
			SendMessage(_width_feet_edit, EM_SETSEL, 0, -1);
			return false;
		}

		if(intlf == 0 && intli == 0)
		{
			MessageBox(_topInput_wnd,"Invalid Length:","Warning",MB_OK);
			SetFocus(_length_feet_edit);
			SendMessage(_length_feet_edit, EM_SETSEL, 0, -1);
			return false;
		}

		return true;
	}

public:

	MeasureWin(HINSTANCE hInstance) : _hInstance(hInstance)
	{
		last_xPos = 0;
		last_yPos = 0;
		motion_part = -1;
		_done_with_input = false;
		set_default_dists();
		reg_win_class();
	}

	int msg_loop()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof (msg));

		_frame_wnd = CreateWindow (FRAME_WIN_CLASS, "Measure", 
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,0, 0, 900, 600,
			NULL, NULL, _hInstance, this);

		size_frame_window();

		SetFocus(_width_feet_edit);

		while(GetMessage(&msg, NULL, 0, 0) > 0)
		{
			//this is for tab key to work
			if(!IsDialogMessage(_topInput_wnd,&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			draw_gl_scene();
		}

		kill_gl_window();

		return (int)msg.message;
	}
};

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	MeasureWin mw (hInstance);
	return mw.msg_loop();
}
