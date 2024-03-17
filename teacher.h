#ifndef TEACHER_H
#define TEACHER_H

#include <QWidget>

namespace Ui {
class teacher;
}

class teacher : public QWidget
{
    Q_OBJECT

public:
    explicit teacher(QWidget *parent = 0);
    ~teacher();

private slots:
    void on_backbt_clicked();
    void on_findbt_clicked();
    void on_deletebt_clicked();
    void on_setbt_clicked();

    void on_refresh_clicked();

private:
    Ui::teacher *ui;
};

#endif // TEACHER_H
