#include "videodevice.h"
#include <stdio.h>

//构造函数
VideoDevice::VideoDevice(QString dev_name)
{

    this->dev_name = dev_name;
    this->fd = -1;
    this->buffers = NULL;
    this->n_buffers = 0;
    this->index = -1;
}

//打开设备文件函数
int VideoDevice::open_device()
{
    //获取设备文件描述符
    fd = open(dev_name.toStdString().c_str(), O_RDWR, 0); //translate char to string and get addres

    if (-1 == fd)
    {
        emit display_error(tr("open: %1").arg(QString(strerror(errno)))); //translate and replace %l
        return -1;
    }
    return 0;
}

//关闭设备文件函数
int VideoDevice::close_device()
{
    if (-1 == close(fd))
    {
        emit display_error(tr("close: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

//设备初始化函数
int VideoDevice::init_device()
{
    v4l2_capability cap;  //设备功能
    v4l2_cropcap cropcap; //设置摄像头捕捉能力
    v4l2_crop crop;       //视频信号边框
    v4l2_format fmt;      //视频帧格式

    //查询设备功能
    if (-1 == ioctl(fd, VIDIOC_QUERYCAP, &cap))
    {
        if (EINVAL == errno)
        {
            emit display_error(tr("%1 is no V4l2 device").arg(dev_name));
        }
        else
        {
            emit display_error(tr("VIDIOC_QUERYCAP: %1").arg(QString(strerror(errno))));
        }
        return -1;
    }
    printf("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\n", cap.driver, cap.card, cap.bus_info, (cap.version >> 16) & 0XFF, (cap.version >> 8) & 0XFF, cap.version & 0XFF);

    //查询是否具有视频捕捉设备且具有流控制模式
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        emit display_error(tr("%1 is no video capture device").arg(dev_name));
        return -1;
    }
    if (!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        emit display_error(tr("%1 does not support streaming i/o").arg(dev_name));
        return -1;
    }

    CLEAR(cropcap); //清空结构体占用内存

    //设置捕获模式并查询
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 == ioctl(fd, VIDIOC_CROPCAP, &cropcap))
    {
        CLEAR(crop);
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect;

        if (-1 == ioctl(fd, VIDIOC_S_CROP, &crop))
        {
            if (EINVAL == errno)
            {
                //                emit display_error(tr("VIDIOC_S_CROP not supported"));
            }
            else
            {
                emit display_error(tr("VIDIOC_S_CROP: %1").arg(QString(strerror(errno))));
                return -1;
            }
        }
    }
    else
    {
        emit display_error(tr("VIDIOC_CROPCAP: %1").arg(QString(strerror(errno))));
        return -1;
    }

    CLEAR(fmt); //清空结构体占用内存

    //设置视频帧格式并查询
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 320;
    fmt.fmt.pix.height = 240;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    if (-1 == ioctl(fd, VIDIOC_S_FMT, &fmt))
    {
        emit display_error(tr("VIDIOC_S_FMT").arg(QString(strerror(errno))));
        return -1;
    }

    if (-1 == init_mmap())
    {
        return -1;
    }

    return 0;
}

//初始化缓冲空间
int VideoDevice::init_mmap()
{
    v4l2_requestbuffers req; //帧缓冲
    CLEAR(req);              //清空结构体占用内存

    //设置帧缓冲数量大小等并查询
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == ioctl(fd, VIDIOC_REQBUFS, &req))
    {
        if (EINVAL == errno)
        {
            emit display_error(tr("%1 does not support memory mapping").arg(dev_name));
            return -1;
        }
        else
        {
            emit display_error(tr("VIDIOC_REQBUFS %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    if (req.count < 2)
    {
        emit display_error(tr("Insufficient buffer memory on %1").arg(dev_name));
        return -1;
    }

    //动态分配存储空间，用于帧缓冲
    buffers = (buffer *)calloc(req.count, sizeof(*buffers));

    if (!buffers)
    {
        emit display_error(tr("out of memory"));
        return -1;
    }

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
    {
        v4l2_buffer buf; //申请图像一帧图像缓存
        CLEAR(buf);      //清空结构体占用内存

        //设置缓存空间格式并查询
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;

        if (-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf))
        {
            emit display_error(tr("VIDIOC_QUERYBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }

        //存储buffer长度、映射空间并查询
        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start =
            mmap(NULL, // start anywhere
                 buf.length,
                 PROT_READ | PROT_WRITE,
                 MAP_SHARED,
                 fd, buf.m.offset);

        if (MAP_FAILED == buffers[n_buffers].start)
        {
            emit display_error(tr("mmap %1").arg(QString(strerror(errno))));
            return -1;
        }
    }
    return 0;
}

//设备启动采集
int VideoDevice::start_capturing()
{
    unsigned int i;
    for (i = 0; i < n_buffers; ++i)
    {
        v4l2_buffer buf;

        CLEAR(buf); //清空结构体占用内存

        //设置帧缓冲并查询
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP; //mmap
        buf.index = i;                 //index

        //read data from the buf
        if (-1 == ioctl(fd, VIDIOC_QBUF, &buf))
        {
            emit display_error(tr("VIDIOC_QBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    //存储捕获模式类型
    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    //启动视频显示
    if (-1 == ioctl(fd, VIDIOC_STREAMON, &type))
    {
        emit display_error(tr("VIDIOC_STREAMON: %1").arg(QString(strerror(errno))));
        return -1;
    }

    return 0;
}

//设备停止采集
int VideoDevice::stop_capturing()
{
    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    //停止视频显示
    if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &type))
    {
        emit display_error(tr("VIDIOC_STREAMOFF: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

//设备卸载
int VideoDevice::uninit_device()
{
    unsigned int i;
    for (i = 0; i < n_buffers; ++i)
    {
        //解除缓存映射
        if (-1 == munmap(buffers[i].start, buffers[i].length))
        {
            emit display_error(tr("munmap: %1").arg(QString(strerror(errno))));
            return -1;
        }
    }
    free(buffers); //释放空间
    return 0;
}

//获取帧数据
int VideoDevice::get_frame(void **frame_buf, size_t *len)
{
    v4l2_buffer queue_buf;
    CLEAR(queue_buf);

    queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    queue_buf.memory = V4L2_MEMORY_MMAP;

    //从视频缓冲队列中取出有数据的缓冲区
    if (-1 == ioctl(fd, VIDIOC_DQBUF, &queue_buf))
    {
        switch (errno)
        {
        case EAGAIN:
            //            perror("dqbuf");
            return -1;
        case EIO:
            return -1;
        default:
            emit display_error(tr("VIDIOC_DQBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    //获取带有数据的帧缓冲区起始地址与长度
    *frame_buf = buffers[queue_buf.index].start;
    *len = buffers[queue_buf.index].length;
    index = queue_buf.index;

    return 0;
}

//释放帧数据
int VideoDevice::unget_frame()
{
    if (index != -1)
    {
        v4l2_buffer queue_buf;

        CLEAR(queue_buf);

        queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        queue_buf.memory = V4L2_MEMORY_MMAP;
        queue_buf.index = index;

        //清空并初始化带数据的缓冲区
        if (-1 == ioctl(fd, VIDIOC_QBUF, &queue_buf))
        {
            emit display_error(tr("VIDIOC_QBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
        return 0;
    }
    return -1;
}
