#ifndef CXATESTPARAMSET_H
#define CXATESTPARAMSET_H

#include <QWidget>

namespace Ui {
class CXATestParamSet;
}

class CXATestParamSet : public QWidget
{
    Q_OBJECT

public:
    explicit CXATestParamSet(QWidget *parent = nullptr);
    ~CXATestParamSet();

private:
    Ui::CXATestParamSet *ui;
};

#endif // CXATESTPARAMSET_H
