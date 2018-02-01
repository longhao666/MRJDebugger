#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <QWidget>
#include <QTimer>
#include "oscilloscope_global.h"
#include "./usercontroloncan.h"
#include "./oscilloscopethread.h"
#include "Component/paintarea.h"
#include "Component/showqueue.h"
#include <qwt/include/qwt_plot.h>
#include <qwt/include/qwt_plot_curve.h>
#include <qwt/include/qwt_plot_grid.h>
#include <qwt/include/qwt_plot_layout.h>
#include <qwt/include/qwt_plot_magnifier.h>
#include <qwt/include/qwt_plot_panner.h>

namespace Ui {
class OscilloScope;
}

class OSCILLOSCOPESHARED_EXPORT OscilloScope : public QWidget {
  Q_OBJECT

public:
  explicit OscilloScope(UserControlOnCan *can1,
                  Joint *jointBeingUsed,
                  QWidget *parent = 0);
  virtual ~OscilloScope();
  QwtPlotCurve *curveTgPOS = NULL;
  QwtPlotCurve *curveTgSPD = NULL;
  QwtPlotCurve *curveTgCUR = NULL;
  QwtPlotCurve *curveRlPOS = NULL;
  QwtPlotCurve *curveRlSPD = NULL;
  QwtPlotCurve *curveRlCUR = NULL;
  UserControlOnCan *can1;                     // 标记所用的CAN总线
  Joint *jointBeingUsed;                      // 标记所控制的关节模块

public slots:
  void OscilloScopeInitialize(uint16_t ID);

signals:

private slots:
  void on_tgPOSPushButton_clicked();
  void on_tgSPDPushButton_clicked();
  void on_tgCURPushButton_clicked();
  void on_rlPOSPushButton_clicked();
  void on_rlSPDPushButton_clicked();
  void on_rlCURPushButton_clicked();
  void on_scopeEnablePushButton_clicked();
//  void on_offset_POSLineEdit_editingFinished();
//  void on_offset_SPDLineEdit_editingFinished();
//  void on_offset_CURLineEdit_editingFinished();
//  void on_prComboBox_currentIndexChanged(int index);
//  void on_srComboBox_currentIndexChanged(int index);
//  void on_crComboBox_currentIndexChanged(int index);
//  void on_ScanFrequencyComboBox_currentIndexChanged(int index);
  void updatePlot();

private:
  void SetValue(double value);
  Ui::OscilloScope *uiOscilloScope = NULL;
  QString tgPOSPushButtonOn;
  QString tgPOSPushButtonOff;
  QString tgSPDPushButtonOn;
  QString tgSPDPushButtonOff;
  QString tgCURPushButtonOn;
  QString tgCURPushButtonOff;
  QString rlPOSPushButtonOn;
  QString rlPOSPushButtonOff;
  QString rlSPDPushButtonOn;
  QString rlSPDPushButtonOff;
  QString rlCURPushButtonOn;
  QString rlCURPushButtonOff;
  QString scopeEnablePushButtonOn;
  QString scopeEnablePushButtonOff;
  OscilloScopeThread* osthread = NULL;
  QTimer * timerOscilloScope  = NULL;
  QwtPlotGrid * grid = NULL;
  QwtPlotCurve * curveGrid = NULL;
  QwtPlotMagnifier * plotMag = NULL;
  QwtPlotMagnifier * plotSPDMag = NULL;
  QwtPlotMagnifier * plotPOSMag = NULL;
};

#endif // OSCILLOSCOPE_H
