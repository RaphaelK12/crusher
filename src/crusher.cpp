// ===========================================================================
// $Source$
// $Revision$
// $Date$
// ===========================================================================
// Copyright (C) 1999 Tom Conder. All Rights Reserved.
//
// BY USING ANY PORTION OF THIS SOFTWARE, YOU AGREE TO THE FOLLOWING
// TERMS AND CONDITIONS:
// 
// Tom Conder, "THE AUTHOR", grants you, "THE USER", a non-exclusive,
// royalty free, license to use this software in source and binary code
// form, provided the user does not utilize the software in a manner
// which is disparaging to the author and the user acknowledges the
// author in any derivative work.
// 
// This software is provided "AS IS," without a warranty of any kind. ALL
// EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
// ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. THE AUTHOR SHALL NOT
// BE LIABLE FOR ANY DAMAGES SUFFERED BY THE USER AS A RESULT OF USING,
// MODIFYING OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT
// WILL THE AUTHOR BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR
// DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE
// DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY,
// ARISING OUT OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN IF THE
// AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
// ===========================================================================
// Project:    Crusher md2 viewer
//
// File:       crusher.cpp
//
// Written:    16 Jun. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This is the source for main portion the md2 viewer.
// ===========================================================================
#include "stdafx.h"
#include "defs.h"
#include "errors.h"
#include "3dmath.h"
#include "pcx.h"
#include "md2.h"
#include "crusher.h"
#include "engine.h"
#include "render.h"
#include "palette.h"
#include "registry.h"
#include <stdio.h>
#include <commdlg.h>
#include <shlobj.h>
#include <malloc.h>
#include <string.h>
#include <limits.h>

#define	NAME			"Crusher"
#define	TITLE			"Crusher md2 Viewer"

// prototypes
LRESULT	CALLBACK		WndProc				(HWND, UINT, WPARAM, LPARAM);
static LONG WINAPI		CommandHandler		(HWND, WPARAM, LPARAM);
static BOOL				InitApp				();
static int				DoMainLoop			();
static BOOL				ParseCmdLine		(LPSTR);

static BOOL CALLBACK	AboutDialogProc		(HWND, UINT, WPARAM, LPARAM);
static int				DoAboutBox			(HWND);
static int				DoUnrExportBox		(HWND);
static void				LaunchBrowser		(LPCTSTR);
static BOOL CALLBACK	SettingsDialogProc	(HWND, UINT, WPARAM, LPARAM);
static BOOL CALLBACK	UnrExportDialogProc	(HWND, UINT, WPARAM, LPARAM);
static void				SetWindowTitle		(HWND, const CMd2Model &);
static BOOL				ValidateQuake2Path	(LPCTSTR);
static UINT				atoui				(LPCTSTR);
static void				ParseFilename		(TCHAR **);
static void				GetBaseName			(TCHAR **);
static void				GetSkinName			(TCHAR **);

static HINSTANCE		hMainInstance = 0;
static HWND				hWnd;

CMd2Model				md2Model;
CEngine					engViewer;

// ===========================================================================
// Name.......: WinMain()
// Description:	This function is the enter point for the program and the main
//				initialization and message loop.
// Parameters.: hInstance				- handle to current instance
//				hPrevInstance			- handle to previous instance
//				lpCmdLine				- pointer to comand line
//				nCmdShow				- show state of window
// Returns....: int						- program return status
// ===========================================================================
int WINAPI
WinMain	(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int			rc;

    hMainInstance = hInstance;

    if (ParseCmdLine(lpCmdLine) == FALSE)
    {
        return FALSE;
    }

    if (InitApp() == FALSE)
    {
        return FALSE;
    }

	rc = DoMainLoop();

    return rc;
} // WinMain

