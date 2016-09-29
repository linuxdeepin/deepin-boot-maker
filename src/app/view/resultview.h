#ifndef RESULTVIEW_H
#define RESULTVIEW_H

#include <QWidget>
#include <QPointer>

class QLabel;
class SuggestButton;

class ResultView : public QWidget
{
    Q_OBJECT
public:
    explicit ResultView(QWidget *parent = 0);

signals:

public slots:
    void onLogLinkActivated(const QString&);
    void updateResult(quint32 error, const QString &title, const QString &description);

private:
    QPointer<QLabel>  m_title;
    QPointer<QLabel> m_hitsTitle;
    QPointer<QLabel> m_resultIcon;
    QPointer<QLabel> m_logHits;
    QPointer<SuggestButton> m_rebootLater;
    QPointer<SuggestButton> m_rebootNow;
};

#endif // RESULTVIEW_H
