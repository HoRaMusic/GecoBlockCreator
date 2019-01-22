#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QTextStream>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
private slots:
    void setInputs(const QString inputs);
    void setOutputs(const QString inputs);
    void setBlockName(const QString inputs);
    void setFileName(const QString inputs);
    void setClassName(const QString inputs);
    void setCategory(const QString inputs);
    void createBlockFiles();
private:
    Ui::Dialog *ui;
    QStringList mInputs;
    QStringList mOutputs;
    QString mBlockName;
    QString mClassName;
    QString mFileName;
    QString mCategory;
};

#endif // DIALOG_H
