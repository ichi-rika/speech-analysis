#include "../gui/canvas.h"
#include "contextmanager.h"
#include "rendercontext.h"
#include <QQmlContext>
#include <iostream>
#include <chrono>

using namespace Main;
using namespace std::chrono_literals;

GuiContext::GuiContext(Config *config, RenderContext *renderContext)
    : mConfig(config),
      mRenderContext(renderContext),
      mSelectedView(nullptr)
{
    QCoreApplication::setApplicationName("InFormant");
    QCoreApplication::setApplicationVersion(INFORMANT_VERSION_STR);
    QCoreApplication::setOrganizationDomain("in-formant.app");
    QCoreApplication::setOrganizationName("InFormant");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QQuickStyle::setStyle("Material");
    qmlRegisterType<Gui::CanvasItem>("IfCanvas", 1, 0, "IfCanvas");

    mApp = std::make_unique<QGuiApplication>(argc, argv);
    mQmlEngine = std::make_unique<QQmlApplicationEngine>();
    mQmlEngine->addImportPath(QCoreApplication::applicationDirPath() + "/qml");
#ifdef __APPLE__
    mQmlEngine->addImportPath(QCoreApplication::applicationDirPath() + "/../Resources/qml");
#endif
    mQmlEngine->rootContext()->setContextProperty("config", mConfig);
    mQmlEngine->load(QUrl("qrc:/main.qml"));
   
    auto window = static_cast<QQuickWindow *>(mQmlEngine->rootObjects().first());
    auto canvasItem = window->findChild<Gui::CanvasItem *>("IfCanvas");
    auto contentItem = window->contentItem();
    
    canvasItem->setRenderContext(mRenderContext);

    canvasItem->installEventFilter(this);
    window->installEventFilter(this);

    mUpdateTimer = new QTimer(this);
    mUpdateTimer->setInterval(33ms);
    mUpdateTimer->setTimerType(Qt::PreciseTimer);
    QObject::connect(mUpdateTimer, &QTimer::timeout, canvasItem, &QQuickItem::update);
    mUpdateTimer->moveToThread(qApp->thread());
}

int GuiContext::exec()
{
    QMetaObject::invokeMethod(mUpdateTimer, "start", Q_ARG(int, 0));
    return mApp->exec();
}

void GuiContext::setView(GuiView *view)
{
    mSelectedView = view;
}

bool GuiContext::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        auto keyEvent = static_cast<QKeyEvent *>(event);
        if (mSelectedView != nullptr && mSelectedView->onKeyPress(keyEvent)) {
            return true;
        }
        else {
        }
    }
    else if (event->type() == QEvent::KeyRelease) {
        auto keyEvent = static_cast<QKeyEvent *>(event);
        if (mSelectedView != nullptr && mSelectedView->onKeyRelease(keyEvent)) {
            return true;
        }
        else {
        }
    }

    return false;
}

void GuiContext::setShowSpectrogram(bool b)
{
    mConfig->setViewShowSpectrogram(b);
}

void GuiContext::setShowPitch(bool b)
{
    mConfig->setViewShowPitch(b);
}

void GuiContext::setShowFormants(bool b)
{
    mConfig->setViewShowFormants(b);
}

