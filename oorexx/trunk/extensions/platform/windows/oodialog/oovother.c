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
#include <mmsystem.h>
#include <oorexx.h>
#include <stdio.h>
#include <dlgs.h>
#include <malloc.h>
#include "oovutil.h"
#include <commctrl.h>

#define FILENAME_BUFFER_LEN 65535

extern LONG SetRexxStem(CHAR * name, INT id, char * secname, CHAR * data);
WORD NumDIBColorEntries(LPBITMAPINFO lpBmpInfo);
extern LPBITMAPINFO LoadDIB(LPSTR szFile);
extern LONG EvaluateListStyle(CHAR * styledesc);


RexxFunction2(int, PlaySoundFile, CSTRING, filename, RexxBoolean, async)
{

    return !sndPlaySound(filename, (async ? SND_ASYNC : SND_SYNC) | SND_NODEFAULT);
}


RexxFunction1(int, PlaySoundFileInLoop, CSTRING, filename)
{
    return !sndPlaySound(filename, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
}

RexxFuntion0(int, StopSoundFile)
{
    return !sndPlaySound(NULL, SND_SYNC | SND_NODEFAULT);
}


UINT APIENTRY OFNSetForegroundHookProc(
    HWND hdlg,    // handle to child dialog window
    UINT uiMsg,    // message identifier
    WPARAM wParam,    // message parameter
    LPARAM lParam     // message parameter
)
{
    if (uiMsg == WM_INITDIALOG)
    {
        HWND h = GetParent(hdlg);
        if (!h) h = hdlg;
        SetForegroundWindow(h);
    }
    return 0;   /* 0 means default routine handles message */
}


bool OpenFileDlg( bool load, char * szFile, char * szFilter, HWND hw, char * title, char * DefExt, bool multi, CHAR chSepChar) /* @DOE005M */
{
   OPENFILENAME OpenFileName;
   bool         fRc;

   OpenFileName.lStructSize       = sizeof(OPENFILENAME);
   OpenFileName.hwndOwner         = hw;
   OpenFileName.hInstance         = 0;
   OpenFileName.lpstrFilter       = szFilter;
   OpenFileName.lpstrCustomFilter = (LPSTR) NULL;
   OpenFileName.nMaxCustFilter    = 0;
   OpenFileName.nFilterIndex      = 1;
   OpenFileName.lpstrFile         = szFile;
   OpenFileName.nMaxFile          = FILENAME_BUFFER_LEN;
   OpenFileName.lpstrFileTitle    = NULL; /* we don't need the selected file */
   OpenFileName.nMaxFileTitle     = 0;    /* we don't need the selected file */
   OpenFileName.lpstrInitialDir   = NULL;
   OpenFileName.lpstrTitle        = title;
   OpenFileName.nFileOffset       = 0;
   OpenFileName.nFileExtension    = 0;
   OpenFileName.lpstrDefExt       = DefExt;
   OpenFileName.lCustData         = 0;
   OpenFileName.lpfnHook          = OFNSetForegroundHookProc;   /* hook to set dialog to foreground */

   /* The OFN_EXPLORER flag is required when using OFN_ENABLEHOOK, otherwise the dialog is old style and does not change directory */
   OpenFileName.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLEHOOK | OFN_EXPLORER;   /* enable hook */

   if (load && multi) OpenFileName.Flags |= OFN_ALLOWMULTISELECT;

   if (load)
   {
       OpenFileName.Flags |= OFN_FILEMUSTEXIST;
       fRc = GetOpenFileName(&OpenFileName);

       if (fRc && multi)
       {
         /* OFN_EXPLORER returns the selected name separated with ASCII 0 instead of spaces */
         char * pChr = szFile;

         while( (*pChr != 0) || (*(pChr+1) != 0))
         {
           if (*pChr == 0)
             *pChr =  chSepChar;
           pChr++;
         }
       }

       return fRc;
   }
   else
   {
       OpenFileName.Flags |= OFN_OVERWRITEPROMPT;
       return GetSaveFileName(&OpenFileName);
   }
}



#define VALIDARG(argn) (argc >= argn) && argv[argn-1].strptr && argv[argn-1].strptr[0]

RexxFunction8(RexxStringObject, GetFileNameWindow, OPTIONAL_CSTRING, files, OPTIONAL_uintptr_t, handle, OPTIONAL_CSTRING, filter,
    RexxBoolean, load, OPTIONAL_CSTRING, title, OPTIONAL_CSTRING, defext, OPTIONAL_RexxBoolean, multi, OPTIONAL_CSTRING, separator)
{
    char * title;
    HWND hWnd = (HWND)handle;
    char * pszFiles = NULL;
    char chSepChar = ' ';  /* default separation character  /                */
                           /* allow to change separation character to handle */
                           /* filenames with blank character                 */

    pszFiles = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, FILENAME_BUFFER_LEN);
    {
        context->InvalidRoutine();
        return 0;
    }
    if (files != NULL)
    {
        strcpy(pszFiles, files);
    }

    if (filter == NULL)
    {
        filter = "Text Files (*.TXT)\0*.TXT\0All Files (*.*)\0*.*\0";
    }
    if (title == NULL)
    {
        title = load ? "Open a File" : "Save File As";
    }

    if (defext == NULL || strlen(defext) == 0)
    {
        defext = "TXT";
    }

    if (separator != NULL)
    {
        chSepChar = *separator;
        if (chSepChar == '\0')
        {
            chSepChar = ' ';
        }
    }

    if (OpenFileDlg(load, pszFiles, szFilter, hWnd, title, defext, multi, chSepChar))
    {
        RexxStringObject result = context->NewStringFromAsciiz(pszFiles);
        GlobalFree(pszFiles);
        return result;
    }

    return context->NewStringFromAsciiz("");
}


RexxFunction3(int, PlaySound, uintptr_t, admin, int, id, RexxBoolean, async)
{
    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)admin;
    return !PlaySound(MAKEINTRESOURCE(id), dlgAdm->TheInstance, SND_RESOURCE | (async ? SND_ASYNC : SND_SYNC) | SND_NODEFAULT);
}


RexxFunction1(int, SleepMS, int, ms)
{
    Sleep(ms);
    return 0;
}

RexxFunction1(uintptr_t, StartWinTimer, int, interval)
{
    return SetTimer(NULL, 1001, interval, NULL);
}

RexxFunction1(int, StopWinTimer, uintptr_t, timer)
{
    KillTimer(NULL, timer);
    return 0;
}

RexxFunction2(int, WaitForWinTimer, uintptr_t, timer)
{
    MSG msg;
    while (!PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE) || (msg.wParam != timer)) {};
    return 0;
}


