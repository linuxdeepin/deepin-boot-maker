#include "xutil.h"

#include <QDebug>
#include <QTimer>
#include <QWidget>

#include <QX11Info>

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>

namespace XUtils
{
// From the WM spec
#define _NET_WM_MOVERESIZE_INVALID          -1
#define _NET_WM_MOVERESIZE_SIZE_TOPLEFT      0
#define _NET_WM_MOVERESIZE_SIZE_TOP          1
#define _NET_WM_MOVERESIZE_SIZE_TOPRIGHT     2
#define _NET_WM_MOVERESIZE_SIZE_RIGHT        3
#define _NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT  4
#define _NET_WM_MOVERESIZE_SIZE_BOTTOM       5
#define _NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT   6
#define _NET_WM_MOVERESIZE_SIZE_LEFT         7
#define _NET_WM_MOVERESIZE_MOVE              8   /* movement only */
#define _NET_WM_MOVERESIZE_SIZE_KEYBOARD     9   /* size via keyboard */
#define _NET_WM_MOVERESIZE_MOVE_KEYBOARD    10   /* move via keyboard */
#define _NET_WM_MOVERESIZE_CANCEL           11   /* cancel operation */

#define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
#define _NET_WM_STATE_ADD           1    /* add/set property */
#define _NET_WM_STATE_TOGGLE        2    /* toggle property  */

const char kAtomNameHidden[] = "_NET_WM_STATE_HIDDEN";
const char kAtomNameFullscreen[] = "_NET_WM_STATE_FULLSCREEN";
const char kAtomNameMaximizedHorz[] = "_NET_WM_STATE_MAXIMIZED_HORZ";
const char kAtomNameMaximizedVert[] = "_NET_WM_STATE_MAXIMIZED_VERT";
const char kAtomNameMoveResize[] = "_NET_WM_MOVERESIZE";
const char kAtomNameWmState[] = "_NET_WM_STATE";
const char kAtomNameWmStateAbove[] = "_NET_WM_STATE_ABOVE";
const char kAtomNameWmStateStaysOnTop[] = "_NET_WM_STATE_STAYS_ON_TOP";
const char kAtomNameWmSkipTaskbar[] = "_NET_WM_STATE_SKIP_TASKBAR";
const char kAtomNameWmSkipPager[] = "_NET_WM_STATE_SKIP_PAGER";

struct MwmHints {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long input_mode;
    unsigned long status;
};

enum {
    MWM_HINTS_FUNCTIONS = (1L << 0),
    MWM_HINTS_DECORATIONS = (1L << 1),

    MWM_FUNC_ALL = (1L << 0),
    MWM_FUNC_RESIZE = (1L << 1),
    MWM_FUNC_MOVE = (1L << 2),
    MWM_FUNC_MINIMIZE = (1L << 3),
    MWM_FUNC_MAXIMIZE = (1L << 4),
    MWM_FUNC_CLOSE = (1L << 5),

    MWM_DECOR_ALL      = (1L << 0),
    MWM_DECOR_BORDER   = (1L << 1),
    MWM_DECOR_RESIZEH  = (1L << 2),
    MWM_DECOR_TITLE    = (1L << 3),
    MWM_DECOR_MENU     = (1L << 4),
    MWM_DECOR_MINIMIZE = (1L << 5),
    MWM_DECOR_MAXIMIZE = (1L << 6),

    MWM_HINTS_INPUT_MODE = (1L << 2),

