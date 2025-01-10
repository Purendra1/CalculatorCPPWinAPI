#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <sstream>

#define TITLE_BOX 1025
#define EDIT_BOX_ID 1026
#define MENU_FILE_ITEM_EXIT 1027

#define BUTTON_1 1028
#define BUTTON_2 1029
#define BUTTON_3 1030
#define BUTTON_C 1031
#define BUTTON_PLUS 1032
#define BUTTON_4 1033
#define BUTTON_5 1034
#define BUTTON_6 1035
#define BUTTON_PLUS_MINUS 1036
#define BUTTON_MINUS 1037
#define BUTTON_7 1038
#define BUTTON_8 1039
#define BUTTON_9 1040
#define BUTTON_SQUARE 1041
#define BUTTON_MULTIPLY 1042
#define BUTTON_BACKSPACE 1043
#define BUTTON_0 1044
#define BUTTON_DOT 1045
#define BUTTON_EQUAL 1046
#define BUTTON_DIVIDE 1047
#define LD long double

std::map<int, std::string> idToButtonText;
void initMaps()
{
    idToButtonText[BUTTON_0]="0";
    idToButtonText[BUTTON_1]="1";
    idToButtonText[BUTTON_2]="2";
    idToButtonText[BUTTON_3]="3";
    idToButtonText[BUTTON_4]="4";
    idToButtonText[BUTTON_5]="5";
    idToButtonText[BUTTON_6]="6";
    idToButtonText[BUTTON_7]="7";
    idToButtonText[BUTTON_8]="8";
    idToButtonText[BUTTON_9]="9";
    idToButtonText[BUTTON_PLUS]="+";
    idToButtonText[BUTTON_MINUS]="-";
    idToButtonText[BUTTON_PLUS_MINUS]="+/-";
    idToButtonText[BUTTON_MULTIPLY]="X";
    idToButtonText[BUTTON_DIVIDE]="/";
    idToButtonText[BUTTON_DOT]=".";
    char32_t superscript2 = U'\u00B2'; //super script 2 unicode
    idToButtonText[BUTTON_SQUARE]="x"+std::string(1, superscript2);
    idToButtonText[BUTTON_EQUAL]="=";
    idToButtonText[BUTTON_BACKSPACE]="<-";
    idToButtonText[BUTTON_C]="C";
}



/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);
void AddControls(HWND);
void changeWindowFontSize(HWND);
void performFunctionOnWindowInputAndDisplay(char* (*func)(char[], char[]), char[]);
char* appendNumber(char[], char[]);
char* calculate(char[], char[]);
std::string longDoubleToString(LD);
HWND editBox;

