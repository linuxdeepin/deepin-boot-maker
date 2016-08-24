#ifndef POPUPMENUSTYLEH
#define POPUPMENUSTYLEH

#include <QProxyStyle>

const QString SHORTCUT_SPLIT_FLAG = "@-_-@";

class QStyleOptionMenuItem;
class PopupMenuStyle : public QProxyStyle {
    Q_OBJECT

public:
    PopupMenuStyle();

    int styleHint(StyleHint hint,
                  const QStyleOption* option = 0,
                  const QWidget* widget = 0,
                  QStyleHintReturn* returnData = 0) const;
    int pixelMetric(PixelMetric metric,
                    const QStyleOption* option = 0,
                    const QWidget* widget = 0) const;
    void drawControl(ControlElement control,
                     const QStyleOption* option,
                     QPainter* painter,
                     const QWidget* widget) const;
    void drawPrimitive(PrimitiveElement element,
                       const QStyleOption *option,
                       QPainter *painter,
                       const QWidget *widget = Q_NULLPTR) const;
    QSize sizeFromContents(ContentsType type,
                           const QStyleOption* option,
                           const QSize& contentsSize,
                           const QWidget* widget = 0) const;
private:
    void drawScroller(const QStyleOption* option,
                      QPainter* painter) const;
    void drawMenuItem(const QStyleOption* option,
                      QPainter* painter) const;
    void drawSeparator(const QStyleOptionMenuItem* menuItem,
                       QPainter* painter) const;
    void drawMenuItemIcon(const QStyleOptionMenuItem* menuItem,
                          QPainter* painter) const;
    void drawMenuItemText(const QStyleOptionMenuItem* menuItem,
                          QPainter* painter) const;
    void drawMenuItemSubIcon(const QStyleOptionMenuItem* menuItem,
                             QPainter* painter) const;
    void drawMenuItemCheckedIcon(const QStyleOptionMenuItem* menuItem,
                                 QPainter* painter) const;

    QSize getItemSize(const QStyleOption* option, const QSize& size) const;

    QColor getItemBGColor(const QStyle::State state) const;
    QPixmap getSubMenuPixmap(const QStyle::State state) const;
    QPixmap getIconPixmap(const QStyle::State state,
                          const QIcon& icon,
                          const QSize& size) const;
};

#endif // POPUPMENUSTYLEH
