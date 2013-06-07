#include <Windows.h>
#include <stdio.h>
#include <vector>

using namespace std;

class MeasureWin
{
#define FRAME_WIN_CLASS		"FRAME_WIN_CLASS"
#define CONTROLS_WIN_CLASS	"CONTROLS_WIN_CLASS"

	struct Measurment
	{
		int _w_f, _w_i, _l_f, _l_i;
		Measurment() : _w_f(0), _w_i(0), _l_f(0), _l_i(0) {}
		Measurment(int w_f,int w_i,int l_f,int l_i) : 
			_w_f(w_f), _w_i(w_i), _l_f(l_f), _l_i(l_i) {}

		bool is_name_length(const Measurment &m)
		{
			return (_l_f == m._l_f) && (_l_i == m._l_i);
		}

		void add_width(const Measurment &m)
		{
			_w_f += m._w_f;
			_w_i += m._w_i;
		}
	};

	HINSTANCE _hInstance;

	HWND _frame_wnd;
	HWND _control_wnd;

	HWND _width_feet_edit;
	HWND _width_inch_edit;
	HWND _length_feet_edit;
	HWND _length_inch_edit;
	HWND _measurements_edit;

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

	void clear_inputs()
	{
		SetWindowText(_width_feet_edit,"");
		SetWindowText(_width_inch_edit,"");
		SetWindowText(_length_feet_edit,"");
		SetWindowText(_length_inch_edit,"");
	}

	void consolidate_needs()
	{
		for (int i = 0; i < _needs.size(); i++)
		{
			for (int j = i+1; j < _needs.size(); j++)
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
		for(int i = 0; i < _needs.size(); i++)
		{
			if(_needs[i]._w_f > 12)
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
						_needs.erase(_needs.begin()+i);
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
			WS_CHILD | WS_VISIBLE,0, 0, 400, 200,
			frame_wnd, NULL, _hInstance, this);
	}
	
	void create_controls(HWND control_wnd)
	{
		CreateWindow ("BUTTON", "Input",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,10, 10, 265, 55,
			control_wnd, NULL, _hInstance, NULL);

		_width_feet_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_LEFT | WS_TABSTOP,
                      17, 30, 35, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "'",WS_VISIBLE | WS_CHILD,
                      54, 30, 5, 25,control_wnd,NULL, _hInstance, NULL);

		_width_inch_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_LEFT | WS_TABSTOP,
                      61, 30, 30, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "\"   X",WS_VISIBLE | WS_CHILD,
                      93, 30, 30, 25,control_wnd,NULL, _hInstance, NULL);

		int buff = 110;

		_length_feet_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_LEFT | WS_TABSTOP,
                      17+buff, 30, 35, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "'",WS_VISIBLE | WS_CHILD,
                      54+buff, 30, 5, 25,control_wnd,NULL, _hInstance, NULL);

		_length_inch_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_LEFT | WS_TABSTOP,
                      61+buff, 30, 30, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "\"",WS_VISIBLE | WS_CHILD,
                      93+buff, 30, 8, 25,control_wnd,NULL, _hInstance, NULL);

		_add_button = CreateWindow("BUTTON", "Add",WS_VISIBLE | WS_CHILD | WS_TABSTOP,
                      105+buff, 30, 50, 25,control_wnd,NULL, _hInstance, NULL);

		_measurements_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_LEFT | WS_TABSTOP,
                      10, 75, 265, 265,control_wnd,NULL, _hInstance, NULL);
	}

	void get_rects(RECT *frame_rect, RECT *gl_rect, RECT *control_rect)
	{
		if(control_rect != NULL)	GetClientRect(_control_wnd,control_rect);
		if(frame_rect != NULL)		GetClientRect(_frame_wnd,frame_rect);
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
				SetFocus(_width_feet_edit);
			}
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
		wc.hbrBackground = (HBRUSH) COLOR_CAPTIONTEXT;
		//wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
		wc.lpszClassName = FRAME_WIN_CLASS;
		RegisterClass (&wc);

		wc.style = CS_CLASSDC;
		wc.lpfnWndProc = WndProcControls;
		//wc.hbrBackground = (HBRUSH) COLOR_CAPTIONTEXT;
		wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
		wc.lpszClassName = CONTROLS_WIN_CLASS;
		RegisterClass (&wc);

	}
	
	void resize_control_window()
	{
		RECT frame_rect, control_rect;
		get_rects(&frame_rect,NULL,&control_rect);
		MoveWindow(_control_wnd,0,0,control_rect.right,frame_rect.bottom,TRUE);
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

	void size_frame_window()
	{
		SendMessage(_control_wnd,WM_SIZE,(WPARAM)NULL,(WPARAM)NULL);
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
			else
			{
				good = true;
			}
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
		}

		return (int)msg.message;
	}
};

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	
	MeasureWin mw (hInstance);

	return mw.msg_loop();
}
