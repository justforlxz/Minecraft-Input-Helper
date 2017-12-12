/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "widget.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <DForeignWindow>
#include <QLabel>
#include <QApplication>
#include <QClipboard>
#include <QX11Info>
#include <QKeyEvent>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#define MC_VERSION "Minecraft 1.12"

DWIDGET_USE_NAMESPACE

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_mcWindow(nullptr)
    , m_isActive(false)
{
    setWindowFlags(Qt::BypassWindowManagerHint);

    m_socket = new QUdpSocket(this);
    m_monitor = new EventMonitor(this);
    m_monitor->start();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    setLayout(layout);

    m_lineEdit = new QLineEdit;
    layout->addWidget(m_lineEdit);

    connect(m_lineEdit, &QLineEdit::returnPressed, this, &Widget::onInputFinished);

    m_wmHelper = DWindowManagerHelper::instance();
    connect(m_wmHelper, &DWindowManagerHelper::windowListChanged, this, &Widget::onWindowListChanged);
    onWindowListChanged();

    connect(m_monitor, &EventMonitor::keyPress, this, [=] (int code) {
        if (!m_isActive && !isVisible())
            return;

        if (code == 9 && isVisible())
            hide();

        if (code == 28 && m_mcWindow && m_isActive && !isVisible()) {
            move(m_mcWindow->geometry().left(), m_mcWindow->geometry().y() + m_mcWindow->geometry().height());
            resize(m_mcWindow->geometry().width(), 20);
            show();
            activateWindow();
        }
    });
}

Widget::~Widget()
{

}

void Widget::onWindowListChanged()
{
    onGetCurrentFocusWindow();
    QList<DForeignWindow*> list = m_wmHelper->currentWorkspaceWindows();
    for (DForeignWindow *window : list) {
        if (window->wmClass() == MC_VERSION) {
            m_mcWindow = window;
            return;
        }
    }

    if (m_mcWindow)
        m_mcWindow->deleteLater();
    m_mcWindow = nullptr;
}

void Widget::onInputFinished()
{
    const QString &value = m_lineEdit->text();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(value, QClipboard::Clipboard);

    m_lineEdit->clear();
    hide();

    Display *display = QX11Info::display();

    XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Control_L), True, CurrentTime);
    XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_V), True, CurrentTime);
    XFlush(display);

    QTimer::singleShot(200, this, [=] {
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_V), False, CurrentTime);
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Control_L), False, CurrentTime);
        XFlush(display);

        XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Return), True, CurrentTime + value.size() * 5);
        XFlush(display);

        XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Return), False, CurrentTime + value.size() * 5);
        XFlush(display);
    });
}

void Widget::onGetCurrentFocusWindow()
{
    QProcess *process = new QProcess(this);
    connect(process, &QProcess::readyRead, this, [=] {
        m_isActive = process->readAll().replace("\n","") == MC_VERSION;
        process->deleteLater();
    });

    process->start("xdotool getwindowfocus getwindowname");
}