HIMAGELIST CreateImageList(HWND h, HBITMAP hBmp, int cx, int cy)
{
   HIMAGELIST iL;
   BITMAP bmpInfo;

   if (hBmp == 0)
   {
       return NULL;
   }

   GetObject(hBmp, sizeof(BITMAP), &bmpInfo);

   if (cx == 0) cx = bmpInfo.bmHeight;  /* height is correct! */
   if (cy == 0) cy = bmpInfo.bmHeight;
   int nr = bmpInfo.bmWidth / cx;

   iL = ImageList_Create( cx, cy, ILC_COLOR8, nr, 0);

   if (ImageList_Add(iL, hBmp, NULL) == -1) {
       ImageList_Destroy( iL);
       return NULL;
   }
   return iL;
}



//*-------------------------------------------------------------------
//| Title:
//|     SubClassProc
//|
//| Parameters:
//|     hWnd            - Handle to the message's destination window
//|     wMessage        - Message number of the current message
//|     wParam          - Additional info associated with the message
//|     lParam          - Additional info associated with the message
//|
//| Purpose:
//|     This is the window procedure used to subclass the edit control.
//*---------------------------------------------------------------------

WNDPROC lpOldEditProc = NULL;


long FAR PASCAL CatchReturnSubProc(HWND hWnd, WORD wMessage,WORD wParam,LONG lParam)
{
    switch (wMessage)
    {
        case WM_GETDLGCODE:
            return (DLGC_WANTALLKEYS |
                    CallWindowProc(lpOldEditProc, hWnd, wMessage,
                                   wParam, lParam));

        case WM_CHAR:
             //Process this message to avoid message beeps.
            if ((wParam == VK_RETURN) || (wParam == VK_ESCAPE))
                return 0;
            else
                return (CallWindowProc(lpOldEditProc, hWnd,wMessage, wParam, lParam));

        default:
            return (CallWindowProc(lpOldEditProc, hWnd, wMessage,
                                      wParam, lParam));
            break;

    } /* end switch */
}

RexxFunction8(uintptr_t, InsertTreeItem, uintptr_t, handle, uintptr_t, parent, uintptr_t, after, CSTRING, text,
    int, image, CSTRING, opts, int, children, int, selectedImage)
{
    HWND h = (HWND)handle;
    TV_INSERTSTRUCT ins;
    TV_ITEM * tvi = &ins.item;

    ins.hParent = (HTREEITEM)parent;
    ins.hInsertAfter = (HTREEITEM)after;
    tvi->mask = TVIF_TEXT;

    tvi->pszText = text;
    tvi->cchTextMax = strlen(text);

    tvi->iImage = image;
    if (tvi->iImage >= 0) tvi->mask |= TVIF_IMAGE;

    tvi->state= 0;

    if (strstr(opts, "BOLD")) tvi->state |= TVIS_BOLD;
    if (strstr(opts, "EXPANDED")) tvi->state |= TVIS_EXPANDED;
    tvi->stateMask = tvi->state;

    if (tvi->state != 0) tvi->mask |= TVIF_STATE;

    tvi->cChildren = children;
    if (tvi->cChildren > 0) tvi->mask |= TVIF_CHILDREN;

    tvi->iSelectedImage = selectedImage;
    if (tvi->iSelectedImage > -1) tvi->mask |= TVIF_SELECTEDIMAGE;

    return TreeView_InsertItem(h, &ins);
}

RexxFunction1(int, DeleteAllTreeItems, uintptr_t, handle)
{
    return !TreeView_DeleteAllItems((HWND)handle);
}

RexxFunction2(int, DeleteTreeItem, uintptr_t, handle, uintptr_t, item)
{
    if (TreeView_GetCount((HWND)handle) >0)
    {
        return !TreeView_DeleteItem((HWND)handle, (HTREEITEM)item);
    }
    else
    {
        return -1;
    }
}

RexxFunction7(int, SetTreeItem, uintptr_t, handle, uintptr_t, item, CSTRING, text,
    int, image, CSTRING, opts, int, children, int, selectedImage)
{
    TV_ITEM tvi;

    HWND h = (HWND)handle;
    tvi.hItem = (HTREEITEM)item;

    // tvi.mask = TVIF_HANDLE;
    tvi.mask = 0;

    if (strlen(text) > 0)
    {
        tvi.pszText = text;
        tvi.cchTextMax = strlen(text);
        tvi.mask |= TVIF_TEXT;
    }

    tvi.iImage = image;
    if (tvi.iImage >= 0) tvi.mask |= TVIF_IMAGE;

    tvi.state= 0;
    tvi.stateMask= 0;

    if (strstr(opts, "NOTBOLD")) tvi.stateMask |= TVIS_BOLD;
    else if (opts, "BOLD")) {tvi.state |= TVIS_BOLD; tvi.stateMask |= TVIS_BOLD;}
    if (strstr(opts, "NOTDROP")) tvi.stateMask |= TVIS_DROPHILITED;
    else if (strstr(opts, "DROP")) {tvi.state |= TVIS_DROPHILITED; tvi.stateMask |= TVIS_DROPHILITED;}
    if (strstr(opts, "NOTSELECTED")) tvi.stateMask |= TVIS_SELECTED;
    else if (strstr(opts, "SELECTED")) {tvi.state |= TVIS_SELECTED; tvi.stateMask |= TVIS_SELECTED;}
    if (strstr(opts, "NOTCUT")) tvi.stateMask |= TVIS_CUT;
    else if (strstr(opts, "CUT")) {tvi.state |= TVIS_CUT; tvi.stateMask |= TVIS_CUT;}
    if (strstr(opts, "NOTEXPANDEDONCE")) tvi.stateMask |= TVIS_EXPANDEDONCE;
    else if (strstr(opts, "EXPANDEDONCE")) {tvi.state |= TVIS_EXPANDEDONCE; tvi.stateMask |= TVIS_EXPANDEDONCE;}
    else if (strstr(opts, "NOTEXPANDED")) tvi.stateMask |= TVIS_EXPANDED;
    else if (strstr(opts, "EXPANDED")) {tvi.state |= TVIS_EXPANDED; tvi.stateMask |= TVIS_EXPANDED;}
    if ((tvi.state != 0) || (tvi.stateMask!= 0)) tvi.mask |= TVIF_STATE;

    tvi.cChildren = children;
    if (tvi.cChildren > -1) tvi.mask |= TVIF_CHILDREN;

    tvi.iSelectedImage = selectedImage;
    if (tvi.iSelectedImage > -1) tvi.mask |= TVIF_SELECTEDIMAGE;

    return TreeView_SetItem(h, &tvi);
}

