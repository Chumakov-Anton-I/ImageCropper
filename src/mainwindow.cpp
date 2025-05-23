#include "mainwindow.h"
#include "tcanvas.h"

#include <QBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
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
    cmdLayout->setContentsMargins(5, 5, 5, 5);
    topLayout->addLayout(cmdLayout);
    m_btnOpen = new QPushButton(QIcon(":/iconOpen.png"), tr("Open Image..."));
    cmdLayout->addWidget(m_btnOpen);
    auto *form = new QFormLayout;
    cmdLayout->addLayout(form);
    m_sbImageScale = new QSpinBox;
    form->addRow(tr("Image scale:"), m_sbImageScale);
    m_btnNaturalSize = new QPushButton(QIcon(":/icon100percent.png"), "100 %");
    form->addRow(m_btnNaturalSize);
    m_btnMirror = new QPushButton(QIcon(":/iconMirror.png"), tr("Flip horizontally"));
    cmdLayout->addWidget(m_btnMirror);
    m_lblImagePos = new QLabel;
    form->addRow(tr("Image position:"), m_lblImagePos);
    // frame
    auto *frmSizesBox = new QGroupBox(tr("Frame size"));
    cmdLayout->addWidget(frmSizesBox);
    auto *sizeGrid = new QGridLayout;
    frmSizesBox->setLayout(sizeGrid);
    m_sbFrameW = new QSpinBox;
    m_sbFrameW->setRange(64, 2000);
    m_sbFrameW->setSuffix(" px");
    m_sbFrameW->setValue(950);
    m_sbFrameH = new QSpinBox;
    m_sbFrameH->setRange(64, 2000);
    m_sbFrameH->setSuffix(" px");
    m_sbFrameH->setValue(950);
    m_btnEqualWH = new QPushButton(tr("="));
    m_btnEqualWH->setCheckable(true);
    m_btnEqualWH->setChecked(true);
    m_btnEqualWH->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    sizeGrid->addWidget(m_sbFrameW, 0, 0);
    sizeGrid->addWidget(m_sbFrameH, 1, 0);
    sizeGrid->addWidget(m_btnEqualWH, 0, 1, 2, 1);

    m_btnSave = new QPushButton(QIcon(":/iconSave.png"), tr("Save cover"));
    cmdLayout->addWidget(m_btnSave);
    cmdLayout->addStretch(1);
    // canvas
    m_canvas = new TCanvas;
    topLayout->addWidget(m_canvas);
    topLayout->setStretch(1, 1);

    m_sbImageScale->setRange(10, 100);  // [10, 100]%
    m_sbImageScale->setValue(m_scale);
    m_sbImageScale->setSuffix(" %");

    // conections
    connect(m_btnOpen, &QPushButton::clicked, this, &MainWindow::openImage);
    connect(m_btnSave, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(m_btnMirror, &QPushButton::clicked, m_canvas, &TCanvas::mirror);
    connect(m_btnNaturalSize, &QPushButton::clicked, this, &MainWindow::scale100percent);
    connect(m_sbImageScale, &QSpinBox::valueChanged, m_canvas, &TCanvas::setScale);
    connect(m_sbFrameW, &QSpinBox::valueChanged, this, &MainWindow::changeFrameW);
    connect(m_sbFrameH, &QSpinBox::valueChanged, this, &MainWindow::changeFrameH);
    connect(m_btnEqualWH, &QPushButton::clicked, this, &MainWindow::setEqualWH);

    connect(m_canvas, &TCanvas::scaleChanged, this, &MainWindow::watchScale);
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

void MainWindow::changeFrameW(int w)
{
    if (m_btnEqualWH->isChecked()) {
        m_sbFrameH->blockSignals(true);
        m_sbFrameH->setValue(w);
        m_sbFrameH->blockSignals(false);
    }
    m_canvas->setFrameSize(QSize(w, m_sbFrameH->value()));
}

void MainWindow::changeFrameH(int h)
{
    if (m_btnEqualWH->isChecked()) {
        m_sbFrameW->blockSignals(true);
        m_sbFrameW->setValue(h);
        m_sbFrameW->blockSignals(false);
    }
    m_canvas->setFrameSize(QSize(m_sbFrameW->value(), h));
}

void MainWindow::setEqualWH(bool b)
{
    if (!b) return;
    int w = m_sbFrameW->value();
    m_sbFrameH->blockSignals(true);
    m_sbFrameH->setValue(w);
    m_sbFrameH->blockSignals(false);
    m_canvas->setFrameSize(QSize(w, w));
}

void MainWindow::traceImage(const QPoint &p)
{
    m_lblImagePos->setText(QString("%1, %2").arg(p.x()).arg(p.y()));
}

void MainWindow::watchScale(int scale)
{
    m_sbImageScale->blockSignals(true);
    m_sbImageScale->setValue(scale);
    m_sbImageScale->blockSignals(false);
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

void MainWindow::scale100percent()
{
    m_scale = 100;
    m_canvas->setScale(m_scale);
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
