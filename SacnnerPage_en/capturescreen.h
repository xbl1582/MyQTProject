#ifndef CAPTURESCREEN_H
#define CAPTURESCREEN_H
//#define MYJSONPATH ":/json/pathjson.json"
#include <QWidget>
#include <QPainter>
#include<QDateTime>
#include<QJsonParseError>
#include<QJsonDocument>
#include<QJsonObject>
enum CaptureState{
    InitCapture = 0,
    BeginCaptureImage,
    FinishCaptureImage,
    BeginMoveCaptureArea,
    FinishMoveCaptureArea,
    BeginMoveStretchRect,
    FinishMoveStretchRect,
    FinishCapture
};//截屏状态

enum StretchRectState{
    NotSelect = 0,
    TopLeftRect,
    TopRightRect,
    BottomLeftRect,
    BottomRightRect,
    LeftCenterRect,
    TopCenterRect,
    RightCenterRect,
    BottomCenterRect
};//鼠标位置

class capturescreen : public QWidget
{
    Q_OBJECT
public:
    explicit capturescreen(QWidget *parent = nullptr,QString imagepath="",int width=0,int height=0,int imagewidth=0,int imageheight=0);
    ~capturescreen();

        void widgetInit();//初始化
        void loadBackgroundPixmap(QString imagepath,int width,int height,int imagewidth,int imageheight);//抓取当前屏幕
        QRect getRect(const QPoint &beginPoint, const QPoint &endPoint);//根据beginPoint,endPoint获取当前选中的矩形
        QRect getMoveRect();//获取移动后的矩形
        QRect getStretchRect();//获取拉伸后的矩形
        bool isPressPointInSelectRect(QPoint mousePressPoint);//当前鼠标坐标是否在选取的矩形区域内
        QRect getSelectRect();//获取当前选中的截图区域
        QPoint getMovePoint();//获取当前鼠标位置,并判断是否移出屏幕
        StretchRectState getStrethRectState(QPoint point);//获取当前鼠标位于哪一个拉伸顶点
        void setStretchCursorStyle(StretchRectState stretchRectState);//鼠标样式设置
        void drawCaptureImage();//绘制选中区域
        void drawStretchRect();//绘制选中矩形拉伸点
        void onSaveTemplateImage(QString imagepath);//右件保存
        QString readjsonreturnpath(QString key);//读取json
        void saveImage();//保存图片
        QString templaImagepath;
        QString myimagepath;


signals:
      void signalCompleteCature(QPixmap catureImage);//截图完成

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);


private:
    QPainter m_painter;
      QPixmap m_loadPixmap;
      QPixmap m_capturePixmap;
      int m_screenwidth;
      int m_screenheight;
      // 保存确定选区的坐标点;
      QPoint m_beginPoint;
      QPoint m_endPoint;
      QPoint m_beginMovePoint;
      QPoint m_endMovePoint;
      // 保存当前截图状态;
      CaptureState m_currentCaptureState;
      // 当前选择区域矩形;
      QRect m_currentSelectRect;
      // 选中矩形8个顶点小矩形;
      QRect m_topLeftRect;
      QRect m_topRightRect;
      QRect m_bottomLeftRect;
      QRect m_bottomRightRect;
      QRect m_leftCenterRect;
      QRect m_topCenterRect;
      QRect m_rightCenterRect;
      QRect m_bottomCenterRect;
      // 当前鼠标所在顶点状态;
      StretchRectState m_stretchRectState;
      double screenwidthtoimagewidth=1;
      double screenheighttoimageheight=1;


};

#endif // CAPTURESCREEN_H
