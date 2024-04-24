#ifndef APPLIST_H
#define APPLIST_H

#include <QWidget>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QtGlobal>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QEasingCurve>
namespace Ui {
class APPList;
}

class APPList : public QWidget
{
    Q_OBJECT

public:
    enum ANIMATION_DIRECT //枚举类型表示动画的方向
    {
        ANIMATION_RIGHT,  //向右
        ANIMATION_LEFT  //向左
    };
    explicit APPList(QList<QWidget *> &list,QWidget *parent = 0);
    ~APPList();

    int getCurrentPageIndex(){return currentPageIndex;}//获取当前页的索引
    QWidget *getCurrentPage(){return widgetList.at(currentPageIndex);}//获取当前页
    void setCurrentPage(int index,bool isAnimation=false);//设置当前显示页
    bool getMoveEnabled(){return moveEnabled;}//获取当前组件是否可以平移
    void setMoveEnabled(bool moveEnabled);//设置组件是否可以平移
    void resizewindow(QSize size);

private:
    void initAnimationGroup();//初始化动画组
    void singleAnimationShow(int targetIndex,int duration=100);//单动画展示
    void parallelAnimationShow(int curTargetIndex,int nextTargetIndex,
                               ANIMATION_DIRECT direct,bool isTransitioned,int duration=250);//并行动画展示


signals:
    void currentPageChanged(int index);//动画结束当前页改变发送此信号

public slots:
    void animationFinished();

protected:
    virtual void mousePressEvent(QMouseEvent *event);//重写鼠标按下事件处理
    virtual void mouseReleaseEvent(QMouseEvent *event);//重写鼠标释放事件处理
    virtual void mouseMoveEvent(QMouseEvent *event);//重写鼠标移动事件处理
    //virtual void paintEvent(QPaintEvent *event);//重写绘图事件

private:
    //需要进行平移操作的部件列表，顺序即从左至右
    QList<QWidget *> widgetList;
    bool moveEnabled;//标识部件组是否可以平移
    //实现动画
    QPropertyAnimation *singleAnimation;//单动画
    QPropertyAnimation *parallelAnimation1;
    QPropertyAnimation *parallelAnimation2;
    QParallelAnimationGroup *animationGroup;//并行动画组
    bool transitioned;//标记是否由动画产生了状态迁移,即是否完成了页面的切换
    //使用常变量取代宏
    const int posX;//页面左上顶点的x坐标　move()方法参数就是左上顶点
    const int posY;//页面左上顶点的y坐标
    const int miniumMoveX;//最小的水平移动像素，以此值确定是否进行平移状态转换，越小越灵敏
    const int maximumMoveX;//首尾部件的最大移动距离

    int cursorX;//鼠标按下时指针的横坐标（相对于该容器本身）
    int moveX;//横轴方向移动的像素个数

    int currentPageIndex;//界面当前显示页号
    int pageNum;//页数　表示容器中的部件个数
    int pageWidth;//页面的宽度
    Ui::APPList *ui;

};

#endif // TRANSLATIONWIDGET_H
