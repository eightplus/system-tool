/*
 * Copyright (C) 2013 ~ 2015 National University of Defense Technology(NUDT) & Kylin Ltd.
 *
 * Authors:
 *  Kobe Lee    xiangli@ubuntukylin.com/kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ANALYSISWIDGET_H
#define ANALYSISWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QDBusInterface>
#include <QDBusConnection>
#include "../commom/workerthread.h"
#include "filesystemlistwidget.h"

class TopBackWidget;
class StackStatusWidget;
class QStackedWidget;
class DiskWidget;
class MainWindow;
//class DeepTable;
class DeviceAnalysisTable;
//class ScanDbus;
class QVBoxLayout;
class FileSystemData;
class FileSystemWorker;
class QVBoxLayout;
class QHBoxLayout;

class AnalysisWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnalysisWidget(QWidget *parent = 0);
    ~AnalysisWidget();

    FileSystemListWidget* getFileSysView();
    void initUI();

    void setDefaultStatus(bool b);
    void setStackedPageIndex(int index);
    void setParentWindow(MainWindow* window) { p_mainwindow = window;}
    void initHomePath();
//    void startThread();

public slots:
    void showDetailPage();
    void do_finish();
    void gotoMainPage();
    void onWorkButtonClicked();
    void onStopButtonClicked();
    void onBackButtonClicked();
    void export_trace_ok();
    void get_deep_scan_detail_process(uint number, QString context);
    void onSendStartGenerateDeepXmlReportFile(bool result);
    void onSendStopDeepScanResult(bool result);
    void onSendDeepScanStartResult(bool result);
    void refreshFileSysList();
    void popupMenu(QPoint pos, const QString &device);

signals:
    void showMenu();
    void showMin();
    void closeWidget();
    void goToMain();
    void emit_data_receive(uint number, QString context);

    void startGenerateDeepXmlReport(QString targetFile/*, QStringList recordList*/);
    void stopDeepScanTask();
    void startDeepScanTask(QString device);

private:
    MainWindow *p_mainwindow;
    TopBackWidget *m_topBackWidget;
    StackStatusWidget *m_stackStatusWidget;
//    DiskWidget *disk_widget;
//    DeepTable *deeptable;
    DeviceAnalysisTable *deeptable;
    QStackedWidget *bottom_stacked_widgets;
    QString cur_date;
    QString target_file;
//    QStringList record_list;
    QString homePath;
//    WorkerThread m_workerThread;


    FileSystemWorker *m_fileSystemWorker = nullptr;
    FileSystemListWidget *m_fileSysListWidget = nullptr;
    QAction *m_refreshAction = nullptr;
    QAction *m_scanAction = nullptr;
    QMenu *m_menu = nullptr;
    QVBoxLayout *m_layout = nullptr;

    QString currentDevice;
};

#endif // ANALYSISWIDGET_H
