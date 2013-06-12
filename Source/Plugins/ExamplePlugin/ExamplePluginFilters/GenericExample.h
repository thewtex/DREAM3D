/*
 * Your License or Copyright Information can go here
 */

#ifndef _GenericExample_H_
#define _GenericExample_H_

#include <string>

#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/Common/IDataArray.h"
#include "DREAM3DLib/Common/AbstractFilter.h"


/**
 * @class GenericExample GenericExample.h ExamplePlugin/Code/ExamplePluginFilters/GenericExample.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class GenericExample : public AbstractFilter
{
  public:
    DREAM3D_SHARED_POINTERS(GenericExample);
    DREAM3D_STATIC_NEW_MACRO(GenericExample);
    DREAM3D_TYPE_MACRO_SUPER(GenericExample, AbstractFilter);

    virtual ~GenericExample();

   /* Place your input parameters here. You can use some of the DREAM3D Macros if you want to */
    DREAM3D_INSTANCE_STRING_PROPERTY(StlFilePrefix)
    DREAM3D_INSTANCE_PROPERTY(int, MaxIterations)
    DREAM3D_INSTANCE_PROPERTY(double, MisorientationTolerance)
    DREAM3D_INSTANCE_STRING_PROPERTY(InputFile)
    DREAM3D_INSTANCE_STRING_PROPERTY(InputPath)
    DREAM3D_INSTANCE_STRING_PROPERTY(OutputFile)
    DREAM3D_INSTANCE_STRING_PROPERTY(OutputPath)
    DREAM3D_INSTANCE_PROPERTY(bool, WriteAlignmentShifts)
    DREAM3D_INSTANCE_PROPERTY(int, ConversionType)

    DREAM3D_INSTANCE_STRING_PROPERTY(SelectedCellArrayName)
    DREAM3D_INSTANCE_STRING_PROPERTY(SelectedFieldArrayName)
    DREAM3D_INSTANCE_STRING_PROPERTY(SelectedEnsembleArrayName)
    DREAM3D_INSTANCE_STRING_PROPERTY(SurfaceMeshPointArrayName)
    DREAM3D_INSTANCE_STRING_PROPERTY(SurfaceMeshFaceArrayName)
    DREAM3D_INSTANCE_STRING_PROPERTY(SurfaceMeshEdgeArrayName)
    DREAM3D_INSTANCE_STRING_PROPERTY(SolidMeshPointArrayName)
    DREAM3D_INSTANCE_STRING_PROPERTY(SolidMeshFaceArrayName)
    DREAM3D_INSTANCE_STRING_PROPERTY(SolidMeshEdgeArrayName)


    DREAM3D_INSTANCE_PROPERTY(IntVec3Widget_t, Dimensions)
    DREAM3D_INSTANCE_PROPERTY(FloatVec3Widget_t, Origin)


    /**
    * @brief This returns the group that the filter belonds to. You can select
    * a different group if you want. The string returned here will be displayed
    * in the GUI for the filter
    */
    virtual const std::string getGroupName() { return "ExamplePlugin"; }

    /**
    * @brief This returns a string that is displayed in the GUI. It should be readable
    * and understandable by humans.
    */
    virtual const std::string getHumanLabel() { return "GenericExample"; }

    /**
    * @brief This returns a string that is displayed in the GUI and helps to sort the filters into
    * a subgroup. It should be readable and understandable by humans.
    */
    virtual const std::string getSubGroupName() { return "Misc"; }

    /**
    * @brief This method will instantiate all the end user settable options/parameters
    * for this filter
    */
    virtual void setupFilterParameters();

    /**
    * @brief This method will write the options to a file
    * @param writer The writer that is used to write the options to a file
    */
    virtual void writeFilterParameters(AbstractFilterParametersWriter* writer);

   /**
    * @brief Reimplemented from @see AbstractFilter class
    */
    virtual void execute();

    /**
    * @brief This function runs some sanity checks on the DataContainer and inputs
    * in an attempt to ensure the filter can process the inputs.
    */
    virtual void preflight();

  protected:
    GenericExample();

    /**
    * @brief Checks for the appropriate parameter values and availability of
    * arrays in the data container
    * @param preflight
    * @param voxels The number of voxels
    * @param fields The number of fields
    * @param ensembles The number of ensembles
    */
    void dataCheck(bool preflight, size_t voxels, size_t fields, size_t ensembles);

  private:

    GenericExample(const GenericExample&); // Copy Constructor Not Implemented
    void operator=(const GenericExample&); // Operator '=' Not Implemented
};

#endif /* _GenericExample_H_ */