    MWM_INPUT_MODELESS                  = 0L,
    MWM_INPUT_PRIMARY_APPLICATION_MODAL = 1L,
    MWM_INPUT_FULL_APPLICATION_MODAL    = 3L
};

static int CornerEdge2WmGravity(const CornerEdge &ce)
{
    switch (ce) {
    case CornerEdge::kTopLeft:     return _NET_WM_MOVERESIZE_SIZE_TOPLEFT;
    case CornerEdge::kTop:         return _NET_WM_MOVERESIZE_SIZE_TOP;
    case CornerEdge::kTopRight:    return _NET_WM_MOVERESIZE_SIZE_TOPRIGHT;
    case CornerEdge::kRight:       return _NET_WM_MOVERESIZE_SIZE_RIGHT;
    case CornerEdge::kBottomRight: return _NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT;
    case CornerEdge::kBottom:      return _NET_WM_MOVERESIZE_SIZE_BOTTOM;
    case CornerEdge::kBottomLeft:  return _NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT;
    case CornerEdge::kLeft:        return _NET_WM_MOVERESIZE_SIZE_LEFT;
    default:                       return _NET_WM_MOVERESIZE_INVALID;
    }
}

static XCursorType CornerEdge2XCursor(const CornerEdge &ce)
{
    switch (ce) {
    case CornerEdge::kTop:         return XCursorType::kTop;
    case CornerEdge::kTopRight:    return XCursorType::kTopRight;
    case CornerEdge::kRight:       return XCursorType::kRight;
    case CornerEdge::kBottomRight: return XCursorType::kBottomRight;
    case CornerEdge::kBottom:      return XCursorType::kBottom;
    case CornerEdge::kBottomLeft:  return XCursorType::kBottomLeft;
    case CornerEdge::kLeft:        return XCursorType::kLeft;
    case CornerEdge::kTopLeft:     return XCursorType::kTopLeft;
    default:                       return XCursorType::kInvalid;
    }
}

void ChangeWindowMaximizedState(QWidget *widget, int wm_state)
{
    const auto display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    const Atom net_wm_state = XInternAtom(display, kAtomNameWmState, false);
    const Atom vertical_maximized = XInternAtom(display,
                                    kAtomNameMaximizedVert,
                                    false);
    const Atom horizontal_maximized = XInternAtom(display,
                                      kAtomNameMaximizedHorz,
                                      false);

    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = net_wm_state;
    xev.xclient.display = display;
    xev.xclient.window = widget->winId();
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = wm_state;
    xev.xclient.data.l[1] = vertical_maximized;
    xev.xclient.data.l[2] = horizontal_maximized;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
    XFlush(display);
}

CornerEdge GetCornerEdge(QWidget *widget, int x, int y, const QMargins &margins, int border_width)
{
    QRect fullRect = widget->rect();
    fullRect = fullRect.marginsRemoved(margins);
    unsigned int ce = static_cast<unsigned int>(CornerEdge::kInvalid);
    if ((y - fullRect.top() >= -border_width)
            && (y < fullRect.top())) {
        ce = ce | static_cast<unsigned int>(CornerEdge::kTop);
    }
    if ((x - fullRect.left() >= -border_width)
            && (x < fullRect.left())) {
        ce = ce | static_cast<unsigned int>(CornerEdge::kLeft);
    }
    if ((y - fullRect.bottom() <= border_width)
            && (y > fullRect.bottom())) {
        ce = ce | static_cast<unsigned int>(CornerEdge::kBottom);
    }
    if ((x - fullRect.right() <= border_width)
            && (x > fullRect.right())) {
        ce = ce | static_cast<unsigned int>(CornerEdge::kRight);
    }
    return static_cast<CornerEdge>(ce);
}

void SendMoveResizeMessage(QWidget *widget, int action)
{
    const auto display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    const Atom net_move_resize = XInternAtom(display, kAtomNameMoveResize, false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = net_move_resize;
    xev.xclient.display = display;
    xev.xclient.window = widget->winId();
    xev.xclient.format = 32;

    const auto global_position = QCursor::pos();
    xev.xclient.data.l[0] = global_position.x();
    xev.xclient.data.l[1] = global_position.y();
    xev.xclient.data.l[2] = action;
    xev.xclient.data.l[3] = 0;
    xev.xclient.data.l[4] = 0;
    XUngrabPointer(display, QX11Info::appTime());

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
    XFlush(display);
}

bool IsCornerEdget(QWidget *widget, int x, int y, const QMargins &margins, int border_width)
{
    return GetCornerEdge(widget, x, y, margins, border_width) != CornerEdge::kInvalid;
}

void MoveWindow(QWidget *widget)
{
    SendMoveResizeMessage(widget, _NET_WM_MOVERESIZE_MOVE);
}

void MoveResizeWindow(QWidget *widget, int x, int y, const QMargins &margins, int border_width)
{
    const CornerEdge ce = GetCornerEdge(widget, x, y, margins, border_width);
    if (ce != CornerEdge::kInvalid) {
        const int action = CornerEdge2WmGravity(ce);
        SendMoveResizeMessage(widget, action);
    }
}

void ResetCursorShape(QWidget *widget)
{
    const auto display = QX11Info::display();
    const WId window_id = widget->winId();
    XUndefineCursor(display, window_id);
    XFlush(display);
}

bool SetCursorShape(QWidget *widget, int cursor_id)
{
    const auto display = QX11Info::display();
    const WId window_id = widget->winId();
    const Cursor cursor = XCreateFontCursor(display, cursor_id);
    if (!cursor) {
        qWarning() << "[ui]::SetCursorShape() call XCreateFontCursor() failed";
        return false;
    }
    const int result = XDefineCursor(display, window_id, cursor);
    XFlush(display);
    return result == Success;
}

void ShowFullscreenWindow(QWidget *widget, bool is_fullscreen)
{
    const auto display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    const Atom net_wm_state = XInternAtom(display, kAtomNameWmState, false);
    const Atom fullscreen = XInternAtom(display, kAtomNameFullscreen, false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = net_wm_state;
    xev.xclient.display = display;
    xev.xclient.window = widget->winId();
    xev.xclient.format = 32;

    if (is_fullscreen) {
        xev.xclient.data.l[0] = _NET_WM_STATE_ADD;
    } else {
        xev.xclient.data.l[0] = _NET_WM_STATE_REMOVE;
    }
    xev.xclient.data.l[1] = fullscreen;
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev
              );
    XFlush(display);
}

void ShowMaximizedWindow(QWidget *widget)
{
    ChangeWindowMaximizedState(widget, _NET_WM_STATE_ADD);
}

void ShowMinimizedWindow(QWidget *widget, bool minimized)
{
    const auto display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    const Atom net_wm_state = XInternAtom(display, kAtomNameWmState, false);
    const Atom hidden = XInternAtom(display, kAtomNameHidden, false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = net_wm_state;
    xev.xclient.display = display;
    xev.xclient.window = widget->winId();
    xev.xclient.format = 32;

    if (minimized) {
        xev.xclient.data.l[0] = _NET_WM_STATE_ADD;
    } else {
        xev.xclient.data.l[0] = _NET_WM_STATE_REMOVE;
    }
    xev.xclient.data.l[1] = hidden;
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev
              );

    XIconifyWindow(display, widget->winId(), screen);
    XFlush(display);
}

void ShowNormalWindow(QWidget *widget)
{
    ChangeWindowMaximizedState(widget, _NET_WM_STATE_REMOVE);
}

void ToggleMaximizedWindow(QWidget *widget)
{
    ChangeWindowMaximizedState(widget, _NET_WM_STATE_TOGGLE);
}

bool UpdateCursorShape(QWidget *widget, int x, int y, const QMargins &margins, int border_width)
{
    const CornerEdge ce = GetCornerEdge(widget, x, y, margins, border_width);
    const XCursorType x_cursor = CornerEdge2XCursor(ce);
    if (x_cursor != XCursorType::kInvalid) {
        return SetCursorShape(widget, static_cast<unsigned int>(x_cursor));
    } else {
        ResetCursorShape(widget);
        return false;
    }
}

void SkipTaskbarPager(QWidget *widget)
{
    Q_ASSERT(widget);

    const auto display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    const auto wmStateAtom = XInternAtom(display, kAtomNameWmState, false);
    const auto taskBarAtom = XInternAtom(display, kAtomNameWmSkipTaskbar, false);
    const auto noPagerAtom = XInternAtom(display, kAtomNameWmSkipPager, false);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));

    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = wmStateAtom;
    xev.xclient.display = display;
    xev.xclient.window = widget->winId();;
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = _NET_WM_STATE_ADD;
    xev.xclient.data.l[1] = taskBarAtom;
    xev.xclient.data.l[2] = noPagerAtom;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
    XFlush(display);
}

