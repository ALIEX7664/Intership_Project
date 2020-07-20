#include "keyboard.h"
#include "ui_keyboard.h"

KeyBoard::KeyBoard(QWidget *parent) : QWidget(parent),
                                      ui(new Ui::KeyBoard)
{
    ui->setupUi(this);

    //标志位初始化
    upper_flag = 0;
    usr_flag = 0;
    pwd_flag = 0;
    key_flag = 0;

    //开始时隐藏自身
    this->hide();
}

KeyBoard::~KeyBoard()
{
    delete ui;
}

void KeyBoard::on_btn_1_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + "1");
}

void KeyBoard::on_btn_2_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + "2");
}

void KeyBoard::on_btn_3_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + "3");
}

void KeyBoard::on_btn_4_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + "4");
}

void KeyBoard::on_btn_5_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + "5");
}

void KeyBoard::on_btn_6_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + "6");
}

void KeyBoard::on_btn_7_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + "7");
}

void KeyBoard::on_btn_8_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + "8");
}

void KeyBoard::on_btn_9_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + "9");
}

void KeyBoard::on_btn_0_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + "0");
}

void KeyBoard::on_q_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "q");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "Q");
    }
}

void KeyBoard::on_w_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "w");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "W");
    }
}

void KeyBoard::on_e_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "e");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "E");
    }
}

void KeyBoard::on_r_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "r");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "R");
    }
}

void KeyBoard::on_t_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "t");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "T");
    }
}

void KeyBoard::on_y_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "y");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "Y");
    }
}

void KeyBoard::on_u_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "u");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "U");
    }
}

void KeyBoard::on_i_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "i");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "I");
    }
}

void KeyBoard::on_o_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "o");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "O");
    }
}

void KeyBoard::on_p_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "p");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "P");
    }
}

void KeyBoard::on_a_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "a");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "A");
    }
}

void KeyBoard::on_s_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "s");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "S");
    }
}

void KeyBoard::on_d_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "d");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "D");
    }
}

void KeyBoard::on_f_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "f");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "F");
    }
}

void KeyBoard::on_g_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "g");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "G");
    }
}

void KeyBoard::on_h_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "h");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "H");
    }
}

void KeyBoard::on_j_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "j");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "J");
    }
}

void KeyBoard::on_k_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "k");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "K");
    }
}

void KeyBoard::on_l_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "l");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "L");
    }
}

void KeyBoard::on_upper_btn_clicked()
{
    upper_flag ^= 1;
    char_change();
}

void KeyBoard::on_z_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "z");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "Z");
    }
}

void KeyBoard::on_x_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "x");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "X");
    }
}

void KeyBoard::on_c_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "c");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "C");
    }
}

void KeyBoard::on_v_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "v");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "V");
    }
}

void KeyBoard::on_b_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "b");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "B");
    }
}

void KeyBoard::on_n_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "n");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "N");
    }
}

void KeyBoard::on_m_btn_clicked()
{
    if (upper_flag == 0)
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "m");
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "M");
    }
}

void KeyBoard::on_delet_btn_clicked()
{

    ui->lineEdit->backspace();
}

void KeyBoard::on_clr_btn_clicked()
{
    ui->lineEdit->clear();
}

//输入完成函数
void KeyBoard::on_finish_btn_clicked()
{
    line = ui->lineEdit->text(); //将文本框内容拷贝至变量line，用于传出到输入框
    this->hide();                //隐藏键盘控件
    emit input_finished();       //发出输入完成信号
}

void KeyBoard::char_change()
{

    if (upper_flag == 0)
    {
        ui->a_btn->setText("a");
        ui->b_btn->setText("b");
        ui->c_btn->setText("c");
        ui->d_btn->setText("d");
        ui->e_btn->setText("e");
        ui->f_btn->setText("f");
        ui->g_btn->setText("g");
        ui->h_btn->setText("h");
        ui->i_btn->setText("i");
        ui->j_btn->setText("j");
        ui->k_btn->setText("k");
        ui->l_btn->setText("l");
        ui->m_btn->setText("m");
        ui->n_btn->setText("n");
        ui->o_btn->setText("o");
        ui->p_btn->setText("p");
        ui->q_btn->setText("q");
        ui->r_btn->setText("r");
        ui->s_btn->setText("s");
        ui->t_btn->setText("t");
        ui->u_btn->setText("u");
        ui->v_btn->setText("v");
        ui->w_btn->setText("w");
        ui->x_btn->setText("x");
        ui->y_btn->setText("y");
        ui->z_btn->setText("z");
    }
    else
    {
        ui->a_btn->setText("A");
        ui->b_btn->setText("B");
        ui->c_btn->setText("C");
        ui->d_btn->setText("D");
        ui->e_btn->setText("E");
        ui->f_btn->setText("F");
        ui->g_btn->setText("G");
        ui->h_btn->setText("H");
        ui->i_btn->setText("I");
        ui->j_btn->setText("J");
        ui->k_btn->setText("K");
        ui->l_btn->setText("L");
        ui->m_btn->setText("M");
        ui->n_btn->setText("N");
        ui->o_btn->setText("O");
        ui->p_btn->setText("P");
        ui->q_btn->setText("Q");
        ui->r_btn->setText("R");
        ui->s_btn->setText("S");
        ui->t_btn->setText("T");
        ui->u_btn->setText("U");
        ui->v_btn->setText("V");
        ui->w_btn->setText("W");
        ui->x_btn->setText("X");
        ui->y_btn->setText("Y");
        ui->z_btn->setText("Z");
    }
}

//开始输入槽函数
void KeyBoard::input_edit()
{
    ui->lineEdit->setText(line); //读取文本内容，内容是通过line从外部文本框获取的
    this->show();                //显示键盘

    //判断内容格式，决定用明文或暗文显示
    if (usr_flag || key_flag)
    {
        ui->lineEdit->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->lineEdit->setEchoMode(QLineEdit::Password);
    }
}