RexxFunction3(int, GetTreeItem, uintptr_t, handle, uintptr_t, item, RexxStemObject, stem)
{
    TV_ITEM tvi;
    char data[256];

    HWND h = (HWND)handle;
    tvi.hItem = (HTREEITEM)item;
    tvi.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_CHILDREN | TVIF_SELECTEDIMAGE;
    tvi.pszText = data;
    tvi.cchTextMax = sizeof(data);
    tvi.stateMask = TVIS_EXPANDED | TVIS_BOLD | TVIS_SELECTED | TVIS_EXPANDEDONCE | TVIS_DROPHILITED | TVIS_CUT;

    if (TreeView_GetItem(h, &tvi))
    {
        context->SetStemElement(stem, "!TEXT", context->NewStringFromAsciiz(tvi.pszText));
        context->SetStemElement(stem, "!CHILDREN", context->NumberToObject(tvi.cChildren));
        context->SetStemElement(stem, "!IMAGE", context->NumberToObject(tvi.iImage));
        context->SetStemElement(stem, "!SELECTEDIMAGE", context->NumberToObject(tvi.iSelectedImage));

        data[0] = '\0';
        if (tvi.state & TVIS_EXPANDED) strcat(data, "EXPANDED ");
        if (tvi.state & TVIS_BOLD) strcat(data, "BOLD ");
        if (tvi.state & TVIS_SELECTED) strcat(data, "SELECTED ");
        if (tvi.state & TVIS_EXPANDEDONCE) strcat(data, "EXPANDEDONCE ");
        if (tvi.state & TVIS_DROPHILITED) strcat(data, "INDROP ");
        if (tvi.state & TVIS_CUT) strcat(data, "CUT ");
        context->SetStemElement(stem, "!STATE", context->NewStringFromAsciiz(data));
        return 0;
    }
    return -1;
}

RexxFunction3(uintptr_t GetTreeItemHandle, uintptr_t, handle, uintptr_t, item, CSTRING, name)
{
    HWND h = (HWND)handle;
    HTREEITEM hItem;
    ULONG flag;

    hItem = (HTREEITEM)item;

    if (!strcmp(name, "CARET")) flag = TVGN_CARET;
    else if (!strcmp(name, "CHILD")) flag = TVGN_CHILD;
    else if (!strcmp(name, "DROP")) flag = TVGN_DROPHILITE;
    else if (!strcmp(name, "FIRSTVISIBLE")) flag = TVGN_FIRSTVISIBLE;
    else if (!strcmp(name, "NEXT")) flag = TVGN_NEXT;
    else if (!strcmp(name, "NEXTVISIBLE")) flag = TVGN_NEXTVISIBLE;
    else if (!strcmp(name, "PARENT")) flag = TVGN_PARENT;
    else if (!strcmp(name, "PREVIOUS")) flag = TVGN_PREVIOUS;
    else if (!strcmp(name, "PREVIOUSVISIBLE")) flag = TVGN_PREVIOUSVISIBLE;
    else if (!strcmp(name, "ROOT")) flag = TVGN_ROOT;
    TreeView_GetNextItem(h, hItem, flag);
}

RexxFunction1(int, GetTreeItemCount, uintptr_t, handle)
{
    return TreeView_GetCount((HWND)handle);
}

RexxFunction1(int, GetTreeItemVisibleCount, uintptr_t, handle)
{
    return TreeView_GetVisibleCount((HWND)handle);
}

RexxFunction2(RexxBoolean, StelectTreeItem, uintptr_t, handle, uintptr_t, item, int, target)
{
    HWND h = (HWND)handle;
    HTREEITEM hItem = (HTREEITEM)item;
    ULONG flag;


    if (target == 2) flag = TVGN_DROPHILITE;
    else if (target == 3) flag = TVGN_FIRSTVISIBLE;
    else flag = TVGN_CARET;
    return !TreeView_Select(h, hItem, flag);
}


RexxFunction3(RexxBoolean, ExpandTreeItem, uintptr_t, handle, uintptr_t, item, int, state)
{
    HWND h = (HWND)handle;
    HTREEITEM hItem = (HTREEITEM)item;
    ULONG flag;

    if (state == 0)) flag = TVE_EXPAND;
    else if (state == 1) flag = TVE_TOGGLE;
    else if (state == 2) flag = TVE_COLLAPSE;
    else if (state == 3) flag = TVE_COLLAPSE | TVE_COLLAPSERESET;
    return !TreeView_Expand(h, hItem, flag);
}

RexxFunction2(RexxBoolean, EnsureTreeItemVisible, uintptr_t, handle, uintptr_t, item)
{
    return !TreeView_EnsureVisible((HWND)handle, (HTREEITEM)item);
}

RexxFunction1(int, GetTreeItemIndent, uintptr_t, handle)
{
    return TreeView_GetIndent((HWND)handle);
}

RexxFunction1(RexxBoolean, SetTreeItemIndent, uintptr_t, handle, uint32_t, indent)
{
    TreeView_SetIndent((HWND)handle, indent);
    return 0;
}

RexxFunction2(uintptr_t, EditTreeItemLabel, uintptr_t, handle, uintptr_t, item)
{
    return TreeView_EditLabel((HWND)handle, (HTREEITEM)item);
}

RexxFunction2(RexxBoolean, EndTreeItemEdit, uinptr_t, handle, RexxBoolean, cancel)
{
    return !TreeView_EndEditLabelNow((HWND)handle, cancel);
}


RexxFunction2(RexxBoolean, SortTreeItemChilden, uintptr_t, handle, uintptr_t, item)
{
    return !TreeView_SortChildren((HWND)handle, (HTREEITEM)item, 0)
}

RexxFunction4(uintptr_t, SetTreeImages, uintptr_t, handle, uintptr_t, bitmap, int, cx, int, cy)
{
    HWND h = (HWND)handle;
    HBITMAP hBmp = (HBITMAP)bitmap;
    HIMAGELIST iL = CreateImageList(2, h, argv, argc);

    if (iL != NULL)
    {
        return TreeView_SetImageList(h, iL, TVSIL_NORMAL);
    }
    return 0;
}

RexxFunction1(RexxBoolean, UnsetTreeImages, uintptr_t, handle)
{
    HWND h = (HWND)handle;
    HIMAGELIST iL TreeView_GetImageList(h, TVSIL_NORMAL);
    if (iL == NULL)
    {
        return 1;
    }
    TreeView_SetImageList(h, 0, TVSIL_NORMAL);

    return !ImageList_Destroy(iL);
}


RexxFunction1(uintptr_t, SubclassTreeEditor, uintptr_t, handle)
{
    HWND h = (HWND)handle;
    HWND ew = TreeView_GetEditControl(h);
    if (ew)
    {
        WNDPROC oldProc = (WNDPROC)SetWindowLong(ew, GWL_WNDPROC, (LONG)CatchReturnSubProc);
        if (oldProc != (WNDPROC)CatchReturnSubProc) lpOldEditProc = oldProc;
        return oldProc;
    }
    return 0;
}


RexxFunction2(uintptr_t, RestoreTreeEditor, uintptr_t, handle)
{
    HWND h = (HWND)handle;
    HWND ew = TreeView_GetEditControl(h);
    if (ew != NULL)
    {
        SetWindowLong(ew, GWL_WNDPROC, (LONG)lpOldEditProc);
        return 0;
    }
    return -1;
}

