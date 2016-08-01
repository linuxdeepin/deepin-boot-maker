#pragma once

class QWidget;
class QPoint;
class QMargins;

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

void ChangeWindowMaximizedState(QWidget *widget, int wm_state);
CornerEdge GetCornerEdge(QWidget *widget, int x, int y, const QMargins &margins, int border_width);
bool UpdateCursorShape(QWidget *widget, int x, int y, const QMargins &margins, int border_width);
bool IsCornerEdget(QWidget *widget, int x, int y, const QMargins &margins, int border_width);
void MoveResizeWindow(QWidget *widget, int x, int y, const QMargins &margins, int border_width);

void StartResizing(QWidget *w, const QPoint &globalPoint, const CornerEdge &ce);
void SendMoveResizeMessage(QWidget *widget, int action, int button);
void MoveWindow(QWidget *widget);

void ResetCursorShape(QWidget *widget);
bool SetCursorShape(QWidget *widget, int cursor_id);
void ShowFullscreenWindow(QWidget *widget, bool is_fullscreen);
void ShowMaximizedWindow(QWidget *widget);
void ShowMinimizedWindow(QWidget *widget, bool minimized);
void ShowNormalWindow(QWidget *widget);
void ToggleMaximizedWindow(QWidget *widget);
void SkipTaskbarPager(QWidget *widget);
void SetStayOnTop(QWidget *widget, bool on);
void SetMouseTransparent(QWidget *widget, bool on);
void SetWindowExtents(QWidget *widget, unsigned long WindowGlowRadius, const int resizeHandlSize);
void PropagateSizeHints(QWidget *w);
void DisableResize(QWidget *w);

}
