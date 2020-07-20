#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QMessageBox>
#include "keyboard.h"
namespace Ui
{
    class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    QString usr; //实际用户名
    QString pwd; //实际密码
    ~Register();

private:
    Ui::Register *ui;
    KeyBoard *reg_board;
    QString key; //密钥

    bool usr_check(); //合法检查

signals:
    void back_login(); //返回登陆界面信号
    void reg_input();  //注册界面键盘输入信号

private slots:
    void on_back_btn_clicked();                      //返回按钮(Back)函数
    void on_update_btn_clicked();                    //更新注册账号按钮(Update)函数
    void catch_edit();                               //键盘内容捕获槽函数
    void in_register();                              //进入注册界面槽函数
    bool eventFilter(QObject *watch, QEvent *event); //事件捕获函数
};

#endif // REGISTER_H
