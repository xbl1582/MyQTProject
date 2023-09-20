#ifndef IMAGESONWIDGET_H
#define IMAGESONWIDGET_H

#include <QWidget>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
class imagesonwidget : public QWidget
{
    Q_OBJECT
public:
    explicit imagesonwidget(QWidget *parent = nullptr);
    void doLoadImage(QString imagepath);
    void doZoomInImage(void);
    void doZoomOutImage(void);
    void doPresetImage(void);
    void doRotate(void);
    QString m_imagepath;


protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    QImage m_Image;
    qreal m_ZoomValue = 1.0;
    int m_XPtInterval = 0;
    int m_YPtInterval = 0;
    QPoint m_OldPos;
    bool m_Pressed = false;
    qreal m_Rotate=0.0;

private slots:
    void onLoadImage(void);
    void onZoomInImage(void);
    void onZoomOutImage(void);
    void onRotateImage(void);
    void onPresetImage(void);

signals:

};

#endif // IMAGESONWIDGET_H
