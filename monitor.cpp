#include "monitor.h"
#include "ui_monitor.h"

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
}

Monitor::Monitor(QWidget *parent) : QWidget(parent)
{
    //创建文件夹
    QDir dir;
    for (long j = 0; j < MAX_VIDEO; j++)
    {
        dir.mkdir("./pic" + QString::number(j));
    }

    //初始化标志位和计数变量
    record_flag = false;
    play_flag = false;
    video_flag = false;
    x2_flag = false;
    video_count = 0;
    pic_count = 0;
    i = 0;

    //动态生成按钮控件并绑定信号和槽
    rec = new QPushButton(this);
    rec->show();
    rec->move(370, 25);
    rec->resize(70, 25);
    rec->setText(tr("Record"));
    connect(rec, SIGNAL(clicked()), this, SLOT(rec_clicked()));

    end = new QPushButton(this);
    end->show();
    end->move(370, 85);
    end->resize(70, 25);
    end->setText(tr("End"));
    connect(end, SIGNAL(clicked()), this, SLOT(end_clicked()));

    video = new QPushButton(this);
    video->show();
    video->move(370, 145);
    video->resize(70, 25);
    video->setText(tr("Video"));
    connect(video, SIGNAL(clicked()), this, SLOT(video_clicked()));

    back = new QPushButton(this);
    back->show();
    back->move(370, 205);
    back->resize(70, 25);
    back->setText(tr("Back"));
    connect(back, SIGNAL(clicked()), this, SLOT(back_clicked()));

    play = new QPushButton(this);
    play->hide();
    play->move(335, 25);
    play->resize(70, 25);
    play->setText(tr("Play"));
    connect(play, SIGNAL(clicked()), this, SLOT(play_clicked()));

    pause = new QPushButton(this);
    pause->hide();
    pause->move(410, 25);
    pause->resize(70, 25);
    pause->setText(tr("Pause"));
    connect(pause, SIGNAL(clicked()), this, SLOT(pause_clicked()));

    pre = new QPushButton(this);
    pre->hide();
    pre->move(335, 85);
    pre->resize(70, 25);
    pre->setText(tr("Pre"));
    connect(pre, SIGNAL(clicked()), this, SLOT(pre_clicked()));

    next = new QPushButton(this);
    next->hide();
    next->move(410, 85);
    next->resize(70, 25);
    next->setText(tr("Next"));
    connect(next, SIGNAL(clicked()), this, SLOT(next_clicked()));

    de = new QPushButton(this);
    de->hide();
    de->move(370, 165);
    de->resize(70, 25);
    de->setText(tr("Delete"));
    connect(de, SIGNAL(clicked()), this, SLOT(de_clicked()));

    x2 = new QPushButton(this);
    x2->hide();
    x2->move(370, 125);
    x2->resize(70, 25);
    x2->setText(tr("x1"));
    connect(x2, SIGNAL(clicked()), this, SLOT(x2_clicked()));

    //动态生成标签
    label = new QLabel(this);
    label->move(100, 125);
    label->setText("empty video");
    label->hide();

    time_label = new QLabel(this);
    time_label->move(5, 5);
    time_label->setStyleSheet("color:white");
    catch_real_time(); //获取当前系统时间

    count_label = new QLabel(this);
    count_label->move(375, 5);
    count_label->resize(100,20);

    //申请存储空间
    p_rgb = (unsigned char *)malloc(320 * 240 * 3 * sizeof(char));
    pics_count = (long *)calloc(MAX_VIDEO, sizeof(long));
    painter = new QPainter(this);
    frame = new QImage(p_rgb, 320, 240, QImage::Format_RGB888);
    play_dir = new QDir();

    type_list << "*.jpg"; //后缀存入类型列表，用于目录文件读取

    //遍历文件夹，将已存在的视频帧数量保存到pics_count数组中，并增加相应得统计
    for (num = 0; num < MAX_VIDEO; num++)
    {

        get_dir();
        if (name_list.count() != 0)
        {

            pics_count[num] = name_list.count();
            video_count++;
        }
    }

    //设置视频数量标签
    count_label->setText("video: " + QString::number(video_count) + "/" + QString::number(MAX_VIDEO));
    num = 0;

    //获取摄像头设备
    vd = new VideoDevice(tr("/dev/video0"));
    connect(vd, SIGNAL(display_error(QString)), this, SLOT(display_error(QString)));

    //开启设备，执行初始化
    rs = vd->open_device();
    if (-1 == rs)
    {
        QMessageBox::warning(this, tr("error"), tr("open /dev/dsp error"), QMessageBox::Yes);
        vd->close_device();
    }

    rs = vd->init_device();
    if (-1 == rs)
    {
        QMessageBox::warning(this, tr("error"), tr("init failed"), QMessageBox::Yes);
        vd->close_device();
    }

    //开始视频采集
    rs = vd->start_capturing();
    if (-1 == rs)
    {
        QMessageBox::warning(this, tr("error"), tr("start capture failed"), QMessageBox::Yes);
        vd->close_device();
    }
    if (-1 == rs)
    {
        QMessageBox::warning(this, tr("error"), tr("get frame failed"), QMessageBox::Yes);
        vd->stop_capturing();
    }

    //设置定时器，绑定更新函数，用于更新绘图事件
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer->start(100); //100毫秒一周期

    //设置标题
    setWindowTitle(tr("Monitor System"));
}

