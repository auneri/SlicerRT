/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Kevin Wang, RMP, PMH
  
==============================================================================*/

// Qt includes
#include <QCheckBox>

// SlicerQt includes
#include "qSlicerIsodoseModuleWidget.h"
#include "ui_qSlicerIsodoseModule.h"

// Isodose includes
#include "vtkSlicerIsodoseModuleLogic.h"
#include "vtkMRMLIsodoseNode.h"

// MRML includes
#include <vtkMRMLVolumeNode.h>

// STD includes
#include <sstream>
#include <string>

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_Isodose
class qSlicerIsodoseModuleWidgetPrivate: public Ui_qSlicerIsodoseModule
{
  Q_DECLARE_PUBLIC(qSlicerIsodoseModuleWidget);
protected:
  qSlicerIsodoseModuleWidget* const q_ptr;
public:
  qSlicerIsodoseModuleWidgetPrivate(qSlicerIsodoseModuleWidget &object);
  ~qSlicerIsodoseModuleWidgetPrivate();
  vtkSlicerIsodoseModuleLogic* logic() const;
public:
  /// Map that associates a string pair containing the structure set plot name (including table row number) and the vtkMRMLDoubleArrayNode id (respectively) to the show/hide in chart checkboxes
  std::map<std::string, double> IsodoseLevelMap;
};

//-----------------------------------------------------------------------------
// qSlicerIsodoseModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerIsodoseModuleWidgetPrivate::qSlicerIsodoseModuleWidgetPrivate(qSlicerIsodoseModuleWidget& object)
  : q_ptr(&object)
{
  this->IsodoseLevelMap.clear();
}

//-----------------------------------------------------------------------------
qSlicerIsodoseModuleWidgetPrivate::~qSlicerIsodoseModuleWidgetPrivate()
{
  this->IsodoseLevelMap.clear();
}

//-----------------------------------------------------------------------------
vtkSlicerIsodoseModuleLogic*
qSlicerIsodoseModuleWidgetPrivate::logic() const
{
  Q_Q(const qSlicerIsodoseModuleWidget);
  return vtkSlicerIsodoseModuleLogic::SafeDownCast(q->logic());
} 

//-----------------------------------------------------------------------------
// qSlicerIsodoseModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerIsodoseModuleWidget::qSlicerIsodoseModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerIsodoseModuleWidgetPrivate(*this) )
{
}

