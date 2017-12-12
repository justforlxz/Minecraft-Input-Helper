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

// Send a fake keystroke event to an X window.
// by Adam Pierce - http://www.doctort.org/adam/
// This is public domain software. It is free to use by anyone for any purpose.

#include <X11/Xlib.h>
#include <X11/keysym.h>

// The key code to be sent.
// A full list of available codes can be found in /usr/include/X11/keysymdef.h

DWIDGET_USE_NAMESPACE

// Function to create a keyboard event
XKeyEvent createKeyEvent(Display *display, Window &win,
                           Window &winRoot, bool press,
                           int keycode, int modifiers)
{
   XKeyEvent event;

   event.display     = display;
   event.window      = win;
   event.root        = winRoot;
   event.subwindow   = None;
   event.time        = CurrentTime;
   event.x           = 1;
   event.y           = 1;
   event.x_root      = 1;
   event.y_root      = 1;
   event.same_screen = True;
   event.keycode     = XKeysymToKeycode(display, keycode);
   event.state       = modifiers;

   if(press)
      event.type = KeyPress;
   else
      event.type = KeyRelease;

   return event;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_mcWindow(nullptr)
    , m_mcIsActive(false)
{
    setWindowFlags(Qt::BypassWindowManagerHint);

    m_socket = new QUdpSocket(this);
    m_monitor = new EventMonitor(this);
    m_monitor->start();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    setLayout(layout);

    QLineEdit *edit = new QLineEdit;
    layout->addWidget(edit);

    connect(edit, &QLineEdit::returnPressed, this, [=] {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(edit->text(), QClipboard::Clipboard);
        edit->clear();
        hide();
        QTimer::singleShot(100, this, [=] {
            QProcess::startDetached("xdotool key --delay 100 --clearmodifiers ctrl+v && xdotool key --delay 100 --clearmodifiers \"Return\"");
        });
    });

    m_wmHelper = DWindowManagerHelper::instance();
    connect(m_wmHelper, &DWindowManagerHelper::windowListChanged, this, &Widget::onWindowListChanged);
    onWindowListChanged();

    connect(m_monitor, &EventMonitor::keyPress, this, [=] (int code) {
        if (code == 28 && m_mcIsActive && m_mcWindow) {
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
    QList<DForeignWindow*> list = m_wmHelper->currentWorkspaceWindows();
    for (DForeignWindow *window : list) {
        if (window->wmClass() == "Minecraft 1.12") {
            Window w;
            int revert_to;
            XGetInputFocus(QX11Info::display(), &w, &revert_to);
            if (w == None) {
                return;
            }
            m_mcIsActive = true;
            m_mcWindow = window;
            return;
        }
    }
    m_mcIsActive = false;
    m_mcWindow = nullptr;
}
