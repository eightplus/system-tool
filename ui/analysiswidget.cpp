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

#include "analysiswidget.h"
#include "analysisbottomwidget.h"
#include "../commom/topbackwidget.h"
#include "../commom/stackstatuswidget.h"
//#include "diskwidget.h"
//#include "deeptable.h"
//#include "../commom/workthread.h"
#include "../main/finishdialog.h"
#include "../main/mainwindow.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "filesystemdata.h"
#include "filesystemworker.h"
#include "deviceanalysistable.h"
#include "../check/datalibrary.h"

#include <QDateTime>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QApplication>
#include <QEvent>
#include <QDebug>
#include <QFileInfo>

AnalysisWidget::AnalysisWidget(QWidget *parent) :
    QWidget(parent)
{
//    record_list.clear();
    this->initHomePath();
    m_stackStatusWidget = NULL;
    m_topBackWidget = NULL;
//    disk_widget = NULL;
    deeptable = NULL;
    bottom_stacked_widgets = NULL;
//    connect(&m_workerThread, SIGNAL(finished()), this, SLOT(export_trace_ok()));

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    m_fileSysListWidget = new FileSystemListWidget();
    connect(m_fileSysListWidget, &FileSystemListWidget::rightMouseClickedItem, this, &AnalysisWidget::popupMenu, Qt::QueuedConnection);
    connect(m_fileSysListWidget, SIGNAL(startDeepScanTask(QString)), this, SIGNAL(startDeepScanTask(QString)));
    m_layout->addWidget(m_fileSysListWidget);

    m_fileSystemWorker = new FileSystemWorker;
    m_fileSystemWorker->moveToThread(qApp->thread());

    m_menu = new QMenu();
    m_refreshAction = new QAction(tr("刷新"), this);
    connect(m_refreshAction, &QAction::triggered, this, &AnalysisWidget::refreshFileSysList);
    m_menu->addAction(m_refreshAction);
    m_scanAction = new QAction(tr("扫描"), this);
    connect(m_scanAction, &QAction::triggered, this, [=] {
        emit this->startDeepScanTask(this->currentDevice);
    });
    m_menu->addAction(m_scanAction);

    this->refreshFileSysList();
}

AnalysisWidget::~AnalysisWidget()
{
//    m_workerThread.quit();
//    m_workerThread.deleteLater();
    m_fileSystemWorker->deleteLater();
    delete m_fileSysListWidget;
    delete m_refreshAction;
    delete m_scanAction;
    delete m_menu;

    if(m_stackStatusWidget != NULL) {
        delete m_stackStatusWidget;
        m_stackStatusWidget = NULL;
    }
    if(m_topBackWidget != NULL) {
        delete m_topBackWidget;
        m_topBackWidget = NULL;
    }
//    if(disk_widget != NULL) {
//        delete disk_widget;
//        disk_widget = NULL;
//    }
    if(deeptable != NULL) {
        delete deeptable;
        deeptable = NULL;
    }
    if(bottom_stacked_widgets != NULL) {
        delete bottom_stacked_widgets;
        bottom_stacked_widgets = NULL;
    }

    delete m_layout;
}