RexxFunction3(RexxStringObject, TestTreeItemHit, uintptr_t, handle, int, x, int, y)
{
    HWND h = (HWND)handle;
    TV_HITTESTINFO hti;
    HTREEITEM hItem;
    char buffer[256];

    hti.pt.x = x;
    hti.pt.y = y;
    hItem = TreeView_HitTest(h, &hti);
    if (hItem)
    {
        ValueDescriptor type;

        type.value.value_uintptr_t = hItem;
        type.type = REXX_VALUE_uintptr_t;

        char *handleString = context->NumberToStringValue(context->ValueToObject(&type));


        strcpy(buffer, handleString); /* removed compiler warning */
        if (hti.flags & TVHT_ABOVE) strcat(buffer, " ABOVE");
        if (hti.flags & TVHT_BELOW) strcat(buffer, " BELOW");
        if (hti.flags & TVHT_NOWHERE) strcat(buffer, " NOWHERE");
        if (hti.flags & TVHT_ONITEM) strcat(buffer, " ONITEM");
        if (hti.flags & TVHT_ONITEMBUTTON) strcat(buffer, " ONBUTTON");
        if (hti.flags & TVHT_ONITEMICON) strcat(buffer, " ONICON");
        if (hti.flags & TVHT_ONITEMINDENT) strcat(buffer, " ONINDENT");
        if (hti.flags & TVHT_ONITEMLABEL) strcat(buffer, " ONLABEL");
        if (hti.flags & TVHT_ONITEMRIGHT) strcat(buffer, " ONRIGHT");
        if (hti.flags & TVHT_ONITEMSTATEICON) strcat(buffer, " ONSTATEICON");
        if (hti.flags & TVHT_TOLEFT) strcat(buffer, " TOLEFT");
        if (hti.flags & TVHT_TORIGHT) strcat(buffer, " TORIGHT");
        return context->NewStringFromAsciiz(buffer);
    }
    return context->NullString();
}


RexxFunction4(uintptr_t, InsertListItem, uintptr_t, handle, int, item, CSTRING, text, int, image)
{
    HWND h = (HWND)handle;
    LV_ITEM lvi;

    lvi.mask = LVIF_TEXT;

    lvi.iItem = item;
    lvi.iSubItem = 0;

    lvi.pszText = text;
    lvi.cchTextMax = strlen(text);

    lvi.iImage = image;
    if (lvi.iImage >= 0) lvi.mask |= LVIF_IMAGE;

    return ListView_InsertItem(h, &lvi);
}


RexxFunction5(RexxBoolean, SetListItem, uintptr_t, handle, int, item, int, subitem, CSTRING, text, int, image)
{
    HWND h = (HWND)handle;
    LV_ITEM lvi;

    lvi.mask = 0;

    lvi.iItem = item;
    lvi.iSubItem = subitem;

    lvi.pszText = text;
    lvi.cchTextMax = strlen(text);
    if (lvi.cchTextMax) lvi.mask |= LVIF_TEXT;

    lvi.iImage = image;
    if (lvi.iImage >= 0) lvi.mask |= LVIF_IMAGE;

    return !ListView_SetItem(h, &lvi);
}


RexxFunction4(RexxBoolean, SetListItemText, uintptr_t, handle, int, item, int, subitem, CSTRING, text)
{
    HWND h = (HWND)handle;
    LV_ITEM lvi;

    lvi.mask = 0;

    lvi.iItem = item;
    lvi.iSubItem = subitem;

    lvi.pszText = text;
    lvi.cchTextMax = strlen(text);
    lvi.mask |= LVIF_TEXT;
    return !SendMessage(h, LVM_SETITEMTEXT, lvi.iItem, (LPARAM)&lvi);
}


RexxFunction3(RexxBoolean, SetListItemState, uintptr_t, handle, int, item, CSTRING, state)
{
    HWND h = (HWND)handle;
    LV_ITEM lvi;

    lvi.mask = 0;

    lvi.iItem = item;
    lvi.iSubItem = 0;

    lvi.state = 0;
    lvi.stateMask = 0;

    if (strstr(state, "NOTCUT"))  lvi.stateMask |= LVIS_CUT;
    else if (strstr(state, "CUT"))  {lvi.state |= LVIS_CUT; lvi.stateMask |= LVIS_CUT;}
    if (strstr(state, "NOTDROP"))  lvi.stateMask |= LVIS_DROPHILITED;
    else if (strstr(state, "DROP"))  {lvi.state |= LVIS_DROPHILITED; lvi.stateMask |= LVIS_DROPHILITED;}
    if (strstr(state, "NOTFOCUSED"))  lvi.stateMask |= LVIS_FOCUSED;
    else if (strstr(state, "FOCUSED"))  {lvi.state |= LVIS_FOCUSED; lvi.stateMask |= LVIS_FOCUSED;}
    if (strstr(state, "NOTSELECTED"))  lvi.stateMask |= LVIS_SELECTED;
    else if (strstr(state, "SELECTED"))  {lvi.state |= LVIS_SELECTED; lvi.stateMask |= LVIS_SELECTED;}

    return !SendMessage(h, LVM_SETITEMSTATE, lvi.iItem, (LPARAM)&lvi);
}


RexxFunction3(RexxStringObject, GetListItemText, uintptr_t, handle, int, item, int, subitem)
{
    HWND h = (HWND)handle;
    LV_ITEM lvi;
    CHAR data[256];

    lvi.iItem = item;
    lvi.iSubItem = subitem;
    lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
    lvi.pszText = data;
    lvi.cchTextMax = sizeof(data);
    lvi.stateMask = LVIS_CUT | LVIS_DROPHILITED | LVIS_FOCUSED | LVIS_SELECTED;
    len = SendMessage(h, LVM_GETITEMTEXT, lvi.iItem, (LPARAM)&lvi);
    return context->NewString(data, len);
}


RexxFunction2(RexxBoolean, GetListItemState, uintptr_t, handle, int, item)
{
    HWND h = (HWND)handle;
    LV_ITEM lvi;
    CHAR data[256];

    lvi.iItem = item;
    lvi.iSubItem = 0;
    lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
    lvi.pszText = data;
    lvi.cchTextMax = sizeof(data);
    lvi.stateMask = LVIS_CUT | LVIS_DROPHILITED | LVIS_FOCUSED | LVIS_SELECTED;
    UINT state;

    state = ListView_GetItemState(h, lvi.iItem, lvi.stateMask);
    data[0] = '\0';
    if (state & LVIS_CUT) strcat(data, "CUT ");
    if (state & LVIS_DROPHILITED) strcat(data, "DROP ");
    if (state & LVIS_FOCUSED) strcat(data, "FOCUSED ");
    if (state & LVIS_SELECTED) strcat(data, "SELECTED ");
    return context->NewStringFromAsciiz(data);
}


