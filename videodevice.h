#ifndef VIDEODEVICE_H
#define VIDEODEVICE_H

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/mman.h>

#include <asm/types.h>
#include <linux/videodev2.h>

#include <QString>
#include <QObject>

#define CLEAR(x) memset(&(x), 0, sizeof(x)) //内存清空函数

class VideoDevice : public QObject
{
    Q_OBJECT
public:
    VideoDevice(QString dev_name);
    int open_device();                //设备文件打开函数
    int close_device();               //设备文件关闭函数
    int init_device();                //设备初始化函数
    int start_capturing();            //视频采集函数
    int stop_capturing();             //停止采集函数
    int uninit_device();              //卸载设备函数
    int get_frame(void **, size_t *); //获取帧数据函数
    int unget_frame();                //释放帧数据函数

private:
    int init_mmap(); //初始化映射空间函数

    struct buffer
    {
        void *start;   //起始地址指针
        size_t length; //buffer长度
    };
    QString dev_name;       //设备名
    int fd;                 //文件描述符
    buffer *buffers;        //缓冲区
    unsigned int n_buffers; //缓冲区数量
    int index;              //缓冲区下标

signals:
    //出错信号
    void display_error(QString);
};

#endif // VIDEODEVICE_H