//析构函数
Monitor::~Monitor()
{
    rs = vd->stop_capturing();
    rs = vd->uninit_device();
    rs = vd->close_device();
}

//每100毫秒触发一次绘图事件
void Monitor::paintEvent(QPaintEvent *)
{
    //通过是否进入回放界面进行判断
    if (!video_flag)
    {
        //获取帧数据并将二进制转换成图片
        rs = vd->get_frame((void **)&p_yuv, &len);
        convert_yuv_to_rgb_buffer(p_yuv, p_rgb, 320, 240);
        frame->loadFromData((uchar *)p_rgb, 320 * 240 * 3 * sizeof(char));

        catch_real_time();

        //在屏幕上绘制捕获的画面
        painter->begin(this);
        painter->drawImage(5, 5, *frame);
        painter->end();

        //开启录制
        if (record_flag)
        {
            //录制时保存图片
            pic_save();
        }
        rs = vd->unget_frame(); //释放帧数据，即释放缓冲区
    }
    else
    {
        //进入回放界面或触发暂停按钮时，只绘制一帧图像
        if (!play_flag)
        {
            QPainter pic_painter(this);
            QString str = name_list.at(i);                                                                 //获取图片名称
            time_label->setText(str.left(19));                                                             //截断系统时间字符串用于显示
            pic_painter.drawPixmap(5, 5, QPixmap("./pic" + QString::number(num) + "/" + name_list.at(i))); //绘制图片
        }
        else
        {
            //录像回放
            pic_load();
        }
    }
}

//出错处理
void Monitor::display_error(QString err)
{
    QMessageBox::warning(this, tr("error"), err, QMessageBox::Yes);
}

//yuv格式转换为rgb格式
int Monitor::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
    unsigned int in, out = 0;
    unsigned int pixel_16;
    unsigned char pixel_24[3];
    unsigned int pixel32;
    int y0, u, y1, v;
    for (in = 0; in < width * height * 2; in += 4)
    {
        pixel_16 =
            yuv[in + 3] << 24 |
            yuv[in + 2] << 16 |
            yuv[in + 1] << 8 |
            yuv[in + 0];
        y0 = (pixel_16 & 0x000000ff);
        u = (pixel_16 & 0x0000ff00) >> 8;
        y1 = (pixel_16 & 0x00ff0000) >> 16;
        v = (pixel_16 & 0xff000000) >> 24;
        pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
        pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
    }
    return 0;
}

int Monitor::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
    unsigned int pixel32 = 0;
    unsigned char *pixel = (unsigned char *)&pixel32;
    int r, g, b;
    r = y + (1.370705 * (v - 128));
    g = y - (0.698001 * (v - 128)) - (0.337633 * (u - 128));
    b = y + (1.732446 * (u - 128));
    if (r > 255)
        r = 255;
    if (g > 255)
        g = 255;
    if (b > 255)
        b = 255;
    if (r < 0)
        r = 0;
    if (g < 0)
        g = 0;
    if (b < 0)
        b = 0;
    pixel[0] = r * 220 / 256;
    pixel[1] = g * 220 / 256;
    pixel[2] = b * 220 / 256;
    return pixel32;
}

