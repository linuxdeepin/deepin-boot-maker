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
#ifndef FORMATDIALOG_H
#define FORMATDIALOG_H
#include <DDialog>
#include <DLabel>

DWIDGET_USE_NAMESPACE


struct NewStr {
    QStringList strList;
    QString resultStr;
    int fontHeifht = 0;
};

NewStr autoCutText(const QString &text, DLabel *pDesLbl);


class FormatDialog : public DDialog
{
    Q_OBJECT

public:
    explicit FormatDialog(QWidget *parent = nullptr);
    ~FormatDialog() override;

    /**
     * @brief initDialog    初始化对话框
     * @param titleText    标题
     * @param strFormatText    描述内容
     * @param btnMsg1        按钮1内容
     * @param btnType1       按钮1类型
     * @param btnMsg2        按钮2内容(可省略)
     * @param btnType2       按钮2类型(可省略)
     * @return              操作返回值
     */

    void initDialog(const QString &titleText = "", const QString &strFormatText = "", const QString btnMsg1 = "", ButtonType btnType1 = ButtonNormal,
                    const QString btnMsg2 = "", ButtonType btnType2 = ButtonNormal);


    /**
     * @brief autoFeed 自动换行
     * @param label
     */
    void autoFeed(DLabel *label);

protected:
    void changeEvent(QEvent *event) override;

private:
    QString m_strFormatText;
    int m_iLabelOldHeight = 0;
    int m_iDialogOldHeight = 0;
};

#endif // FORMATDIALOG_H
