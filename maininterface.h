#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QMainWindow>
namespace Ui {
class MainInterface;
}

class MainInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainInterface(QWidget *parent = nullptr);
    ~MainInterface();
//  设置管理员
    void setAdmin(QString name);
private slots:
    void on_stubt_clicked();
    void on_recordbt_clicked();
    void on_noticebt_clicked();
    void on_edmitbt_clicked();
    void on_backbt_clicked();

private:
    Ui::MainInterface *ui;
//    设置哪个管理员进入
    QString Admin_name;
};

#endif // MAININTERFACE_H
