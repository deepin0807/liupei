#include "remotewindow.h"
#include <DHiDPIHelper>
#include <DToolButton>
#include <QString>
#include <DTitlebar>
#include <DLabel>
#include <QLayout>
#include <QBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <DPushButton>
#include <DSpinner>
#include <QLineEdit>
#include <QTimer>
#include <QTime>
#include <QClipboard>
#include <QApplication>
#include <QDebug>
#include <qvalidator.h>
#include <QValidator>


DWIDGET_USE_NAMESPACE

class RemoteWindowPrivate
{
public:
    RemoteWindowPrivate(RemoteWindow *parent) : q_ptr(parent) {}

    void initUI();
    quint32 getRandomData();

//private slots:
//    void updateLookfor();

private:
    QWidget *createLookforView();
    QWidget *createHelpView();
    QWidget *createmainView();
    QWidget *createCodeView();
    QWidget *createSuccView();
    //help
    QWidget *createConnView();
    QWidget *createFailView();
    QWidget *createNormalView();

    //help page


    QWidget *wrapLayout(QLayout *layout);
    //QStackedWidget *stackMainWindows;

    DToolButton *lookBt             = Q_NULLPTR;
    DToolButton *helpBt             = Q_NULLPTR;
//    DPushButton *cancellBt             = Q_NULLPTR;
//    DPushButton *cancelhBt             = Q_NULLPTR;
    QLabel *logo                    = Q_NULLPTR;
    DLabel *word                    = Q_NULLPTR;
    DLabel *numLabel                = Q_NULLPTR;
    QLineEdit *valid                 =Q_NULLPTR;
    //QStack<QWidget *> viewStack;
//    QVBoxLayout *centralLayout      = Q_NULLPTR;
    QWidget *lookforView           = Q_NULLPTR;
    QWidget *helpView           = Q_NULLPTR;
    QWidget *mainView           = Q_NULLPTR;
    QWidget *codeView           =Q_NULLPTR;
    QWidget *succView           =Q_NULLPTR;

    //helpview
    QWidget *connView           =Q_NULLPTR;
    QWidget *failView           =Q_NULLPTR;
    QWidget *normalView         =Q_NULLPTR;

    quint32 m_data;

    QTimer *m_timer                 =Q_NULLPTR;
    QTimer *m_timerh                 =Q_NULLPTR;

     quint64 state;
     bool isError;
//    QWidget *wrapWidget         = Q_NULLPTR;

    RemoteWindow *q_ptr;
private:
    Q_DECLARE_PUBLIC(RemoteWindow)
};

RemoteWindow::RemoteWindow(QWidget *parent):
    DMainWindow(parent),dd_ptr(new RemoteWindowPrivate(this))
{ 
    setProperty("_d_useDxcb", true);
    Q_D(RemoteWindow);
    stackMainWindows = new QStackedWidget(this);
    setCentralWidget(stackMainWindows);

if (titlebar()) {
    titlebar()->setFixedHeight(50);
    titlebar()->setTitle("远程帮助");
    }

    setFixedSize(360, 320);
    d->initUI();
}

RemoteWindow::~RemoteWindow()
{

}

void RemoteWindowPrivate::initUI()
{
    Q_Q(RemoteWindow);
    m_timer = new QTimer();
    m_timerh =new QTimer();
    m_timer->connect(m_timer, SIGNAL(timeout()), q, SLOT(updateLookfor()));
    m_timer->stop();
    lookforView = createLookforView();
    helpView = createHelpView();
    codeView =createCodeView();
    succView =createSuccView();

    //help
    connView =createConnView();
    failView =createFailView();
    normalView =createNormalView();

    lookBt = new DToolButton();
    helpBt = new DToolButton();
    logo = new QLabel();
    word =new DLabel();
    //valid =new QLineEdit();
   // numLabel=new DLabel();
//    cancellBt = new DPushButton();
//    cancelhBt = new DPushButton();

    state =0;
    isError =false;

    mainView = createmainView();
    //stackMainWindows = new QStackedWidget();
    q->stackMainWindows->addWidget(mainView);
    q->stackMainWindows->addWidget(lookforView);
    q->stackMainWindows->addWidget(codeView);
    q->stackMainWindows->addWidget(succView);
    //help
    q->stackMainWindows->addWidget(helpView);
    q->stackMainWindows->addWidget(connView);
    q->stackMainWindows->addWidget(failView);
    q->stackMainWindows->addWidget(normalView);


    q->stackMainWindows->setCurrentWidget(mainView);

}

