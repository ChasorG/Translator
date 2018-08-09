#include <iostream>
#include <sstream>
#include <windows.h>
#include <curl/curl.h>  
#include <openssl/md5.h>
#include <json/json.h>

#pragma warning(disable:4996)  //ʹ����ʹ��sprintf()�Ⱥ���
using namespace std;

//ȫ�ֱ���
HINSTANCE hInst;
static TCHAR szClassName[] = TEXT("Translator");  //��������
TCHAR szInText[1000];    //�������ı�
//TCHAR szOutText[1000];  //������
string szOutText;
int mode;               //����ģʽ 0-����ģʽ 1-Ӣ���� 2-����Ӣ


//��������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //���ڹ���
void RegistWindowClass(HINSTANCE hInstance);          //ע�ᴰ����
HWND InitWindow(HINSTANCE hInstance);                 //��ʼ������
HWND InitInBox(HWND hWnd, HINSTANCE hInstance);        //��ʼ���༭���ı������
HWND InitOutBox(HWND hWnd, HINSTANCE hInstance);      //��ʼ���ı��������������
HWND InitBotton(HWND hWnd, HINSTANCE hInstance);      //��ʼ����ť  �����밴ť��
HWND InitModeGroup(HWND hWnd, HINSTANCE hInstance);    //��ʼ�������ģʽѡ�
HWND InitCheckBox(HWND hPreWnd, HINSTANCE hInstance);  //��ʼ����ѡ�� (����ģʽ)
HWND InitMode1(HWND hPreWnd, HINSTANCE hInstance);    //��ʼ����ѡ��1 (Ӣ����)
HWND InitMode2(HWND hPreWnd, HINSTANCE hInstance);    //��ʼ����ѡ��2������Ӣ��

HFONT InitFont();                                 //��ʼ������
void SetFont(HWND target, HFONT hFont);            //Ϊ���ڿؼ���������

size_t write_func(void *contents, size_t size, size_t nmemb, void * userp); //url�Ļص�����
int Translate();

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

  ShowWindow(hwnd, iCmdShow);//��ʾ����
  UpdateWindow(hwnd);//���£����ƣ�����

//��Ϣѭ��
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);  //������Ϣ
    DispatchMessage(&msg);  //������Ϣ
  }

  return msg.wParam;
}

//���ڹ���
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;
  //HDC hdcStatic;
  int wmID;//�������İ�ťid

  static HFONT hFont;
  static HWND hOutBox;
  static HWND hInBox;
  static HWND hButton;
  static HWND hModeGroup;
  static HWND hMode1;
  static HWND hMode2;
  static HWND hCheckBox;

  static HBRUSH hBrushWhite;

  switch (message) {
  case  WM_CREATE:
    hBrushWhite = CreateSolidBrush(RGB(0xff, 0xff, 0xff)); //��ɫ��ˢ



    //��ʼ���������
    hFont = InitFont();
    hOutBox = InitOutBox(hWnd, hInst);
    hButton = InitBotton(hWnd, hInst);
    hInBox = InitInBox(hWnd, hInst);
    hModeGroup = InitModeGroup(hWnd, hInst);
    hCheckBox = InitCheckBox(hModeGroup, hInst);
    hMode1 = InitMode1(hModeGroup, hInst);
    hMode2 = InitMode2(hModeGroup, hInst);

    //�����Ż�
    SetFont(hOutBox, hFont);
    SetFont(hInBox, hFont);
    SetFont(hButton, hFont);
    SetFont(hModeGroup, hFont);
    SetFont(hCheckBox, hFont);
    SetFont(hMode1, hFont);
    SetFont(hMode2, hFont);


    break;
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    // TODO:  �ڴ���������ͼ����...
    EndPaint(hWnd, &ps);
    break;
  case WM_CTLCOLORSTATIC:
    return (INT_PTR)hBrushWhite;  //���ػ�ˢ���������̬�ı�����ɫ
  case WM_COMMAND:
    //����ť�¼�
    wmID = LOWORD(wParam); //�������İ�ťid
    switch (wmID)
    {
    case 3:  // ���밴ť
      GetWindowText(hInBox, szInText, 1000);
      Translate();
      SetWindowText(hOutBox, szOutText.c_str());
    default:
      //���������ϢҪ���� DefWindowProc ����
      return DefWindowProc(hWnd, message, wParam, lParam);
    }

  case WM_DESTROY:
    DeleteObject(hFont);//ɾ������������

    PostQuitMessage(0);
    break;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
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
    WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LOWERCASE | ES_AUTOVSCROLL,
    10, 40, 460, 200,
    hWnd, (HMENU)1, hInst, NULL
  );
}

