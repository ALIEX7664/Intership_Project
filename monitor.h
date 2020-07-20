#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>
#include <QPushButton>
#include <QtGui>
#include <QDir>
#include "videodevice.h"
#include <QLabel>
#include <QDateTime>
#define MAX_VIDEO 10 //最大可存储视频数量

class Monitor : public QWidget
{
    Q_OBJECT

public:
    explicit Monitor(QWidget *parent = 0);
    ~Monitor();

private:
    QPushButton *rec;   //录像按钮
    QPushButton *end;   //结束录像
    QPushButton *video; //进入回放界面按钮
    QPushButton *back;  //返回按钮

    QPushButton *pre;   //前一个视频
    QPushButton *next;  //后一个视频
    QPushButton *play;  //播放按钮
    QPushButton *pause; //暂停按钮
    QPushButton *de;    //删除视频按钮
    QPushButton *x2;    //倍速按钮

    QDir *play_dir;        //目录指针
    QStringList type_list; //文件类型列表
    QStringList name_list; //目录中图片名称列表
    QString time_str;      //时间字符串，用于显示和保存系统时间
    QPainter *painter;     //视频绘制
    QImage *frame;         //图像显示
    QTimer *timer;         //定时器
    QLabel *label;         //无视频提示标签
    QLabel *count_label;   //视频数量标签
    QLabel *time_label;    //监控时间标签
    VideoDevice *vd;       //视频设备

    long i;           //播放图像序号
    long pic_count;   //录制图像计数
    long *pics_count; //多个视频的图片数量指针，相当于数组
    long num;         //当前视频序号，即视频下标
    int video_count;  //已录制视频数量统计

    uchar *p_rgb;     //rgb数据指针
    uchar *p_yuv;     //yuv数据指针
    unsigned int len; //缓冲区长度
    int rs;           //返回码

    bool record_flag; //录像标志位
    bool play_flag;   //播放标志位
    bool video_flag;  //回放界面标志位
    bool x2_flag;     //倍速标志位

    void update_btn();      //更新回放界面按钮状态与label标签的函数
    void pic_save();        //图片保存函数
    void pic_load();        //图片加载函数
    void catch_real_time(); //获取实际时间函数
    void get_dir();         //获取目录内容函数

    //像素转换函数
    int convert_yuv_to_rgb_pixel(int y, int u, int v);
    //像素存储函数
    int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);

signals:
    //返回登录界面信号
    void back_login();

private slots:
    void paintEvent(QPaintEvent *);  //绘图事件函数
    void display_error(QString err); //出错函数

    //点击事件函数
    void rec_clicked();
    void end_clicked();
    void play_clicked();
    void back_clicked();
    void video_clicked();
    void pre_clicked();
    void next_clicked();
    void pause_clicked();
    void de_clicked();
    void x2_clicked();

    void in_monitor();
};

#endif // MONITOR_H
