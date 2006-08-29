/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <oorexx.h>
#include <stdio.h>
#include <dlgs.h>
#ifdef __CTL3D
#include <ctl3d.h>
#endif
#include "oovutil.h"


extern LPBITMAPINFO LoadDIB(LPSTR szFile);


bool DrawButton(DIALOGADMIN * aDlg, INT id)
{
   HWND hW = NULL;
   DRAWITEMSTRUCT dis;
   RECT r;
   bool rc;

   dis.CtlType = ODT_BUTTON;
   dis.CtlID = id;
   dis.itemAction = ODA_DRAWENTIRE;
   dis.itemState = SendDlgItemMessage(aDlg->TheDlg, dis.CtlID, BM_GETSTATE, 0, 0);
   hW = GetDlgItem(aDlg->TheDlg, dis.CtlID);
   dis.hDC = GetWindowDC(hW);
   dis.hwndItem = hW;
   GetWindowRect(hW, &r);
   r.bottom = r.bottom - r.top;
   r.right = r.right - r.left;
   r.top = 0;
   r.left = 0;
   dis.rcItem = r;
   dis.itemData = 0;
   rc = SendMessage(aDlg->TheDlg, WM_DRAWITEM, (WPARAM)dis.CtlID, (LPARAM)&dis);
   ReleaseDC(hW, dis.hDC);
   return rc;
}


RexxFunction2(int, ClearWindow, uintptr_t, admin, uintptr_t, handle)
{

    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)handle;
    HWND w = (HWND)handle;

    HDC hDC;

    if ((hDC=GetWindowDC(w)))
    {
        GetClientRect(w, &r);      /* no coordinates, so get coordinates */

        if (dlgAdm->Use3DControls)
        {
           hbr = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
           hpen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
        }
        else
        {
           hbr = GetStockObject(WHITE_BRUSH);
           hpen = GetStockObject(WHITE_PEN);
        }
        oP = SelectObject(hDC, hpen);
        oB = SelectObject(hDC, hbr);
        Rectangle(hDC, r.left, r.top, r.right, r.bottom);
        SelectObject(hDC, oB);
        SelectObject(hDC, oP);

        if (dlgAdm->Use3DControls)
        {
           DeleteObject(hpen);
           DeleteObject(hbr);
        }
        ReleaseDC(w, hDC);
        return 0;
    }
    return 1;
}


RexxFunction6(int, ClearWindowRectangle, uintptr_t, admin, uintptr_t, handle, int, left, int, top, int, right, int, bottom)
{

    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)handle;
    HWND w = (HWND)handle;

    HDC hDC;

    if ((hDC=GetWindowDC(w)))
    {
        if (dlgAdm->Use3DControls)
        {
           hbr = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
           hpen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
        }
        else
        {
           hbr = GetStockObject(WHITE_BRUSH);
           hpen = GetStockObject(WHITE_PEN);
        }
        oP = SelectObject(hDC, hpen);
        oB = SelectObject(hDC, hbr);
        Rectangle(hDC, left, top, right, bottom);
        SelectObject(hDC, oB);
        SelectObject(hDC, oP);

        if (dlgAdm->Use3DControls)
        {
           DeleteObject(hpen);
           DeleteObject(hbr);
        }
        ReleaseDC(w, hDC);
        return 0;
    }
    return 1;
}


RexxFunction2(int, RedrawWindow, uintptr_t, handle, RexxBoolean, eraseBackground)
{
    HWND w = (HWND)handle;
    RECT r;

    GetClientRect(w, &r);               /* no coordinates, so get coordinates */
    if (InvalidateRect(w, &r, eraseBackground))
    {
       UpdateWindow(w);
       return 0;
    }
    return 1;
}


RexxFunction1(int, RedrawWindowRectangle, uintptr_t, handle, RexxBoolean, eraseBackground, int, left, int, top, int, right, int, bottom)
{
    HWND w = (HWND)handle;
    RECT r;

    r.left = left;
    r.top =  top;
    r.right = right;
    r.bottom = bottom;
    if (InvalidateRect(w, &r, eraseBackground)
    {
       UpdateWindow(w);
       return 0;
    }
    return 1;
}


RexxFunction2(RexxStringObject, GetClientRectangle, uintptr_t, handle)
{
    HWND w = (HWND)handle;
    RECT r;

    char buffer[256];
    buffer[0] = '\0';

    if (GetClientRect(w, &r))
    {
       sprintf(buffer, "%ld %ld %ld %ld", r.left, r.top, r.right, r.bottom);
    }
    return context->NewStringFromAsciiz(buffer);
}


RexxFunction2(RexxStringObject, GetWindowRectangle, uintptr_t, handle)
{
    HWND w = (HWND)handle;
    RECT r;

    char buffer[256];
    buffer[0] = '\0';

    if (GetWindowRect(w, &r))
    {
       sprintf(buffer, "%ld %ld %ld %ld", r.left, r.top, r.right, r.bottom);
    }
    return context->NewStringFromAsciiz(buffer);
}

RexxFunction2(int, RedrawButton, uintptr_t, admin, int, id)
{
    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)admin;

    return  DrawButton(dlgAdm, id) ? 0 : 1;
}


