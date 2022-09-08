// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

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