HWND InitOutBox(HWND hWnd, HINSTANCE hInstance)
{
  return CreateWindow(
    TEXT("edit"),     //�������
    TEXT("C����������hhhhhhhhfasff������ʿhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh���asfsadfsa"),  //�ؼ����ı�
    WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LOWERCASE | ES_AUTOVSCROLL | ES_READONLY,
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
    TEXT("button"), TEXT("Ӣ�� �� ����"),
    WS_CHILD | WS_VISIBLE | BS_LEFT | BS_AUTORADIOBUTTON,
    110, 15, 120, 40,
    hPreWnd, (HMENU)6, hInst, NULL
  );
}

HWND InitMode2(HWND hPreWnd, HINSTANCE hInstance)
{
  return CreateWindow(
    TEXT("button"), TEXT("���� �� Ӣ��"),
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

void SetFont(HWND target, HFONT hFont)
{
  SendMessage(
    target,  //����������Ŀؼ����
    WM_SETFONT,  //��Ϣ������Ϣ���ͣ�
    (WPARAM)hFont,  //������
    NULL  //����ֵ����
  );
}



/***********************
* ����ֵ˵����
* 52000	�ɹ�
* 52001	����ʱ
* 52002	ϵͳ����
* 52003	δ��Ȩ�û�
* 54000	�������Ϊ��
* 54001	ǩ������
* 54003	����Ƶ������
* 54004	�˻�����
* 54005	��query����Ƶ��
* 58000	�ͻ���IP�Ƿ�
* 58001	�������Է���֧��
***********************/
int Translate()
{
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();//��ʼ��url����
  if (curl)
  {
    stringstream sign;
    stringstream myurl;
    myurl<<"http://api.fanyi.baidu.com/api/trans/vip/translate?";
    string appid = "xxxxxxxxxxxx";    //replace myAppid with your own appid
    string q = "apple,and,banana";          //replace apple with your own text to be translate, ensure that the input text is encoded with UTF-8!
    string from = "en";          //replace en with your own language type of input text
    string to = "zh";            //replace zh with your own language type of output text
    int salt = rand();
    string secret_key = "xxxxxxxxxxxxx";   //replace mySecretKey with your own mySecretKey
    sign << appid << q << salt << secret_key;
    //MD5����
    unsigned char md[16];
    char tmp[3] = { '\0' }, buf[33] = { '\0' };
    MD5((unsigned char*)sign.str().c_str(), sign.str().length(), md);
    for (int i = 0; i < 16; i++)
    {
      sprintf(tmp, "%2.2x", md[i]);
      strcat(buf, tmp);
    }    //�õ�MD5���ܺ�Ĵ� buf
    myurl << "q=" << q
      << "&from=" << from
      << "&to=" << to
      << "&appid=" << appid
      << "&salt=" << salt
      << "&sign=" << buf;
    //����url����
    string json_str;
    curl_easy_setopt(curl, CURLOPT_URL, myurl.str());   //��ַ
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);  //�ص�����
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&json_str);//����
    res = curl_easy_perform(curl);  //ִ��
    //����json��
    Json::Reader reader;
    Json::Value value;
    if (reader.parse(json_str, value))
    {
      int i = 0;
      szOutText = (value["trans_result"][i]["dst"]).asString();
    }
    //����
    curl_easy_cleanup(curl);
    curl_global_cleanup();
  }
  return 0;
}

size_t write_func(void * contents, size_t size, size_t nmemb, void * userp)
{
  size_t realsize = size * nmemb;
  string *mem = (string*)userp;
  (*mem).append((char*)contents, realsize);
  return realsize;
}
