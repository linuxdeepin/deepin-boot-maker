#ifndef DABSTRACTWIDGET_H
#define DABSTRACTWIDGET_H

#include <QWidget>
#include <dobject.h>
#include <dwidget_global.h>

DWIDGET_BEGIN_NAMESPACE

class DX11WidgetPrivate;
class DTitlebar;
class DMenu;

class LIBDTKWIDGETSHARED_EXPORT DX11Widget : public QWidget, public DObject
{
    Q_OBJECT
public:
    explicit DX11Widget(QWidget *parent = 0);
    explicit DX11Widget(DObjectPrivate &dd, QWidget *parent = 0);

    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(int shadowWidth READ shadowWidth WRITE setShadowWidth)
    Q_PROPERTY(int border READ border WRITE setBorder)
    Q_PROPERTY(int titlebarHeight READ titlebarHeight WRITE setTitlebarFixedHeight)
    Q_PROPERTY(QPixmap backgroundImage READ backgroundImage WRITE setBackgroundImage)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

    Qt::WindowFlags windowFlags();
    void setWindowFlags(Qt::WindowFlags type);

    void setTitle(const QString &);
    void setTitleIcon(const QPixmap &icon);

    void setTitlebarMenu(DMenu *);
    void setTitlebarWidget(QWidget *, bool fixCenterPos = false);
    int titlebarHeight() const;
    void setTitlebarFixedHeight(int h);

    QLayout *layout() const;
    void setLayout(QLayout *);

    int radius() const;
    void setRadius(int r);

    int shadowWidth() const;
    void setShadowWidth(int w);

    int border() const;
    void setBorder(int b);

    const QPixmap &backgroundImage() const;
    void setBackgroundImage(const QPixmap &);

    void setContentsMargins(int left, int top, int right, int bottom);
    void setContentsMargins(const QMargins &margins);
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;
    QMargins contentsMargins() const;

    void setFixedSize(const QSize &);
    void setFixedSize(int w, int h);
    void setFixedWidth(int w);
//    void setFixedHeight(int h);

    void resize(const QSize &);

    void removeLayout();

    void adjustSize();

    void move(int x, int y);
    void move(const QPoint &);

    QRect frameGeometry() const;
    const QRect &geometry() const;
    QRect normalGeometry() const;

    int x() const;
    int y() const;
    QPoint pos() const;
    QSize frameSize() const;
    QSize size() const;
    int width() const;
    int height() const;
    QRect rect() const;
    QRect childrenRect() const;
    QRegion childrenRegion() const;

    QSize minimumSize() const;
    QSize maximumSize() const;
    int minimumWidth() const;
    int minimumHeight() const;
    int maximumWidth() const;
    int maximumHeight() const;
    void setMinimumSize(const QSize &);
    void setMinimumSize(int minw, int minh);
    void setMaximumSize(const QSize &);
    void setMaximumSize(int maxw, int maxh);
    void setMinimumWidth(int minw);
    void setMinimumHeight(int minh);
    void setMaximumWidth(int maxw);
    void setMaximumHeight(int maxh);

    QColor backgroundColor() const;

protected:
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *)Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *)Q_DECL_OVERRIDE;
public slots:
    void showMinimized();
    void showMaximized();
    void showFullScreen();
    void showNormal();

    void moveWindow();
    void toggleMaximizedWindow();

    void setBackgroundColor(QColor backgroundColor);
protected:
    void setShadow();

Q_SIGNALS:
    void optionClicked();
    void backgroundColorChanged(QColor backgroundColor);

private:

    D_DECLARE_PRIVATE(DX11Widget)
};

class FilterMouseMove : public QObject
{
    Q_OBJECT

public:
    explicit FilterMouseMove(QObject *object = nullptr);
    ~FilterMouseMove();

    bool eventFilter(QObject *obj, QEvent *event);

//private:
    QWidget *m_rootWidget = nullptr;
};

DWIDGET_END_NAMESPACE

#endif // DABSTRACTWIDGET_H
