#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

signals:
    void sendingDialogData(const QString& name, const bool& visible, const QColor& colour);

public slots:
    void handleTextInput();
    void handleRedSlider();
    void handleGreenSlider();
    void handleBlueSlider();
    void handleVisibleCheck();
    void updateColourDisplay();
    void accept() override;

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