void RemoteWindow::updateLookfor()
{
    Q_D(RemoteWindow);

    if (d->state==1)
    {
        if(d->m_timer->isActive())
                d->m_timer->stop();
        adjustSize();
        stackMainWindows->setCurrentWidget(d->codeView);
        /////
        QString str = QString::number(d->getRandomData());
        d->m_data = str.toInt();
        qDebug()<<"-------str"<<str;
        d->numLabel->setText(str);

    }
    else if(d->state==2)
    {
        if(d->m_timer->isActive())
                d->m_timer->stop();
        if(d->valid->text().toInt()==d->m_data){
            d->isError=true;
            qDebug()<<"dddddd"<<d->isError;
            adjustSize();
            stackMainWindows->setCurrentWidget(d->normalView);
        }
        else {
            qDebug()<<"mmmmmmm"<<d->isError;
            adjustSize();
            stackMainWindows->setCurrentWidget(d->failView);
        }
//        if(!d->isError)
//        {
//            stackMainWindows->setCurrentWidget(d->failView);
//        }
//        else
//        {
//            stackMainWindows->setCurrentWidget(d->normalView);
//        }

    }

}
QWidget *RemoteWindowPrivate::wrapLayout(QLayout *layout)
{
    auto wrapWidget = new QWidget;
    wrapWidget->setLayout(layout);
    return wrapWidget;
}

QWidget *RemoteWindowPrivate::createmainView()
{
    Q_Q(RemoteWindow);
    auto centralLayout = new QVBoxLayout;
    //centralLayout->setContentsMargins(0, 30, 0, 0);
    //logo
    logo->setObjectName("Logo");
    logo->setFixedSize(48,48);
    QPixmap *pixmap = new QPixmap(":/images/help.svg");
    pixmap->scaled(logo->size(), Qt::KeepAspectRatio);
    logo->setScaledContents(true);
    logo->setPixmap(*pixmap);
    //word
    QHBoxLayout *hh =new QHBoxLayout();
    word->setText("这是由deepin团队开发的远程协助应用,如果您在使用过程中遇到了困难，或者有人求助于你，请点击下方的我要求助或者帮助别人。");
    word->setFont(QFont( "Timers" ,10,  QFont::Thin));
    word->adjustSize();
    word->setAlignment(Qt::AlignHCenter);
    word->setWordWrap(true);
    word->setScaledContents(true);
    hh->addWidget(word);
    hh->setContentsMargins(30,0,30,0);

    //button;
    lookBt->setText("我要求助");
    lookBt->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
    QPixmap *pixmap_hand = new QPixmap(":/images/hand1.svg");
    lookBt->setIcon(*pixmap_hand);
    lookBt->setFixedSize(160,40);
    lookBt->connect(lookBt,&DToolButton::clicked, q,[=](){
        //adjustSize();
        q->stackMainWindows->setCurrentWidget(lookforView);
        q->titlebar()->setTitle("我要求助");
        q->titlebar()->setIcon(*pixmap_hand);
        state=1;
        m_timer->start(2000);

    });

    helpBt->setText("我要帮助");
    helpBt->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
    QPixmap *pixmap_heart = new QPixmap(":/images/heart.svg");
    helpBt->setIcon(*pixmap_heart);
    helpBt->setFixedSize(160,40);

    helpBt->connect(helpBt,&DToolButton::clicked, q,[=]()
    {
        q->adjustSize();
        q->stackMainWindows->setCurrentWidget(helpView);
        q->titlebar()->setTitle("我要帮助");
        q->titlebar()->setIcon(*pixmap_heart);

    });
    centralLayout->addWidget(logo,0,Qt::AlignHCenter);
    centralLayout->addLayout(hh);
    centralLayout->addWidget(lookBt,0,Qt::AlignHCenter);
    centralLayout->addWidget(helpBt,0,Qt::AlignHCenter);
    centralLayout->setContentsMargins(0, 30, 0, 20);
    return wrapLayout(centralLayout);
}

QWidget *RemoteWindowPrivate::createLookforView()
{
    Q_Q(RemoteWindow);
    auto centralLayout = new QVBoxLayout;
    //spin
    auto spin =new DSpinner();
    spin->start();
    spin->setFixedSize(48,48);
    centralLayout->addWidget(spin,0,Qt::AlignCenter);

    //label
    auto textLabel =new DLabel();
    textLabel->setText("正在生成验证码，请稍等");
    textLabel->adjustSize();
    textLabel->setWordWrap(true);
    centralLayout->addWidget(textLabel,0,Qt::AlignHCenter);

    auto cancellBt =new DPushButton;
    cancellBt->setText("取消");
    centralLayout->setContentsMargins(0, 30, 0, 40);
    centralLayout->addWidget(cancellBt,0,Qt::AlignHCenter);

    cancellBt->connect(cancellBt, &DPushButton::clicked, q,[=](){

            q->adjustSize();
            m_timer->stop();
            q->stackMainWindows->setCurrentWidget(mainView);
            q->titlebar()->setTitle("远程帮助");
        });
    return wrapLayout(centralLayout);
}

