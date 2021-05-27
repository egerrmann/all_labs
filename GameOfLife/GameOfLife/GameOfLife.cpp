// GameOfLife.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GameOfLife.h"
#include <vector>
#include <exception>
#include <string>
#include <algorithm>
#include <Windowsx.h>

#define MAX_LOADSTRING 100

POINT pt_for_mouse;

class State
{
    bool _isAlive;

public:
    State(bool isAlive)
    {
        _isAlive = isAlive;
    }

    bool IsAlive()
    {
        return _isAlive;
    }

    void SetIsAlive(bool isAlive)
    {
        _isAlive = isAlive;
    }
};

class Cell
{
    State _state = State(false);

public:
    Cell(State state)
    {
        _state = state;
    }

    Cell()
    {

    }

    bool IsAlive()
    {
        return _state.IsAlive();
    }

    void SetIsAlive(bool isAlive)
    {
        _state.SetIsAlive(isAlive);
    }
};

class Grid
{
    std::vector<std::vector<Cell>> _cells;
    int _width;
    int _height;

public:
    Grid(int width, int height) : _width(width), _height(height)
    {
        _cells.resize(height);

        for (auto rowIndex = 0; rowIndex < height; ++rowIndex)
        {
            _cells[rowIndex].resize(width);
        }
    }

    int GetWidth()
    {
        return _width;
    }

    int GetHeight()
    {
        return _height;
    }

    Cell& GetCell(int rowIndex, int columnIndex)
    {
        if (rowIndex < 0 || rowIndex >= _height) throw std::exception("Wrong rowIndex");
        if (columnIndex < 0 || columnIndex >= _width) throw std::exception("Wrong columnIndex");

        return _cells[rowIndex][columnIndex];
    }
};

class LifeEngine
{
public:
    Grid CalculateNext(Grid currentGrid)
    {
        auto newGrid = Grid(currentGrid.GetWidth(), currentGrid.GetHeight());

        auto distance = 1;

        for (auto rowIndex = 0; rowIndex < currentGrid.GetHeight(); ++rowIndex)
        {
            for (auto columnIndex = 0; columnIndex < currentGrid.GetWidth(); ++columnIndex)
            {
                auto aliveCellCount = CalculateAliveNeighbour(rowIndex, columnIndex, currentGrid, distance);

                if (aliveCellCount == 3 && !currentGrid.GetCell(rowIndex, columnIndex).IsAlive())
                {
                    newGrid.GetCell(rowIndex, columnIndex).SetIsAlive(true);
                }
                else if ((aliveCellCount == 2 || aliveCellCount == 3) && currentGrid.GetCell(rowIndex, columnIndex).IsAlive())
                {
                    newGrid.GetCell(rowIndex, columnIndex).SetIsAlive(true);
                }
                else
                {
                    newGrid.GetCell(rowIndex, columnIndex).SetIsAlive(false);
                }
            }
        }

        return newGrid;
    }

private:
    int GetLeftStartIndex(int columnIndex, int distance)
    {
        return max(columnIndex - distance, 0);
    }

    int GetTopStartIndex(int rowIndex, int distance)
    {
        return max(rowIndex - distance, 0);
    }

    int GetRightStartIndex(int columnIndex, int gridWidth, int distance)
    {
        return min(columnIndex + distance, gridWidth - 1);
    }

    int GetBottomStartIndex(int rowIndex, int gridHeight, int distance)
    {
        return min(rowIndex + distance, gridHeight - 1);
    }

    int CalculateAliveNeighbour(int rowIndex, int columnIndex, Grid currentGrid, int distance)
    {
        auto aliveCellCount = 0;

        auto leftStartIndex = GetLeftStartIndex(columnIndex, distance);
        auto rightStartIndex = GetRightStartIndex(columnIndex, currentGrid.GetWidth(), distance);
        auto topStartIndex = GetTopStartIndex(rowIndex, distance);
        auto bottomStartIndex = GetBottomStartIndex(rowIndex, currentGrid.GetHeight(), distance);

        for (auto neighbourColumnIndex = leftStartIndex; neighbourColumnIndex <= rightStartIndex; ++neighbourColumnIndex)
        {
            for (auto neighbourRowIndex = topStartIndex; neighbourRowIndex <= bottomStartIndex; ++neighbourRowIndex)
            {
                if (rowIndex == neighbourRowIndex && columnIndex == neighbourColumnIndex) continue;

                if (currentGrid.GetCell(neighbourRowIndex, neighbourColumnIndex).IsAlive())
                {
                    ++aliveCellCount;
                }
            }
        }

        return aliveCellCount;
    }
};