//存储每一帧图片
void Monitor::pic_save()
{
    //将系统时间作为图片名称进行存储
    frame->save("./pic" + QString::number(num) + "/" + time_str + ".jpg");
    pic_count++;
}

//播放存储的图片
void Monitor::pic_load()
{
    QPainter pic_painter(this);
    if (pics_count[num] != 0)
    {
        QString str = name_list.at(i);     //获取图片名称
        time_label->setText(str.left(19)); //截断系统时间字符串用于显示

        //拼接当前文件夹命名+文件名
        pic_painter.drawPixmap(5, 5, QPixmap("./pic" + QString::number(num) + "/" + name_list.at(i++)));
        if (i == pics_count[num] - 1)
        {
            i = 0;
        }
    }
}

//录像按钮
void Monitor::rec_clicked()
{
    int j;
    if (!record_flag)
    {
        //遍历数组，找到空的视频位置
        for (j = 0; j < MAX_VIDEO; j++)
        {
            if (pics_count[j] == 0)
            {
                num = j;
                break;
            }
        }

        //视频存储已满
        if (j == MAX_VIDEO)
        {
            QMessageBox::warning(this, tr("error"), tr("max videos,please delete \nvideo and try again"), QMessageBox::Yes);
            return;
        }

        //设置标志位和帧数量记录变量
        record_flag = true;
        pic_count = 0;
        rec->setStyleSheet("background-color:#EB3300;border-radius:2");
    }
}

//结束按钮
void Monitor::end_clicked()
{
    if (record_flag)
    {
        //当前正在录制，暂停录制，保存pic_count图片数量到对应的视频下标
        record_flag = false;
        pics_count[num] = pic_count;
        rec->setStyleSheet("");
        video_count++; //视频数量加1
        update_btn();  //更新count_label标签表示的视频数量
    }
}

//播放按钮
void Monitor::play_clicked()
{
    //判断当前下标是否已录制视频
    if (pics_count[num] == 0)
    {
        QMessageBox::warning(this, tr("error"), tr("no video to play"), QMessageBox::Yes);
        return;
    }

    //开启播放
    if (!play_flag)
    {
        play_flag = true;
        pause->setStyleSheet("");
        play->setStyleSheet("background-color:#009E6F;border-radius:2");
    }
}

//暂停按钮
void Monitor::pause_clicked()
{

    //暂停当前播放，根据绘图事件函数，将图像停在下一帧
    if (play_flag)
    {
        play_flag = false;
        play->setStyleSheet("");
        pause->setStyleSheet("background-color:#EB3300;border-radius:2");
    }
}

//返回按钮，录制与回放界面共用
void Monitor::back_clicked()
{
    //判断是在回放或录制界面
    if (!video_flag)
    {
        //返回登陆界面操作
        record_flag = false;
        rec->setStyleSheet("");
        play->setStyleSheet("");
        this->hide();

        emit back_login(); //返回登陆界面信号
    }
    else
    {
        //返回录制界面操作

        //隐藏录制界面按钮
        play->hide();
        pre->hide();
        next->hide();
        pause->hide();
        de->hide();
        x2->hide();

        //显示回放界面按钮
        rec->show();
        end->show();
        video->show();

        //对回放界面还原
        play_flag = false;
        video_flag = false;
        label->hide();
        update_btn(); //更新按钮状态
    }
}

//回放界面按钮
void Monitor::video_clicked()
{
    //判断当前是否在录制状态，只有停止录制状态才可进入
    if (!record_flag)
    {
        //判断当前是否有录像存在，没有则禁止进入回放界面
        if (video_count == 0)
        {
            QMessageBox::warning(this, tr("error"), tr("no video"), QMessageBox::Yes);
            return;
        }

        //隐藏录像界面按钮
        rec->hide();
        end->hide();
        video->hide();

        //显示回放界面按钮
        play->show();
        pre->show();
        next->show();
        pause->show();
        de->show();
        x2->show();

        //初始化
        play_flag = false;
        video_flag = true;

        //获取数组中存在视频的下标
        for (int j = 0; j < MAX_VIDEO; j++)
        {
            if (pics_count[j] != 0)
            {
                num = j;
                break;
            }
        }
        i = 0;
        update_btn(); //更新按钮状态

        get_dir(); //获取首个视频的目录内容
    }
}

