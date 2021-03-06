#pragma once

#include <QtCore>

QT_FORWARD_DECLARE_CLASS(QFileInfo)

namespace Utils
{

enum VideoRendererId
{
    OpenGLWidget,
    GLWidget2,
    Widget,
    GDI,
    Direct2D
};

QStringList externalFilesToLoad(const QFileInfo &originalMediaFile, const QString &fileType);
void moveToCenter(QObject *window);
bool run(const QString &path, const QStringList &params = QStringList{}, bool needAdmin = false, bool needHide = false);
bool isVideo(const QString &fileName);
bool isAudio(const QString &fileName);
bool isPicture(const QString &fileName);
int getVideoRendererId(const VideoRendererId vid);
void activateWindow(QObject *window, bool moveCenter = true);
bool enableBlurBehindWindow(QObject *window);

}
