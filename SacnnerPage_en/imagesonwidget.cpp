#include "imagesonwidget.h"
#include<QMenu>
#include<QContextMenuEvent>
#include<QStyleOption>
#include<QPainter>
#include<QFileDialog>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
imagesonwidget::imagesonwidget(QWidget *parent) : QWidget(parent)
{

}

void imagesonwidget::doLoadImage(QString imagepath)
{
    m_imagepath=imagepath;
    onLoadImage();
}

void imagesonwidget::doZoomInImage()
{
    onZoomInImage();
}

void imagesonwidget::doZoomOutImage()
{
    onZoomOutImage();
}

void imagesonwidget::doPresetImage()
{
    onPresetImage();
}

void imagesonwidget::doRotate()
{
    onRotateImage();
}

void imagesonwidget::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pos = event->pos();
        pos = this->mapToGlobal(pos);
        QMenu *menu = new QMenu(this);

        QAction *loadImage = new QAction(tr("Load Image"));
        QObject::connect(loadImage, &QAction::triggered, this, &imagesonwidget::onLoadImage);
        menu->addAction(loadImage);
        menu->addSeparator();

        QAction *zoomInAction = new QAction(tr("Zoom In"));
        QObject::connect(zoomInAction, &QAction::triggered, this, &imagesonwidget::onZoomInImage);
        menu->addAction(zoomInAction);

        QAction *zoomOutAction = new QAction(tr("Zoom Out"));
        QObject::connect(zoomOutAction, &QAction::triggered, this, &imagesonwidget::onZoomOutImage);
        menu->addAction(zoomOutAction);

        QAction *presetAction = new QAction(tr("Preset"));
        QObject::connect(presetAction, &QAction::triggered, this, &imagesonwidget::onPresetImage);
        menu->addAction(presetAction);

        menu->exec(pos);

}

void imagesonwidget::paintEvent(QPaintEvent *event)
{
    // 绘制样式
        QStyleOption opt;
        opt.init(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

        if (m_Image.isNull())
            return QWidget::paintEvent(event);

        // 根据窗口计算应该显示的图片的大小
        int width = qMin(m_Image.width(), this->width());
        int height = width * 1.0 / (m_Image.width() * 1.0 / m_Image.height());
        height = qMin(height, this->height());
        width = height * 1.0 * (m_Image.width() * 1.0 / m_Image.height());

        // 平移
        painter.translate(this->width() / 2 + m_XPtInterval, this->height() / 2 + m_YPtInterval);

        // 缩放
        painter.scale(m_ZoomValue, m_ZoomValue);

        //旋转
        painter.rotate(m_Rotate);

        // 绘制图像
        QRect picRect(-width / 2, -height / 2, width, height);
        painter.drawImage(picRect, m_Image);

}

void imagesonwidget::wheelEvent(QWheelEvent *event)
{
    int value = event->delta();
        if (value > 0)
            onZoomInImage();
        else
            onZoomOutImage();

        this->update();
}

void imagesonwidget::mousePressEvent(QMouseEvent *event)
{
    m_OldPos = event->pos();
        m_Pressed = true;

}

void imagesonwidget::mouseMoveEvent(QMouseEvent *event)
{

    if (!m_Pressed)
            return QWidget::mouseMoveEvent(event);

        this->setCursor(Qt::SizeAllCursor);
        QPoint pos = event->pos();
        int xPtInterval = pos.x() - m_OldPos.x();
        int yPtInterval = pos.y() - m_OldPos.y();

        m_XPtInterval += xPtInterval;
        m_YPtInterval += yPtInterval;

        m_OldPos = pos;
        this->update();

}

void imagesonwidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_Pressed = false;
        this->setCursor(Qt::ArrowCursor);
}

void imagesonwidget::onLoadImage()
{
//    QFileDialog::getOpenFileName(this, "Open Image", "./", tr("Images (*.png *.xpm *.jpg)"));
    QString imageFile = m_imagepath;

        QFile file(imageFile);
        if (!file.exists())
            return;
        m_Image.load(imageFile);

}

void imagesonwidget::onZoomInImage()
{
    if(m_ZoomValue<=2){
    m_ZoomValue += 0.2;
    this->update();
    }
}

void imagesonwidget::onZoomOutImage()
{
    m_ZoomValue -= 0.2;
        if (m_ZoomValue <= 0.2)
        {
            m_ZoomValue = 0.2;
            return;
        }

        this->update();
}

void imagesonwidget::onRotateImage()
{
    m_Rotate += 90;
    if(m_Rotate==360){
        m_Rotate=0;
    }
    this->update();
}

void imagesonwidget::onPresetImage()
{
       m_ZoomValue = 1.0;
       m_Rotate=0;
        m_XPtInterval = 0;
        m_YPtInterval = 0;
        this->update();
}