void SetStayOnTop(QWidget *widget, bool on)
{
    Q_ASSERT(widget);

    const auto display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    const auto wmStateAtom = XInternAtom(display, kAtomNameWmState, false);
    const auto stateAboveAtom = XInternAtom(display, kAtomNameWmStateAbove, false);
    const auto stateStaysOnTopAtom = XInternAtom(display,
                                     kAtomNameWmStateStaysOnTop,
                                     false);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));

    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = wmStateAtom;
    xev.xclient.display = display;
    xev.xclient.window = widget->winId();
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = on ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
    xev.xclient.data.l[1] = stateAboveAtom;
    xev.xclient.data.l[2] = stateStaysOnTopAtom;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
    XFlush(display);
}

void SetMouseTransparent(QWidget *widget, bool on)
{
    Q_ASSERT(widget);

    const auto display = QX11Info::display();
    XRectangle XRect;
    XRect.x = 0;
    XRect.y = 0;
    int nRects = 0;

    if (on) {
        XRect.width = 0;
        XRect.height = 0;
        nRects = 0;
    } else {
        XRect.width = widget->width();
        XRect.height = widget->height();
        nRects = 1;
    }
    XShapeCombineRectangles(display, widget->winId(), ShapeInput,
                            0, 0,
                            &XRect, nRects, ShapeSet, YXBanded);
}