Grid grid = Grid(10, 10);
LifeEngine lifeEngine;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];  // the main window class name
bool killTimer = true;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMEOFLIFE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEOFLIFE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEOFLIFE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GAMEOFLIFE);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void DrawGrid(HWND hWnd, HDC hdc)
{
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

    auto cellWidth = clientRect.right / grid.GetWidth();

    for (auto j = 0; j < grid.GetWidth() + 1; ++j)
    {
        MoveToEx(hdc, cellWidth * j, 0, nullptr);
        LineTo(hdc, cellWidth * j, clientRect.bottom);
    }

    auto cellHeight = clientRect.bottom / grid.GetHeight();

    for (auto i = 0; i < grid.GetHeight() + 1; ++i)
    {
        MoveToEx(hdc, 0, cellHeight * i, nullptr);
        LineTo(hdc, clientRect.right, cellHeight * i);
    }
}

void DrawCells(HWND hWnd, HDC hdc)
{
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

    auto cellWidth = clientRect.right / grid.GetWidth();
    auto cellHeight = clientRect.bottom / grid.GetHeight();

    auto brush = CreateSolidBrush(RGB(0, 0, 0));
    auto oldBrush = SelectObject(hdc, brush);

    for (auto i = 0; i < grid.GetHeight(); ++i)
    {
        for (auto j = 0; j < grid.GetWidth(); ++j)
        {
            if (grid.GetCell(i, j).IsAlive())
            {
                Ellipse(hdc, j * cellWidth, i * cellHeight, (j + 1) * cellWidth, (i + 1) * cellHeight);
            }
        }
    }

    SelectObject(hdc, oldBrush);
    DeleteObject(brush);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        POINT pt;
        

    //LEFT BUTTON CLICK



    case WM_LBUTTONDOWN: {
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        auto cellWidth = clientRect.right / grid.GetWidth();
        auto cellHeight = clientRect.bottom / grid.GetHeight();
        int row = (pt.y / cellHeight);
        int column = (pt.x / cellWidth);
        grid.GetCell(row, column).SetIsAlive(true);
        InvalidateRect(hWnd, &clientRect, true);
        break;
    }


    //RIGHT CLICK BUTTON


    case WM_RBUTTONDOWN: {
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        auto cellWidth = clientRect.right / grid.GetWidth();
        auto cellHeight = clientRect.bottom / grid.GetHeight();
        int row = (pt.y / cellHeight);
        int column = (pt.x / cellWidth);
        grid.GetCell(row, column).SetIsAlive(false);
        InvalidateRect(hWnd, &clientRect, true);
        break;
    }


    //SPACE


    case WM_KEYDOWN: {
        if (wParam == VK_SPACE) {
            if (killTimer) {
                KillTimer(hWnd, 0);
                killTimer = false;
            }
            else
            {
                SetTimer(hWnd, 0, 500, 0);
                killTimer = true;
            }
        }
        /*if (wParam == 0x79) {
            lifeEngine.CalculateNext(grid);
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            InvalidateRect(hWnd, &clientRect, true);
        }*/
        break;
    }


    //F10


    case WM_SYSKEYDOWN: {
        if (wParam == VK_F10 && !killTimer) {
            grid = lifeEngine.CalculateNext(grid);
            InvalidateRect(hWnd, nullptr, true);
        }
        break; 
    }


    //  ???  CTRL + F10  ???
    // added accelarator, but it doesn't work anyway


    case ID_CTRLF10: {
        if (!killTimer) {
            for (int i = 0; i < 5; i++) {
                grid = lifeEngine.CalculateNext(grid);
                InvalidateRect(hWnd, nullptr, true);
            }
        }
        break;
    }

    case WM_CREATE:

        grid.GetCell(0, 1).SetIsAlive(true);
        grid.GetCell(1, 2).SetIsAlive(true);
        grid.GetCell(2, 0).SetIsAlive(true);
        grid.GetCell(2, 1).SetIsAlive(true);
        grid.GetCell(2, 2).SetIsAlive(true);

        SetTimer(hWnd, 0, 500, 0);
        break;

    case WM_TIMER:

        grid = lifeEngine.CalculateNext(grid);
        InvalidateRect(hWnd, nullptr, true);
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_MOUSEMOVE: {
        pt_for_mouse.x = GET_X_LPARAM(lParam);
        pt_for_mouse.y = GET_Y_LPARAM(lParam);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        RECT clientRect;
        HDC hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &clientRect);
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
        SelectObject(hdc, brush);
        Rectangle(hdc,
            (pt_for_mouse.x / (clientRect.right / grid.GetWidth())) * (clientRect.right / grid.GetWidth()),
            (pt_for_mouse.y / (clientRect.bottom / grid.GetHeight())) * (clientRect.bottom / grid.GetHeight()),
            ((pt_for_mouse.x / (clientRect.right / grid.GetWidth())) + 1) * (clientRect.right / grid.GetWidth()),
            ((pt_for_mouse.y / (clientRect.bottom / grid.GetHeight())) + 1)* (clientRect.bottom / grid.GetHeight())
        );
        DrawGrid(hWnd, hdc);
        DrawCells(hWnd, hdc);
        DeleteObject(hdc);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
