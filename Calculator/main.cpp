#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <iostream>

#define TEXT_BOX_ID 1025
#define EDIT_BOX_ID 1026
#define MENU_FILE_ITEM_EXIT 1027
#define PRESS_BUTTON_ID 1028


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);
void AddControls(HWND);
HWND EditWindow;

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("Calculator");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
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
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
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
                case PRESS_BUTTON_ID:
                    char txt[100];
                    GetWindowText(EditWindow,txt,100);
                    SetWindowText(hwnd,txt);
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
            "Enter text here",                     // Label
            WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, // Style: child, visible, border, auto horizontal scroll
            10, 10,                  // Position (x, y)
            150, 30,                 // Size (width, height)
            hWnd,                    // Parent window
            (HMENU)TEXT_BOX_ID,                // ID of the control (can be used to reference the control later)
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), // Instance handle
            NULL                     // No additional data
        );

        EditWindow = CreateWindowEx(
            WS_EX_CLIENTEDGE,        // Extended style for client edge
            "EDIT",                 // Control class name (EDIT control for text box)
            "",                     // Initial text is empty
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL, // Style: child, visible, border, auto horizontal scroll
            10, 50,                  // Position (x, y)
            200, 30,                 // Size (width, height)
            hWnd,                    // Parent window
            (HMENU)EDIT_BOX_ID,                // ID of the control (can be used to reference the control later)
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), // Instance handle
            NULL                     // No additional data
        );

        CreateWindowEx(
            0,                    // Extended style
            "BUTTON",             // Class name (narrow-character string)
            "Change Title to the above text",           // Text (narrow-character string)
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | SS_CENTER , // Style
            10, 80,               // Position (x, y)
            200, 50,              // Size (width, height)
            hWnd,                 // Parent window
            (HMENU)PRESS_BUTTON_ID,             // ID of the control
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), // Instance handle
            NULL                  // No additional data
        );


}
