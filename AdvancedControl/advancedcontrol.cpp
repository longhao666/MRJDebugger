#include "advancedcontrol.h"
#include "ui_advancedcontrol.h"
#include <QMessageBox>
#include <QTextEdit>

AdvancedControl::AdvancedControl(QWidget *parent, UserControlOnCan *can) :
    QDialog(parent),
    ui(new Ui::AdvancedControl),
    can1(can)
{
    ui->setupUi(this);

}

AdvancedControl::~AdvancedControl()
{
    delete ui;
}

void AdvancedControl::on_buttonBox_accepted()
{
    bool sendFlag = true;
    bool ok = false;
    uint16_t ID = ui->lineEdit_ID->text().toUInt(&ok, 0); // 基数是0可以识别0x、0打头的数字
    if (ok == false) {
        sendFlag = false;
        QMessageBox::warning(this,"警告","ID出错！");
    }

    uint8_t cmd = 0;
    switch (ui->comboBox_CMDType->currentIndex()) { // 按照下拉菜单中的顺序
        case 0: cmd = CMDTYPE_RD; break;
        case 1: cmd = CMDTYPE_WR; break;
        case 2: cmd = CMDTYPE_WR_NR; break;
        case 3: cmd = CMDTYPE_WR_REG; break;
        case 4: cmd = CMDTYPE_SCP; break;
        case 5: cmd = CMDTYPE_RD_HP; break;
    }

    uint8_t index = ui->lineEdit_index->text().toUInt(&ok, 0);
    if (ok == false) {
        sendFlag = false;
        QMessageBox::warning(this,"警告","索引出错！");
    }

    int tmp = ui->lineEdit_data->text().toInt(&ok, 0);;
    uint8_t data[2] = { 0, 0 }; // 读指令时，API会忽略data
    data[1] = (uint8_t)((tmp & 0xff00) >> 8);
    data[0] = (uint8_t)(tmp & 0xff);

    int data_length = ui->lineEdit_dataLength->text().toInt(&ok, 0);
    if (ok == false) {
        sendFlag = false;
        QMessageBox::warning(this,"警告","数据长度出错！");
    }

    if (sendFlag) {
        can1->controller.SendMsg(ID, cmd, index, data, data_length);
        QMessageBox::information(this,"提示","命令已发送！");
    }
}

void AdvancedControl::on_comboBox_CMDType_currentIndexChanged(int index)
{
    switch (index) { // 按照下拉菜单中的顺序
        case 1:
        case 2:
        case 3: ui->lineEdit_dataLength->setText("2"); break;
    }
}