LD firstN=99;
UINT operation = BUTTON_PLUS;

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("Calculator");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    initMaps();
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Calculator"),       /* Title Text */
           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           600,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_COMMAND:
            switch (wParam)
            {
                case MENU_FILE_ITEM_EXIT:
                    DestroyWindow(hwnd);
                    break;
                case BUTTON_0:
                case BUTTON_1:
                case BUTTON_2:
                case BUTTON_3:
                case BUTTON_4:
                case BUTTON_5:
                case BUTTON_6:
                case BUTTON_7:
                case BUTTON_8:
                case BUTTON_9:
                    char num[1];
                    strcpy(num,idToButtonText[wParam].c_str());
                    performFunctionOnWindowInputAndDisplay(appendNumber,num);
                    break;
                case BUTTON_EQUAL:
                    performFunctionOnWindowInputAndDisplay(calculate,NULL);
                    break;
                case BUTTON_BACKSPACE:
                    performFunctionOnWindowInputAndDisplay(NULL,NULL);
                    break;
                case BUTTON_DOT:
                    performFunctionOnWindowInputAndDisplay(NULL,NULL);
                    break;
                case BUTTON_C:
                    performFunctionOnWindowInputAndDisplay(NULL,NULL);
                    break;
                case BUTTON_PLUS_MINUS:
                    performFunctionOnWindowInputAndDisplay(NULL,NULL);
                    break;
                case BUTTON_SQUARE:
                    performFunctionOnWindowInputAndDisplay(NULL,NULL);
                    break;
                case BUTTON_PLUS:
                case BUTTON_MINUS:
                case BUTTON_MULTIPLY:
                case BUTTON_DIVIDE:
                    performFunctionOnWindowInputAndDisplay(NULL,NULL);
                    break;

            }
            break;
        case WM_CREATE:
            AddMenus(hwnd);
            AddControls(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_LBUTTONUP:
            std::cout<<"Click\n";
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
char* calculate(char* onScreen, char* arg)
{
    char* ans = (char*)calloc(100,sizeof(char));
    if (firstN==NULL)
    {
        return onScreen;
    }

    LD secondN = std::stold(onScreen), result=NULL;

    switch (operation)
    {
        case BUTTON_PLUS:
            result = firstN + secondN;
            strcpy(ans, longDoubleToString(result).c_str());
            break;
        case BUTTON_MINUS:
            result = firstN - secondN;
            strcpy(ans, longDoubleToString(result).c_str());
            break;
        case BUTTON_MULTIPLY:
            result = firstN * secondN;
            strcpy(ans, longDoubleToString(result).c_str());
            break;
        case BUTTON_DIVIDE:
            if (secondN == 0) {
                ans[0]='I';
                ans[0]='N';
                ans[0]='F';
            } else {
                result = firstN / secondN;
                strcpy(ans, longDoubleToString(result).c_str());
            }
            break;
        default:
            ans[0]='H';
            ans[1]='2';
            ans[2]='\0';
            break;
    }
    firstN = result;
    return ans;

}

char* appendNumber(char* onScreen, char* num)
{
    char* ans = (char*)calloc(100,sizeof(char));
    int i=0;
    while(onScreen[i]!='\0') {
        ans[i]=onScreen[i];
        i++;
    }

    ans[i]=num[0];
    i++;
    ans[i]='\0';

    return ans;
}

void performFunctionOnWindowInputAndDisplay(char* (*func)(char[],char[]), char* arg)
{
    char txt[100], *ans;
    GetWindowText(editBox,txt,100);
    ans = (char*)func(txt,arg);
    SetWindowText(editBox,ans);
}

void AddMenus(HWND hwnd)
{
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    AppendMenu(hFileMenu, MF_STRING, MENU_FILE_ITEM_EXIT, "Exit");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
    SetMenu(hwnd, hMenu);
}

void AddControls(HWND hWnd)
{
        CreateWindowEx(
            WS_EX_CLIENTEDGE,        // Extended style for client edge
            "STATIC",                 // Control class name
            "CALCULATOR",                     // Label
            WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, // Style: child, visible, border, auto horizontal scroll
            0, 0,                  // Position (x, y)
            595, 30,                 // Size (width, height)
            hWnd,                    // Parent window
            (HMENU)TITLE_BOX,                // ID of the control (can be used to reference the control later)
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), // Instance handle
            NULL                     // No additional data
        );

        editBox = CreateWindowEx(
            WS_EX_CLIENTEDGE,        // Extended style for client edge
            "EDIT",                 // Control class name
            "",                     // Label
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_READONLY, // Style: child, visible, border, auto horizontal scroll
            5, 35,                  // Position (x, y)
            582.5, 70,                 // Size (width, height)
            hWnd,                    // Parent window
            (HMENU)EDIT_BOX_ID,                // ID of the control (can be used to reference the control later)
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), // Instance handle
            NULL                     // No additional data
        );

        changeWindowFontSize(editBox);

        for(int y=120,i=0;i<4;i++,y+=110)
        {
            for(int x=15,j=0;j<5;j++,x+=110)
            {
                HWND button = CreateWindowEx(WS_EX_CLIENTEDGE,
                "BUTTON", idToButtonText[(i*5+j+1028)].c_str(),
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                x, y, 100, 100,
                hWnd, (HMENU)(i*5+j+1028), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
                changeWindowFontSize(button);
            }
        }
}

void changeWindowFontSize(HWND window)
{
        // Create a LOGFONT structure to define the font with a static size
        LOGFONT logFont = {};
        logFont.lfHeight = 30;  // Set the font size to 30 (static size)
        lstrcpy(logFont.lfFaceName, TEXT("Arial"));  // Set the font name to Arial

        // Create the font with CreateFontIndirect
        HFONT hFont = CreateFontIndirect(&logFont);

        // Set the font for the Edit Control (static size)
        SendMessage(window, WM_SETFONT, (WPARAM)hFont, TRUE);
}


std::string longDoubleToString(long double num) {
    std::ostringstream oss;
    oss << std::fixed << num;
    std::string result = oss.str();
    if (result.find('.') != std::string::npos) {
        result.erase(result.find_last_not_of('0') + 1, std::string::npos);
        if (result.back() == '.') {
            result.pop_back();
        }
    }
    return result;
}