RexxFunction4(int, GetListItem, uintptr_t, handle, int, item, int, subitem, RexxStemObject, stem)
{
    HWND h = (HWND)handle;
    LV_ITEM lvi;
    CHAR data[256];

    lvi.iItem = item;
    lvi.iSubItem = subitem;
    lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
    lvi.pszText = data;
    lvi.cchTextMax = sizeof(data);
    lvi.stateMask = LVIS_CUT | LVIS_DROPHILITED | LVIS_FOCUSED | LVIS_SELECTED;
    if (ListView_GetItem(h, &lvi))
    {
        context->SetStemElement(stem, "!TEXT", context->NewStringFromAsciiz(lvi.pszText));
        context->SetStemElement(stem, "!IMAGE", context->NumberToObject(lvi.iImage));
        data[0] = '\0';
        if (lvi.state & LVIS_CUT) strcat(data, "CUT ");
        if (lvi.state & LVIS_DROPHILITED) strcat(data, "DROP ");
        if (lvi.state & LVIS_FOCUSED) strcat(data, "FOCUSED ");
        if (lvi.state & LVIS_SELECTED) strcat(data, "SELECTED ");
        context->SetStemElement(stem, "!STATE", context->NewStringFromAsciiz(data));
        return 0;
    }
    return -1;
}

RexxFunction2(int, DeleteListItem, uintptr_t, handle, int, item)
{
    HWND h = (HWND)handle;
    if (ListView_GetItemCount(h) > 0)
    {
        return ListView_DeleteItem(h, item) != 0;
    }
    return -1;
}

RexxFunction1(RexxBoolean, DeleteAllListItems, uintptr_t)
{
    HWND h = (HWND)handle;
    return !ListView_DeleteAllItems(h);
}

RexxFunction3(uintptr_t, GetNextListItem, uintptr_t, handle, int, item, CSTRING, which)
{
    HWND h = (HWND)handle;
    ULONG flag;
    LONG startItem;

    if (!strcmp(which, "FIRSTVISIBLE"))
    {
        return ListView_GetTopIndex(h),
    }

    flag = 0;
    if (strstr(which, "ABOVE")) flag |= LVNI_ABOVE;
    if (strstr(which, "BELOW")) flag |= LVNI_BELOW;
    if (strstr(which, "TOLEFT")) flag |= LVNI_TOLEFT;
    if (strstr(which, "TORIGHT")) flag |= LVNI_TORIGHT;
    if (!flag) flag = LVNI_ALL;

    if (strstr(which, "CUT")) flag |= LVNI_CUT;
    else if (strstr(which, "DROP")) flag |= LVNI_DROPHILITED;
    else if (strstr(which, "FOCUSED")) flag |= LVNI_FOCUSED;
    else if (strstr(which, "SELECTED")) flag |= LVNI_SELECTED;

    return ListView_GetNextItem(h, item, flag);
}

RexxFunction5(uintptr_t, SetListImages, uintptr_t, handle, uintptr_t, bitmap, int, x, int, y, RexxBoolean, small)
{
    HWND h = (HWND)handle;
    HIMAGELIST iL;
    WORD ilt;
    iL = CreateImageList(h, bitmap, x, y);

    if (small) ilt = LVSIL_SMALL;
    else ilt = LVSIL_NORMAL;

    if (iL != NULL)
    {
        return ListView_SetImageList(h, iL, ilt);
    }
    return 0;
}

RexxFunction2(RexxBoolean, UnsetListImages, uintptr_t, handle, RexxBoolean, small)
{
    HWND h = (HWND)handle;
    HIMAGELIST iL;
    WORD ilt;

    if (small) ilt = LVSIL_SMALL;
    else ilt = LVSIL_NORMAL;

    iL = ListView_GetImageList(h, ilt);
    if (iL == NULL)
    {
        return 1;
    }
    ListView_SetImageList(h, 0, ilt);

    return !ImageList_Destroy( iL);
}

RexxFunction4(uintptr_t, FindListItem, uintptr_t, handle, int, item, CSTRING, opt, CSTRING, text)
{
    HWND h = (HWND)handle;
    LV_FINDINFO finfo;

    finfo.flags = LVFI_STRING;

    if (strstr(opt, "PARTIAL")) finfo.flags |= LVFI_PARTIAL;
    if (strstr(opt, "WRAP")) finfo.flags |= LVFI_WRAP;

    finfo.psz = argv[5].strptr;
    return ListView_FindItem(h, item, &finfo);
}

RexxFunction4(uintptr_t, FindNearestListItem, uintptr_t, handle, int, x, int, y, CSTRING, direction)
{
    HWND h = (HWND)handle;
    LV_FINDINFO finfo;

    finfo.flags = LVFI_NEARESTXY;

    finfo.pt.x = x;
    finfo.pt.y = y;
    if (!strcmp(direction, "UP")) finfo.vkDirection = VK_UP;
    else if (!strcmp(direction, "LEFT")) finfo.vkDirection  = VK_LEFT;
    else if (!strcmp(direction, "RIGHT")) finfo.vkDirection  = VK_RIGHT;
    else finfo.vkDirection  = VK_DOWN;

    return ListView_FindItem(h, startItem, &finfo);
}

RexxFunction1(uintptr_t, EditListItem, uintptr_t, handle, int, item)
{
    return ListView_EditLabel(h, item);
}

RexxFunction1(uintptr_t, EndListItemEdit, uintptr_t, handle)
{
    return ListView_EditLabel(h, -1);
}


RexxFunction1(uintptr_t, SubclassListEditor, uintptr_t, handle)
{
    HWND h = (HWND)handle;
    HWND ew = TreeView_GetEditControl(h);
    if (ew)
    {
        WNDPROC oldProc = (WNDPROC)SetWindowLong(ew, GWL_WNDPROC, (LONG)CatchReturnSubProc);
        if (oldProc != (WNDPROC)CatchReturnSubProc) lpOldEditProc = oldProc;
        return oldProc;
    }
    return 0;
}


RexxFunction1(uintptr_t, RestoreListEditor, uintptr_t, handle)
{
    HWND h = (HWND)handle;
    HWND ew = TreeView_GetEditControl(h);
    if (ew != NULL)
    {
        SetWindowLong(ew, GWL_WNDPROC, (LONG)lpOldEditProc);
        return 0;
    }
    return -1;
}


RexxFunction1(int, GetListCount, uintptr_t, handle)
{
    return ListView_GetItemCount((HWND)handle);
}


RexxFunction1(int, GetListSelectedCount, uintptr_t, handle)
{
    return ListView_GetSelectedCount((HWND)handle);
}


RexxFunction3(RexxBoolean, RedrawListItems, uintptr_t, handle, int, first, int, last)
{
    return !ListView_RedrawItems((HWND)handle, first, last);
}


