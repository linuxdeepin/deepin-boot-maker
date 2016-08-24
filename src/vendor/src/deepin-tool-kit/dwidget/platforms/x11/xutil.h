#pragma once

#include <qnamespace.h>

QT_BEGIN_NAMESPACE
class QWidget;
class QPoint;
class QMargins;
class QRect;
QT_END_NAMESPACE

namespace XUtils
{

enum class CornerEdge
{
    kInvalid = 0,
    kTop = 1,
    kRight = 2,
    kBottom = 4,
    kLeft = 8,
    kTopLeft = 1 | 8,
    kTopRight = 1 | 2,
    kBottomLeft = 4 | 8,
    kBottomRight = 4 | 2,
};

enum class XCursorType
{
    kInvalid = -1,
    kArrow = 2,
    kTop = 138,
    kTopRight = 136,
    kRight = 96,
    kBottomRight = 14,
    kBottom = 16,
    kBottomLeft = 12,
    kLeft = 70,
    kTopLeft = 134,
};

void ChangeWindowMaximizedState(const QWidget *widget, int wm_state);
CornerEdge GetCornerEdge(const QWidget *widget, int x, int y, const QMargins &margins, int border_width);
bool UpdateCursorShape(const QWidget *widget, int x, int y, const QMargins &margins, int border_width);
bool IsCornerEdget(const QWidget *widget, int x, int y, const QMargins &margins, int border_width);
void MoveResizeWindow(const QWidget *widget, Qt::MouseButton qbutton, int x, int y, const QMargins &margins, int border_width);

void StartResizing(const QWidget *w, const QPoint &globalPoint, const CornerEdge &ce);
void SendMoveResizeMessage(const QWidget *widget, Qt::MouseButton qbutton, int action);
void MoveWindow(const QWidget *widget, Qt::MouseButton qbutton);
void CancelMoveWindow(const QWidget *widget, Qt::MouseButton qbutton);

void ResetCursorShape(const QWidget *widget);
bool SetCursorShape(const QWidget *widget, int cursor_id);
void ShowFullscreenWindow(const QWidget *widget, bool is_fullscreen);
void ShowMaximizedWindow(const QWidget *widget);
void ShowMinimizedWindow(const QWidget *widget, bool minimized);
void ShowNormalWindow(const QWidget *widget);
void ToggleMaximizedWindow(const QWidget *widget);
void SkipTaskbarPager(const QWidget *widget);
void SetStayOnTop(const QWidget *widget, bool on);
void SetMouseTransparent(const QWidget *widget, bool on);
void SetWindowExtents(const QWidget *widget, const QMargins &margins, const int resizeHandlSize);
void SetWindowExtents(uint wid, const QRect &windowRect, const QMargins &margins, const int resizeHandleSize);
void PropagateSizeHints(const QWidget *w);
void DisableResize(const QWidget *w);

}
