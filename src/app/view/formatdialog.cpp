// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "formatdialog.h"

#include <DFontSizeManager>
#include <QEvent>
#include <QFontMetrics>
#include <QDebug>
#include <QMessageBox>

NewStr autoCutText(const QString &text, DLabel *pDesLbl)
{
    if (text.isEmpty() || nullptr == pDesLbl) {
        qDebug() << "Invalid input for autoCutText: empty text or null label";
        return NewStr();
    }

    QFont font; // 应用使用字体对象
    QFontMetrics font_label(font);
    QString strText = text;
    int titlewidth = font_label.horizontalAdvance(strText);
    QString str;
    NewStr newstr;
    int width = pDesLbl->width();
    if (titlewidth < width) {
        qDebug() << "Text fits in one line:" << strText;
        newstr.strList.append(strText);
        newstr.resultStr += strText;
    } else {
        qDebug() << "Text needs multiple lines, width:" << width << "text width:" << titlewidth;
        for (int i = 0; i < strText.count(); i++) {
            str += strText.at(i);
            
            if (font_label.horizontalAdvance(str) > width) { //根据label宽度调整每行字符数
                str.remove(str.count() - 1, 1);
                newstr.strList.append(str);
                newstr.resultStr += str + "\n";
                str.clear();
                --i;
            }
        }
        newstr.strList.append(str);
        newstr.resultStr += str;
    }
    newstr.fontHeifht = font_label.height();
    return newstr;
}


FormatDialog::FormatDialog(QWidget *parent)
    : DDialog(parent)
{
    qDebug() << "Initializing FormatDialog";
    setFixedWidth(380);
    // 设置对话框图标
    setIcon(QMessageBox::standardIcon(QMessageBox::Icon::Warning));
}

FormatDialog::~FormatDialog()
{

}

void FormatDialog::initDialog(const QString &titleText, const QString &strFormatText, const QString btnMsg1, DDialog::ButtonType btnType1, const QString btnMsg2, DDialog::ButtonType btnType2)
{
    qDebug() << "Initializing dialog with title:" << titleText;
    // 设置title
    if (!titleText.isEmpty())
        setTitle(titleText);
    m_strFormatText = strFormatText;
    // 描述内容
    DLabel *pFormatLbl = new DLabel(this);
    pFormatLbl->setObjectName("ContentLabel");
    pFormatLbl->setFixedWidth(340);
    DFontSizeManager::instance()->bind(pFormatLbl, DFontSizeManager::T6, QFont::Medium);
    pFormatLbl->setText(strFormatText);
    pFormatLbl->setAlignment(Qt::AlignCenter);

    // 按钮
    if (btnMsg2.isEmpty()) {
        qDebug() << "Adding single button:" << btnMsg1;
        addButton(btnMsg1, false, btnType1);
    } else {
        qDebug() << "Adding two buttons:" << btnMsg1 << "and" << btnMsg2;
        addButton(btnMsg1, false, btnType1);
        addButton(btnMsg2, true, btnType2);
    }
    addContent(pFormatLbl, Qt::AlignHCenter); // 使用Qt::AlignHCenter效果最好

    autoFeed(pFormatLbl);
//    这里主要是对1050dtk对DDialog的改动进行的微调，给pFormatLbl足够的空间做伸展。bug#105285
    m_iDialogOldHeight = this->height() + 10;
}


void FormatDialog::autoFeed(DLabel *label)
{
    qDebug() << "Auto feeding text for label";
    NewStr newstr = autoCutText(m_strFormatText, label);
    label->setText(newstr.resultStr);
    int height_lable = newstr.strList.size() * newstr.fontHeifht;
    label->setFixedHeight(height_lable);
    if (0 == m_iLabelOldHeight) { // 第一次exec自动调整
        qDebug() << "First time auto feed, adjusting size";
        adjustSize();
    } else {
        qDebug() << "Adjusting height from" << m_iLabelOldHeight << "to" << height_lable;
        setFixedHeight(m_iDialogOldHeight - m_iLabelOldHeight + height_lable); //字号变化后自适应调整
    }
    m_iLabelOldHeight = height_lable;
    m_iDialogOldHeight = height();
}

void FormatDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::FontChange) {
        qDebug() << "Font change event detected";
        Dtk::Widget::DLabel *p = findChild<Dtk::Widget::DLabel *>("ContentLabel");
        if (nullptr != p) {
            autoFeed(p);
        }
    }
    DDialog::changeEvent(event);
}

