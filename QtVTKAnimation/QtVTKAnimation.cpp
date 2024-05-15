#include "QtVTKAnimation.h"

#include <vtkRenderWindow.h> 
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkVectorText.h>
#include <vtkImplicitModeller.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkAnimationScene.h>
#include <vtkAnimationCue.h>

QtVTKAnimation::QtVTKAnimation(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	_pVTKWidget = new QVTKOpenGLStereoWidget();
	this->setCentralWidget(_pVTKWidget);
	this->showMaximized();

	vtkNew<vtkRenderer> renderer;
	this->_pVTKWidget->renderWindow()->AddRenderer(renderer);
	this->_pVTKWidget->renderWindow()->Render();

	vtkNew<vtkVectorText> letterV;
	letterV->SetText("V");
	vtkNew<vtkVectorText> letterT;
	letterT->SetText("T");
	vtkNew<vtkVectorText> letterK;
	letterK->SetText("K");

	vtkNew<vtkImplicitModeller> bloddyV;
	bloddyV->SetInputConnection(letterV->GetOutputPort());
	bloddyV->SetMaximumDistance(0.2);
	bloddyV->SetSampleDimensions(50, 50, 12);
	bloddyV->SetModelBounds(-0.5, 1.5, -0.5, 1.5, -0.5, 0.5);
	vtkNew<vtkImplicitModeller> bloddyT;
	bloddyT->SetInputConnection(letterT->GetOutputPort());
	bloddyT->SetMaximumDistance(0.2);
	bloddyT->SetSampleDimensions(50, 50, 12);
	bloddyT->SetModelBounds(-0.5, 1.5, -0.5, 1.5, -0.5, 0.5);
	vtkNew<vtkImplicitModeller> bloddyK;
	bloddyK->SetInputConnection(letterK->GetOutputPort());
	bloddyK->SetMaximumDistance(0.2);
	bloddyK->SetSampleDimensions(50, 50, 12);
	bloddyK->SetModelBounds(-0.5, 1.5, -0.5, 1.5, -0.5, 0.5);

	// 插值实现动画过渡
	interpolate->AddInputConnection(bloddyV->GetOutputPort());
	interpolate->AddInputConnection(bloddyT->GetOutputPort());
	interpolate->AddInputConnection(bloddyK->GetOutputPort());
	interpolate->SetT(0.0);

	vtkNew<vtkContourFilter> bloddyIso;
	bloddyIso->AddInputConnection(interpolate->GetOutputPort());
	bloddyIso->SetValue(0, 0.1);

	vtkNew<vtkPolyDataMapper> bloddyMapper;
	bloddyMapper->AddInputConnection(bloddyIso->GetOutputPort());
	bloddyMapper->ScalarVisibilityOff();

	vtkNew<vtkActor> bloddyActor;
	bloddyActor->SetMapper(bloddyMapper);
	bloddyActor->GetProperty()->EdgeVisibilityOn();

	renderer->AddActor(bloddyActor);

	startTimer(100);
}

QtVTKAnimation::~QtVTKAnimation()
{}

void QtVTKAnimation::timerEvent(QTimerEvent * event)
{
	_nTimeStep++;
	int nLen = 100;
	if (_nTimeStep <= 100)
	{
		interpolate->SetT(_nTimeStep * 2.0 / nLen);
		interpolate->Modified(); // 更新
		this->_pVTKWidget->renderWindow()->Render(); // 渲染
	}
}