RexxFunction1(uintptr_t, GetDrawingContext, uintptr_t, handle)
{
    return GetWindowDC((HWND)handle);
}


RexxFunction2(int, FreeDrawingContext, uintptr_t, handle, uintptr_t, dc)
{
    return ReleaseDC((HWND)handle, (HDC)dc) ? 0 : 1;
}

RexxFunction2(uintptr_t, SelectDrawingObject, uinptr_t, dc, uinptr_t, object)
{
    return SelectObject((HDC)dc, (HGDIOBJ)object);
}

RexxFunction1(int, DeleteDrawingObject, uinptr_t, object)
{
    return !DeleteObject((HGDIOBJ)object);
}

RexxFunction3(uintptr_t, CreatePen, int, width, CSTRING, options, int, palette)
{
    UINT style;

    if (!strcmp(options, "DASH")) style = PS_DASH; else
    if (!strcmp(options, "DOT")) style = PS_DOT; else
    if (!strcmp(options, "DASHDOT")) style = PS_DASHDOT; else
    if (!strcmp(options, "DASHDOTDOT")) style = PS_DASHDOTDOT; else
    if (!strcmp(options, "NULL")) style = PS_NULL; else
    style = PS_SOLID;

    return CreatePen(style, width, PALETTEINDEX(palette));
}


RexxFunction2(uintptr_t, CreateSolidBrush, OPTIONAL_int, palette, OPTIONAL_CSTRING, options)
{
    HBRUSH hB;
    HBITMAP hBmp;
    LOGBRUSH lb;
    LONG lbmp;

    if (options != NULL)                   /* styled color brush */
    {
        lb.lbStyle = BS_HATCHED;
        lb.lbColor = PALETTEINDEX(pallette);
        if (!stricmp(options, "UPDIAGONAL")) lb.lbHatch = HS_BDIAGONAL;
        else if (!stricmp(options, "DOWNDIAGONAL")) lb.lbHatch = HS_FDIAGONAL;
        else if (!stricmp(options, "CROSS")) lb.lbHatch = HS_CROSS;
        else if (!stricmp(options, "DIAGCROSS")) lb.lbHatch = HS_DIAGCROSS;
        else if (!stricmp(options, "HORIZONTAL")) lb.lbHatch = HS_HORIZONTAL;
        else if (!stricmp(options, "VERTICAL")) lb.lbHatch = HS_VERTICAL;
        else lb.lbStyle = BS_SOLID;
        return CreateBrushIndirect(&lb);
    }
    else if (argumentExists(1))
    {
        return CreateSolidBrush(PALETTEINDEX(palette));
    }
    return GetStockObject(HOLLOW_BRUSH);
}


RexxFunction3(uintptr_t, CreatePatternBrush, uintptr_t, admin, OPTIONAL_int, id, OPTIONAL_CSTRING, filename)
{
    HBRUSH hB;
    HBITMAP hBmp;
    LOGBRUSH lb;
    LONG lbmp;

    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)admin;
    // give a file name to load?
    if (filename != NULL)
    {
        hBmp = (HBITMAP) LoadDIB(filename);           /* we have a file name */
        lb.lbStyle = BS_DIBPATTERNPT;
        lb.lbColor = DIB_RGB_COLORS;
        lb.lbHatch = (LONG)hBmp;
        hB = CreateBrushIndirect(&lb);
        LocalFree((void *)hBmp);
    }
    else   // have a resource id
    {

        hBmp = LoadBitmap(dlgAdm->TheInstance, MAKEINTRESOURCE(id));
        hB = CreatePatternBrush(hBmp);
        DeleteObject(hBmp);
    }
    return hB;
}


RexxFunction3(int, DrawLineTo, uintptr_t, dc, int, x, int, y)
{
    return !LineTo((HDC)dc, x, y);
}

RexxFunction5(int, DrawRectangle, uintptr_t, dc, int, left, int, top, int, right, int, bottom)
{
    return !Rectangle((HDC)dc, left, top, right, bottom)
}

RexxFunction4(int, DrawPixel, uintptr_t, dc, int, x, int, y, int, palette)
{
    return !SetPixel((HDC)dc, x, y, PALETTEINDEX(palette));
}


RexxFunction9(int, DrawArc, uintptr_t, dc, int, left, int, top, int, right, int, bottom, int, x, int, y, int, endx, int, endy)
{
    return !Arc((HDC)dc, left, top, right, bottom, x, y, endx, endy);
}

