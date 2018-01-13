#include <windows.h>

//ȫ�ֱ���
HINSTANCE hInst;
static TCHAR szClassName[] = TEXT("Translator");  //��������

//��������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //���ڹ���
void RegistWindowClass(HINSTANCE hInstance);      //ע�ᴰ����
HWND InitWindow(HINSTANCE hInstance);             //��ʼ������
HWND InitInBox(HWND hWnd,HINSTANCE hInstance);    //��ʼ���༭���ı������
HWND InitOutBox(HWND hWnd, HINSTANCE hInstance);  //��ʼ���ı��������������
HWND InitBotton(HWND hWnd, HINSTANCE hInstance);  //��ʼ����ť  �����밴ť��
HWND InitModeGroup(HWND hWnd,HINSTANCE hInstance);//��ʼ�������ģʽѡ�
HWND InitCheckBox(HWND hPreWnd,HINSTANCE hInstance); //��ʼ����ѡ�� (����ģʽ)
HWND InitMode1(HWND hPreWnd, HINSTANCE hInstance);   //��ʼ����ѡ��1������Ӣ��
HWND InitMode2(HWND hPreWnd, HINSTANCE hInstance);   //��ʼ����ѡ��2 (Ӣ����)

HFONT InitFont();                                 //��ʼ������
void SetFont(HWND target,HFONT hFont);            //Ϊ���ڿؼ���������

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PSTR szCmdLine,
    int iCmdShow
)
{
    HWND     hwnd;  //���ھ��
    MSG      msg;   //��Ϣ

    hInst = hInstance;

	RegistWindowClass(hInstance);
	hwnd = InitWindow(hInstance);

    ShowWindow (hwnd, iCmdShow);//��ʾ����
    UpdateWindow (hwnd);//���£����ƣ�����

	//��Ϣѭ��
    while( GetMessage(&msg, NULL, 0, 0) )
	{
        TranslateMessage(&msg);  //������Ϣ
        DispatchMessage (&msg);  //������Ϣ
    }

    return msg.wParam;
}

//���ڹ���
LRESULT CALLBACK WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    PAINTSTRUCT ps;
    HDC hdc;
	//HDC hdcStatic;

	static HFONT hFont;
    static HWND hOutBox;
	static HWND hInBox;
	static HWND hBotton;
	static HWND hModeGroup;
	static HWND hMode1;
	static HWND hMode2;
	static HWND hCheckBox;
	
	static HBRUSH hBrushWhite;

    switch (message){
        case  WM_CREATE:
			hBrushWhite = CreateSolidBrush(RGB(0xff, 0xff, 0xff));

			hFont = InitFont();
			hOutBox = InitOutBox(hWnd,hInst);
			hBotton = InitBotton(hWnd,hInst);
			hInBox = InitInBox(hWnd,hInst);
			hModeGroup = InitModeGroup(hWnd,hInst);
			hCheckBox = InitCheckBox(hModeGroup,hInst);
			hMode1 = InitMode1(hModeGroup,hInst);
			hMode2 = InitMode2(hModeGroup,hInst);

		


			SetFont(hOutBox,hFont);
			SetFont(hInBox, hFont);
			SetFont(hBotton ,hFont);
			SetFont(hModeGroup,hFont);
			SetFont(hCheckBox, hFont);
			SetFont(hMode1, hFont);
			SetFont(hMode2, hFont);
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            // TODO:  �ڴ���������ͼ����...
            EndPaint(hWnd, &ps);
            break;
		case WM_CTLCOLORBTN:
			return (INT_PTR)hBrushWhite;  //������뷵�ػ�ˢ���
		case WM_CTLCOLORSTATIC:
			return (INT_PTR)hBrushWhite;  //������뷵�ػ�ˢ���
        case WM_DESTROY:
			DeleteObject(hFont);//ɾ������������
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam) ;
}

void RegistWindowClass(HINSTANCE hInstance)
{
	
	WNDCLASS wndclass;  //������

	//Ϊ������ĸ����ֶθ�ֵ
	wndclass.style = CS_HREDRAW | CS_VREDRAW;  //���ڷ��
	wndclass.lpfnWndProc = WndProc;  //���ڹ���
	wndclass.cbClsExtra = 0;  //��ʱ����Ҫ���
	wndclass.cbWndExtra = 0;  //��ʱ����Ҫ���
	wndclass.hInstance = hInstance;  //��ǰ���ھ��
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);  //����ͼ��
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);  //�����ʽ
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //���ڱ�����ˢ����ɫ��
	wndclass.lpszMenuName = NULL;  //���ڲ˵�
	wndclass.lpszClassName = szClassName;  //��������
	//ע�ᴰ��
	RegisterClass(&wndclass);
}