RexxFunction2(RexxBoolean, UpdateListItem, uintptr_t, handle, int, item)
{
    return !ListView_Update((HWND)handle, item);
}

RexxFunction3(RexxBoolean, EnsureListItemVisible(uintptr_t, handle, uintptr_t, item, RexxBoolean, partial)
{
    return !ListView_EnsureVisible(h, item, partial);
}


RexxFunction1(int, GetListPerPageCount, uintptr_t, handle)
{
    return ListView_GetCountPerPage((HWND)handle);
}


RexxFunction3(RexxBoolean, ScrollList, uintptr_t, handle, int, dx, int, dy)
{
    return !ListView_Scroll(h, dx, dy);
}

RexxFunction2(int, GetListColor, uintptr_t, handle, int, which)
{
    HWND h = (HWND)handle;
    COLORREF cr = 0;

    switch (which)
    {
        case 0:   // background
            cr = ListView_GetBkColor(h);
            break;

        case 1: // text
            cr = ListView_GetTextColor(h);

        case 2: // text backround
            cr = ListView_GetTextBkColor(h);

    }
    for (i = 0; i< 256; i++)
    {
        if (cr == PALETTEINDEX(i))
        {
            return i;
        }
    }
    return -1;
}
RexxFunction3(RexxBoolean, SetListColor, uintptr_t, handle, int, which, int, color)
{
    HWND h = (HWND)handle;

    switch (which)
    {
        case 0:   // background
            return !ListView_SetBkColor(h, PALETTEINDEX(color));
            break;

        case 1: // text
            return !ListView_SetTextColor(h, PALETTEINDEX(color));
            cr = ListView_GetTextColor(h);

        case 2: // text backround
            return !ListView_SetTextBkColor(h, PALETTEINDEX(color));
    }
    return 0;
}

RexxFunction2(int, GetListItemStringWidth, uintptr_t, handle, CSTRING, text)
{
    return ListView_GetStringWidth((HWND)handle, text);
}

RexxFunction2(RexxBoolean, ArrangeList, uintptr_t, handle, int, where)
{
    HWND h = (HWND)handle;
    UINT flag;
    switch (where)
    {
        case 0:      // left
            flag = LVA_ALIGNLEFT;
            break;
        case 1:      // top
            flag = LVA_ALIGNTOP;
            break;
        case 2:      // snap to grid
            flag = LVA_SNAPTOGRID;
            break;
        default:     // use the default
            flag = LVA_DEFAULT;
            break;
    }
    return !ListView_Arrange(h, flag);
}

RexxFunction2(RexxBoolean, SetListItemCount, uintptr_t, handle, int, count)
{
    ListView_SetItemCount((HWND)handle, count);
    return 0;
}

RexxFunction2(RexxBoolean, SetListItemCount, uintptr_t, handle, int, count)
{
    ListView_SetItemCount((HWND)handle, count);
    return 0;
}

RexxFunction2(RexxStringObject, GetListItemPosition, uintptr_t, handle, int, item)
{
    POINT pt;
    if (ListView_GetItemPosition((HWND)handle, item, &pt))
    {
        char buffer[256];

        sprintf(buffer, "%d %d",pt.x, pt.y);
        return context->NewStringFromAsciiz(buffer);
    }
    return context->NullString();
}

RexxFunction4(RexxBoolean, SetListItemPosition, uintptr_t, handle, int, item, int, x, int, y)
{
    return !SendMessage((HWND)handle, LVM_SETITEMPOSITION, (WPARAM)item, MAKELPARAM(x, y));
}

RexxFunction2(int, GetListSpacing, uintptr_t, handle, RexxBoolean, small)
{
    return ListView_GetItemSpacing((HWND)handle, small);
}

RexxFunction2(int, AddListStyle, uintptr_t, handle, CSTRING, style)
{
    LONG lStyle;
    lStyle |= EvaluateListStyle(style);
    return SetWindowLong((HWND)handle, GWL_STYLE, lStyle));
}

RexxFunction2(int, RemoveListStyle, uintptr_t, handle, CSTRING, style)
{
    LONG lStyle;
    lStyle &= ~EvaluateListStyle(style);
    return SetWindowLong((HWND)handle, GWL_STYLE, lStyle));
}

RexxFunction5(RexxBoolean, InsertListColumn, uintptr_t, handle, int, item, CSTRING, text, int, width, int, format)
{
    HWND h = (HWND)handle;

    switch (format)
    {
        default:
        case 1:   // not found is treated as left
            lvi.fmt = LVCFMT_LEFT;

        case 2:   // right
            lvi.fmt = LVCFMT_RIGHT;

        case 3:   // center
            lvi.fmt = LVCFMT_CENTER;
            break;
    }

    lvi.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT;

    lvi.iSubItem = item;

    lvi.pszText = text;
    lvi.cchTextMax = strlen(text);

    lvi.cx = width;
    if (lvi.cx >= 0) lvi.mask |= LVCF_WIDTH;

    return ListView_InsertColumn(h, lvi.iSubItem, &lvi));
}

RexxFunction5(uintptr_t, ModifyListColumn, uintptr_t, handle, int, item, CSTRING, text, int, width, int, format)
{
    HWND h = (HWND)handle;

    switch (format)
    {
        case 1:   // not found is treated as left
            lvi.fmt = LVCFMT_LEFT;
            lvi.mask |= LVCF_FMT;
            break;

        case 2:   // right
            lvi.fmt = LVCFMT_RIGHT;
            lvi.mask |= LVCF_FMT;
            break;

        case 3:   // center
            lvi.fmt = LVCFMT_CENTER;
            lvi.mask |= LVCF_FMT;
            break;

        default:  // default is not to change the style
    }

    lvi.pszText = text;
    lvi.cchTextMax = strlen(text);
    if (lvi.cchTextMax) lvi.mask |= LVCF_TEXT;

    lvi.cx = width;
    if (lvi.cx >= 0) lvi.mask |= LVCF_WIDTH;

    return !ListView_SetColumn(h, item, &lvi);
}


RexxFunction3(int, GetListColumnInfo, uintptr_t, handle, int, item, RexxStemObject, stem)
{
    HWND h = (HWND)handle;
    LV_COLUMN lvi;
    char data[256];
    lvi.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT | LVCF_WIDTH;
    lvi.pszText = data;
    lvi.cchTextMax = sizeof(data);

    if (ListView_GetColumn(h, item, &lvi))
    {
        context->SetStemElement(stem, "!TEXT", context->NewStringFromAsciia(data));
        context->SetStemElement(stem, "!COLUMN", context->NumberToObject(lvi.iSubItem));
        context->SetStemElement(stem, "!WIDTH", context->NumberToObject(lvi.cx));

        SetRexxStem(argv[4].strptr, -1, "!Text", lvi.pszText);
        itoa(lvi.iSubItem, data, 10);
        SetRexxStem(argv[4].strptr, -1, "!Column", data);
        itoa(lvi.cx, data, 10);
        SetRexxStem(argv[4].strptr, -1, "!Width", data);
        data[0] = '\0';

        char *fmt = NULL;
        if (lvi.fmt == LVCFMT_CENTER) fmt = "CENTER";
        else if (lvi.fmt == LVCFMT_RIGHT) fmt = "RIGHT";
        else fmt = "LEFT";
        context->SetStemElement(stem, "!ALIGN", context->NewStringFromAsciia(fmt));
        return 0;
    }
    return -1;
}


