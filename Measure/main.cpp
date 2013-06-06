#include <Windows.h>

class MeasureWin
{
#define FRAME_WIN_CLASS		"FRAME_WIN_CLASS"
#define CONTROLS_WIN_CLASS	"CONTROLS_WIN_CLASS"

	HINSTANCE _hInstance;

	HWND _frame_wnd;
	HWND _control_wnd;

	HWND _width_feet_edit;
	HWND _width_inch_edit;
	HWND _length_feet_edit;
	HWND _length_inch_edit;

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

		/*case WM_COMMAND:
			switch(HIWORD(wParam))
			{
			case EN_CHANGE:
				mw->process_key_board(lParam);
				break;
			}
			break;*/
		}

		return DefWindowProc(control_wnd, message, wParam, lParam);
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
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,10, 10, 300, 175,
			control_wnd, NULL, _hInstance, NULL);

		_width_feet_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
                      17, 30, 30, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "'",WS_VISIBLE | WS_CHILD,
                      49, 30, 5, 25,control_wnd,NULL, _hInstance, NULL);

		_width_inch_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
                      56, 30, 30, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "\"   X",WS_VISIBLE | WS_CHILD,
                      88, 30, 30, 25,control_wnd,NULL, _hInstance, NULL);

		int buff = 110;

		_length_feet_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
                      17+buff, 30, 30, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "'",WS_VISIBLE | WS_CHILD,
                      49+buff, 30, 5, 25,control_wnd,NULL, _hInstance, NULL);

		_length_inch_edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
                      56+buff, 30, 30, 25,control_wnd,NULL, _hInstance, NULL);

		CreateWindow("STATIC", "\"",WS_VISIBLE | WS_CHILD,
                      88+buff, 30, 8, 25,control_wnd,NULL, _hInstance, NULL);	
	}

	void get_rects(RECT *frame_rect, RECT *gl_rect, RECT *control_rect)
	{
		if(control_rect != NULL)	GetClientRect(_control_wnd,control_rect);
		if(frame_rect != NULL)		GetClientRect(_frame_wnd,frame_rect);
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
		wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
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

	void size_frame_window()
	{
		SendMessage(_control_wnd,WM_SIZE,(WPARAM)NULL,(WPARAM)NULL);
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
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return (int)msg.message;
	}
};

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	
	MeasureWin mw (hInstance);

	return mw.msg_loop();
}