HWND InitWindow(HINSTANCE hInstance)
{
	return CreateWindow(
		szClassName,  //��������
		TEXT("Translator"),  //���ڱ��⣨�����ڱ�������
		WS_OVERLAPPEDWINDOW,  //���ڷ��
		CW_USEDEFAULT,  //��ʼ��ʱx���λ��
		CW_USEDEFAULT,  //��ʼ��ʱy���λ��
		500,  //���ڿ��
		600,  //���ڸ߶�
		NULL,  //�����ھ��
		NULL,  //���ڲ˵����
		hInstance,  //��ǰ���ڵľ��
		NULL  //��ʹ�ø�ֵ
	);
}

HWND InitInBox(HWND hWnd, HINSTANCE hInstance)
{
	return CreateWindow(
		TEXT("edit"),
		TEXT(""),
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE| ES_LOWERCASE | ES_AUTOVSCROLL,
		10, 40, 460, 200,
		hWnd, (HMENU)1, hInst, NULL
	);
}

HWND InitOutBox(HWND hWnd, HINSTANCE hInstance)
{
	return CreateWindow(
		TEXT("edit"),     //�������
		TEXT("C����������hhhhhhhhfasff������ʿhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh���asfsadfsa"),  //�ؼ����ı�
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LOWERCASE | ES_AUTOVSCROLL| ES_READONLY,
		10 /*X����*/, 350 /*Y����*/, 460 /*���*/, 200 /*�߶�*/,
		hWnd,  //�����ھ��
		(HMENU)2,  //Ϊ�ؼ�ָ��һ��Ψһ��ʶ��
		hInstance,  //��ǰ����ʵ�����
		NULL
	);
}

HWND InitBotton(HWND hWnd, HINSTANCE hInstance)
{
	return CreateWindow(
		TEXT("button"), //��ť�ؼ�������
		TEXT("����"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
		320 /*X����*/, 270 /*Y����*/, 80 /*���*/, 50/*�߶�*/,
		hWnd, (HMENU)3 /*�ؼ�Ψһ��ʶ��*/, hInst, NULL
	);
}

HWND InitModeGroup(HWND hWnd, HINSTANCE hInstance)
{
	return CreateWindow(
		TEXT("button"), TEXT("MODE"),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 240, 240, 105,
		hWnd, (HMENU)4, hInst, NULL
	);
}

HWND InitCheckBox(HWND hPreWnd, HINSTANCE hInstance)
{
	return CreateWindow(
			TEXT("button"), 
			TEXT("����ģʽ"),
			WS_CHILD | WS_VISIBLE | BS_LEFT | BS_AUTOCHECKBOX/*��ѡ��*/,
			5, 25, 100, 40,
			hPreWnd, (HMENU)5, hInst, NULL
		);
}

HWND InitMode1(HWND hPreWnd, HINSTANCE hInstance)
{
	return  CreateWindow(
		TEXT("button"), TEXT("���� �� Ӣ��"),
		WS_CHILD | WS_VISIBLE | BS_LEFT | BS_AUTORADIOBUTTON ,
		110, 15, 120, 40,
		hPreWnd, (HMENU)6, hInst, NULL
	);
}

HWND InitMode2(HWND hPreWnd, HINSTANCE hInstance)
{
	return CreateWindow(
		TEXT("button"), TEXT("Ӣ�� �� ����"),
		WS_CHILD | WS_VISIBLE | BS_LEFT | BS_AUTORADIOBUTTON,
		110, 55, 120, 40,
		hPreWnd, (HMENU)7, hInst, NULL
	);
}

HFONT InitFont()
{
	return CreateFont(
		-16/*�߶�*/, -8/*���*/, 0/*���ù�*/, 0/*���ù�*/, 400 /*һ�����ֵ��Ϊ400*/,
		FALSE/*����б��*/, FALSE/*�����»���*/, FALSE/*����ɾ����*/,
		DEFAULT_CHARSET,  //��������ʹ��Ĭ���ַ��������������� _CHARSET ��β�ĳ�������
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //���в������ù�
		DEFAULT_QUALITY,  //Ĭ���������
		FF_DONTCARE,  //��ָ��������
		TEXT("΢���ź�")  //������
	);
}

void SetFont(HWND target,HFONT hFont)
{
	SendMessage(
		target,  //����������Ŀؼ����
		WM_SETFONT,  //��Ϣ������Ϣ���ͣ�
		(WPARAM)hFont,  //������
		NULL  //����ֵ����
	);
}