RexxFunction2(int, GetListColumnWidth, uintptr_t, handle, int, item)
{
    return ListView_GetColumnWidth((HWND)handle, item);
}

RexxFunction4(RexxBoolean, SetListColumnWidth, uintptr_t, handle, int, item, int, width, CSTRING, option)
{
    HWND h = (HWND)handle;

    if (!strcmp(option, "AUTO")) width = LVSCW_AUTOSIZE;
    else if (!strcmp(option, "AUTOHEADER")) width = LVSCW_AUTOSIZE_USEHEADER;

    return !ListView_SetColumnWidth(h, item, width);
}


RexxFunction2(int, DeleteListColumn, uintptr_t, handle, int, item)
{
    return ListView_DeleteColumn((HWND)handle, item);
}

RexxFunction1(int, StepProgressDialog, uintptr_t, handle)
{
    return SendMessage((HWND)handle, PBM_STEPIT, 0, 0);
}

RexxFunction2(int, SetProgressDialogPosition, uintptr_t, handle, int, pos)
{
    return SendMessage((HWND)handle, PBM_SETPOS, pos, 0);
}

RexxFunction2(int, SetProgressDialogStep, uintptr_t, handle, int, step)
{
    return SendMessage((HWND)handle, PBM_SETPOS, step, 0);
}

RexxFunction2(int, SetProgressDialogDeltaPos, uintptr_t, handle, int, delta)
{
    return SendMessage((HWND)handle, PBM_DELTAPOS, delta, 0);
}

RexxFunction3(int, SetProgressDialogRange, uintptr_t, handle, int, low, int, high)
{
    return SendMessage((HWND)handle, PBM_SETRANGE, 0, MAKELPARAM(low, high));
}

RexxFunction3(int, SetSliderPosition, uintptr_t, handle, int, pos, RexxBoolean, redraw)
{
    return SendMessage((HWND)handle, TBM_SETPOS, redraw, pos);
}

RexxFunction3(int, GetSliderPosition, uintptr_t, handle, int, pos, RexxBoolean, redraw)
{
    SendMessage((HWND)handle, TBM_GETPOS, 0, 0))
    return 0;
}

RexxFunction3(int, SetSliderMin, uintptr_t, handle, int, val, RexxBoolean, redraw)
{
    SendMessage((HWND)handle, TBM_SETRANGEMIN, redraw, val);
    return 0;
}

RexxFunction3(int, SetSliderMax, uintptr_t, handle, int, val, RexxBoolean, redraw)
{
    SendMessage((HWND)handle, TBM_SETRANGEMAX, redraw, val);
    return 0;
}

RexxFunction4(int, SetSliderRange, uintptr_t, handle, int, low, high, RexxBoolean, redraw)
{
    SendMessage((HWND)handle, TBM_SETRANGEMAX, redraw, MAKELONG(low, high));
    return 0;
}

RexxFunction1(RexxStringObject, GetSliderRange, uintptr_t, handle)
{
    char buffer[256];

    sprintf(buffer, "%d %d",SendMessage((HWND)handle, TBM_GETRANGEMIN, 0,0), SendMessage((HWND)handle, TBM_GETRANGEMAX, 0,0));
    return context->NewStringFromAsciiz(buffer);
}

RexxFunction1(int, ClearSliderTicks, uintptr_t, handle, RexxBoolean, redraw)
{
    SendMessage((HWND)h, TBM_CLEARTICS, redraw, 0);
    return 0;
}

RexxFunction1(int, GetSliderTickCount, uintptr_t, handle)
{
    return SendMessage((HWND)h, TBM_GETNUMTICS, 0, 0);
}

RexxFunction2(RexxBoolean, SetSliderTick, uintptr_t, handle, int, pos)
{
    return !SendMessage((HWND)handle, TBM_SETTIC, 0, pos);
}

RexxFunction2(int, GetSliderTick, uintptr_t, handle, int, tic)
{
    return SendMessage((HWND)handle, TBM_GETTIC, 0, tic);
}

RexxFunction2(RexxBoolean, SetSliderTickFrequency, uintptr_t, handle, int, freq)
{
    return !SendMessage((HWND)handle, TBM_SETTICFREQ, freq, 0);
}

RexxFunction1(int, GetSliderLineSize, uintptr_t, handle)
{
    return SendMessage((HWND)handle, TBM_GETLINESIZE, 0, 0);
}

RexxFunction1(int, GetSliderPageSize, uintptr_t, handle)
{
    return SendMessage((HWND)handle, TBM_GETPAGESIZE, 0, 0);
}

RexxFunction2(int, SetSliderLineSize, uintptr_t, handle, int, size)
{
    return SendMessage((HWND)handle, TBM_SETLINESIZE, 0, size);
}

RexxFunction2(int, SetSliderPageSize, uintptr_t, handle)
{
    return SendMessage((HWND)handle, TBM_SETPAGESIZE, 0, size);
}

RexxFunction3(int, SetSliderSelStart, uintptr_t, handle, int, val, RexxBoolean, redraw)
{
    SendMessage((HWND)handle, TBM_SETSELSTART, redraw, val);
    return 0;
}

RexxFunction3(int, SetSliderSelEnd, uintptr_t, handle, int, val, RexxBoolean, redraw)
{
    SendMessage((HWND)handle, TBM_SETSELEND, redraw, val);
    return 0;
}

RexxFunction2(int, ClearSliderSel, uintptr_t, handle, RexxBoolean, redraw)
{
    SendMessage((HWND)handle, TBM_CLEARSEL, redraw, 0);
    return 0;
}

RexxFunction4(int, SetSliderSelRange, uintptr_t, handle, int, low, int, high, RexxBoolean, redraw)
{
    SendMessage((HWND)handle, TBM_SETSEL, redraw, MAKELONG(low, high));
    return 0;
}

RexxFunction1(RexxStringObject, GetSliderSelRange, uintptr_t, handle)
{
    char buffer[256];

    sprintf(buffer, "%d %d",SendMessage((HWND)handle, TBM_GETSELSTART, 0,0), SendMessage((HWND)handle, TBM_GETSELEND, 0,0));
    return context->NewStringFromAsciiz(buffer);
}


