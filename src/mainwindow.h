/*
 * Класс MainWindow - без комментариев ;)
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QPushButton;
class QLabel;
class TCanvas;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void openImage();
    void saveImage();
    void scaleUp();
    void scaleDown();
    void scaleUp1();
    void scaleDown1();
    void scale100percent();

private:
    QPushButton *m_btnOpen;
    QPushButton *m_btnSave;
    QPushButton *m_btnScaleUp;
    QPushButton *m_btnScaleDown;
    QPushButton *m_btnScaleUp1;
    QPushButton *m_btnScaleDown1;
    QPushButton *m_btnNaturalSize;
    QPushButton *m_btnMirror;
    QLabel *m_lblImageScale;
    QLabel *m_lblFrameScale;
    QLabel *m_lblImagePos;
    QLabel *m_lblFramePos;
    TCanvas *m_canvas;
    int m_scale = 100;    // per cent
    QString m_path;

    void openImagePrivate(const QString &filePath);

private slots:
    void traceFrame(const QPoint &p);
    void traceImage(const QPoint &p);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // MAINWINDOW_H
