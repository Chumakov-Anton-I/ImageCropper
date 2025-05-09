/*
 * Класс MainWindow - без комментариев ;)
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QPushButton;
class QLabel;
class QSpinBox;
class TCanvas;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void openImage();
    void saveImage();
    void scale100percent();

private:
    QPushButton *m_btnOpen;
    QPushButton *m_btnSave;
    QPushButton *m_btnNaturalSize;
    QPushButton *m_btnMirror;
    QLabel *m_lblImagePos;
    QSpinBox *m_sbImageScale;
    TCanvas *m_canvas;
    int m_scale = 100;    // per cent
    QString m_path;

    void openImagePrivate(const QString &filePath);

private slots:
    void traceImage(const QPoint &p);
    void watchScale(int scale);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // MAINWINDOW_H
