
#pragma once

#include <memory>

#include <QMainWindow>


class Ui_Viewer;

class QLabel;
class QSurfaceFormat;
class QShortcut;


class Viewer : public QMainWindow
{
    Q_OBJECT

public:
    Viewer(
        QWidget * parent = nullptr
    ,   Qt::WindowFlags flags = NULL);

    virtual ~Viewer();

signals:
    void toggleSwapInterval();

public slots:
    void fpsChanged(float fps);
    void mouseChanged(const QPoint & mouse);
    void objChanged(const QVector3D & obj);

protected slots:
    void on_toggleFullScreenAction_triggered(bool checked);
    void toggleFullScreen();

    void on_toggleSwapIntervalAction_triggered(bool checked);

protected:
	const std::unique_ptr<Ui_Viewer> m_ui;

    QLabel * m_fpsLabel;
    QLabel * m_mouseLabel;
    QLabel * m_objLabel;

    QShortcut * m_fullscreenShortcut;
    QShortcut * m_swapIntervalShortcut;
};
