#include "mainwindow.h"
#include "tcanvas.h"

#include <QBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent)
    : QWidget{parent}
{
    auto *topLayout = new QHBoxLayout;
    topLayout->setContentsMargins(1, 1, 1, 1);
    setLayout(topLayout);
    // control panel
    auto *cmdLayout = new QVBoxLayout;
    cmdLayout->setContentsMargins(1, 1, 1, 1);
    topLayout->addLayout(cmdLayout);
    m_btnOpen = new QPushButton(tr("Open Image..."));
    m_btnSave = new QPushButton(tr("Save cover"));
    m_btnMirror = new QPushButton(tr("Flip horizontally"));
    cmdLayout->addWidget(m_btnOpen);
    cmdLayout->addWidget(m_btnSave);
    cmdLayout->addWidget(m_btnMirror);
    m_btnScaleUp = new QPushButton("+5%");
    m_btnScaleDown = new QPushButton("-5%");
    m_btnScaleUp1 = new QPushButton("+1%");
    m_btnScaleDown1 = new QPushButton("-1%");
    m_btnNaturalSize = new QPushButton("100 %");
    auto *gridLayout = new QGridLayout;
    cmdLayout->addLayout(gridLayout);
    gridLayout->addWidget(m_btnScaleUp, 0, 0);
    gridLayout->addWidget(m_btnScaleDown, 0, 1);
    gridLayout->addWidget(m_btnScaleUp1, 1, 0);
    gridLayout->addWidget(m_btnScaleDown1, 1, 1);
    gridLayout->addWidget(m_btnNaturalSize, 2, 0, 1, 2);
    auto *form = new QFormLayout;
    cmdLayout->addLayout(form);
    m_lblImageScale = new QLabel(QString::number(m_scale));
    m_lblFrameScale = new QLabel;
    form->addRow(tr("Image scale:"), m_lblImageScale);
    form->addRow(tr("Frame scale:"), m_lblFrameScale);
    m_lblImagePos = new QLabel;
    m_lblFramePos = new QLabel;
    form->addRow(tr("Image position:"), m_lblImagePos);
    form->addRow(tr("Frame position:"), m_lblFramePos);

    cmdLayout->addStretch(1);
    // canvas
    m_canvas = new TCanvas;
    topLayout->addWidget(m_canvas);
    topLayout->setStretch(1, 1);

    // conections
    connect(m_btnOpen, &QPushButton::clicked, this, &MainWindow::openImage);
    connect(m_btnSave, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(m_btnMirror, &QPushButton::clicked, m_canvas, &TCanvas::mirror);
    connect(m_btnScaleUp, &QPushButton::clicked, this, &MainWindow::scaleUp);
    connect(m_btnScaleDown, &QPushButton::clicked, this, &MainWindow::scaleDown);
    connect(m_btnScaleUp1, &QPushButton::clicked, this, &MainWindow::scaleUp1);
    connect(m_btnScaleDown1, &QPushButton::clicked, this, &MainWindow::scaleDown1);
    connect(m_btnNaturalSize, &QPushButton::clicked, this, &MainWindow::scale100percent);

    connect(m_canvas, &TCanvas::frameMoved, this, &MainWindow::traceFrame);
    connect(m_canvas, &TCanvas::imageMoved, this, &MainWindow::traceImage);

    setAcceptDrops(true);
}

void MainWindow::openImagePrivate(const QString &filePath)
{
    if (filePath.isEmpty()) return;
    QFileInfo path(filePath);
    m_path = path.absolutePath();
    if (!m_canvas->loadImage(filePath))
        QMessageBox::critical(this, tr("Error"), tr("Cannot open file %1").arg(filePath));
    scale100percent();
}

void MainWindow::traceFrame(const QPoint &p)
{
    m_lblFramePos->setText(QString("%1, %2").arg(p.x()).arg(p.y()));
}

void MainWindow::traceImage(const QPoint &p)
{
    m_lblImagePos->setText(QString("%1, %2").arg(p.x()).arg(p.y()));
}

void MainWindow::openImage()
{
    QString fname = QFileDialog::getOpenFileName(this, tr("Open image file"), m_path,
                                                 tr("Images (*.jpg *.jpeg *.bmp *.png)"));
    openImagePrivate(fname);
}

void MainWindow::saveImage()
{
    QString fname = QFileDialog::getSaveFileName(this, tr("Save cover"),
                                                 m_path + "/cover.jpg", "JPEG Image (*.jpg)");
    if (fname.isEmpty())
        return;
    m_canvas->saveCover(fname);
}

// TODO: заменить это непотребство обработкой мышьего колеса
void MainWindow::scaleUp()
{
    m_scale += 5;
    if (m_scale >= 100)
        m_scale = 100;
    m_canvas->setScale(m_scale / 100.0);
    m_lblImageScale->setText(QString("%1 %").arg(m_scale));
}
void MainWindow::scaleDown()
{
    m_scale -= 5;
    if (m_scale <= 10)
        m_scale = 10;
    m_canvas->setScale(m_scale / 100.0);
    m_lblImageScale->setText(QString("%1 %").arg(m_scale));
}
void MainWindow::scaleUp1()
{
    m_scale += 1;
    if (m_scale >= 100)
        m_scale = 100;
    m_canvas->setScale(m_scale / 100.0);
    m_lblImageScale->setText(QString("%1 %").arg(m_scale));
}
void MainWindow::scaleDown1()
{
    m_scale -= 1;
    if (m_scale <= 10)
        m_scale = 10;
    m_canvas->setScale(m_scale / 100.0);
    m_lblImageScale->setText(QString("%1 %").arg(m_scale));
}
// END TODO

void MainWindow::scale100percent()
{
    m_scale = 100;
    m_canvas->setScale(1.0);
    m_lblImageScale->setText(QString("%1 %").arg(m_scale));
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    auto *mimeData = event->mimeData();
    if (mimeData->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    auto *mimeData = event->mimeData();
    QString fname = mimeData->urls().constFirst().toLocalFile();
    openImagePrivate(fname);
    event->acceptProposedAction();
    update();
}