// ===========================================================================
// Name.......: CommandHandler()
// Description:	This function handles WM_COMMAND messages.
// Parameters.: hWnd					- handle for window
//				wParam					- first message parameter
//				lParam					- second message parameter
// Returns....: LONG WINAPI				- return status
// ===========================================================================
static LONG WINAPI
    CommandHandler (HWND hInWnd, WPARAM wParam, LPARAM lParam)
{
    OPENFILENAME	ofn;
    LPTSTR			lpstrFile;
    LPSTR			cTemp;
    UINT			rc;
    static TCHAR	tInitDir[iMAX_PATH_LEN] = "\0";
	HDC				hDC;

    switch (LOWORD(wParam))
    {
    case IDM_OPEN_MODEL:
        if (*tInitDir == NULL)
        {
            strcpy (tInitDir, md2Model.m_modelname);
            cTemp = tInitDir + strlen(tInitDir) - 1;
            while (cTemp != tInitDir && *cTemp != '\\')
            {
                cTemp--;
            }

            if (tInitDir == cTemp)
            {
                strcpy (tInitDir, tQUAKE2_PLAYER_DIR);
                strcat (tInitDir, tQUAKE2_MD2_PATHNAME);
                strcat (tInitDir, "\\");
            }
            else
            {
                *cTemp = '\0';
            }
        }

        lpstrFile = (TCHAR *) calloc(iMAX_PATH_LEN, sizeof(TCHAR));

        ofn.lStructSize = sizeof (OPENFILENAME);
        ofn.hwndOwner = hInWnd;
        ofn.hInstance = NULL;
        ofn.lpstrFilter = "Quake 2 Model Files (.md2)\0*.md2\0"
                          "All Files (*.*)\0*.*\0\0";
        ofn.lpstrCustomFilter = NULL;
        ofn.nMaxCustFilter = NULL;
        ofn.nFilterIndex = 0;
        ofn.lpstrFile = lpstrFile;
        ofn.nMaxFile = iMAX_PATH_LEN;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = NULL;
        ofn.lpstrInitialDir = tInitDir;
        ofn.lpstrTitle = NULL;
        ofn.Flags = NULL;
        ofn.nFileOffset = 0;
        ofn.nFileExtension = 0;
        ofn.lpstrDefExt = NULL;
        ofn.lCustData = NULL ;
        ofn.lpfnHook = NULL;
        ofn.lpTemplateName = NULL;

        if (GetOpenFileName (&ofn) > 0)
        {
            // set new init dir to chosen dir
            strncpy (tInitDir, ofn.lpstrFile, ofn.nFileOffset);
            *(tInitDir + ofn.nFileOffset) = '\0';

            if (ofn.nFileExtension != 0)
            {
                if (stricmp (ofn.lpstrFile + ofn.nFileExtension, "md2") == 0)
                {
                    // load model
                    if ((rc = LoadModel (ofn.lpstrFile, NULL, md2Model, engViewer)) != iRC_OK)
                    {
                        DisplayErrorMessage (rc);
                    }
                    else
                    {
                        SetWindowTitle (hInWnd, md2Model);
                    }

                    // load skin
                }
            }
        }

        free (lpstrFile);

        SendMessage (hInWnd, WM_PAINT, 0, 0);
        break;

    case IDM_OPEN_TEX:
        if (*tInitDir == NULL)
        {
            strcpy (tInitDir, md2Model.m_skinname);
            cTemp = tInitDir + strlen(tInitDir) - 1;
            while (cTemp != tInitDir && *cTemp != '\\')
            {
                cTemp--;
            }

            if (tInitDir == cTemp)
            {
                strcpy (tInitDir, tQUAKE2_PLAYER_DIR);
                strcat (tInitDir, tQUAKE2_MD2_PATHNAME);
                strcat (tInitDir, "\\");
            }
            else
            {
                *cTemp = '\0';
            }
        }

        lpstrFile = (TCHAR *) calloc(iMAX_PATH_LEN, sizeof(TCHAR));

        ofn.lStructSize = sizeof (OPENFILENAME);
        ofn.hwndOwner = hInWnd;
        ofn.hInstance = NULL;
        ofn.lpstrFilter = "PCX Texture Files (.pcx)\0*.pcx\0"
                          "All Files (*.*)\0*.*\0\0";
        ofn.lpstrCustomFilter = NULL;
        ofn.nMaxCustFilter = NULL;
        ofn.nFilterIndex = 0;
        ofn.lpstrFile = lpstrFile;
        ofn.nMaxFile = iMAX_PATH_LEN;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = NULL;
        ofn.lpstrInitialDir = tInitDir;
        ofn.lpstrTitle = NULL;
        ofn.Flags = NULL;
        ofn.nFileOffset = 0;
        ofn.nFileExtension = 0;
        ofn.lpstrDefExt = NULL;
        ofn.lCustData = NULL ;
        ofn.lpfnHook = NULL;
        ofn.lpTemplateName = NULL;

        if (GetOpenFileName (&ofn) > 0)
        {
            // set new init dir to chosen dir
            strncpy (tInitDir, ofn.lpstrFile, ofn.nFileOffset);
            *(tInitDir + ofn.nFileOffset) = '\0';

            if (ofn.nFileExtension != 0)
            {
                if (stricmp (ofn.lpstrFile + ofn.nFileExtension, "pcx") == 0)
                {
                    // load pcx
                    if ((rc = md2Model.LoadTexture (ofn.lpstrFile)) != iRC_OK)
                    {
                        DisplayErrorMessage (rc);
                    }
                    else
                    {
                        strcpy (md2Model.m_skinname, ofn.lpstrFile);

                        EnableRendMenuItem (iREND_TEXTURED, TRUE);
                        engViewer.RendState(iREND_TEXTURED);

                        SetWindowTitle (hInWnd, md2Model);
                    }
                }
            }
        }

        free (lpstrFile);

        SendMessage (hInWnd, WM_PAINT, 0, 0);
        break;

	case IDM_EXPORT_UNREALMESH:
		DoUnrExportBox(hInWnd);
		break;

    case IDM_ABOUT:
        DoAboutBox(hInWnd);
        break;

    case IDM_WIREFRAME:
        engViewer.RendState(iREND_WIREFRAME);

		hDC = GetDC(hInWnd);
		SwitchToGreyscale(hDC);
		ReleaseDC(hInWnd, hDC);

        SendMessage (hInWnd, WM_PAINT, 0, 0);
        break;

    case IDM_FLATSHADE:
        engViewer.RendState (iREND_FLATSHADE);

		hDC = GetDC(hInWnd);
		SwitchToGreyscale(hDC);
		ReleaseDC(hInWnd, hDC);

        SendMessage (hInWnd, WM_PAINT, 0, 0);
        break;

    case IDM_TEXTURED:
        engViewer.RendState(iREND_TEXTURED);

		hDC = GetDC(hInWnd);
		SwitchToColor(hDC);
		ReleaseDC(hInWnd, hDC);

        SendMessage (hInWnd, WM_PAINT, 0, 0);
        break;

    case IDM_RECENTER:
        engViewer.ClearHorzOffset();
        engViewer.ClearVertOffset();
        engViewer.ClearXRot();
        engViewer.ClearYRot();
        engViewer.ClearZRot();

        SendMessage (hInWnd, WM_PAINT, 0, 0);
        break;

    case IDM_GAZPACHO_URL:
        LaunchBrowser (tGAZPACHO_NET_URL);
        break;

    case IDM_CRUSHER_URL:
        LaunchBrowser (tCRUSHER_URL);
        break;

    case IDM_EXIT:
        SendMessage (hInWnd, WM_CLOSE, 0, 0);
        break;

        default:
        return FALSE;
    }

    return TRUE;
} // CommandHandler