QWidget *RemoteWindowPrivate::createCodeView()
{
    Q_Q(RemoteWindow);
    auto centralLayout = new QVBoxLayout;
    //num

    numLabel =new DLabel();
    numLabel->adjustSize();
    numLabel->setWordWrap(true);
    numLabel->setText("");
    numLabel->setAlignment( Qt::AlignCenter|Qt::AlignBottom|Qt::AlignHCenter);
    numLabel->setFont(QFont( "Timers" , 28 ,  QFont::Thin));

    centralLayout->addWidget(numLabel,0,Qt::AlignHCenter);

    //label
    auto textLabel =new DLabel();
     auto hh= new QHBoxLayout;
    textLabel->setText("要开始共享您的页面，请将上面的验证码提供给协助你的人员，他们输入验证码后"
                       "您的共享会话会立即开始");
    textLabel->setFont(QFont( "Timers" , 8,  QFont::Thin));
    textLabel->adjustSize();
    textLabel->setWordWrap(true);
    hh->addWidget(textLabel,0,Qt::AlignHCenter);
    hh->setContentsMargins(10,0,10,0);
    centralLayout->addLayout(hh);

    auto cancellBt =new DPushButton;
    cancellBt->setText("复制");
    centralLayout->setContentsMargins(0, 30, 0, 40);
    centralLayout->addWidget(cancellBt,0,Qt::AlignHCenter);


    //chufa
    cancellBt->connect(cancellBt, &DPushButton::clicked, q,[=](){
        auto clipboard = QApplication::clipboard();
        qDebug()<<"clipboard"<<m_data;
        clipboard->setText(QString::number(m_data));
        q->adjustSize();
        q->stackMainWindows->setCurrentWidget(succView);
    });
    return wrapLayout(centralLayout);
}

QWidget *RemoteWindowPrivate::createSuccView()
{
    Q_Q(RemoteWindow);
    auto centralLayout = new QVBoxLayout;
    //num
    auto wordLabel =new DLabel();
    wordLabel->setText("成功复制到剪贴板");
    wordLabel->adjustSize();
    wordLabel->setWordWrap(true);
    wordLabel->setAlignment( Qt::AlignCenter|Qt::AlignBottom|Qt::AlignHCenter);
    wordLabel->setFont(QFont( "Timers" , 28 ,  QFont::Thin));
    centralLayout->addWidget(wordLabel,0,Qt::AlignHCenter);

    //label
    auto textLabel =new DLabel();
    textLabel->setText("正在等待链接，请稍候。。。链接成功后此界面会自动隐藏到任务栏");
    textLabel->adjustSize();
    textLabel->setWordWrap(true);
    centralLayout->addWidget(textLabel,0,Qt::AlignHCenter);

    auto cancellBt =new DPushButton;
    cancellBt->setText("返回");
    centralLayout->setContentsMargins(0, 30, 0, 40);
    centralLayout->addWidget(cancellBt,0,Qt::AlignHCenter);
    //chufa
    cancellBt->connect(cancellBt, &DPushButton::clicked, q,[=](){
        q->adjustSize();
        q->stackMainWindows->setCurrentWidget(mainView);
        q->titlebar()->setTitle("远程帮助");
    });
    return wrapLayout(centralLayout);

}

QWidget *RemoteWindowPrivate::createConnView()
{

    Q_Q(RemoteWindow);
    auto centralLayout = new QVBoxLayout;
    auto spin_h =new DSpinner();
    spin_h->start();
    spin_h->setFixedSize(48,48);
    centralLayout->addWidget(spin_h,0,Qt::AlignHCenter);

    auto note =new QLabel("正在建立连接，请稍候。");
    //note->setFixedSize(300,40);
    note->adjustSize();
    note->setWordWrap(true);
    centralLayout->addWidget(note,0,Qt::AlignCenter);

    auto cancelhBt = new DPushButton;
    cancelhBt->setText("取消");
    centralLayout->addWidget(cancelhBt,0, Qt::AlignHCenter);
    centralLayout->setContentsMargins(30, 30, 30,40);
    cancelhBt->connect(cancelhBt, &DPushButton::clicked, q,[=](){
        if(m_timer->isActive())
                m_timer->stop();
            qDebug()<<"ttttt";
            valid->setText("");
            q->adjustSize();
            q->stackMainWindows->setCurrentWidget(mainView);
            q->titlebar()->setTitle("远程帮助");
        });

    return wrapLayout(centralLayout);

}

