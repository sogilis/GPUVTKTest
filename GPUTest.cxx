/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestGPURayCastCompositeToMIP.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// This test covers composite to MIP to methods switching.
// This test volume renders a synthetic dataset with unsigned char values,
// first with the composite method, then with the MIP method.

#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkSphereSource.h"
#include <vtkDICOMImageReader.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cout << "Please choose a dicom directory" << std::endl;
    return -1;
  }

  std::string folder = argv[1];
  vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
   reader->SetDirectoryName(folder.c_str());
   reader->Update();

  vtkRenderWindow *renWin=vtkRenderWindow::New();
  vtkRenderer *ren1=vtkRenderer::New();
  ren1->SetBackground(0.1,0.4,0.2);

  renWin->AddRenderer(ren1);
  renWin->SetSize(301,300); // intentional odd and NPOT  width/height

  vtkRenderWindowInteractor *iren=vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);

  vtkSmartVolumeMapper *volumeMapper;
  vtkVolume *volume;
  volumeMapper = vtkSmartVolumeMapper::New();
  volumeMapper->SetBlendModeToComposite(); // composite first
  volumeMapper->SetInputConnection(reader->GetOutputPort());
  volumeMapper->Update();

  volume=vtkVolume::New();
  volume->SetMapper(volumeMapper);
  ren1->AddViewProp(volume);

  renWin->Render();

  int mode=volumeMapper->GetLastUsedRenderMode();
  std::cout << mode << std::endl;
  if (mode != vtkSmartVolumeMapper::GPURenderMode) {
    cout << "GPU extensions not supported." << endl;
  } else {
    cout << "GPU extensions is supported." << endl;
  }
}