// ===========================================================================
// Name.......: InitApp()
// Description:	This function sets up the window parameters, including the
//				window title and size.
// Parameters.: NIL
// Returns....: BOOL						- boolean indicating success
// ===========================================================================
static BOOL
    InitApp ()
{
    DWORD		uExStyle;
    DWORD		uStyle;
    LPCTSTR 	pszWindowClass = TEXT(NAME);
    LPCTSTR 	pszWindowTitle = TEXT(TITLE);
    RECT		sRect;
    WNDCLASSEX	wc;
    int			xPos;
    int			yPos;

    // set up and register window class
    wc.cbSize			= sizeof (wc);
    wc.hCursor			= LoadCursor (NULL, IDC_ARROW);
    wc.hIcon			= LoadIcon (hMainInstance, MAKEINTRESOURCE (IDI_CRUSHER));
    wc.hIconSm			= LoadIcon (hMainInstance, MAKEINTRESOURCE (IDI_CRUSHER));
    wc.lpszMenuName 	= MAKEINTRESOURCE(IDR_MENU);
    wc.lpszClassName	= pszWindowClass;
    wc.hbrBackground	= NULL;
    wc.hInstance		= hMainInstance;
    wc.lpfnWndProc		= WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;

    xPos		= (GetSystemMetrics(SM_CXSCREEN) - iWIDTH ) / 2;
    yPos		= (GetSystemMetrics(SM_CYSCREEN) - iHEIGHT ) / 2;
    uStyle		= WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    uExStyle	= 0;

    if (! RegisterClassEx (&wc))
    {
        return FALSE;
    }

    // create the window
    hWnd = CreateWindowEx (uExStyle,
                           pszWindowClass,
                           pszWindowTitle,
                           uStyle,
                           xPos,
                           yPos,
                           iWIDTH,
                           iHEIGHT,
                           (HWND) NULL,
                           (HMENU) NULL,
                           hMainInstance,
                           (LPVOID) NULL);

    if (! hWnd)
    {
        return FALSE;
    }

    // client area needs to be iWIDTH x iHEIGHT
    SetRect(&sRect, 0, 0, iWIDTH, iHEIGHT);

    AdjustWindowRectEx(&sRect,
                       GetWindowStyle(hWnd),
                       GetMenu(hWnd) != NULL,
                       GetWindowExStyle(hWnd));

    SetWindowPos(hWnd,
                 NULL,
                 0,
                 0,
                 sRect.right - sRect.left,
                 sRect.bottom - sRect.top,
                 SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

    SetWindowPos(hWnd,
                 HWND_NOTOPMOST,
                 0,
                 0,
                 0,
                 0,
                 SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

    return TRUE;
}

// ===========================================================================
// Name.......: DoMainLoop()
// Description:	This function handles our processing loop.
// Parameters.: NIL
// Returns....: int							- a return code
// ===========================================================================
static int
DoMainLoop ()
{
    HACCEL		hAccel;
    MSG 		msg;

    hAccel = LoadAccelerators (hMainInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

    while (1)
    {
        if (!GetMessage( &msg, NULL, 0, 0))
        {
            break;
        }

        if (!TranslateAccelerator (hWnd, hAccel, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}

// ===========================================================================
// Name.......: ParseCmdLine()
// Description:	This function parse the command line arguments.
// Parameters.: lpCmdLine					- string containing command line
// Returns....: BOOL						- boolean indicating success
// ===========================================================================
static BOOL
ParseCmdLine (LPSTR lpCmdLine)
{
	TCHAR	*pResult;
	TCHAR	tFilename[iMAX_PATH_LEN];

	if (*lpCmdLine == '\0')
	{
		return TRUE;
	}

	if ((pResult = strstr (lpCmdLine, "-model")) != NULL)
	{
		pResult += strlen("-model");
		strncpy (tFilename, pResult, iMAX_PATH_LEN);
		pResult = tFilename;
		ParseFilename(&pResult);
		strcpy (engViewer.tModelFilename, pResult);
	}

	if ((pResult = strstr (lpCmdLine, "-skin")) != NULL)
	{
		pResult += strlen("-skin");
		strncpy (tFilename, pResult, iMAX_PATH_LEN);
		pResult = tFilename;
		ParseFilename(&pResult);
		strcpy (engViewer.tSkinFilename, pResult);
	}

	if ((pResult = strstr (lpCmdLine, "-xrot")) != NULL)
	{
		engViewer.XRot(atoui(pResult + strlen("-xrot")));
	}

	if ((pResult = strstr (lpCmdLine, "-yrot")) != NULL)
	{
		engViewer.YRot(atoui(pResult + strlen("-yrot")));
	}

	if ((pResult = strstr (lpCmdLine, "-zrot")) != NULL)
	{
		engViewer.ZRot(atoui(pResult + strlen("-zrot")));
	}

	if ((pResult = strstr (lpCmdLine, "-wireframe")) != NULL)
	{
		engViewer.RendState(iREND_WIREFRAME);
	}

	if ((pResult = strstr (lpCmdLine, "-flatshade")) != NULL)
	{
		engViewer.RendState(iREND_FLATSHADE);
	}

	if ((pResult = strstr (lpCmdLine, "-help")) != NULL)
	{
		MessageBox (hWnd, tUSAGE, "Crusher - Usage", MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}

	if ((pResult = strstr (lpCmdLine, "-?")) != NULL)
	{
		MessageBox (hWnd, tUSAGE, "Crusher - Usage", MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}

	if ((pResult = strstr (lpCmdLine, "-version")) != NULL)
	{
		MessageBox (hWnd, tVERSION, "Crusher - Version", MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}

	if ((pResult = strstr (lpCmdLine, "-h")) != NULL)
	{
		MessageBox (hWnd, tUSAGE, "Crusher - Usage", MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}

	if ((pResult = strstr (lpCmdLine, "-v")) != NULL)
	{
		MessageBox (hWnd, tVERSION, "Crusher - Version", MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}

	if ((pResult = strstr (lpCmdLine, "-V")) != NULL)
	{
		MessageBox (hWnd, tVERSION, "Crusher - Version", MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}

    return TRUE;
}

static void
ParseFilename (TCHAR **lpString)
{
	TCHAR	*pBegin;
	TCHAR	*pCh;
	TCHAR	ch;
	int		len;
	int		iState;
	BOOL	bDone;

	len = 0;
	pBegin = *lpString;

	if (!pBegin)
	{
		*lpString = NULL;
		return;
	}

	// skip leading space
	while ((ch = *pBegin) <= ' ')
	{
		if (ch == '\0')
		{
			*lpString = NULL;
			return;
		}

		pBegin++;
	}

	// handle quoted string
	if (ch == '\"')
	{
		len++;
		pBegin++;
		*lpString = pBegin;

		pCh = pBegin;

		bDone = FALSE;
		while (!bDone)
		{
			ch = *pCh++;

			if (ch == '\"' || !ch)
			{
				*--pCh = '\0';
				bDone = TRUE;
			}

			if (len == iMAX_PATH_LEN)
			{
				*pCh = '\0';
				bDone = TRUE;
			}
		}

		return;
	}

	*lpString = pBegin;

	bDone = FALSE;
	iState = 0;
	pCh = pBegin;
	while (!bDone)
	{
		if (len < iMAX_PATH_LEN)
		{
			if (ch == '\0')
			{
				bDone = TRUE;
			}
			else if (ch <= ' ' && iState == 0)
			{
				iState = 1;
			}
			else if (ch == '-' && iState == 1)
			{
				pCh -= 2;
				bDone = TRUE;
			}
			else
			{
				iState = 0;
			}
		}
		else
		{
			bDone = TRUE;
		}

		if (!bDone)
		{
			len++;
			ch = *pCh++;
		}
	}
	
	*pCh = '\0';
}

// ===========================================================================
// Name.......: WndProc()
// Description:	This function is the main callback for windows procedures.
// Parameters.: hWnd					- handle for window
//				message					- message identifier
//				wParam					- first message parameter
//				lParam					- second message parameter
// Returns....: LRESULT CALLBACK		- return status
// ===========================================================================
LRESULT	CALLBACK
WndProc (HWND hInWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static BOOL		bLeftDown = FALSE;
    static BOOL		bRightDown = FALSE;
    static int		iNewX, iNewY;
    HDC				hDC;
    HMENU			hMenu;
    int				iLastX, iLastY;
    POINT			ptCursor;
    UINT			rc;
    LPMINMAXINFO	lpmmi;

    switch (message)
    {
    case WM_CREATE:
        hWnd = hInWnd;

        hMenu = GetMenu(hInWnd);
        hMenu = GetSubMenu(hMenu, 1);

        CheckMenuRadioItem(hMenu, 0, 2, 2, MF_BYPOSITION|MF_CHECKED);

        hDC = GetDC(hInWnd);

        if (CheckRegForQuake2Path() == FALSE)
        {
            if (DoSettingsBox(hInWnd) == FALSE)
            {
                SendMessage (hInWnd, WM_CLOSE, 0, 0);
                break;
            }
        }

        if ((rc = engViewer.SetValuesFromRegistry()) != iRC_OK)
        {
            DisplayErrorMessage(rc);
            SendMessage (hInWnd, WM_CLOSE, 0, 0);
            return rc;
        }

        if ((rc = LoadModel (engViewer.ModelFilename(),
                             engViewer.SkinFilename(),
                             md2Model,
							 engViewer)) != iRC_OK)
        {
            DisplayErrorMessage(rc);
            SendMessage (hInWnd, WM_CLOSE, 0, 0);
            break;
        }

		SetRendState (engViewer.RendState());

        if ((rc = Init3D(hInWnd)) != iRC_OK)
        {
            ReleaseDC (hInWnd, hDC);
            DisplayErrorMessage (rc);
            SendMessage (hInWnd, WM_CLOSE, 0, 0);
            break;
        }

        SetWindowTitle (hInWnd, md2Model);

        if ((rc = InitPalette(hDC)) != iRC_OK)
        {
            DisplayErrorMessage (rc);
        }

        ReleaseDC (hInWnd, hDC);
        break;

    case WM_CLOSE:
        GdiFlush();

        End3D(md2Model);

        PostQuitMessage(0);
        break;

    case WM_COMMAND:
        return CommandHandler (hInWnd, wParam, lParam);
        break;

    case WM_PAINT:
        hDC = GetDC(hInWnd);

        if (!Uses256Palette())
        {
            DrawFrame(hDC, engViewer, md2Model);
        }
        else
        {
            Realize256Palette (hDC);
            DrawFrame(hDC, engViewer, md2Model);
            Restore256Palette (hDC);
        }

        ReleaseDC(hInWnd, hDC);
        break;

    case WM_QUERYNEWPALETTE:
        hDC = GetDC(hInWnd);

        // Select and realize the palette
        Realize256Palette (hDC);
        DrawFrame(hDC, engViewer, md2Model);
        Restore256Palette (hDC);

        ReleaseDC(hInWnd, hDC);
        break;

    case WM_PALETTECHANGED:
        if (wParam != (WPARAM)hInWnd)
        {
            hDC = GetDC(hInWnd);

            Realize256Palette (hDC);
            UpdateColors(hDC);
            DrawFrame(hDC, engViewer, md2Model);
            Restore256Palette (hDC);

            ReleaseDC(hInWnd, hDC);
        }
        break;

    case WM_GETMINMAXINFO:
        lpmmi = (LPMINMAXINFO) lParam;
        lpmmi->ptMinTrackSize.x = iMIN_WIDTH;
        lpmmi->ptMinTrackSize.y = iMIN_HEIGHT;
        break;

    case WM_LBUTTONDOWN:
        if (!bLeftDown && !bRightDown)
        {
            SetCapture (hInWnd);
        }

        bLeftDown = TRUE;

        GetCursorPos (&ptCursor);

        iNewX = ptCursor.x;
        iNewY = ptCursor.y;
        break;

    case WM_LBUTTONUP:
        if (bLeftDown && !bRightDown)
        {
            ReleaseCapture ();
        }

        bLeftDown = FALSE;
        break;

    case WM_RBUTTONDOWN:
        if (!bLeftDown && !bRightDown)
        {
            SetCapture (hInWnd);
        }

        bRightDown = TRUE;

        GetCursorPos (&ptCursor);

        iNewX = ptCursor.x;
        iNewY = ptCursor.y;
        break;

    case WM_RBUTTONUP:
        if (!bLeftDown && bRightDown)
        {
            ReleaseCapture ();
        }

        bRightDown = FALSE;
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case 'c':
        case 'C':
            engViewer.ClearHorzOffset();
            engViewer.ClearVertOffset();
            engViewer.ClearXRot();
            engViewer.ClearYRot();
            engViewer.ClearZRot();

            hDC = GetDC(hInWnd);

            if (! Uses256Palette())
            {
                DrawFrame(hDC, engViewer, md2Model);
            }
            else
            {
                Realize256Palette (hDC);
                DrawFrame(hDC, engViewer, md2Model);
                Restore256Palette (hDC);
            }

            ReleaseDC(hInWnd, hDC);
            break;

        case VK_LEFT:
            md2Model.PrevFrame();

            hDC = GetDC(hInWnd);

            if (! Uses256Palette())
            {
                DrawFrame(hDC, engViewer, md2Model);
            }
            else
            {
                Realize256Palette (hDC);
                DrawFrame(hDC, engViewer, md2Model);
                Restore256Palette (hDC);
            }

            ReleaseDC(hInWnd, hDC);
            break;

        case VK_RIGHT:
            md2Model.NextFrame();

            hDC = GetDC(hInWnd);

            if (! Uses256Palette())
            {
                DrawFrame(hDC, engViewer, md2Model);
            }
            else
            {
                Realize256Palette (hDC);
                DrawFrame(hDC, engViewer, md2Model);
                Restore256Palette (hDC);
            }

            ReleaseDC(hInWnd, hDC);
            break;
        }
        break;

    case WM_MOUSEMOVE:
        iLastX = iNewX;
        iLastY = iNewY;

        GetCursorPos (&ptCursor);

        iNewX = ptCursor.x;
        iNewY = ptCursor.y;

        if (bLeftDown)
        {
            engViewer.AdjustXRot ((int) (iNewY - iLastY));
            engViewer.AdjustZRot ((int) (iLastX - iNewX));
        }

        if (bRightDown)
        {
            engViewer.AdjustHorzOffset ((int) (iNewX - iLastX));
            engViewer.AdjustVertOffset ((int) (iNewY - iLastY));
        }

        hDC = GetDC(hInWnd);

        if (! Uses256Palette())
        {
            DrawFrame(hDC, engViewer, md2Model);
        }
        else
        {
            Realize256Palette (hDC);
            DrawFrame(hDC, engViewer, md2Model);
            Restore256Palette (hDC);
        }

        ReleaseDC(hInWnd, hDC);
        break;

    case WM_SIZE:
        if ((hDC = GetDC(hInWnd)) == NULL)
        {
            DisplayErrorMessage (iERR_RENDER_DC);
        }

        if ((rc = Resize(hDC, engViewer.RendState(), LOWORD(lParam), HIWORD(lParam))) != iRC_OK)
        {
            DisplayErrorMessage(rc);
        }

        ReleaseDC (hInWnd, hDC);

        break;
    }

    return DefWindowProc (hInWnd, message, wParam, lParam);
} // WndProc

// ========================================================================
// Name.......: CheckRegForQuake2Path()
// Description: This function checks the registry to see if it contains the
//				Quake 2 directory name.
// Parameters.: NIL
// Returns....: BOOL					- boolean for exists in registry
// ========================================================================
BOOL
CheckRegForQuake2Path ()
{
    TCHAR	tSelection[iREG_KEY_BUF_SIZE];

    if (LoadRegValue (tREG_KEY_Q2PATH, tSelection) == iRC_OK)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
} // CheckRegForQuake2Path

// ===========================================================================
// Name.......: DisplayMessage()
// Description:	This function displays a string in a message box.
// Parameters.: lpMsg					- message string
// Returns....: NIL
// ===========================================================================
void
DisplayMessage (LPCTSTR lpMsg)
{
    MessageBox (hWnd, lpMsg, "Crusher - Information", MB_ICONINFORMATION|MB_OK);
} // DisplayMessage

// ========================================================================
// Name.......: DoSettingsBox()
// Description: This function launches the 'settings' dialog box.
// Parameters.: hWnd					- handle to current window
// Returns....: int						- message parameter
// ========================================================================
int
DoSettingsBox (HWND hWnd)
{
    return DialogBox (hMainInstance,
                      MAKEINTRESOURCE(IDD_SETTINGS),
                      hWnd,
                      (DLGPROC) SettingsDialogProc);
} // DoSettingsBox

// ========================================================================
// Name.......: DoUnrExportBox()
// Description: This function launches the 'Export to Unreal' dialog box.
// Parameters.: hWnd					- handle to current window
// Returns....: int						- message parameter
// ========================================================================
int
DoUnrExportBox (HWND hWnd)
{
    return DialogBox (hMainInstance,
                      MAKEINTRESOURCE(IDD_UNREAL),
                      hWnd,
                      (DLGPROC) UnrExportDialogProc);
} // DoUnrExportBox

// ========================================================================
// Name.......: EnableRendMenuItem()
// Description: This function enables or disables the render state menu items.
// Parameters.: iMode					- the menu item to disable
//				bEnable					- boolean for action; enable if true
// Returns....: NIL
// ========================================================================
void EnableRendMenuItem (UINT iMode, BOOL bEnable)
{
    HMENU			hMenu;
    MENUITEMINFO	mnuInfo;

    hMenu = GetMenu(hWnd);
    hMenu = GetSubMenu(hMenu, 1);

    mnuInfo.cbSize = sizeof (MENUITEMINFO);
    mnuInfo.fMask = MIIM_STATE;
    mnuInfo.fState = bEnable ? MFS_ENABLED : MFS_DISABLED;

	switch (iMode)
	{
	case iREND_WIREFRAME:
	    SetMenuItemInfo (hMenu, 0, TRUE, &mnuInfo);
		break;
	case iREND_FLATSHADE:
	    SetMenuItemInfo (hMenu, 1, TRUE, &mnuInfo);
		break;
	case iREND_TEXTURED:
	    SetMenuItemInfo (hMenu, 2, TRUE, &mnuInfo);
		break;
	}

    SendMessage (hWnd, WM_PAINT, 0, 0);
} // EnableRendMenuItem

// ===========================================================================
// Name.......: SetRendState()
// Description:	This function sets the state of the renderer.
// Parameters.: iGiven					- the given state
// Returns....: UINT					- value of the renderer state
// ===========================================================================
void
SetRendState (UINT iGiven)
{
    HMENU		hMenu;

    switch (iGiven)
    {
    case iREND_WIREFRAME:
        hMenu = GetMenu(hWnd);
        hMenu = GetSubMenu(hMenu, 1);

        CheckMenuRadioItem(hMenu, 0, 2, 0, MF_BYPOSITION|MF_CHECKED);
        break;

    case iREND_FLATSHADE:
        hMenu = GetMenu(hWnd);
        hMenu = GetSubMenu(hMenu, 1);

        CheckMenuRadioItem(hMenu, 0, 2, 1, MF_BYPOSITION|MF_CHECKED);
        break;

    case iREND_TEXTURED:
        hMenu = GetMenu(hWnd);
        hMenu = GetSubMenu(hMenu, 1);

        CheckMenuRadioItem(hMenu, 0, 2, 2, MF_BYPOSITION|MF_CHECKED);
        break;
    }
}

// ========================================================================
// Name.......: AboutDialogProc()
// Description: This function provides the processing of window messages for
//              the About dialog box.
// Parameters.: hWnd					- handle to the window
//              uMsg					- the window message
//              wParam					- first message parameter
//              lParam					- second message parameter
// Returns....: BOOL CALLBACK			- result of message processing
// ========================================================================
static BOOL CALLBACK
AboutDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        EnableWindow (hDlg, TRUE);
        SetDlgItemText(hDlg,
                       IDC_INSTRUCTIONS,
						"INSTRUCTIONS\r\n\r\n"
                       "Use the left and right arrows to step and back "
                       "through the frames.\r\n\r\n"
                       "Press and hold the left mouse to rotate the model. "
                       "The right mouse button pans the model.\r\n\r\n"
                       "Ctrl+W displays the model in wireframe mode.\r\n"
                       "Ctrl+F displays the model in flat shaded mode.\r\n"
                       "Ctrl+T displays the model in textured mode.\r\n\r\n"
                       "'C' recenters the model.\r\n\r\n"
						"COMMAND LINE OPTIONS\r\n\r\n"
						"The following options are accepted as command line "
						"options:\r\n\r\n"
						"-model filename\tloads the model file\r\n"
						"-skin filename\tloads the skin file\r\n"
						"-xrot value\trotates about x-axis by value amount\r\n"
						"-yrot value\trotates about y-axis by value amount\r\n"
						"-zrot value\trotates about z-axis by value amount\r\n"
						"-wireframe\tsets the viewer to wireframe mode\r\n"
						"-flatshade\t\tset the viewer to flatshading mode\r\n"
						"-h, -help, -?\tdisplays this screen\r\n"
						"-v, -V, -version\tdisplays the program version number"
					   );


        ShowWindow (hDlg, SW_SHOW);
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            EndDialog (hDlg, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
} // AboutDialogProc

// ========================================================================
// Name.......: DoAboutBox()
// Description: This function launches the 'about' dialog box.
// Parameters.: hWnd					- handle to current window
// Returns....: int						- message parameter
// ========================================================================
static int
DoAboutBox (HWND hWnd)
{
    return DialogBox (hMainInstance,
                      MAKEINTRESOURCE(IDD_ABOUT),
                      hWnd,
                      (DLGPROC) AboutDialogProc);
} // DoAboutBox

// ========================================================================
// Name.......: LaunchBrowser()
// Description: This function launches the default Internet browser with the
//				given URL.
// Parameters.: tURL					- string containing the URL
// Returns....: NIL
// ========================================================================
static void
LaunchBrowser (LPCTSTR tURL)
{
    FILE		*pTempFile;
    LPTSTR		tResult[iMAX_PATH_LEN];
    HINSTANCE	hRetVal;

    pTempFile = fopen(tTEMP_HTML_FILENAME, "wb");
    if (pTempFile == (FILE *) 0)
    {
        DisplayErrorMessage(iERR_RENDER_FILE_WRITE);
        return;
    }

    fprintf (pTempFile, "<HTML> <\\HTML>\n");
    fclose (pTempFile);

    *tResult = '\0';
    hRetVal = FindExecutable (tTEMP_HTML_FILENAME, NULL, (LPTSTR) tResult);
    if ((int)hRetVal <= 32 || *tResult == '\0')
    {
        DisplayErrorMessage(iERR_RENDER_BROWSER);
        DeleteFile(tTEMP_HTML_FILENAME);
        return;
    }
    else
    {
        hRetVal = ShellExecute(hWnd, "open", (LPCTSTR) tResult, tURL, NULL, SW_SHOWNORMAL);
        if ((int)hRetVal <= 32)
        {
            DisplayErrorMessage(iERR_RENDER_URL);
            DeleteFile(tTEMP_HTML_FILENAME);
            return;
        }
    }

    DeleteFile(tTEMP_HTML_FILENAME);
}

// ========================================================================
// Name.......: SettingsDialogProc()
// Description: This function provides the processing of window messages for
//              the Settings dialog box.
// Parameters.: hDlg					- handle to the window
//              uMsg					- the window message
//              wParam					- first message parameter
//              lParam					- second message parameter
// Returns....: BOOL CALLBACK			- result of message processing
// ========================================================================
static BOOL CALLBACK
SettingsDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL			bSuccess;
    HWND			hTextBox;
    TCHAR			tSelection[iREG_KEY_BUF_SIZE];
    TCHAR			szTempDir[MAX_PATH];
    TCHAR			szTempName[MAX_PATH];
    BROWSEINFO		biInfo;
    LPITEMIDLIST	pidl;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        hTextBox = GetDlgItem(hDlg, IDC_Q2PATH);
        SendMessage(hTextBox, WM_SETTEXT, 0, (LPARAM) " ");

        if (LoadRegValue (tREG_KEY_Q2PATH, tSelection) == iRC_OK)
        {
            SendMessage(hTextBox, WM_SETTEXT, 0, (LPARAM) tSelection);
        }
        else
        {
            SendMessage(hTextBox, WM_SETTEXT, 0, (LPARAM) tQUAKE2_PATH);
        }

        EnableWindow (hDlg, TRUE);
        ShowWindow (hDlg, SW_SHOW);
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDBROWSE:
            biInfo.hwndOwner = GetParent(hDlg);
            biInfo.pidlRoot = NULL;
            biInfo.pszDisplayName = szTempName;
            biInfo.lpszTitle = "Please select the Quake 2 directory:";
            biInfo.ulFlags = BIF_RETURNONLYFSDIRS;
            biInfo.lpfn = NULL;
            biInfo.lParam = NULL;
            biInfo.iImage = 0;
            pidl = SHBrowseForFolder (&biInfo);
            if (pidl != NULL)
            {
                bSuccess = SHGetPathFromIDList(pidl, szTempDir);
                if (bSuccess == TRUE)
                {
                    hTextBox = GetDlgItem(hDlg, IDC_Q2PATH);
                    SendMessage(hTextBox, WM_SETTEXT, 0, (LPARAM) szTempDir);
                }

                ILFree (pidl);
            }
            break;

        case IDOK:
            SendDlgItemMessage(hDlg,
                               IDC_Q2PATH,
                               WM_GETTEXT,
                               sizeof(tSelection),
                               (LPARAM) tSelection);

            if (ValidateQuake2Path((LPCTSTR)tSelection) == TRUE)
            {
                SaveRegValue (tREG_KEY_Q2PATH, tSelection);

                EndDialog (hDlg, TRUE);
                return TRUE;
            }
            else
            {
                MessageBox (hDlg, "Unable to find the 'players' subfolder. Please check the folder name.", "Crusher Error", MB_ICONWARNING | MB_OK);
                return FALSE;
            }

        case IDCANCEL:
            EndDialog (hDlg, FALSE);
            return TRUE;
        }
        break;
    }
    return FALSE;
} // SettingsDialogProc

// ========================================================================
// Name.......: UnrExportDialogProc()
// Description: This function provides the processing of window messages for
//              the 'Export to Unreal' dialog box.
// Parameters.: hDlg					- handle to the window
//              uMsg					- the window message
//              wParam					- first message parameter
//              lParam					- second message parameter
// Returns....: BOOL CALLBACK			- result of message processing
// ========================================================================
static BOOL CALLBACK
UnrExportDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL			bSuccess;
	BOOL			bChecked;
    BROWSEINFO		biInfo;
    HWND			hTextBox;
    LPITEMIDLIST	pidl;
    TCHAR			tSelection[iREG_KEY_BUF_SIZE];
    TCHAR			szTempDir[MAX_PATH];
    TCHAR			szTempName[MAX_PATH];
	TCHAR			tTempProjDir[iMAX_PATH_LEN];
	TCHAR			tTempBaseName[iMAX_PATH_LEN];
	TCHAR			tTempSkinName[iMAX_PATH_LEN];
	TCHAR			*pBaseName;
	TCHAR			*pProjDir;
	TCHAR			*pSkinName;
	UINT			rc;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        hTextBox = GetDlgItem(hDlg, IDC_PROJDIR);
        if (LoadRegValue (tREG_KEY_UNR_PROJDIR, tSelection) == iRC_OK)
        {
            SendMessage(hTextBox, WM_SETTEXT, 0, (LPARAM) tSelection);
        }
        else
        {
            SendMessage(hTextBox, WM_SETTEXT, 0, (LPARAM) tUNREAL_PROJECT_DIR);
        }

		pBaseName = tTempBaseName;
		GetBaseName(&pBaseName);

        hTextBox = GetDlgItem(hDlg, IDC_CLASSNAME);
        SendMessage(hTextBox, WM_SETTEXT, 0, (LPARAM) pBaseName);

		hTextBox = GetDlgItem(hDlg, IDC_CHKSEQ);
		if (md2Model.m_num_frames == 198)
		{
			EnableWindow (hTextBox, TRUE);
			CheckDlgButton (hDlg, IDC_CHKSEQ, BST_CHECKED);
		}
		else
		{
			EnableWindow (hTextBox, FALSE);
		}

        EnableWindow (hDlg, TRUE);
        ShowWindow (hDlg, SW_SHOW);
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDBROWSE:
            biInfo.hwndOwner = GetParent(hDlg);
            biInfo.pidlRoot = NULL;
            biInfo.pszDisplayName = szTempName;
            biInfo.lpszTitle = "Please select the Unreal project directory:";
            biInfo.ulFlags = BIF_RETURNONLYFSDIRS;
            biInfo.lpfn = NULL;
            biInfo.lParam = NULL;
            biInfo.iImage = 0;
            pidl = SHBrowseForFolder (&biInfo);
            if (pidl != NULL)
            {
                bSuccess = SHGetPathFromIDList(pidl, szTempDir);
                if (bSuccess == TRUE)
                {
                    hTextBox = GetDlgItem(hDlg, IDC_PROJDIR);
                    SendMessage(hTextBox, WM_SETTEXT, 0, (LPARAM) szTempDir);
                }

                ILFree (pidl);
            }
            break;

        case IDOK:
            SendDlgItemMessage(hDlg,
                               IDC_PROJDIR,
                               WM_GETTEXT,
                               sizeof(tSelection),
                               (LPARAM) tSelection);

			pProjDir = tTempProjDir;
			strcpy (pProjDir, tSelection);

            SendDlgItemMessage(hDlg,
                               IDC_CLASSNAME,
                               WM_GETTEXT,
                               sizeof(tSelection),
                               (LPARAM) tSelection);

			pBaseName = tTempBaseName;
			strcpy (pBaseName, tSelection);

			pSkinName = tTempSkinName;
			GetSkinName(&pSkinName);

	        if (IsDlgButtonChecked(hDlg, IDC_CHKSEQ) == BST_CHECKED)
			{
				bChecked = TRUE;
			}
			else
			{
				bChecked = FALSE;
			}

			if ((rc = md2Model.ExportToUnrealMesh (pProjDir,
											pBaseName,
											pSkinName,
											bChecked)) == iRC_OK)
			{
				SaveRegValue (tREG_KEY_UNR_PROJDIR, pProjDir);
			}
			else
			{
				DisplayErrorMessage(rc);
			}

            EndDialog (hDlg, FALSE);
			return TRUE;

        case IDCANCEL:
            EndDialog (hDlg, FALSE);
            return TRUE;
        }
        break;
    }
    return FALSE;
} // UnrExportDialogProc

// ========================================================================
// Name.......: SetWindowTitle()
// Description: This function sets the window title.
// Parameters.: hWnd					- handle to window
//				md2Model					- a pointer to the model
// Returns....: NIL
// ========================================================================
static void
SetWindowTitle (HWND hWnd, const CMd2Model &mdlMd2)
{
    TCHAR			tWindowTitle[255];
    TCHAR			tFilename[iMAX_PATH_LEN];
    LPSTR			cTemp;

    sprintf (tWindowTitle, "%s - [", TITLE);

    strcpy (tFilename, mdlMd2.m_modelname);
    cTemp = tFilename + strlen(tFilename) - 1;
    while (cTemp != tFilename && *cTemp != '\\')
    {
        cTemp--;
    }

    if (cTemp == tFilename)
    {
        strcpy (tFilename, tDEFAULT);
    }
    else
    {
        cTemp--;
        while (cTemp != tFilename && *cTemp != '\\')
        {
            cTemp--;
        }

        if (cTemp == tFilename)
        {
            strcpy (tFilename, tDEFAULT);
        }
        else
        {
            strcpy (tFilename, cTemp+1);
            cTemp = tFilename;
            while (cTemp != '\0' && *cTemp != '\\')
            {
                cTemp++;
            }
            *cTemp = '\0';
        }
    }

    sprintf (tWindowTitle, "%s %s :", tWindowTitle, tFilename);

    strcpy (tFilename, mdlMd2.m_modelname);

    cTemp = tFilename + strlen(tFilename) - 1;
    while (cTemp != tFilename && *cTemp != '\\')
    {
        cTemp--;
    }

    if (cTemp == tFilename)
    {
        strcpy (tFilename, tDEFAULT);
    }
    else
    {
        strcpy (tFilename, cTemp+1);
    }

    strcpy (tFilename, mdlMd2.m_modelname);
    cTemp = tFilename + strlen(tFilename) - 1;
    while (cTemp != tFilename && *cTemp != '\\')
    {
        cTemp--;
    }

    if (cTemp == tFilename)
    {
        strcpy (tFilename, tDEFAULT);
    }
    else
    {
        strcpy (tFilename, cTemp+1);
    }

    sprintf (tWindowTitle, "%s %s /", tWindowTitle, tFilename);

    strcpy (tFilename, mdlMd2.m_skinname);
    cTemp = tFilename + strlen(tFilename) - 1;
    while (cTemp != tFilename && *cTemp != '\\')
    {
        cTemp--;
    }

    if (cTemp == tFilename)
    {
        strcpy (tFilename, tDEFAULT);
    }
    else
    {
        strcpy (tFilename, cTemp+1);
    }

    sprintf (tWindowTitle, "%s %s ]", tWindowTitle, tFilename);

    SetWindowText (hWnd, tWindowTitle);
} // SetWindowTitle

// ========================================================================
// Name.......: ValidateQuake2Path()
// Description: This function checks to see if the given directory is a
//				valid Quake 2 directory.
// Parameters.: tQuake2Path				- the given Quake 2 path
// Returns....: BOOL					- boolean for valid path
// ========================================================================
static BOOL
ValidateQuake2Path (LPCTSTR tQuake2Path)
{
    TCHAR	tQ2Player[iMAX_PATH_LEN];
    TCHAR	ch;

    strcpy (tQ2Player, tQuake2Path);

    if (SetCurrentDirectory (tQuake2Path) == FALSE)
    {
        return FALSE;
    }

    ch = *(tQ2Player + strlen(tQ2Player));
    if (ch != '\\' && ch != '/')
    {
        strcat (tQ2Player, "\\");
    }

    strcat (tQ2Player, tQUAKE2_PLAYER_DIR);

    if (SetCurrentDirectory (tQ2Player) == TRUE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
} // ValidateQuake2Path

// ========================================================================
// Name.......: atoui()
// Description: This function converts a text string into an unsigned int.
// Parameters.: sGiven					- the given text string
// Returns....: UINT					- the resulting unsigned int
// ========================================================================
static UINT
atoui (LPCTSTR sGiven)
{
	UINT uValue = 0;
	UINT uMax;

	while ((*sGiven == ' ' || *sGiven == '\t') && *sGiven != '\0')
	{
		sGiven++;
	}

	if (*sGiven == '\0')
	{
		return 0;
	}
	
	uMax = (UINT) (UINT_MAX / 10);
	uMax++;
	while ((*sGiven >= TEXT('0')) && (*sGiven <= TEXT('9')))
	{
		if (uValue < uMax)
		{
			uValue = ((uValue * 10) + (*sGiven++ - TEXT('0')));
		}
	}
	
	return uValue;
} // atoui

static void
GetBaseName(TCHAR **lpString)
{
    LPSTR			cTemp;

    strcpy (*lpString, md2Model.m_modelname);
    cTemp = *lpString + strlen(*lpString) - 1;
    while (cTemp != *lpString && *cTemp != '\\')
    {
        cTemp--;
    }

    if (cTemp == *lpString)
    {
        strcpy (*lpString, tDEFAULT);
    }
    else
    {
        cTemp--;
        while (cTemp != *lpString && *cTemp != '\\')
        {
            cTemp--;
        }

        if (cTemp == *lpString)
        {
            strcpy (*lpString, tDEFAULT);
        }
        else
        {
            strcpy (*lpString, cTemp+1);
            cTemp = *lpString;
            while (cTemp != '\0' && *cTemp != '\\')
            {
                cTemp++;
            }
            *cTemp = '\0';
        }
    }
}

static void
GetSkinName(TCHAR **lpString)
{
    LPSTR			cTemp;

    strcpy (*lpString, md2Model.m_skinname);
    cTemp = *lpString + strlen(*lpString) - 1;
    while (cTemp != *lpString && *cTemp != '\\')
    {
        cTemp--;
    }

    if (cTemp == *lpString)
    {
        strcpy (*lpString, tDEFAULT);
    }
    else
    {
        strcpy (*lpString, cTemp+1);
    }
}