//前翻按钮
void Monitor::pre_clicked()
{

    //找到存在视频的下标并返回，num代表视频下标
    while (num > 0)
    {
        if (pics_count[--num] != 0)
        {
            i = 0;             //初始化播放变量
            play_flag = false; //防止切换视频后继续播放
            break;
        }
    }
    update_btn(); //更新按钮状态

    //若存在视频则获取该视频目录的内容
    if (pics_count[num] != 0)
    {
        get_dir();
    }
}

//后翻按钮
void Monitor::next_clicked()
{
    //找到存在视频的下标并返回，num代表视频下标
    while (num < MAX_VIDEO - 1)
    {
        if (pics_count[++num] != 0)
        {
            i = 0;             //初始化播放变量
            play_flag = false; //防止切换视频后继续播放
            break;
        }
    }
    update_btn(); //更新按钮状态

    //若存在视频则获取该视频目录的内容
    if (pics_count[num] != 0)
    {
        get_dir();
    }
}

//删除视频按钮
void Monitor::de_clicked()
{
    //创建QDir变量，用来获取目录描述符
    QDir *de_dir = new QDir("./pic" + QString::number(num));
    name_list = play_dir->entryList(type_list);
    play_flag = false; //防止删除视频时播放

    //将目录里的所有帧图片删除
    for (int k = 0; k < name_list.count(); k++)
    {
        if (!de_dir->remove("./" + name_list.at(k)))
        {
            return;
        }
    }

    //后续清除操作
    pics_count[num] = 0;                                                 //重置图片数量
    video_count--;                                                       //视频数量减少
    update_btn();                                                        //更新count_label表示的视频数量和播放按钮样式
    QMessageBox::information(this, "Delete", "delete video successful"); //提示删除状态已成功

    free(de_dir); //释放空间
}

//倍速播放
void Monitor::x2_clicked()
{
    x2_flag ^= 1;

    //通过调节计时器来增快速度
    if (x2_flag == true)
    {
        timer->start(50);
        x2->setText("x2");
    }
    else
    {
        timer->start(100);
        x2->setText("x1");
    }
}

//按钮状态与count_label变量状态更新更新
void Monitor::update_btn()
{
    //当前在回放页面
    if (video_flag)
    {
        //清空播放按钮与暂停按钮样式
        play->setStyleSheet("");
        pause->setStyleSheet("");

        //判断是否把前翻按钮设为可用
        if (num == 0)
        {
            pre->setEnabled(false);
        }
        else
        {
            pre->setEnabled(true);
        }

        //判断是否把后翻按钮设为可用
        if (num == MAX_VIDEO - 1)
        {
            next->setEnabled(false);
        }
        else
        {
            next->setEnabled(true);
        }

        //根据当前下标，判断是否存在视频
        if (pics_count[num] == 0)
        {
            label->show();
            time_label->hide();
            de->setEnabled(false);
        }
        else
        {
            label->hide();
            time_label->show();
            de->setEnabled(true);
        }
    }

    //重置倍速播放按钮情况
    if (x2_flag == true)
    {
        x2_clicked();
    }

    //设置表示视频数量的标签
    count_label->setText("video:" + QString::number(video_count) + "/" + QString::number(MAX_VIDEO));
}

//进入监控界面槽函数
void Monitor::in_monitor()
{
    this->show(); //显示监控界面
}

//系统时间捕获
void Monitor::catch_real_time()
{
    //获取系统当前时间，将其转化为固定格式并保存
    QDateTime time = QDateTime::currentDateTime();
    time_str = time.toString("yyyy-MM-dd hh:mm:ss:zzz");
    time_label->setText(time_str.left(19));
}

//目录获取
void Monitor::get_dir()
{
    //获取相关目录的内容，并获取其名称存入定义的名称列表中，用于遍历
    play_dir->setPath("./pic" + QString::number(num));
    name_list = play_dir->entryList(type_list); //根据过滤条件获取名称
}
