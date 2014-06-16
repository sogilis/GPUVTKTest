#include "vtkSmartVolumeMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

void createImageData(vtkImageData* imageData, int width, int height, double scalarToFill)
{
    // Specify the size of the image data
    imageData->SetDimensions(width, height, 1);
    imageData->SetNumberOfScalarComponents(1);
    imageData->SetScalarTypeToDouble();
    imageData->AllocateScalars();

    int *dims = imageData->GetDimensions();
    for (int z = 0; z < dims[2]; z++) {
        for (int y = 0; y < dims[1]; y++) {
            for (int x = 0; x < dims[0]; x++) {
                double* pixel = static_cast<double*>(imageData->GetScalarPointer(x,y,z));
                pixel[0] = scalarToFill;
            }
        }
    }
    imageData->Modified();
}

int main(int argc, char *argv[])
{
  vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
  createImageData(imageData, 100, 100, 1000);

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
  volumeMapper->SetInput(imageData);
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