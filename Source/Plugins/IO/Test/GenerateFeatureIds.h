#pragma once

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/SIMPLib.h"

#include "IOTestFileLocations.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class GenerateFeatureIds : public AbstractFilter
{
  Q_OBJECT
    PYB11_CREATE_BINDINGS(GenerateFeatureIds SUPERCLASS AbstractFilter)
public:
  SIMPL_SHARED_POINTERS(GenerateFeatureIds)
  SIMPL_FILTER_NEW_MACRO(GenerateFeatureIds)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(GenerateFeatureIds, AbstractFilter)

  ~GenerateFeatureIds()
  {
  }
  SIMPL_INSTANCE_STRING_PROPERTY(DataContainerName)
  //------ Created Cell Data
  SIMPL_INSTANCE_STRING_PROPERTY(CellAttributeMatrixName)
  SIMPL_INSTANCE_STRING_PROPERTY(CellFeatureAttributeMatrixName)
  SIMPL_INSTANCE_STRING_PROPERTY(CellEnsembleAttributeMatrixName)

  virtual const QString getGroupName() const override
  {
    return "UnitTest";
  }
  virtual const QString getHumanLabel() const override
  {
    return "Generate Feature Ids";
  }
  virtual void execute() override
  {
    setErrorCondition(0);
    setWarningCondition(0);
    dataCheck();
    if(getErrorCondition() < 0)
    {
      return;
    }

    DataContainer::Pointer m = getDataContainerArray()->getDataContainer(SIMPL::Defaults::DataContainerName);

    int size = UnitTest::FeatureIdsTest::XSize * UnitTest::FeatureIdsTest::YSize * UnitTest::FeatureIdsTest::ZSize;
    QVector<size_t> tDims(3, 0);
    tDims[0] = UnitTest::FeatureIdsTest::XSize;
    tDims[1] = UnitTest::FeatureIdsTest::YSize;
    tDims[2] = UnitTest::FeatureIdsTest::ZSize;
    m->getAttributeMatrix(getCellAttributeMatrixName())->resizeAttributeArrays(tDims);
    m->getAttributeMatrix(getCellAttributeMatrixName())->removeAttributeArray(m_FeatureIdsArrayName);

    dataCheck();
    // Set the default data into the FeatureIds
    for(int i = 0; i < size; ++i)
    {
      m_FeatureIds[i] = i + UnitTest::FeatureIdsTest::Offset;
    }
  }
  virtual void preflight() override
  {
    dataCheck();
  }

  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override
  {
    reader->openFilterGroup(this, index);
    reader->closeFilterGroup();
  }

protected:
  GenerateFeatureIds()
  : m_DataContainerName(SIMPL::Defaults::DataContainerName)
  , m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
  , m_CellFeatureAttributeMatrixName(SIMPL::Defaults::CellFeatureAttributeMatrixName)
  , m_FeatureIds(nullptr)
  , m_FeatureIdsArrayName(SIMPL::CellData::FeatureIds)
  {
  }

private:
  DEFINE_DATAARRAY_VARIABLE(int32_t, FeatureIds)
  QString m_FeatureIdsArrayName;

  void dataCheck()
  {
    setErrorCondition(0);
    setWarningCondition(0);
    DataContainer::Pointer m = getDataContainerArray()->getPrereqDataContainer(this, getDataContainerName());
    if(getErrorCondition() < 0)
    {
      return;
    }
    AttributeMatrix::Pointer cellAttrMat = m->getPrereqAttributeMatrix(this, getCellAttributeMatrixName(), -301);
    if(getErrorCondition() < 0)
    {
      return;
    }

    QVector<size_t> dims(1, 1);
    m_FeatureIdsPtr =
        cellAttrMat->createNonPrereqArray<DataArray<int32_t>, AbstractFilter>(this, m_FeatureIdsArrayName, 0, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
    if(nullptr != m_FeatureIdsPtr.lock()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
    {
      m_FeatureIds = m_FeatureIdsPtr.lock()->getPointer(0);
    } /* Now assign the raw pointer to data from the DataArray<T> object */
  }

public:
  GenerateFeatureIds(const GenerateFeatureIds&) = delete; // Copy Constructor Not Implemented
  GenerateFeatureIds(GenerateFeatureIds&&) = delete;      // Move Constructor
  GenerateFeatureIds& operator=(const GenerateFeatureIds&) = delete; // Copy Assignment Not Implemented
  GenerateFeatureIds& operator=(GenerateFeatureIds&&) = delete;      // Move Assignment Not Implemented
};

/**
 * @class CreateImageGeomDataContainer
 * @brief This class simply adds a DataContainer with default name to the DataContainerArray
 * @author
 * @date
 * @version 1.0
 */
class CreateImageGeomDataContainer : public AbstractFilter
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(CreateImageGeomDataContainer)
  SIMPL_FILTER_NEW_MACRO(CreateImageGeomDataContainer)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(CreateImageGeomDataContainer, AbstractFilter)

  SIMPL_FILTER_PARAMETER(int64_t, XDim)
  SIMPL_FILTER_PARAMETER(int64_t, YDim)
  SIMPL_FILTER_PARAMETER(int64_t, ZDim)

  ~CreateImageGeomDataContainer()
  {
  }

  /**
  * @brief This returns the group that the filter belonds to. You can select
  * a different group if you want. The string returned here will be displayed
  * in the GUI for the filter
  */
  virtual const QString getGroupName() const override
  {
    return "UnitTest";
  }

  /**
  * @brief This returns a string that is displayed in the GUI. It should be readable
  * and understandable by humans.
  */
  virtual const QString getHumanLabel() const override
  {
    return "CreateImageGeomDataContainer Filter";
  }

  /**
  * @brief Reimplemented from @see AbstractFilter class
  */
  virtual void execute() override
  {
    dataCheck();
  }

  /**
  * @brief This function runs some sanity checks on the DataContainer and inputs
  * in an attempt to ensure the filter can process the inputs.
  */
  virtual void preflight() override
  {
    dataCheck();
  }

  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override
  {
    reader->openFilterGroup(this, index);
    reader->closeFilterGroup();
  }

protected:
  CreateImageGeomDataContainer()
  : m_XDim(0)
  , m_YDim(0)
  , m_ZDim(0)
  {
  }

  /**
  * @brief Checks for the appropriate parameter values and availability of
  * arrays in the data container
  * @param preflight
  * @param voxels The number of voxels
  * @param features The number of features
  * @param ensembles The number of ensembles
  */
  void dataCheck()
  {
    DataContainer::Pointer m = DataContainer::New("DataContainer");
    ImageGeom::Pointer image = ImageGeom::CreateGeometry(SIMPL::Geometry::ImageGeometry);
    m->setGeometry(image);

    m->setName(SIMPL::Defaults::DataContainerName);
    int64_t nx = m_XDim;
    int64_t ny = m_YDim;
    int64_t nz = m_ZDim;
    m->getGeometryAs<ImageGeom>()->setDimensions(std::make_tuple(nx, ny, nz));
    getDataContainerArray()->addDataContainer(m);
    QVector<size_t> tDims(3, 0);
    tDims[0] = nx;
    tDims[1] = ny;
    tDims[2] = nz;
    AttributeMatrix::Pointer attrMat = AttributeMatrix::New(tDims, SIMPL::Defaults::CellAttributeMatrixName, AttributeMatrix::Type::Cell);
    m->addAttributeMatrix(attrMat->getName(), attrMat);
  }

private:
  CreateImageGeomDataContainer(const CreateImageGeomDataContainer&) = delete; // Copy Constructor Not Implemented
  void operator=(const CreateImageGeomDataContainer&);               // Move assignment Not Implemented
};

