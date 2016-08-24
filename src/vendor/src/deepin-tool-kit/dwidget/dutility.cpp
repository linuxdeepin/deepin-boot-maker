#include "dutility.h"

#include <QPixmap>
#include <QPainter>
#include <QTextLayout>

QT_BEGIN_NAMESPACE
//extern Q_WIDGETS_EXPORT void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed = 0);
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

QImage DUtility::dropShadow(const QPixmap &px, qreal radius, const QColor &color)
{
    if (px.isNull())
        return QImage();

    QSize size = px.size();

    QImage tmp(size + QSize(radius * 2, radius * 2), QImage::Format_ARGB32_Premultiplied);
    tmp.fill(0);
    QPainter tmpPainter(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.drawPixmap(QPoint(radius, radius), px);
    tmpPainter.end();

    // blur the alpha channel
    QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
    blurred.fill(0);
    QPainter blurPainter(&blurred);
    qt_blurImage(&blurPainter, tmp, radius, false, true);
    blurPainter.end();

    if (color == QColor(Qt::black))
        return blurred;

    tmp = blurred;

    // blacken the image...
    tmpPainter.begin(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    tmpPainter.fillRect(tmp.rect(), color);
    tmpPainter.end();

    return tmp;
}

QStringList DUtility::wordWrapText(const QString &text, int width,
                                   QTextOption::WrapMode wrapMode, int *lineCount)
{
    QTextLayout textLayout(text);
    QTextOption &text_option = *const_cast<QTextOption*>(&textLayout.textOption());
    text_option.setWrapMode(wrapMode);

    textLayout.beginLayout();

    QTextLine line = textLayout.createLine();
    QStringList list;

    int text_line_count = 0;

    while (line.isValid()) {
        line.setLineWidth(width);

        const QString &tmp_str = text.mid(line.textStart(), line.textLength());

        list << tmp_str;

        if (tmp_str.indexOf('\n') >= 0)
            ++text_line_count;

        ++text_line_count;

        line = textLayout.createLine();
    }

    textLayout.endLayout();

    if(lineCount)
        *lineCount = text_line_count;

    return list;
}

QStringList DUtility::elideText(const QString &text, const QSize &size, const QFontMetrics &fontMetrics,
                                QTextOption::WrapMode wordWrap, Qt::TextElideMode mode, int flags)
{
    int height = 0;
    int text_line_height = fontMetrics.height();

    QTextLayout textLayout(text);
    QStringList list;

    const_cast<QTextOption*>(&textLayout.textOption())->setWrapMode(wordWrap);

    textLayout.beginLayout();

    QTextLine line = textLayout.createLine();

    while (line.isValid()) {
        height += text_line_height;

        if (height + text_line_height >= size.height()) {
            list << fontMetrics.elidedText(text.mid(line.textStart() + line.textLength() + 1), mode, size.width(), flags);

            break;
        }

        line.setLineWidth(size.width());

        const QString &tmp_str = text.mid(line.textStart(), line.textLength());

        if (tmp_str.indexOf('\n'))
            height += text_line_height;

        list << tmp_str;

        line = textLayout.createLine();
    }

    textLayout.endLayout();

    return list;
}

DWIDGET_END_NAMESPACE
