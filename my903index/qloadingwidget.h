#ifndef QLOADINGWIDGET_H
#define QLOADINGWIDGET_H

#include <QWidget>
#include<QLabel>
#include<QTime>
#include<QTimer>
class QLoadingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QLoadingWidget(QWidget *parent = nullptr);

    QString bgimagepath;
    int screenwidth=0;
    int screenhight=0;
    QString doworkspace;
    QString doshellcode;
    QLabel *ql;
    QTimer *timer;
    QTime *time;
    QString language="cn";
    bool open=true;
    QPixmap pixmap;
    //time
    int m_nBeginCount;
    void  initWidge(QString imagepath);
    void mouseReleaseEvent(QMouseEvent * ev);
    void timeoutShow();
    void BeginCountdown();
    void OnTimerCountdown();
    void changelanguage(QString lange);
    void changebg(QString imagepath);
signals:
    void clicked();

public slots:
    void mouseClicked();
};

#endif // QLOADINGWIDGET_H
