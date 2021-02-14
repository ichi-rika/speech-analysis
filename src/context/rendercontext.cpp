#include "rendercontext.h"
#include "config.h"
#include "../context/timings.h"
#include <iostream>

using namespace Main;

RenderContext::RenderContext(Config *config, DataStore *dataStore)
    : mConfig(config),
      mDataStore(dataStore),
      mSelectedView(nullptr)
{
}

void RenderContext::render(QPainterWrapper *painter)
{
    timer_guard timer(timings::render);

    painter->setRenderHints(
            QPainter::Antialiasing
            | QPainter::TextAntialiasing
            | QPainter::SmoothPixmapTransform);

    painter->setMinFrequency(mConfig->getViewMinFrequency());
    painter->setMaxFrequency(mConfig->getViewMaxFrequency());
    painter->setMinGain(mConfig->getViewMinGain());
    painter->setMaxGain(mConfig->getViewMaxGain());

    if (mSelectedView != nullptr) {
        mSelectedView->render(painter, mConfig, mDataStore);
    }

    QRect viewport = painter->viewport();

    painter->setPen(Qt::white);
    painter->setFont(QFont(":/Montserrat.otf", 14, QFont::Medium));
    painter->drawText(viewport.translated(10, 10), Qt::TextWordWrap, QString("Render: %1 ms\nUpdate: %2 ms").arg(timings::render).arg(timings::update));
}

void RenderContext::setView(RenderView *view)
{
    mSelectedView = view;
}
