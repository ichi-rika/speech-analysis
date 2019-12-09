//
// Created by rika on 06/12/2019.
//

#ifndef SPEECH_ANALYSIS_MAINWINDOW_H
#define SPEECH_ANALYSIS_MAINWINDOW_H

#include <QtWidgets>
#include <QSharedPointer>
#include "../canvas/AnalyserCanvas.h"

extern QFont appFont;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent * event);

private:
    void updateFields();

    Analyser analyser;

    QTimer timer;

    QWidget * central;
    QVBoxLayout * vLayout1;
    QHBoxLayout * hLayout2;
    QHBoxLayout * hLayout3;
    QFormLayout * fLayout4;
    QHBoxLayout * hLayout5;

    QCheckBox * inputToggleSpectrum;
    QComboBox * inputFftSize;
    QSpinBox * inputLpOrder;
    QSpinBox * inputMaxFreq;
    QComboBox * inputFreqScale;
    QSpinBox * inputFrameSpace;
    QDoubleSpinBox * inputWindowSpan;
    std::array<QPushButton *, 4> inputFormantColor;

    AnalyserCanvas * canvas;

    QLineEdit * fieldPitch;
    std::vector<QLineEdit *> fieldFormant;

};

#endif //SPEECH_ANALYSIS_MAINWINDOW_H
