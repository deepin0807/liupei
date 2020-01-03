#ifndef REMOTEWINDOW_H
#define REMOTEWINDOW_H
#include <DMainWindow>
#include <QScopedPointer>
DWIDGET_USE_NAMESPACE
#include <QWidget>
#include <QStackedWidget>
#include <DPushButton>
#include <QVBoxLayout>

class RemoteWindowPrivate;
class RemoteWindow:public DMainWindow
{
    Q_OBJECT
public:
    //remoteWindow();
    explicit RemoteWindow(QWidget *parent = Q_NULLPTR);
    ~RemoteWindow() Q_DECL_OVERRIDE;
private slots:
    void updateLookfor();
private:
    QStackedWidget *stackMainWindows;
    QVBoxLayout *centralLayout;

    QScopedPointer<RemoteWindowPrivate> dd_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(dd_ptr), RemoteWindow)
};

#endif // REMOTEWINDOW_H