void SetWindowExtents(QWidget *widget, unsigned long windowExtentWidth, const int resizeHandleWidth)
{
    Atom frameExtents;
    unsigned long value[4] = {
        windowExtentWidth,
        windowExtentWidth,
        windowExtentWidth,
        windowExtentWidth
    };
    frameExtents = XInternAtom(QX11Info::display(), "_GTK_FRAME_EXTENTS", False);
    if (frameExtents == None) {
        qWarning() << "Failed to create atom with name DEEPIN_WINDOW_SHADOW";
        return;
    }
    XChangeProperty(QX11Info::display(),
                    widget->winId(),
                    frameExtents,
                    XA_CARDINAL,
                    32,
                    PropModeReplace,
                    (unsigned char *)value,
                    4);

    XRectangle contentXRect;
    contentXRect.x = 0;
    contentXRect.y = 0;
    contentXRect.width = widget->width() - windowExtentWidth * 2 + resizeHandleWidth * 2;
    contentXRect.height = widget->height() - windowExtentWidth * 2 + resizeHandleWidth * 2;
    XShapeCombineRectangles(QX11Info::display(),
                            widget->winId(),
                            ShapeInput,
                            windowExtentWidth - resizeHandleWidth,
                            windowExtentWidth - resizeHandleWidth,
                            &contentXRect, 1, ShapeSet, YXBanded);
}


void PropagateSizeHints(QWidget *w)
{
    const auto display = QX11Info::display();
    XSizeHints *sh = XAllocSizeHints();
    sh->flags = PPosition | PSize | PMinSize | PMaxSize | PResizeInc;
    sh->x = w->x();
    sh->y = w->y();
    sh->min_width = w->minimumWidth();
    sh->min_height = w->minimumHeight();
    sh->base_width = w->baseSize().width();
    sh->base_height = w->baseSize().height();
    sh->max_width = w->maximumWidth();
    sh->max_height = w->maximumHeight();
    sh->width_inc =  w->sizeIncrement().width();
    sh->height_inc = w->sizeIncrement().height();
    XSetWMNormalHints(display, w->winId(), sh);
    XFree(sh);
}

void DisableResize(QWidget *w)
{
    Display *display = QX11Info::display();
    Atom mwmHintsProperty = XInternAtom(display, "_MOTIF_WM_HINTS", 0);
    struct MwmHints *hints;
    unsigned char *wm_data;
    Atom wm_type;
    int wm_format;
    unsigned long wm_nitems, wm_bytes_after;

    XGetWindowProperty(display,
                       w->winId(),
                       mwmHintsProperty,
                       0,
                       sizeof(MwmHints) / sizeof(long),
                       false,
                       AnyPropertyType,
                       &wm_type,
                       &wm_format,
                       &wm_nitems,
                       &wm_bytes_after,
                       &wm_data);

    hints = (MwmHints *) wm_data;

    hints->flags |= MWM_HINTS_FUNCTIONS;
    if (hints->functions == MWM_FUNC_ALL) {
        hints->functions = MWM_FUNC_MOVE | MWM_FUNC_MINIMIZE;
    } else {
        hints->functions &= ~MWM_FUNC_RESIZE;
    }

    if (hints->decorations == MWM_DECOR_ALL) {
        hints->flags |= MWM_HINTS_DECORATIONS;
        hints->decorations = (MWM_DECOR_BORDER
                              | MWM_DECOR_TITLE
                              | MWM_DECOR_MENU);
    } else {
        hints->decorations &= ~MWM_DECOR_RESIZEH;
    }
    XChangeProperty(display,
                    w->winId(),
                    mwmHintsProperty,
                    mwmHintsProperty,
                    32,
                    PropModeReplace,
                    (unsigned char *)hints,
                    5);
}

void StartResizing(QWidget *w, const QPoint &globalPoint, const CornerEdge &ce)
{
    const auto display = QX11Info::display();
    const auto winId = w->winId();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    const Atom netMoveResize = XInternAtom(display, "_NET_WM_MOVERESIZE", false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = netMoveResize;
    xev.xclient.display = display;
    xev.xclient.window = winId;
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = globalPoint.x();
    xev.xclient.data.l[1] = globalPoint.y();
    xev.xclient.data.l[2] = CornerEdge2WmGravity(ce);
    xev.xclient.data.l[3] = Button1;
    xev.xclient.data.l[4] = 1;
    XUngrabPointer(display, QX11Info::appTime());

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
    XFlush(display);
}

}
