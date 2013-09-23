/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

#ifndef __vtkMRMLPlanarImageNode_h
#define __vtkMRMLPlanarImageNode_h

// MRML includes
#include <vtkMRML.h>
#include <vtkMRMLNode.h>

#include "vtkSlicerPlanarImageModuleLogicExport.h"

class VTK_SLICER_PLANARIMAGE_LOGIC_EXPORT vtkMRMLPlanarImageNode : public vtkMRMLNode
{
public:
  static vtkMRMLPlanarImageNode *New();
  vtkTypeMacro(vtkMRMLPlanarImageNode,vtkMRMLNode);

  static const char* PlanarImageVolumeNodeReferenceRole;
  static const char* DisplayedModelNodeReferenceRole;
  static const char* TextureVolumeNodeReferenceRole;

  /// Create instance of a GAD node. 
  virtual vtkMRMLNode* CreateNodeInstance();

  /// Get unique node XML tag name (like Volume, Model) 
  virtual const char* GetNodeTagName() {return "PlanarImage";};

protected:
  vtkMRMLPlanarImageNode();
  ~vtkMRMLPlanarImageNode();
  vtkMRMLPlanarImageNode(const vtkMRMLPlanarImageNode&);
  void operator=(const vtkMRMLPlanarImageNode&);
};

#endif