RexxFunction6(int, DrawAngleArc, uintptr_t, dc, int, x, int, y, int, radius, float, start, float, sweep)
{
    return !AngleArc((HDC)dc, x, y, radius, start, sweep);
}

RexxFunction9(int, DrawPie, uintptr_t, dc, int, left, int, top, int, right, int, bottom, int, firstx, int, firsty, int, secondx, int, secondy)
{
    return !Pie((HDC)dc, left, top, right, bottom, firstx, firsty, secondx, secondy);
}


RexxFunction4(int, FloodFill, uintptr_t, dc, int, x, int, y, int, color)
{
    return !ExtFloodFill((HDC)dc, x, y, PALETTEINDEX(color));
}

RexxFunction5(int, DimBitmap, uintptr_t, dc, int, sx, int, sy, int, stepsx, int, stepx, int, stepy)
{
    int   diff = steps * stepy;
    int   diffx = steps * stepx;

    for (int alpha = 0; alpha < steps; alpha++)
    {
        for (y = a*stepy, i = 0; i < sy / steps; y+=diff, i++)
        {
            for (x = a*stepx, j = 0; j < sx / steps; x+=diffx, j++)
            {
                Rectangle((HDC)dc, x - alpha * stepx, y - alpha * stepy, x + stepx + 1, y + stepy + 1);
            }
        }
    }
    return 0;
}


RexxFunction3(int, DrawMoveTo, uintptr_t, dc, int, x, int, y)
{
    return !MoveToEx((HDC)dc, x, y);
}

RexxFunction3(int, GetPixelColor, uintptr_t, dc, int, x, int, y)
{
    return GetPixel((HDC)dc, x, y);
}

RexxFunction2(int, SetTextColor, uintptr_t, dc, int, color)
{
    return !SetTextColor((HDC)dc, PALETTEINDEX(color));
}

RexxFunction2(int, SetBackgroundMode, uintptr_t, dc, CSTRING, mode)
{
    INT bkmode;
    if (!strcmp(mode, "TRANSPARENT")) bkmode = TRANSPARENT; else bkmode = OPAQUE;
    return SetBkMode((HDC)dc, bkmode);
}

RexxFunction1(RexxBoolean, GetArcDirection, uintptr_t, dc)
{
    return GetArcDirection((HDC)dc) == AD_CLOCKWISE))
}

RexxFunction2(int, SetArcDirection, uintptr_t, dc, RexxBoolean, dir)
{
    return SetArcDirection((HDC)dc, dir ? AD_CLOCKWISE : AD_COUNTERCLOCKWISE);
}

RexxFunction2(int, SetBackgroundBitmap, uintptr_t, admin, uintptr_t, id)
{
    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)admin;
    dlgAdm->BkgBitmap = (HBITMAP)id;
}

RexxFunction2(int, SetBackgroundBrush, uintptr_t, admin, uintptr_t, id)
{
    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)admin;
    dlgAdm->BkgBrush = (HBRUSH)id;
}

RexxFunction4(int, SetBackgroundColor, uintptr_t, admin, int, id, int, bkColor, int, fgColor)
{
    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)admin;
    HBRUSH hbrush;
    int i;

    if (!dlgAdm->ColorTab)
    {
        dlgAdm->ColorTab = LocalAlloc(LMEM_FIXED, sizeof(COLORTABLEENTRY) * MAXTABLEENTRIES);
        if (!dlgAdm->ColorTab)
        {
            MessageBox(0,"No memory available","Error",MB_OK | MB_ICONHAND);
            return -1;
        }
        dlgAdm->CT_size = 0;
    }
    SEARCHBRUSH(dlgAdm, i, id, hbrush);
    if (hbrush)
    {
        DeleteObject(hbrush);
        dlgAdm->ColorTab[i].ColorBk = bkColor;
        dlgAdm->ColorTab[i].ColorFG = fgColor;
        dlgAdm->ColorTab[i].ColorBrush = (HBRUSH)CreateSolidBrush(PALETTEINDEX(dlgAdm->ColorTab[i].ColorBk));
    }
    else
    {
        dlgAdm->ColorTab[dlgAdm->CT_size].itemID = id;
        dlgAdm->ColorTab[dlgAdm->CT_size].ColorBk = bkColor;
        if (argc == 5) dlgAdm->ColorTab[i].ColorFG = fgColor;
        dlgAdm->ColorTab[dlgAdm->CT_size].ColorBrush = (HBRUSH)CreateSolidBrush(PALETTEINDEX(dlgAdm->ColorTab[dlgAdm->CT_size].ColorBk));
        dlgAdm->CT_size++;
    }
    return 1;
}