void AnalysisWidget::initUI()
{
    m_topBackWidget = new TopBackWidget;
    m_stackStatusWidget = new StackStatusWidget(2);
    m_stackStatusWidget->setFixedSize(900, 111);
    m_topBackWidget->insertWidget(m_stackStatusWidget);
    m_stackStatusWidget->setTextInfo("深度痕迹检测", "双击下面表格中的某行，可开始进行对应设备的深度扫描");

    m_stackStatusWidget->show_gif();
    m_stackStatusWidget->setNormalButton(/*"://btn_start_scan.png", */true);
    m_stackStatusWidget->hideButtons();
    m_topBackWidget->show_or_hide_back_button(true);
    connect(m_stackStatusWidget, SIGNAL(workClicked()), this, SLOT(onWorkButtonClicked()));
    connect(m_stackStatusWidget, SIGNAL(stopClicked()), this, SLOT(onStopButtonClicked()));
    connect(m_stackStatusWidget, SIGNAL(backClicked()), this, SLOT(onBackButtonClicked()));

    m_topBackWidget->setFixedHeight(150);
    connect(m_topBackWidget, SIGNAL(showMenu()), this, SIGNAL(showMenu()));
    connect(m_topBackWidget, SIGNAL(showMin()), this, SIGNAL(showMin()));
    connect(m_topBackWidget, SIGNAL(topBackButtonClicked()), this, SLOT(gotoMainPage()));
//    connect(m_topBackWidget, SIGNAL(closeWidget()), this, SIGNAL(closeWidget()));
    connect(m_topBackWidget, &TopBackWidget::closeWidget, this, [=] {
        DataLibrary::Instance()->clearMap();
        deeptable->clearUI();
        emit this->closeWidget();
    });
    bottom_stacked_widgets = new QStackedWidget(this);
//    deeptable = new DeepTable(this);
    deeptable = new DeviceAnalysisTable(this);

    /*disk_widget = new DiskWidget(this);
//    connect(disk_widget, SIGNAL(emitChangePage()), this, SLOT(showDetailPage()));
    connect(disk_widget, SIGNAL(startDeepScanTask(QString)), this, SIGNAL(startDeepScanTask(QString)));
    */

    connect(deeptable, SIGNAL(emit_process(int)), m_stackStatusWidget, SLOT(set_process_label(int)));
    connect(deeptable, SIGNAL(emit_process_finish()), this, SLOT(do_finish()));
    connect(this, SIGNAL(emit_data_receive(uint, QString)), deeptable, SLOT(get_deep_scan_detail_process(uint,QString)));

    bottom_stacked_widgets->addWidget(m_fileSysListWidget);
//    bottom_stacked_widgets->addWidget(disk_widget);
    bottom_stacked_widgets->addWidget(deeptable);

    m_layout->addWidget(m_topBackWidget);
    m_layout->addWidget(bottom_stacked_widgets);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0,0,0,0);

    this->setLayout(m_layout);
}

void AnalysisWidget::refreshFileSysList()
{
    m_fileSystemWorker->onFileSystemListChanged();

    QList<FileSystemListItem*> items;
    for (FileSystemData *info : m_fileSystemWorker->diskInfoList()) {
        FileSystemListItem *item = new FileSystemListItem(info);
        items << item;
    }
    m_fileSysListWidget->refreshFileSystemItems(items);
}

void AnalysisWidget::popupMenu(QPoint pos, const QString &device)
{
    this->currentDevice = device;
    m_menu->exec(pos);
}

FileSystemListWidget* AnalysisWidget::getFileSysView()
{
    return m_fileSysListWidget;
}

void AnalysisWidget::do_finish()
{
    m_stackStatusWidget->set_process_finish_flag();
    m_stackStatusWidget->stop_gif();
    m_stackStatusWidget->showWorkButton();
}

void AnalysisWidget::get_deep_scan_detail_process(uint number, QString context)
{
    emit this->emit_data_receive(number, context);
//    if (context.contains("+") && context.split("+").length() == 7) {
//        record_list.append(context);
//    }
}

void AnalysisWidget::gotoMainPage()
{
    DataLibrary::Instance()->clearMap();
    deeptable->clearUI();
    emit this->goToMain();
}

void AnalysisWidget::initHomePath()
{
    uid_t userid;
    struct passwd* pwd;
    userid=getuid();
    pwd=getpwuid(userid);
    this->homePath = QString(QLatin1String(pwd->pw_dir));
}

void AnalysisWidget::onStopButtonClicked()
{
    emit this->stopDeepScanTask();
}

void AnalysisWidget::onSendStopDeepScanResult(bool result)
{
    if (result) {
        m_stackStatusWidget->showBackButton();
        m_stackStatusWidget->stop_gif();
        m_stackStatusWidget->hide_process_label();
        m_stackStatusWidget->setTextInfo("扫描已经被停止", "请点击返回按钮返回深度扫描首页");
    }
}