//-----------------------------------------------------------------------------
qSlicerIsodoseModuleWidget::~qSlicerIsodoseModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::setMRMLScene(vtkMRMLScene* scene)
{
  Q_D(qSlicerIsodoseModuleWidget);

  this->Superclass::setMRMLScene(scene);

  //qvtkReconnect( d->logic(), scene, vtkMRMLScene::EndImportEvent, this, SLOT(onSceneImportedEvent()) );

  // Find parameters node or create it if there is no one in the scene
  if (scene &&  d->logic()->GetIsodoseNode() == 0)
  {
    vtkMRMLNode* node = scene->GetNthNodeByClass(0, "vtkMRMLIsodoseNode");
    if (node)
    {
      this->setIsodoseNode( vtkMRMLIsodoseNode::SafeDownCast(node) );
    }
  }
}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::onSceneImportedEvent()
{
  this->onEnter();
}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::enter()
{
  this->onEnter();
  this->Superclass::enter();
}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::onEnter()
{
  if (!this->mrmlScene())
  {
    return;
  }

  Q_D(qSlicerIsodoseModuleWidget);

  // First check the logic if it has a parameter node
  if (d->logic() == NULL)
  {
    return;
  }
  vtkMRMLIsodoseNode* paramNode = d->logic()->GetIsodoseNode();

  // If we have a parameter node select it
  if (paramNode == NULL)
  {
    vtkMRMLNode* node = this->mrmlScene()->GetNthNodeByClass(0, "vtkMRMLIsodoseNode");
    if (node)
    {
      paramNode = vtkMRMLIsodoseNode::SafeDownCast(node);
      d->logic()->SetAndObserveIsodoseNode(paramNode);
      return;
    }
    else 
    {
      vtkSmartPointer<vtkMRMLIsodoseNode> newNode = vtkSmartPointer<vtkMRMLIsodoseNode>::New();
      this->mrmlScene()->AddNode(newNode);
      d->logic()->SetAndObserveIsodoseNode(newNode);
    }
  }

  updateWidgetFromMRML();
}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::updateWidgetFromMRML()
{
  Q_D(qSlicerIsodoseModuleWidget);

  vtkMRMLIsodoseNode* paramNode = d->logic()->GetIsodoseNode();
  if (paramNode && this->mrmlScene())
  {
    d->MRMLNodeComboBox_ParameterSet->setCurrentNode(paramNode);
    if (paramNode->GetDoseVolumeNodeId() && stricmp(paramNode->GetDoseVolumeNodeId(),""))
    {
      d->MRMLNodeComboBox_DoseVolume->setCurrentNode(paramNode->GetDoseVolumeNodeId());
    }
    else
    {
      this->doseVolumeNodeChanged(d->MRMLNodeComboBox_DoseVolume->currentNode());
    }
  }

}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::onLogicModified()
{
  Q_D(qSlicerIsodoseModuleWidget);

  updateWidgetFromMRML();
}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::setup()
{
  Q_D(qSlicerIsodoseModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  d->tableWidget_DoseLevels->setColumnWidth(0, 24);
  d->tableWidget_DoseLevels->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents); 
  d->tableWidget_DoseLevels->setSelectionMode(QAbstractItemView::SingleSelection);

  // Make connections
  connect( d->MRMLNodeComboBox_ParameterSet, SIGNAL(currentNodeChanged(vtkMRMLNode*)), this, SLOT( setIsodoseNode(vtkMRMLNode*) ) );
  connect( d->MRMLNodeComboBox_DoseVolume, SIGNAL( currentNodeChanged(vtkMRMLNode*) ), this, SLOT( doseVolumeNodeChanged(vtkMRMLNode*) ) );

  connect( d->tableWidget_DoseLevels, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onTableItemChanged(QTableWidgetItem*)) );
  connect( d->pushButton_Add, SIGNAL( clicked() ), this, SLOT( addClicked() ) );
  connect( d->pushButton_Remove, SIGNAL( clicked() ), this, SLOT( removeClicked() ) );
  connect( d->pushButton_Apply, SIGNAL(clicked()), this, SLOT(applyClicked()) );

  connect( d->MRMLNodeComboBox_OutputHierarchy, SIGNAL( currentNodeChanged(vtkMRMLNode*) ), this, SLOT( outputHierarchyNodeChanged(vtkMRMLNode*) ) );

  // Handle scene change event if occurs
  qvtkConnect( d->logic(), vtkCommand::ModifiedEvent, this, SLOT( onLogicModified() ) );
}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::setIsodoseNode(vtkMRMLNode *node)
{
  Q_D(qSlicerIsodoseModuleWidget);

  vtkMRMLIsodoseNode* paramNode = vtkMRMLIsodoseNode::SafeDownCast(node);

  // Each time the node is modified, the qt widgets are updated
  qvtkReconnect( d->logic()->GetIsodoseNode(), paramNode, vtkCommand::ModifiedEvent, this, SLOT(updateWidgetFromMRML()) );

  d->logic()->SetAndObserveIsodoseNode(paramNode);
  updateWidgetFromMRML();
}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::doseVolumeNodeChanged(vtkMRMLNode* node)
{
  Q_D(qSlicerIsodoseModuleWidget);

  vtkMRMLIsodoseNode* paramNode = d->logic()->GetIsodoseNode();
  if (!paramNode || !this->mrmlScene() || !node)
  {
    return;
  }

  paramNode->DisableModifiedEventOn();
  paramNode->SetDoseVolumeNodeId(node->GetID());
  paramNode->DisableModifiedEventOff();

  //if (d->logic()->DoseVolumeContainsDose())
  //{
  //  d->label_NotDoseVolumeWarning->setText("");
  //}
  //else
  //{
  //  d->label_NotDoseVolumeWarning->setText(tr(" Selected volume is not a dose"));
  //}
}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::outputHierarchyNodeChanged(vtkMRMLNode* node)
{
  Q_D(qSlicerIsodoseModuleWidget);

  vtkMRMLIsodoseNode* paramNode = d->logic()->GetIsodoseNode();
  if (!paramNode || !this->mrmlScene() || !node)
  {
    return;
  }

  paramNode->DisableModifiedEventOn();
  paramNode->SetOutputHierarchyNodeId(node->GetID());
  paramNode->DisableModifiedEventOff();
}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::storeSelectedTableItemText(QTableWidgetItem* selectedItem, QTableWidgetItem* previousItem)
{
  Q_D(qSlicerIsodoseModuleWidget);

}

