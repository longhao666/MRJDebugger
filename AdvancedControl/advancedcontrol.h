#ifndef ADVANCEDCONTROL_H
#define ADVANCEDCONTROL_H

#include "usercontroloncan.h"
#include <QDialog>

namespace Ui {
class AdvancedControl;
}

class AdvancedControl : public QDialog
{
    Q_OBJECT

public:
    explicit AdvancedControl(QWidget *parent = 0, UserControlOnCan *can = NULL);
    ~AdvancedControl();

private slots:
    void on_buttonBox_accepted();

    void on_comboBox_CMDType_currentIndexChanged(int index);

private:
    Ui::AdvancedControl *ui;
    UserControlOnCan *can1;
};

#endif // ADVANCEDCONTROL_H
