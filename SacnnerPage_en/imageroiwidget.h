#ifndef IMAGEROIWIDGET_H
#define IMAGEROIWIDGET_H

#include <QWidget>
#include <QLabel>

///* 缩放方向 */
enum EmDirection
{
    DIR_TOP = 0,
    DIR_BOTTOM,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_LEFTTOP,
    DIR_LEFTBOTTOM,
    DIR_RIGHTTOP,
    DIR_RIGHTBOTTOM,
    DIR_MIDDLE,
    DIR_NONE
};
#define EDGPADDING       5        //四周边缘可拉伸宽度
#define CORPADDING       6        //四角可拉伸宽度

#define MIN_WIDTH        5        //可拉伸的最小宽度
#define MIN_HEIGHT       5        //可拉伸的最小高度

#define POINT_WIDTH      6        //边缘9点的宽度
#define POINT_HEIGHT     6        //边缘9点的高度

#define EDGE_WIDTH       3        //边框的宽度
#define MIDDLELINE_WIDTH 2        //辅助线的宽度

#define DRAW_TEN_POINT            //绘制十个点
#define DRAW_SUB_LINE             //绘制辅助线

class QMenu;
class QAction;
class ImageRoiwidget : public QLabel
{
    Q_OBJECT
public:
    explicit ImageRoiwidget(QLabel *parent = nullptr);
    ~ImageRoiwidget();
    QRect getRoiRect() const;               //获取已经圈选的框 外部调用
    void setBackImage(const QImage &img);   //设置背景图片  外部调用
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void keyPressEvent(QKeyEvent *ev);
    void contextMenuEvent(QContextMenuEvent *ev);
private:
    void initViewer();                         //初始化
    void saveROIImage();                       //把ROI区域的图片存储下来
    EmDirection region(const QPoint &point);   //根据鼠标位置设置鼠标形状
    void scaleRect(const QPoint &mousePoint);  //缩放矩形
    void paintRect(const QPoint &mousePoint);  //绘制矩形
    void moveRect(const QPoint &mousePoint);   //移动矩形
private:
    bool m_bPainterPressed;        //是否正在绘制
    bool m_bMovedPressed;          //是否正在拖动
    bool m_bScalePressed;          //是否正在缩放大小
    QPoint m_paintStartPoint;      //绘制的初始位置
    QPoint m_moveStartPoint;       //拖动的初始位置
    QRect m_roiRect;               //绘制的ROI
    EmDirection m_emCurDir;        //拖动的方向

    QImage m_backImage;            //背景图

    QMenu *m_pOptMenu;
    QAction *m_pDelAction;
    QAction *m_pSaveAction;

signals:

};

#endif // IMAGEROIWIDGET_H
