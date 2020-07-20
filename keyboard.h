#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>

namespace Ui
{
    class KeyBoard;
}

class KeyBoard : public QWidget
{
    Q_OBJECT

public:
    explicit KeyBoard(QWidget *parent = 0);

    //输入框标志位
    int usr_flag; //用户名文本框
    int pwd_flag; //密码文本框
    int key_flag; //密钥文本框

    QString line; //键盘自带文本框传输的中间变量
    ~KeyBoard();

private:
    Ui::KeyBoard *ui;
    int upper_flag; //字母大小写切换标志

signals:
    void input_finished(); //输入完成信号

private slots:
    void input_edit(); //开始输入槽函数

    //按键反馈函数
    void on_finish_btn_clicked(); //输入完毕
    void on_clr_btn_clicked();    //清空文本框
    void on_delet_btn_clicked();  //退格

    void on_m_btn_clicked();
    void on_n_btn_clicked();
    void on_b_btn_clicked();
    void on_v_btn_clicked();
    void on_c_btn_clicked();
    void on_x_btn_clicked();
    void on_z_btn_clicked();
    void on_upper_btn_clicked();
    void on_l_btn_clicked();
    void on_k_btn_clicked();
    void on_j_btn_clicked();
    void on_h_btn_clicked();
    void on_g_btn_clicked();
    void on_f_btn_clicked();
    void on_d_btn_clicked();
    void on_s_btn_clicked();
    void on_a_btn_clicked();
    void on_p_btn_clicked();
    void on_o_btn_clicked();
    void on_i_btn_clicked();
    void on_u_btn_clicked();
    void on_y_btn_clicked();
    void on_t_btn_clicked();
    void on_r_btn_clicked();
    void on_w_btn_clicked();
    void on_q_btn_clicked();
    void on_btn_0_clicked();
    void on_btn_9_clicked();
    void on_btn_8_clicked();
    void on_btn_7_clicked();
    void on_btn_6_clicked();
    void on_btn_5_clicked();
    void on_btn_4_clicked();
    void on_btn_3_clicked();
    void on_btn_2_clicked();
    void on_btn_1_clicked();
    void on_e_btn_clicked();
    void char_change();
};

#endif // KEYBOARD_H
