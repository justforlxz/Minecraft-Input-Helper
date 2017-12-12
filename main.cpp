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

#include <DApplication>
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.setTheme("light");

//    QCommandLineParser parser;
//    parser.addHelpOption();

//    QCommandLineOption p(QStringList() << "m" << "mod", "If the mod is used, the message will be sent using the network", "");
//    parser.addOption(p);

//    parser.process(a);

//    QString strValue = parser.value("m");

//    // 从命令行启动，发送虚假的T，打开输入界面
//    Display *display = QX11Info::display();

//    XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_T), True, CurrentTime);
//    XFlush(display);

//    XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_T), False, CurrentTime);
//    XFlush(display);

    Widget w;

    return a.exec();
}