QWidget *RemoteWindowPrivate::createFailView()
{
    Q_Q(RemoteWindow);
    auto centralLayout = new QVBoxLayout;
    auto result =new DLabel();
    result->setText("连接失败");
    result->setFont(QFont( "Timers" , 14,QFont::Bold));
    centralLayout->addWidget(result,0,Qt::AlignHCenter);

    auto note =new QLabel("网络状态不稳定，点击重试按钮再次尝试链接");
    result->setFont(QFont( "Timers" , 12,  QFont::Thin));
    note->adjustSize();
    note->setWordWrap(true);
    centralLayout->addWidget(note,0,Qt::AlignHCenter);

    auto retryBt = new DPushButton;
    retryBt->setText("重试");
    centralLayout->addWidget(retryBt,0, Qt::AlignHCenter);
    centralLayout->setContentsMargins(0, 30, 0, 40);

    retryBt->connect(retryBt, &DPushButton::clicked, q,[=](){
    m_timer->stop();
    q->adjustSize();
    valid->setText("");
    q->stackMainWindows->setCurrentWidget(helpView);
    });

    return wrapLayout(centralLayout);
}

QWidget *RemoteWindowPrivate::createNormalView()
{

    Q_Q(RemoteWindow);
    auto centralLayout = new QVBoxLayout;
    auto result =new DLabel();
    result->setText("您正在远程协助");
    centralLayout->addWidget(result,0,Qt::AlignCenter);

    auto note =new QLabel("当前已经有一个连接会话，如果您想建立新的链接，请先断开");
    note->setFixedSize(300,40);
    note->adjustSize();
    note->setWordWrap(true);
    centralLayout->addWidget(note,0,Qt::AlignHCenter);
    auto cancelhBt = new DPushButton;

    cancelhBt->setText("断开");
    cancelhBt->connect(cancelhBt, &DPushButton::clicked, q,[=](){
            m_timer->stop();
            q->adjustSize();
            valid->setText("");
            q->stackMainWindows->setCurrentWidget(mainView);
            q->titlebar()->setTitle("远程帮助");
        });
    centralLayout->addWidget(cancelhBt,0, Qt::AlignHCenter);
    centralLayout->setContentsMargins(0, 30, 0, 40);
    return wrapLayout(centralLayout);

}

QWidget *RemoteWindowPrivate::createHelpView()
{
    Q_Q(RemoteWindow);
    auto centralLayout = new QVBoxLayout;
    //edit
    class QValidator *validator=new QIntValidator(0,999999);
    valid = new QLineEdit();
    valid->setValidator(validator);
    valid->setFixedSize(330,80);
    valid->setAlignment( Qt::AlignCenter|Qt::AlignBottom|Qt::AlignHCenter);
    valid->setFont(QFont( "Timers" , 28 ,  QFont::Thin));
    centralLayout->addWidget(valid,0, Qt::AlignHCenter);

    //label
    auto note =new QLabel("请在上方输入你想帮助的人提供给你的验证码，完成后点击连接开始帮助对方");
    note->setFixedSize(300,40);
    note->adjustSize();
    note->setWordWrap(true);
    centralLayout->addWidget(note,0, Qt::AlignHCenter);

    //button
    auto cancelhBt = new DPushButton;
    cancelhBt->setText("取消");
    centralLayout->addWidget(cancelhBt,0, Qt::AlignHCenter);
    centralLayout->setContentsMargins(0, 30, 0, 40);

//    if(valid->text().toInt()==m_data){
//        isError=true;
//        qDebug()<<"dddddd"<<isError;
//    }
//    else {
//        qDebug()<<"mmmmmmm"<<isError;
//    }
    cancelhBt->connect(cancelhBt,&DPushButton::clicked,q,[=](){
        if(m_timer->isActive())
            m_timer->stop();
        if((cancelhBt->text().compare(QString::fromLocal8Bit("取消"))) == 0){
           valid->setText("");
           qDebug()<<"==="<<cancelhBt->text();
            q->stackMainWindows->setCurrentWidget(mainView);
            q->titlebar()->setTitle("远程帮助");
        }
        if((cancelhBt->text().compare(QString::fromLocal8Bit("连接"))) == 0)
        {
            //valid->setText("");
            state=2;
            m_timer->start(5000);
            cancelhBt->setText("取消");
            q->stackMainWindows->setCurrentWidget(connView);
        }


    });


    cancelhBt->connect(valid,&QLineEdit::textChanged,q,[=](const QString &str){
        if(str.count() == 6)
        {
            cancelhBt->setText("连接");
        }
    });

//    cancelhBt->connect(cancelhBt,&DPushButton::clicked,q,[=](){
//         q->stackMainWindows->setCurrentWidget(connView);
//         state=2;
//         m_timer->start(5000);
//    });

    return wrapLayout(centralLayout);
}

quint32 RemoteWindowPrivate::getRandomData()
{
//     m_data=qrand()%(100000);
//     if(m_data<100000){
//         m_data+=100000;
//     }
     qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    // QString str = QString::number((qrand() % (999999 - 100000) + 100000));
     //qDebug()<<"*****"<<str;
     return (qrand() % (999999 - 100000) + 100000);
     //return m_data;
}

