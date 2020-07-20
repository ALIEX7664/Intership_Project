#include "login.h"
#include "ui_login.h"
#include "register.h"
Login::Login(QWidget *parent) : QWidget(parent),
                                ui(new Ui::Login)
{
    //界面初始化
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QPalette pal;
    pal.setBrush(QPalette::Background, QBrush(QPixmap("/login_pic.jpg")));
    this->setPalette(pal);
    this->setWindowTitle(tr("Login"));

    //装载组件并初始化默认用户名与密码
    ui->usr_lineEdit->installEventFilter(this);
    ui->pwd_lineEdit->installEventFilter(this);
    ui->usr_lineEdit->setText("root");
    ui->pwd_lineEdit->setEchoMode(QLineEdit::Password);
    ui->pwd_lineEdit->setText("1234");
    ui->usr_lineEdit->raise();
    ui->pwd_lineEdit->raise();

    //创建相关对象
    mo = new Monitor();
    re = new Register();
    log_board = new KeyBoard(this); //this表示在本ui界面上创建
    log_board->move(30, 20);        //移动键盘组件位置

    //页面进入信号与槽连接
    connect(this, SIGNAL(show_register()), re, SLOT(in_register()));
    connect(this, SIGNAL(show_monitor()), mo, SLOT(in_monitor()));

    //返回页面信号与槽连接
    connect(re, SIGNAL(back_login()), this, SLOT(in_login()));
    connect(mo, SIGNAL(back_login()), this, SLOT(in_login()));

    //键盘操作信号与槽连接
    connect(this, SIGNAL(log_input()), log_board, SLOT(input_edit()));
    connect(log_board, SIGNAL(input_finished()), this, SLOT(catch_edit()));
}

Login::~Login()
{
    delete ui;
}

//合法检查
bool Login::usr_check()
{
    //从文本框中获取用户名与密码
    usr = ui->usr_lineEdit->text();
    pwd = ui->pwd_lineEdit->text();

    //与注册的用户名和密码匹配
    if (usr == re->usr && pwd == re->pwd)
    {
        //清空提示
        ui->c_label->setText("");
        return true;
    }
    else
    {
        //出错提示
        ui->c_label->setStyleSheet("color:red");
        ui->c_label->setText("username or password error!");
        return false;
    }
}

//登录按钮
void Login::on_login_btn_clicked()
{
    //判断账号合法性，决定是否进入视频监控界面
    if (usr_check())
    {
        this->hide();         //隐藏登录界面
        mo->resize(480, 260); //设定监控界面大小
        emit show_monitor();  //发出监控界面启动信号
    }
}

//注册按钮
void Login::on_reg_btn_clicked()
{
    ui->c_label->setText(""); //清空提示
    this->hide();             //隐藏登陆界面
    emit show_register();     //发出注册界面启动信号
}

//事件捕获器
bool Login::eventFilter(QObject *watch, QEvent *event)
{
    //捕获发生鼠标按压的组件
    if (watch == ui->usr_lineEdit && event->type() == QEvent::MouseButtonPress)
    {
        log_board->usr_flag = 1;                    //将键盘用户标志位置为真，表示当前输入为用户
        log_board->line = ui->usr_lineEdit->text(); //键盘文本框获取用户名文本框内容，用于拼接
        emit log_input();                           //发出键盘输入信号
    }

    //同上
    if (watch == ui->pwd_lineEdit && event->type() == QEvent::MouseButtonPress)
    {
        log_board->pwd_flag = 1;
        log_board->line = ui->pwd_lineEdit->text();
        emit log_input();
    }

    return QWidget::eventFilter(watch, event);
}

//键盘输入完毕，捕获键盘文本框内容
void Login::catch_edit()
{
    //判断调用的组件，然后将键盘文本框内容捕获，同时将标志位清空。
    if (log_board->usr_flag)
    {
        ui->usr_lineEdit->setText(log_board->line);
        log_board->usr_flag = 0;
    }
    else
    {
        ui->pwd_lineEdit->setText(log_board->line);
        log_board->pwd_flag = 0;
    }
}

//返回登陆界面信号
void Login::in_login()
{
    //清空之前保留的内容并显示登陆界面
    ui->usr_lineEdit->setText("");
    ui->pwd_lineEdit->setText("");
    this->show();
}
