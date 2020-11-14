/*  -== ExeLoader ==-
 *
 *  Load .exe / .dll from memory and remap functions
 *  Run your binaries on any x86 hardware
 *
 *  @autors
 *   - Maeiky
 *  
 * Copyright (c) 2020 - V·Liance / SPinti-Software. All rights reserved.
 *
 * The contents of this file are subject to the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * If a copy of the Apache License Version 2.0 was not distributed with this file,
 * You can obtain one at https://www.apache.org/licenses/LICENSE-2.0.html

* Description:
* 
* Some utils function to view pixels from memory, 
* we need to create window to view visually the memory
*
*/

struct pixel {
  union {
	struct {unsigned char b, g, r, a;};
    int val;
  };
  pixel():val(0){}
};

// Window client size
const int width = 800;
const int height = 600;

HBITMAP hbmp;
HANDLE hTickThread;
HWND hwnd;
HDC hdcMem;

pixel* pixels;
pixel** container_pixels;

void MakeSurface(HWND hwnd) {
  BITMAPINFO bmi;
  bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight =  -height; //Order pixels from top to bottom
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32; //last byte not used, 32 bit for alignment
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = 0;
  bmi.bmiHeader.biXPelsPerMeter = 0;
  bmi.bmiHeader.biYPelsPerMeter = 0;
  bmi.bmiHeader.biClrUsed = 0;
  bmi.bmiHeader.biClrImportant = 0;
  bmi.bmiColors[0].rgbBlue = 0;
  bmi.bmiColors[0].rgbGreen = 0;
  bmi.bmiColors[0].rgbRed = 0;
  bmi.bmiColors[0].rgbReserved = 0;

  HDC hdc = GetDC( hwnd );
  container_pixels = &pixels;
  hbmp = CreateDIBSection( hdc, &bmi, DIB_RGB_COLORS, (void**)container_pixels, NULL, 0 );
  DeleteDC( hdc );
}

LRESULT CALLBACK WndProc( HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
  switch ( msg ) {
    case WM_CREATE:
      {
        MakeSurface( hwnd );
      }
      break;
    case WM_PAINT:
      {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint( hwnd, &ps );

        // Draw pixels to window when window needs repainting
        BitBlt( hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY );

        EndPaint( hwnd, &ps );
      }
      break;
    case WM_CLOSE:
      {
        DestroyWindow( hwnd );
      }
      break;
    case WM_DESTROY:
      {
        TerminateThread( hTickThread, 0 );
        PostQuitMessage( 0 );
      }
      break;
    default:
      return DefWindowProc( hwnd, msg, wParam, lParam );
  }
  return 0;
}

HWND pixView_createWindow( HINSTANCE hInstance){
  WNDCLASSEX wc;
  
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.cbSize = sizeof( WNDCLASSEX );
  wc.hbrBackground = CreateSolidBrush( 0 );
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );
  wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
  wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
  wc.hInstance = hInstance;
  wc.lpfnWndProc = WndProc;
  wc.lpszClassName = "pixview_class";
  wc.lpszMenuName = NULL;
  wc.style = 0;

  if ( !RegisterClassEx(&wc) ) {
    MessageBox( NULL, "Failed to register window class.", "Error", MB_OK );
    return 0;
  }

  hwnd = CreateWindowEx(
    WS_EX_APPWINDOW,
    "pixview_class",
    "pixview",
    WS_MINIMIZEBOX | WS_SYSMENU | WS_POPUP | WS_CAPTION,
    300, 200, width, height,
    NULL, NULL, hInstance, NULL );

  RECT rcClient, rcWindow;
  POINT ptDiff;

  GetClientRect( hwnd, &rcClient );
  GetWindowRect( hwnd, &rcWindow );
  ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
  ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
  MoveWindow( hwnd, rcWindow.left, rcWindow.top, width + ptDiff.x, height + ptDiff.y, false);
  
  return hwnd;
}

void pixView_update(HWND _hwnd){
	if(_hwnd == 0){return;}
	UpdateWindow( _hwnd );
	MSG _msg;
	ShowWindow( _hwnd, SW_SHOW );
	
	while ( PeekMessageA(&_msg, 0, 0, 0, PM_REMOVE) > 0 ) {
		TranslateMessage( &_msg );
		DispatchMessage( &_msg );
	}
	HDC hdc = GetDC( _hwnd );
	hdcMem = CreateCompatibleDC( hdc );
	HBITMAP hbmOld = (HBITMAP)SelectObject( hdcMem, hbmp );

	BitBlt( hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY );

	SelectObject( hdcMem, hbmOld );
	DeleteDC( hdc );
}