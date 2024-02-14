#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

/**
 * @class Dialog
 * @brief A custom dialog class derived from QDialog.
 * 
 * This class represents a dialog window that allows the user to adjust RGB sliders
 * and display the selected color. It emits a signal when the user accepts the dialog,
 * providing the selected color information.
 */
class Dialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Dialog object.
     * @param parent The parent widget.
     */
    explicit Dialog(QWidget *parent = nullptr);

    /**
     * @brief Destroys the Dialog object.
     */
    ~Dialog();

signals:
    /**
     * @brief Signal emitted when the dialog data is ready to be sent.
     * @param name The name sent back.
     * @param visible The visibility state.
     * @param colour The selected color.
     */
    void sendingDialogData(const QString& name, const bool& visible, const QColor& colour);

public slots:
    /**
     * @brief Slot for handling the red slider value change.
     */
    void handleRedSlider();

    /**
     * @brief Slot for handling the green slider value change.
     */
    void handleGreenSlider();

    /**
     * @brief Slot for handling the blue slider value change.
     */
    void handleBlueSlider();

    /**
     * @brief Slot for updating the color display based on the slider values.
     */
    void updateColourDisplay();

    /**
     * @brief Overridden slot for accepting the dialog.
     */
    void accept() override;

private:
    Ui::Dialog *ui; /**< The user interface object for the dialog. */
};

#endif // DIALOG_H
