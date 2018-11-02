/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkGDALRasterReader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkGDALRasterReader
 * @brief   Read raster file formats using GDAL.
 *
 * vtkGDALRasterReader is a source object that reads raster files and
 * uses GDAL as the underlying library for the task. GDAL library is
 * required for this reader. The output of the reader is a
 * vtkUniformGrid (vtkImageData with blanking) with cell data.
 *
 *
 * @sa
 * vtkUniformGrid, vtkImageData
*/

#ifndef vtkGDALRasterReader_h
#define vtkGDALRasterReader_h

#include <vtkImageReader2.h>
#include <vtkIOGDALModule.h> // For export macro

// C++ includes
#include <string> // string is required
#include <vector> // vector is required

class VTKIOGDAL_EXPORT vtkGDALRasterReader : public vtkImageReader2
{
public:
  static vtkGDALRasterReader* New();
  vtkTypeMacro(vtkGDALRasterReader, vtkImageReader2);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  vtkGDALRasterReader();
  ~vtkGDALRasterReader() override;

  /**
   * Is this file supported
   */
  int CanReadFile(const char* fname) override;

  /**
   * Return proj4 spatial reference
   */
  const char*  GetProjectionString() const;

  /**
   * Return geo-referenced corner points (Upper left,
   * lower left, lower right, upper right)
   */
  const double* GetGeoCornerPoints();

  /**
   * Get/Set if bands are collated in one scalar array.
   * Currently we collate RGB, RGBA, gray alpha and pallete.
   * The default is true.
   */
  vtkSetMacro(CollateBands, bool);
  vtkGetMacro(CollateBands, bool);
  vtkBooleanMacro(CollateBands, bool);

  //@{
  /**
   * Set desired width and height of the image
   */
  vtkSetVector2Macro(TargetDimensions, int);
  vtkGetVector2Macro(TargetDimensions, int);
  //@}

  //@{
  /**
   * Get raster width and height
   */
  vtkGetVector2Macro(RasterDimensions, int);
  //@}

  /**
   * Return metadata as reported by GDAL
   */
  const std::vector<std::string>& GetMetaData();

  /**
   * Return the invalid value for a pixel (for blanking purposes) in
   * a specified raster band. Note bandIndex is a 0 based index while
   * GDAL bands are 1 based indexes.
   */
  double GetInvalidValue(int bandIndex = 0);

  /**
   * Return domain metadata
   */
  std::vector<std::string> GetDomainMetaData(const std::string& domain);

  //@{
  /**
   * Return driver name which was used to read the current data
   */
  const std::string& GetDriverShortName();
  const std::string& GetDriverLongName();
  //@}

  /**
   * Return the number of cells that are not set to GDAL NODATA
   */
  vtkIdType GetNumberOfCells();

  //@{
  /**
   * The following methods allow selective reading of bands.
   * By default, ALL bands are read.
   */
  int GetNumberOfCellArrays();
  const char* GetCellArrayName(int index);
  int GetCellArrayStatus(const char* name);
  void SetCellArrayStatus(const char* name, int status);
  void DisableAllCellArrays();
  void EnableAllCellArrays();
  //@}


protected:

  int RequestData(vtkInformation* request,
                  vtkInformationVector** inputVector,
                  vtkInformationVector* outputVector) override;

  int RequestInformation(vtkInformation* request,
                         vtkInformationVector** inputVector,
                         vtkInformationVector* outputVector) override;

  int FillOutputPortInformation(int port,
                                vtkInformation* info) override;

protected:
  int TargetDimensions[2];
  int RasterDimensions[2];
  std::string Projection;
  std::string DomainMetaData;
  std::string DriverShortName;
  std::string DriverLongName;
  std::vector<std::string> Domains;
  std::vector<std::string> MetaData;
  bool CollateBands;

  class vtkGDALRasterReaderInternal;
  vtkGDALRasterReaderInternal* Impl;

private:
  vtkGDALRasterReader(const vtkGDALRasterReader&) = delete;
  void operator=(const vtkGDALRasterReader&) = delete;
};

#endif // vtkGDALRasterReader_h
