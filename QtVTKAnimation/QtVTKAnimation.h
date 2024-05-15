#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtVTKAnimation.h"

#include <QTimer>
#include <QVTKOpenGLStereoWidget.h>
#include <vtkCameraOrientationWidget.h>
#include <vtkInterpolateDataSetAttributes.h>

class QtVTKAnimation : public QMainWindow
{
	Q_OBJECT

public:
	QtVTKAnimation(QWidget* parent = nullptr);
	~QtVTKAnimation();

private:
	Ui::QtVTKAnimationClass ui;

	QVTKOpenGLStereoWidget* _pVTKWidget = NULL;

	vtkNew<vtkInterpolateDataSetAttributes> interpolate;
	int _nTimeStep = 0;
	void timerEvent(QTimerEvent* event);
};