void AnalysisWidget::onBackButtonClicked()
{
    DataLibrary::Instance()->clearMap();
    deeptable->clearUI();
    this->setDefaultStatus(false);
    m_topBackWidget->show_or_hide_back_button(true);
//    deeptable->initUI();
//    bottom_stacked_widgets->setCurrentWidget(disk_widget);
}

// 开启线程
//void AnalysisWidget::startThread()
//{
//    qDebug() << "AnalysisWidget Thread : " << QThread::currentThreadId();
    /*if (!m_workerThread.isRunning()) {
        m_workerThread.setTargetPath(this->target_file);
        m_workerThread.setWorkFlag("DEEP");
        m_workerThread.setDeepData(this->record_list);
        m_workerThread.start();
    }*/
//}

void AnalysisWidget::onWorkButtonClicked()
{
    m_stackStatusWidget->setTextInfo("正在导出......", "可以在当前用户主目录下查看导出的以导出时间命名的HTML或XML文件");
    m_stackStatusWidget->hideButtons();
    QDateTime current_date_time = QDateTime::currentDateTime();
//    cur_date = current_date_time.toString("yyyy-MM-dd-hh:mm:ss-ddd");
    this->cur_date = current_date_time.toString("yyyy-MM-dd-hh:mm:ss");
    target_file = this->homePath + "/" + this->cur_date + ".xml";
    emit this->startGenerateDeepXmlReport(this->target_file/*, this->record_list*/);


    //将记录导入到PDF文件中
    /*WorkThread *work_thread = new WorkThread(this);
    work_thread->setTargetPath(this->target_file);
    work_thread->setWorkFlag("DEEP");
    work_thread->setDeepData(this->record_list);
    connect(work_thread, SIGNAL(finished()), this, SLOT(export_trace_ok()));
    connect(work_thread, SIGNAL(finished()), work_thread, SLOT(deleteLater()));
    work_thread->start();*/
}

void AnalysisWidget::onSendStartGenerateDeepXmlReportFile(bool result)
{
    DataLibrary::Instance()->clearMap();
    deeptable->clearUI();
    if (result == false) {
        m_stackStatusWidget->setTextInfo("导出失败", "本次导出操作发生未知错误");
        m_stackStatusWidget->showBackButton();
//        record_list.clear();
    }
    else {
        this->export_trace_ok();
    }
}

void AnalysisWidget::export_trace_ok()
{
    m_stackStatusWidget->setTextInfo("导出完成", "可以在当前用户主目录下查看导出的以导出时间命名的HTML或XML文件");
    m_stackStatusWidget->showBackButton();
//    record_list.clear();

    FinishDialog finishDialog;
    finishDialog.setFinishTextInfo(this->cur_date, this->target_file, "", "");
    int w_x = p_mainwindow->frameGeometry().topLeft().x() + (900 / 2) - (441  / 2);
    int w_y = p_mainwindow->frameGeometry().topLeft().y() + (600 /2) - (333 / 2);
    finishDialog.move(w_x, w_y);
    if(finishDialog.exec() == QDialog::Accepted) {

    }
}

void AnalysisWidget::setDefaultStatus(bool b)
{
    if (b)
        m_stackStatusWidget->setTextInfo("深度痕迹检测", "双击下面表格中的某行，可开始进行对应设备的深度扫描");
    else
        m_stackStatusWidget->setTextInfo("深度痕迹检测", "可以详细检测到文件信息");
//    bottom_stacked_widgets->setCurrentWidget(disk_widget);
    m_stackStatusWidget->hideButtons();
    m_topBackWidget->show_or_hide_back_button(true);
}

void AnalysisWidget::setStackedPageIndex(int index)
{
    bottom_stacked_widgets->setCurrentIndex(index);
}

void AnalysisWidget::showDetailPage()
{
//    record_list.clear();
//    deeptable->initUI();
    m_stackStatusWidget->start_gif();
    m_stackStatusWidget->show_process_label();
    m_topBackWidget->show_or_hide_back_button(false);
    bottom_stacked_widgets->setCurrentWidget(deeptable);
    m_stackStatusWidget->showStopButton();
}

void AnalysisWidget::onSendDeepScanStartResult(bool result)
{
    if (result)
        this->showDetailPage();
}
