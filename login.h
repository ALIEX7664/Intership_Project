#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "monitor.h"
#include "register.h"
#include "keyboard.h"
namespace Ui
{
    class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private:
    Ui::Login *ui;

    Monitor *mo;         //监控界面
    Register *re;        //注册界面
    KeyBoard *log_board; //键盘

    QString usr;      //用户名
    QString pwd;      //密码
    bool usr_check(); //合法用户检查函数

signals:

    void show_register(); //注册界面跳转信号
    void show_monitor();  //监控界面跳转信号
    void log_input();     //登录界面键盘输入信号

private slots:
    void on_reg_btn_clicked();                       //注册按钮(Usr Update)点击函数
    void on_login_btn_clicked();                     //登录按钮(Login)点击函数
    void in_login();                                 //进入登录界面槽函数
    void catch_edit();                               //获取键盘内容槽函数
    bool eventFilter(QObject *watch, QEvent *event); //事件捕获函数
};

#endif // LOGIN_H