//-----------------------------------------------------------------------------
QString qSlicerIsodoseModuleWidget::generateNewIsodoseLevel() const
{
  Q_D(const qSlicerIsodoseModuleWidget);

  QString newIsodoseLevelBase("New level");
  QString newIsodoseLevel(newIsodoseLevelBase);
  int i=0;
  //while (d->InspectedNode->GetAttribute(newAttributeName.toLatin1()))
  //  {
  //  newAttributeName = QString("%1%2").arg(newAttributeNameBase).arg(++i);
  //  }
  return newIsodoseLevel;
}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::addClicked()
{
  Q_D(qSlicerIsodoseModuleWidget);

  vtkMRMLIsodoseNode* paramNode = d->logic()->GetIsodoseNode();
  if (!paramNode || !this->mrmlScene())
  {
    return;
  }

  int wasModifying = paramNode->StartModify();

  int rowCountBefore = d->tableWidget_DoseLevels->rowCount();
  //std::stringstream ss;//create a stringstream
  char stringNumber[4];
  sprintf(stringNumber, "%d", rowCountBefore);
  std::string ss(stringNumber);//add number to the stream
  QString qs = QString::fromAscii(ss.data(), ss.size());
  d->tableWidget_DoseLevels->insertRow( rowCountBefore );

  // Block signals so that onAttributeChanged function is not called when populating
  d->tableWidget_DoseLevels->blockSignals(true);

  d->tableWidget_DoseLevels->setItem( rowCountBefore, 0, new QTableWidgetItem( qs ) );
  d->tableWidget_DoseLevels->setItem( rowCountBefore, 1, new QTableWidgetItem( qs ) );

  // Unblock signals
  d->tableWidget_DoseLevels->blockSignals(false);

  std::vector<DoseLevelStruct>* isodoseLevelVector = paramNode->GetIsodoseLevelVector();
  DoseLevelStruct tempLevel;
  tempLevel.DoseLevelName = ss;
  tempLevel.DoseLevelValue = qs.toDouble();
  (*isodoseLevelVector).push_back(tempLevel);

  paramNode->Modified();
  paramNode->EndModify(wasModifying);

}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::removeClicked()
{
  Q_D(qSlicerIsodoseModuleWidget);

  vtkMRMLIsodoseNode* paramNode = d->logic()->GetIsodoseNode();
  if (!paramNode || !this->mrmlScene())
  {
    return;
  }

  // Extract selected row indices out of the selected table widget items list
  // (there may be more items selected in a row)
  QList<QTableWidgetItem*> selectedItems = d->tableWidget_DoseLevels->selectedItems();
  QSet<int> affectedRowNumbers;
  foreach (QTableWidgetItem* item, selectedItems)
    {
    affectedRowNumbers.insert(item->row());
    }

  int wasModifying = paramNode->StartModify();

  for (QSet<int>::iterator it = affectedRowNumbers.begin(); it != affectedRowNumbers.end(); ++it)
    {
    QString attributeNameToDelete( d->tableWidget_DoseLevels->item((*it), 0)->text() );
    d->tableWidget_DoseLevels->removeRow((*it));
    paramNode->GetIsodoseLevelVector()->erase( (*(paramNode->GetIsodoseLevelVector())).begin() + (*it) );
    }

  paramNode->Modified();
  paramNode->EndModify(wasModifying);
}

////-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::onTableItemChanged(QTableWidgetItem* changedItem)
{
  Q_D(qSlicerIsodoseModuleWidget);

  // Do nothing if unselected
  if (!changedItem)
  {
    return;
  }

  vtkMRMLIsodoseNode* paramNode = d->logic()->GetIsodoseNode();
  if (!paramNode || !this->mrmlScene())
  {
    return;
  }

  //// Set weight if the selected cell is "editable"
  //int wasModifying = paramNode->StartModify();

  //QTableWidgetItem* cellItem = (QTableWidgetItem*)(d->tableWidget_DoseLevels->cellWidget(changedItem->row(), 0));
  //(*paramNode->GetIsodoseLevelMap())[cellItem->text().toStdString()] = changedItem->text().toDouble();
  //paramNode->Modified();
  //paramNode->EndModify(wasModifying);

  //return;

  // If dose level name has been changed
  if (changedItem->column() == 0)
    {
    QTableWidgetItem* valueItem = d->tableWidget_DoseLevels->item( changedItem->row(), 1 );
    QString valueText;
    if (valueItem)
      {
      valueText = valueItem->text();
      }
     (*paramNode->GetIsodoseLevelVector())[changedItem->row()].DoseLevelName = valueText.toStdString();
    // Don't set if there is another dose level with the same☺ name (would overwrite it)
    //if ((*paramNode->GetIsodoseLevelMap())[changedItem->text().toLatin1().constData()])
    //if ((*paramNode->GetIsodoseLevelVector())[changedItem->row()])
    //  {
    //  //d->setMessage(tr("There is already an attribute with the same name"));
    //  //d->NodeAttributesTable->blockSignals(true);
    //  //changedItem->setText( d->SelectedAttributeTableItemText );
    //  //d->NodeAttributesTable->blockSignals(false);
    //  }
    //else
    //  {
    //  int wasModifying = paramNode->StartModify();

    //  //(*paramNode->GetIsodoseLevelVector())[changedItem->text().toStdString()] = valueText.toDouble();
    //  //d->InspectedNode->SetAttribute(
    //  //  d->SelectedAttributeTableItemText.toLatin1().constData(), 0);
    //  //d->InspectedNode->SetAttribute(
    //  //  changedItem->text().toLatin1().constData(), valueText.toLatin1().constData());

    //  paramNode->EndModify(wasModifying);
    // }
    }
  // If attribute value has been changed
  else if (changedItem->column() == 1)
    {
    QTableWidgetItem* nameItem = d->tableWidget_DoseLevels->item( changedItem->row(), 0 );
    QString nameText;
    if (nameItem)
      {
      nameText = nameItem->text();
      }
    (*paramNode->GetIsodoseLevelVector())[changedItem->row()].DoseLevelValue = changedItem->text().toDouble();
    //d->InspectedNode->SetAttribute( nameText.toLatin1().constData(), changedItem->text().toLatin1().constData() );
    }
}

//-----------------------------------------------------------------------------
void qSlicerIsodoseModuleWidget::applyClicked()
{
  Q_D(qSlicerIsodoseModuleWidget);

  d->logic()->ComputeIsodose();
}

