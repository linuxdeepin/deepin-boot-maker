/*
* Copyright (C) 2021 ~ 2022 Uniontech Software Technology Co.,Ltd.
*
* Author:     wanjian <wanjian@uniontech.com>
*
* Maintainer: wanjian <wanjian@uniontech.com>
* Maintainer: wanjian <wanjian@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "formatdialog.h"

#include <DFontSizeManager>
#include <QEvent>
#include <QFontMetrics>
#include <QDebug>
#include <QMessageBox>

NewStr autoCutText(const QString &text, DLabel *pDesLbl)
{
    if (text.isEmpty() || nullptr == pDesLbl) {
        return NewStr();
    }

    QFont font; // 应用使用字体对象
    QFontMetrics font_label(font);
    QString strText = text;
    int titlewidth = font_label.width(strText);
    QString str;
    NewStr newstr;
    int width = pDesLbl->width();
    if (titlewidth < width) {
        newstr.strList.append(strText);
        newstr.resultStr += strText;
    } else {
        for (int i = 0; i < strText.count(); i++) {
            str += strText.at(i);

            if (font_label.width(str) > width) { //根据label宽度调整每行字符数
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
    setFixedWidth(380);
    // 设置对话框图标
    setIcon(QMessageBox::standardIcon(QMessageBox::Icon::Warning));
}

FormatDialog::~FormatDialog()
{

}

void FormatDialog::initDialog(const QString &titleText, const QString &strFormatText, const QString btnMsg1, DDialog::ButtonType btnType1, const QString btnMsg2, DDialog::ButtonType btnType2)
{
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
        addButton(btnMsg1, false, btnType1);
    } else {
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
    NewStr newstr = autoCutText(m_strFormatText, label);
    label->setText(newstr.resultStr);
    int height_lable = newstr.strList.size() * newstr.fontHeifht;
    label->setFixedHeight(height_lable);
    if (0 == m_iLabelOldHeight) { // 第一次exec自动调整
        adjustSize();
    } else {
        setFixedHeight(m_iDialogOldHeight - m_iLabelOldHeight + height_lable); //字号变化后自适应调整
    }
    m_iLabelOldHeight = height_lable;
    m_iDialogOldHeight = height();
}

void FormatDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::FontChange) {
        Dtk::Widget::DLabel *p = findChild<Dtk::Widget::DLabel *>("ContentLabel");
        if (nullptr != p) {
            autoFeed(p);
        }
    }
    DDialog::changeEvent(event);
}

