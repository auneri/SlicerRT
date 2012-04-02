// .NAME vtkPolyDataToLabelmapFilter - Converts PolyData model to Labelmap image data
// .SECTION Description
// !!! Copied from ModelToLabelMap CLI module !!! TODO: Make the DVH plugin use that module and depend on it instead of having this class (or both use this class)


#ifndef __vtkPolyDataToLabelmapFilter_h
#define __vtkPolyDataToLabelmapFilter_h

// VTK includes
#include <vtkPolyData.h>
#include <vtkImageData.h>

// STD includes
#include <cstdlib>

#include "vtkSlicerDoseVolumeHistogramModuleLogicExport.h"

/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_DOSEVOLUMEHISTOGRAM_MODULE_LOGIC_EXPORT vtkPolyDataToLabelmapFilter :
  public vtkObject
{
public:

  static vtkPolyDataToLabelmapFilter *New();
  vtkTypeMacro(vtkPolyDataToLabelmapFilter, vtkObject );
  void PrintSelf(ostream& os, vtkIndent indent);

  virtual void SetReferenceImage(vtkImageData* reference);
  virtual vtkImageData* GetOutput();

  virtual void Update();

  vtkSetObjectMacro(InputPolyData, vtkPolyData);
  vtkSetMacro(LabelValue, unsigned short);

  vtkGetMacro(UseReferenceValues, bool);
  vtkSetMacro(UseReferenceValues, bool);
  vtkBooleanMacro(UseReferenceValues, bool);

protected:
  vtkSetObjectMacro(OutputLabelmap, vtkImageData);
  vtkSetObjectMacro(ReferenceImageData, vtkImageData);

protected:
  vtkPolyData* InputPolyData;
  vtkImageData* OutputLabelmap;
  vtkImageData* ReferenceImageData;
  unsigned short LabelValue;
  bool UseReferenceValues;

protected:
  vtkPolyDataToLabelmapFilter();
  virtual ~vtkPolyDataToLabelmapFilter();

private:

  vtkPolyDataToLabelmapFilter(const vtkPolyDataToLabelmapFilter&); // Not implemented
  void operator=(const vtkPolyDataToLabelmapFilter&);               // Not implemented
};

#endif