RexxFunction1(uintptr_t, InsertTabItem, uintptr_t, handle, int, item, CSTRING, text, int, image, int, lparam)
{
    TC_ITEM tab;

    tab.mask = TCIF_TEXT;

    tab.pszText = text;
    tab.cchTextMax = strlen(text);

    tab.iImage = image;
    if (tab.iImage >= 0) tab.mask |= TCIF_IMAGE;

    tab.lParam = lparam;
    if (lparam > 0)
    {
        tab.mask |= TCIF_PARAM;
    }
    return TabCtrl_InsertItem((HWND)handle, item, &tab);
}


RexxFunction1(RexxBoolean, ModifyTabItem, uintptr_t, handle, int, item, CSTRING, text, int, image, int, lparam)
{
    TC_ITEM tab;
    tab.mask = 0;

    tab.pszText = text;
    tab.cchTextMax = strlen(text);
    if (tab.cchTextMax > 0) tab.mask |= TCIF_TEXT;

    tab.iImage = image;
    if (tab.iImage >= 0) tab.mask |= TCIF_IMAGE;

    tab.lParam = lparam;
    if (lparam > 0)
    {
        tab.mask |= TCIF_PARAM;
    }
    return !TabCtrl_SetItem((HWND)handle, item, &tab);
}

RexxFunction3(RexxBoolean, GetTabItem, uintptr_t, handle, int, item, RexxStemObject, stem)
{
    TC_ITEM tab;
    char data[255];

    tab.mask = TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM;
    tab.pszText = data;
    tab.cchTextMax = sizeof(data);

    if (TabCtrl_GetItem((HWND)handle, item, &tab))
    {
        context->SetStemElement(stem, "!TEXT", context->NewStringFromAsciiz(tab.pszText));
        context->SetStemElement(stem, "!IMAGE", context->NumberToObject(tab.iImage));
        context->SetStemElement(stem, "!PARAM", context->NumberToObject(tab.lParam));
        return 0;
    }
    return -1;
}


RexxFunction1(int, GetSelectedTabIndex, uintptr_t, handle)
{
    return TabCtrl_GetCurSel((HWND)handle);
}

RexxFunction1(RexxStringObject, GetSelectedTabText, uintptr_t, handle)
{
    char buffer[256];

    tab.mask = TCIF_TEXT;

    tab.pszText = buffer;
    tab.cchTextMax = sizeof(buffer);

    if (TabCtrl_GetItem(h, TabCtrl_GetCurSel(9HWND)handle), &tab))
    {
        return context->NewStringFromAsciiz(buffer);
    }
    return context->NullString();
}


RexxFunction2(int, SetSelectedTabByText, uintptr_t, handle, CSTRING, text)
{
    HWND h = (HWND)handle;
    LONG cnt, i = 0;
    cnt = TabCtrl_GetItemCount(h);
    if (cnt == 0)
    {
        return -1;
    }

    for (i = 0; i < cnt; i++)
    {
        char buffer[256];

        tab.mask = TCIF_TEXT;
        tab.pszText = buffer;
        tab.cchTextMax = sizeof(buffer);
        if (!TabCtrl_GetItem(h, i, &tab))
        {
            return -1;
        }
        if (!stricmp(tab.pszText, text))
        {
            return TabCtrl_SetCurSel(h, i);
        }
        i++;
    }
    return -1;
}


RexxFunction2(int, SetSelectedTab, uintptr_t, handle, int, index)
{
    return TabCtrl_SetCurSel((HWND)h, index);
}

RexxFunction2(int, GetTabFocus, uintptr_t, handle)
{
    return TabCtrl_GetCurFocus((HWND)h);
}


RexxFunction2(int, SetTabFocus, uintptr_t, handle, int, index)
{
    TabCtrl_SetCurFocus((HWND)h, index);
    return 0;
}


RexxFunction2(int, DeleteTab, uintptr_t, handle, int, index)
{
    if (TabCtrl_GetItemCount((HWND)handle) > 0)
    {
        return !TabCtrl_DeleteItem((HWND)h, index);
    }
    return -1;
}


RexxFunction2(int, DeleteAllTabs, uintptr_t, handle, int, index)
{
    return !TabCtrl_DeleteAllItems((HWND)h, index);
}


RexxFunction2(int, GetTabCount, uintptr_t, handle)
{
    return TabCtrl_GetItemCount((HWND)handle);
}

RexxFunction2(int, GetTabRowCount, uintptr_t, handle)
{
    return TabCtrl_GetRowCount((HWND)handle);
}

RexxFunction5(uintptr_t, SetTabImages, uintptr_t, handle, uintptr_t, bitmap, int, x, int, y)
{
    HWND h = (HWND)handle;
    HIMAGELIST iL;
    WORD ilt;
    iL = CreateImageList(h, bitmap, x, y);

    if (iL != NULL)
    {
        return TabCtrl_SetImageList(h, iL);
    }
    return 0;
}

RexxFunction1(RexxBoolean, UnsetTabImages, uintptr_t, handle)
{
    HWND h = (HWND)handle;
    HIMAGELIST iL TabCtrl_GetImageList(h);
    if (iL == NULL)
    {
        return 1;
    }
    TabCtrl_SetImageList(h, NULL);

    return !ImageList_Destroy(iL);
}

RexxFunction3(int, SetTabPadding, uintptr_t, handle, int, x, int, y)
{
    TabCtrl_SetPadding((HWND)handle, x, y);
    return 0;
}

RexxFunction3(RexxStringObject, SetTabItemSize, uintptr_t, handle, int, x, int, y)
{
    char buffer[256];
    LONG prevsize;

    prevsize = TabCtrl_SetItemSize((HWND)handle, x, y);
    sprintf(buffer, "%d %d", LOWORD(prevsize), HIWORD(prevsize));
    return context->NewStringFromAsciiz(buffer);
}

RexxFunction2(RexxStringObject, GetTabItemRectangle, uintptr_t, handle, int, item)
{
    RECT r;
    char buffer[256];

    if (TabCtrl_GetItemRect((HWND)handle, item, &r))
    {
         sprintf(buffer, "%d %d %d %d", r.left, r.top, r.right, r.bottom);
         return context->NewStringFromAsciiz(buffer);
    }
    return context->NullString();
}


RexxFunction6(RexxStringObject, AdjustTab, uintptr_t, handle, RexxBoolean, adapt, int, left, int, top, int, right, int, bottom)
{
    RECT r;

    r.left = left;
    r.top = top;
    r.right = right;
    r.bottom = bottom;

    /* if adapt, the rectangle is adjusted to the given rectangle,
       if not adapt, the window size that could store the given handle is returned */
    TabCtrl_AdjustRect((HWND)h, adapt, &r);

    char buffer[256];
    sprintf(buffer, "%d %d %d %d", r.left, r.top, r.right, r.bottom);
    return context->NewStringFromAsciiz(buffer);
}


