///////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/window.h
// Purpose:     wxWindow class
// Author:      Peter Most, Javier Torres
// Created:     09/08/09
// RCS-ID:      $Id$
// Copyright:   (c) Peter Most, Javier Torres
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_WINDOW_H_
#define _WX_QT_WINDOW_H_

#include <QtCore/QPointer>
#include <QtGui/QWidget>
#include <QtGui/QScrollBar>

class WXDLLIMPEXP_FWD_CORE wxScrollBar;

/* wxQt specific notes:
 *
 * Remember to implement the Qt object getters on all subclasses:
 *  - GetHandle() returns the Qt object
 *  - QtGetScrollBarsContainer() returns the widget where scrollbars are placed
 *  - QtGetContainer() returns the widget where the children are placed. Usually
 *    there is no need to reimplement this one.
 * For example, for wxFrame, GetHandle() is the QMainWindow,
 * QtGetScrollBarsContainer() is the central widget and QtGetContainer() is a widget
 * in a layout inside the central widget that also contains the scrollbars.
 * Return 0 from QtGetScrollBarsContainer() to disable SetScrollBar() and friends
 * for wxWindow subclasses.
 *
 *
 * Event handling is achieved by using the template class wxQtEventForwarder
 * found in winevent_qt.(h|cpp) to send all Qt events here to QtHandleXXXEvent()
 * methods. All these methods receive the Qt event and the receiver. This is
 * done because events of the containers (the scrolled part of the window) are
 * sent to the same wxWindow instance, that must be able to differenciate them
 * as some events need different handling (paintEvent) depending on that.
 * We pass the QWidget pointer to all event handlers for consistency.
 */
class WXDLLIMPEXP_CORE wxWindow : public wxWindowBase
{
public:
    wxWindow();
    wxWindow(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxPanelNameStr);
    
    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxPanelNameStr);
    
    virtual bool Show( bool show = true );

    virtual void SetLabel(const wxString& label);
    virtual wxString GetLabel() const;

    virtual void SetFocus();
    
    // Z-order
    virtual void Raise();
    virtual void Lower();
    
    // move the mouse to the specified position
    virtual void WarpPointer(int x, int y);

    virtual void Update();
    virtual void Refresh( bool eraseBackground = true,
                          const wxRect *rect = (const wxRect *) NULL );

    virtual bool SetFont(const wxFont& font);
    
    // get the (average) character size for the current font
    virtual int GetCharHeight() const;
    virtual int GetCharWidth() const;
    
    virtual void SetScrollbar( int orient,
                               int pos,
                               int thumbvisible,
                               int range,
                               bool refresh = true );
    virtual void SetScrollPos( int orient, int pos, bool refresh = true );
    virtual int GetScrollPos( int orient ) const;
    virtual int GetScrollThumb( int orient ) const;
    virtual int GetScrollRange( int orient ) const;

        // scroll window to the specified position
    virtual void ScrollWindow( int dx, int dy,
                               const wxRect* rect = NULL );
    
    virtual WXWidget GetHandle() const;
    
    virtual void SetDropTarget( wxDropTarget *dropTarget );
    
    // wxQt implementation internals:

    virtual WXWidget QtGetContainer() const;

    virtual bool QtHandlePaintEvent  ( QWidget *receiver, QPaintEvent *event );
    virtual bool QtHandleResizeEvent ( QWidget *receiver, QResizeEvent *event );
    virtual bool QtHandleWheelEvent  ( QWidget *receiver, QWheelEvent *event );
    virtual bool QtHandleKeyEvent    ( QWidget *receiver, QKeyEvent *event );
    
protected:
    virtual void DoGetTextExtent(const wxString& string,
                                 int *x, int *y,
                                 int *descent = NULL,
                                 int *externalLeading = NULL,
                                 const wxFont *font = NULL) const;

    // coordinates translation
    virtual void DoClientToScreen( int *x, int *y ) const;
    virtual void DoScreenToClient( int *x, int *y ) const;

    // capture/release the mouse, used by Capture/ReleaseMouse()
    virtual void DoCaptureMouse();
    virtual void DoReleaseMouse();

    // retrieve the position/size of the window
    virtual void DoGetPosition(int *x, int *y) const;
    virtual void DoGetSize(int *width, int *height) const;
    virtual void DoGetClientSize(int *width, int *height) const;

    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);

    // same as DoSetSize() for the client size
    virtual void DoSetClientSize(int width, int height);
    
    virtual void DoMoveWindow(int x, int y, int width, int height);

#if wxUSE_TOOLTIPS
    virtual void DoSetToolTip( wxToolTip *tip );
#endif // wxUSE_TOOLTIPS

#if wxUSE_MENUS
    virtual bool DoPopupMenu(wxMenu *menu, int x, int y);
#endif // wxUSE_MENUS

    virtual WXWidget QtGetScrollBarsContainer() const;

private:
    QPointer< QWidget > m_qtWindow;
    QPointer< QWidget > m_qtContainer;

    wxScrollBar *m_horzScrollBar;
    wxScrollBar *m_vertScrollBar;
    void QtOnScrollBarEvent( wxScrollEvent& event );
    
    wxScrollBar *QtGetScrollBar( int orientation ) const;
    
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS_NO_COPY( wxWindow )
};

#endif // _WX_QT_WINDOW_H_
