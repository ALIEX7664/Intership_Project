#include "register.h"
#include "ui_register.h"

#include <QMessageBox>
Register::Register(QWidget *parent) : QWidget(parent),
                                      ui(new Ui::Register)
{
    //界面初始化
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QPalette pal;
    pal.setBrush(QPalette::Background, QBrush(QPixmap("/login_pic.jpg")));
    this->setPalette(pal);
    this->setWindowTitle(tr("User Update"));

    //信息初始化
    key = QString("default");
    usr = QString("root");
    pwd = QString("1234");

    //装载组件
    ui->usr_lineEdit->installEventFilter(this);
    ui->pwd_lineEdit->installEventFilter(this);
    ui->key_lineEdit->installEventFilter(this);

    //修改密码显示为暗文
    ui->pwd_lineEdit->setEchoMode(QLineEdit::Password);

    //创建键盘对象
    reg_board = new KeyBoard(this);
    reg_board->move(30, 20);

    //键盘操作信号与槽连接
    connect(this, SIGNAL(reg_input()), reg_board, SLOT(input_edit()));
    connect(reg_board, SIGNAL(input_finished()), this, SLOT(catch_edit()));
}

Register::~Register()
{
    delete ui;
}

//合法检查
bool Register::usr_check()
{
    //检测密钥是否正确
    if (ui->key_lineEdit->text() == key)
    {
        //修改真实用户名与密码
        usr = ui->usr_lineEdit->text();
        pwd = ui->pwd_lineEdit->text();
        return true;
    }
    else
    {
        //出错提示
        ui->c_label_2->setText("wrong key,please check your key");
        ui->c_label_2->setStyleSheet("color:red");
        return false;
    }
}

//更新按钮
void Register::on_update_btn_clicked()
{
    //信息合法，完成相关操作后发出信号
    if (usr_check())
    {
        ui->c_label_2->setText("");
        QMessageBox::information(this, "Usr Update", "update successful");
        this->hide();
        emit back_login();
    }
}

//返回按钮
void Register::on_back_btn_clicked()
{
    ui->c_label_2->setText("");
    this->hide();
    emit back_login();
}

//事件捕获器
bool Register::eventFilter(QObject *watch, QEvent *event)
{
    //捕获发生鼠标按压的组件
    if (watch == ui->usr_lineEdit && event->type() == QEvent::MouseButtonPress)
    {
        reg_board->usr_flag = 1;                    //将键盘用户标志位置为真，表示当前输入为用户
        reg_board->line = ui->usr_lineEdit->text(); //键盘文本框获取用户名文本框内容，用于拼接
        emit reg_input();                           //发出键盘输入信号
    }

    //同上
    if (watch == ui->pwd_lineEdit && event->type() == QEvent::MouseButtonPress)
    {
        reg_board->pwd_flag = 1;
        reg_board->line = ui->pwd_lineEdit->text();
        emit reg_input();
    }

    if (watch == ui->key_lineEdit && event->type() == QEvent::MouseButtonPress)
    {
        reg_board->key_flag = 1;
        reg_board->line = ui->key_lineEdit->text();
        emit reg_input();
    }

    return QWidget::eventFilter(watch, event);
}

//键盘输入完毕，捕获键盘文本框内容
void Register::catch_edit()
{
    //判断调用的组件，然后将键盘文本框内容捕获，同时将标志位清空。
    if (reg_board->usr_flag)
    {
        ui->usr_lineEdit->setText(reg_board->line);
        reg_board->usr_flag = 0;
    }
    else if (reg_board->pwd_flag)
    {
        ui->pwd_lineEdit->setText(reg_board->line);
        reg_board->pwd_flag = 0;
    }
    else
    {
        ui->key_lineEdit->setText(reg_board->line);
        reg_board->key_flag = 0;
    }
}

//进入注册界面槽函数
void Register::in_register()
{
    //清空之前输入的内容
    ui->usr_lineEdit->setText("");
    ui->pwd_lineEdit->setText("");
    ui->key_lineEdit->setText("");
    this->show();   //显示注册界面
}
