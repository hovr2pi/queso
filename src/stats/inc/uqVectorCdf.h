//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// QUESO - a library to support the Quantification of Uncertainty
// for Estimation, Simulation and Optimization
//
// Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor, 
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
// 
// $Id$
//
//--------------------------------------------------------------------------

#ifndef __UQ_VECTOR_CUMULATIVE_DISTRIBUTION_FUNCTION_H__
#define __UQ_VECTOR_CUMULATIVE_DISTRIBUTION_FUNCTION_H__

#include <uqArrayOfOneDGrids.h>
#include <uqArrayOfOneDTables.h>
#include <uqScalarCdf.h>
#include <uqEnvironment.h>
#include <math.h>

namespace QUESO {

//*****************************************************
// Classes to accommodate a cumulative distribution function
//*****************************************************
/*! \file uqVectorCdf.h
 * \brief Classes to accommodate a cumulative distribution function of a vector RV.
 * 
 * \class BaseVectorCdfClass
 * \brief A templated (base) class for handling CDFs of vector functions.
 * 
 * In many applications is necessary to consider the properties of two or more RVs 
 * simultaneously (represented within QUESO as Random Vectors, via BaseVectorRVClass 
 * and derived classes). When dealing simultaneously with more than one RV, ie, a vector
 * RV, the joint cumulative distribution function must also be defined. This class handles
 * the CDFs of vector RV, which are referred to as  multivariate/vector/joint CDFs.*/

//*****************************************************
// Base class
//*****************************************************
template<class V, class M>
class BaseVectorCdfClass {
public:
  //! @name Constructor/Destructor methods
  //@{ 
  //! Default constructor.
  /*! Instantiates an object of the class  given a prefix and the support (image set) of the PDF that is 
   * related to this CDF (recall that the CDF of a continuous RV is the integral of the PDF of that RV).*/
  BaseVectorCdfClass(const char*                  prefix,
		       const VectorSetClass<V,M>& pdfSupport);
  
  //! Virtual destructor.
  virtual ~BaseVectorCdfClass();
  //@}

  //! @name Mathematical methods
  //@{
  //! Returns the image set (support) of the PDF; access to protected attribute \c m_pdfSupport.
  const VectorSetClass<V,M>&        pdfSupport      ()                                const;
  
  //! Finds the value of the vector CDF at each element of \c paramValue, and saves it in \c cdfVec. See template specialization.  
  virtual void                                values          (const V& paramValues, V& cdfVec) const = 0;
  
  //!
  virtual const BaseScalarCdfClass<double>& cdf             (unsigned int rowId)              const = 0;
  //@}
  
  //! @name I/O methods
  //@{ 
  //! Prints the vector CDF. See template specialization. 
  virtual void                                print           (std::ostream& os)                const = 0;
  
  
  //! Writes the CDF of an allowed sub-environment to a file. 
  /*! This function does nothing and should \n not be called by the user.*/
  virtual void                                subWriteContents(const std::string&            varNamePrefix,
                                                               const std::string&            fileName,
                                                               const std::string&            fileType,
                                                               const std::set<unsigned int>& allowedSubEnvIds) const;
  //@}
protected:

  const   BaseEnvironmentClass& m_env;
          std::string             m_prefix;
  const   VectorSetClass<V,M>&  m_pdfSupport;
};
// Default constructor -----------------------------
template<class V, class M>
BaseVectorCdfClass<V,M>::BaseVectorCdfClass(
  const char*                  prefix,
  const VectorSetClass<V,M>& pdfSupport)
  :
  m_env       (pdfSupport.env()),
  m_prefix    ((std::string)(prefix)+"Cdf_"),
  m_pdfSupport(pdfSupport)
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Entering BaseVectorCdfClass<V,M>::constructor()"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Leaving BaseVectorCdfClass<V,M>::constructor()"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }
}
// Destructor ---------------------------------------
template<class V, class M>
BaseVectorCdfClass<V,M>::~BaseVectorCdfClass()
{
}
// Math methods--------------------------------------
template<class V, class M>
const VectorSetClass<V,M>&
BaseVectorCdfClass<V,M>::pdfSupport() const
{
  return m_pdfSupport;
}
// I/O methods---------------------------------------
template<class V, class M>
void
BaseVectorCdfClass<V,M>::subWriteContents(
  const std::string&            varNamePrefix,
  const std::string&            fileName,
  const std::string&            fileType,
  const std::set<unsigned int>& allowedSubEnvIds) const
{
  std::cerr << "WARNING: BaseVectorCdfClass<V,M>::subWriteContents() being used..."
            << std::endl;

  if (&varNamePrefix)    {}; // just to remove compiler warning
  if (&fileName)         {}; // just to remove compiler warning
  if (&fileType)         {}; // just to remove compiler warning
  if (&allowedSubEnvIds) {}; // just to remove compiler warning
  return;
}
// --------------------------------------------------
// Operator defined outside class limits-------------
// --------------------------------------------------
//! Operator to be used with print().
template <class V, class M>
  std::ostream& operator<< (std::ostream& os, const BaseVectorCdfClass<V,M>& obj)
{
  obj.print(os);
  return os;
}

//*****************************************************
// Generic cumulative distribution function class
//*****************************************************
/*! 
 * \class GenericVectorCdfClass
 * \brief A class for handling generic vector CDFs.
 *
 * This class \b will implement a generic vector cumulative distribution function (CDF).*/
 
template<class V, class M>
class GenericVectorCdfClass : public BaseVectorCdfClass<V,M> {
public:
    //! @name Constructor/Destructor methods
  //@{ 
  //! Constructor.
  /*! Instantiates an object of the class given a prefix, the support of the related-PDF, and 
   * a routine that calculates data (like a math function). */
  GenericVectorCdfClass(const char*                  prefix,
                          const VectorSetClass<V,M>& pdfSupport,
                          double (*routinePtr)(const V& paramValues, const void* routineDataPtr, V& cdfVec),
                          const void*                  routineDataPtr);
  //! Destructor
  ~GenericVectorCdfClass();
  //@}

    //! @name Mathematical method
  //@{
  //! TODO: Returns the values of the vector CDF at each element of \c paramValues, by calling \c m_routinePtr. 
  void values(const V& paramValues, V& cdfVec) const;
  //@}
  
  //! @name I/O method
  //@{ 
  //! TODO: Prints the vector CDF. 
  /*! \todo: implement me!*/
  void print (std::ostream& os) const;
  //@}
  
protected:
  double (*m_routinePtr)(const V& paramValues, const void* routineDataPtr, V& cdfVec);
  const void* m_routineDataPtr;

  using BaseVectorCdfClass<V,M>::m_env;
  using BaseVectorCdfClass<V,M>::m_prefix;
  using BaseVectorCdfClass<V,M>::m_pdfSupport;
};
// Constructor ----------------------------------------
template<class V, class M>
GenericVectorCdfClass<V,M>::GenericVectorCdfClass(
  const char*                    prefix,
  const VectorSetClass<V,M>& pdfSupport,
  double (*routinePtr)(const V& paramValues, const void* routineDataPtr, V& cdfVec),
  const void* routineDataPtr)
  :
  BaseVectorCdfClass<V,M>(prefix,pdfSupport),
  m_routinePtr    (routinePtr),
  m_routineDataPtr(routineDataPtr)
{
}
// Destructor ---------------------------------------
template<class V, class M>
GenericVectorCdfClass<V,M>::~GenericVectorCdfClass()
{
}
// Math method --------------------------------------
template<class V, class M>
void
GenericVectorCdfClass<V,M>::values(
  const V& paramValues,
        V& cdfVec) const
{
  m_routinePtr(paramValues, m_routineDataPtr, cdfVec);
  return;
}
// I/O methods---------------------------------------
template <class V, class M>
void
GenericVectorCdfClass<V,M>::print(std::ostream& os) const
{
  return;
}

//*****************************************************
// Gaussian cumulative distribution function class
//*****************************************************
/*! 
 * \class GaussianVectorCdfClass
 * \brief TODO: A class for handling Gaussian CDFs.
 *
 * This class \b will implement a Gaussian vector cumulative distribution function (CDF). 
 * \todo: Implement me! */

template<class V, class M>
class GaussianVectorCdfClass : public BaseVectorCdfClass<V,M> {
public:
  //! @name Constructor/Destructor methods
  //@{ 
  //! TODO: Constructor.
  /*! \todo: implement me! This method calls commonConstructor() which is not yet implemented.
   *  Instantiates an object of the class given a prefix, the support of the related-PDF, and 
   * the domain mean and expected values. 
   */
  GaussianVectorCdfClass(const char*                  prefix,
                           const VectorSetClass<V,M>& pdfSupport,
                           const V&                     domainExpectedValues,
                           const V&                     domainVarianceValues);
  //! TODO: Constructor.
  /*! \todo: implement me! This method calls commonConstructor() which is not yet implemented.
   * Instantiates an object of the class given a prefix, the support of the related-PDF, and 
   * the domain mean values and covariance matrix.*/
  GaussianVectorCdfClass(const char*                  prefix,
                           const VectorSetClass<V,M>& pdfSupport,
                           const V&                     domainExpectedValues,
                           const M&                     covMatrix);
  // Destructor
  ~GaussianVectorCdfClass();
  //@}

  //! @name Mathematical method
  //@{
  //! TODO: Returns the values of the vector CDF at each element of \c paramValues. 
  /*! \todo: implement me!*/
  void values(const V& paramValues, V& cdfVec) const;
  //@}
  
  //! @name I/O method
  //@{ 
  //! TODO: Prints the vector CDF. 
  /*! \todo: implement me!*/
  void print (std::ostream& os) const;
  //@}

protected:
  const M*                         m_covMatrix;

  using BaseVectorCdfClass<V,M>::m_env;
  using BaseVectorCdfClass<V,M>::m_prefix;
  using BaseVectorCdfClass<V,M>::m_pdfSupport;

  //! A common constructor to be used both class constructors.
  void commonConstructor();
};
// Constructor -------------------------------------
template<class V,class M>
GaussianVectorCdfClass<V,M>::GaussianVectorCdfClass(
  const char*                    prefix,
  const VectorSetClass<V,M>& pdfSupport,
  const V&                       domainExpectedValues,
  const V&                       domainVarianceValues)
  :
  BaseVectorCdfClass<V,M>(prefix,pdfSupport),
  m_covMatrix              (m_pdfSupport.newDiagMatrix(domainVarianceValues*domainVarianceValues))
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Entering GaussianVectorCdfClass<V,M>::constructor() [1]"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }

  commonConstructor();

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Leaving GaussianVectorCdfClass<V,M>::constructor() [1]"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }
}
// Constructor -------------------------------------
template<class V,class M>
GaussianVectorCdfClass<V,M>::GaussianVectorCdfClass(
  const char*                    prefix,
  const VectorSetClass<V,M>& pdfSupport,
  const V&                       domainExpectedValues,
  const M&                       covMatrix)
  :
  BaseVectorCdfClass<V,M>(prefix,pdfSupport),
  m_covMatrix              (new M(covMatrix))
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Entering GaussianVectorCdfClass<V,M>::constructor() [2]"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }

  commonConstructor();

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Leaving GaussianVectorCdfClass<V,M>::constructor() [2]"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }
}
// Destructor -------------------------------------
template<class V,class M>
GaussianVectorCdfClass<V,M>::~GaussianVectorCdfClass()
{
  delete m_covMatrix;
}
// Math method -------------------------------------
template<class V, class M>
void
GaussianVectorCdfClass<V,M>::values(
  const V& paramValues,
        V& cdfVec) const
{
  UQ_FATAL_TEST_MACRO(true,
                      m_env.worldRank(),
                      "GaussianVectorCdfClass<V,M>::cdfVec()",
                      "incomplete code");
  return;
}
// I/O method --------------------------------------
template <class V, class M>
void
GaussianVectorCdfClass<V,M>::print(std::ostream& os) const
{
  return;
}

// Protected member function------------------------
template<class V,class M>
void
GaussianVectorCdfClass<V,M>::commonConstructor()
{
  UQ_FATAL_TEST_MACRO(true,
                      m_env.worldRank(),
                      "GaussianVectorCdfClass<V,M>::commonConstructor()",
                      "incomplete code");
  return;
}

//*****************************************************
// Sampled cumulative distribution function class
//*****************************************************
/*! 
 * \class SampledVectorCdfClass
 * \brief A class for handling sampled vector CDFs.
 *
 * This class implements a sampled vector cumulative distribution function (CDF), given 
 * the grid points where it will be sampled and it returns its values.*/

template<class V, class M>
class SampledVectorCdfClass : public BaseVectorCdfClass<V,M> {
public:
  //! @name Constructor/Destructor methods
  //@{ 
  //! Default constructor.
  /*! Instantiates an object of the class given a prefix and the grid points 
   * where it will be sampled/evaluated.*/
  SampledVectorCdfClass(const char*                          prefix,
                          const ArrayOfOneDGridsClass <V,M>& oneDGrids,
                          const ArrayOfOneDTablesClass<V,M>& cdfValues);
  //! Destructor
  ~SampledVectorCdfClass();
  //@}
  
  //! @name Mathematical methods
  //@{
  //! TODO: Returns the values of the vector CDF at each element of \c paramValues. 
  /*! \todo: implement me!*/
  void                          values(const V& paramValues, V& cdfVec) const;
  
  //! Returns a scalar CDF stored at row \c rowId of the vector CDF.
  const BaseScalarCdfClass<double>& cdf   (unsigned int rowId)              const;
  //@}
  
    //! @name I/O methods
  //@{ 
  //! Prints the vector CDF (values of the grid points and of the CDF at such grid points). 
  void                          print (std::ostream& os)                const;
  
  //!Writes the CDF of an allowed sub-environment to a file. 
  void                          subWriteContents(const std::string&            varNamePrefix,
						 const std::string&            fileName,
						 const std::string&            fileType,
						 const std::set<unsigned int>& allowedSubEnvIds) const;
  //@}						 
protected:
  using BaseVectorCdfClass<V,M>::m_env;
  using BaseVectorCdfClass<V,M>::m_prefix;
  using BaseVectorCdfClass<V,M>::m_pdfSupport;

  DistArrayClass<SampledScalarCdfClass<double>*> m_cdfs;
};
// Default constructor -----------------------------
template<class V,class M>
SampledVectorCdfClass<V,M>::SampledVectorCdfClass(
  const char*                          prefix,
  const ArrayOfOneDGridsClass <V,M>& oneDGrids,
  const ArrayOfOneDTablesClass<V,M>& cdfValues)
  :
  BaseVectorCdfClass<V,M>(prefix,oneDGrids.rowSpace()),
  m_cdfs(m_pdfSupport.vectorSpace().map(),1)
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Entering SampledVectorCdfClass<V,M>::constructor()"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }

  char strI[65];
  for (unsigned int i = 0; i < (unsigned int) m_cdfs.MyLength(); ++i) {
    sprintf(strI,"%u_",i);
    m_cdfs(i,0) = new SampledScalarCdfClass<double>(m_env,
                                                      ((std::string)(m_prefix)+strI).c_str(),
                                                      oneDGrids.grid(i),
                                                      cdfValues.oneDTable(i));
  }

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Leaving SampledVectorCdfClass<V,M>::constructor()"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }
}
// Destructor ---------------------------------------
template<class V,class M>
SampledVectorCdfClass<V,M>::~SampledVectorCdfClass()
{
  for (unsigned int i = 0; i < (unsigned int) m_cdfs.MyLength(); ++i) {
    if (m_cdfs(i,0)) delete m_cdfs(i,0);
  }
}
// Math methods--------------------------------------
template<class V, class M>
void
SampledVectorCdfClass<V,M>::values(
  const V& paramValues,
        V& cdfVec) const
{
  UQ_FATAL_TEST_MACRO(true,
                      m_env.worldRank(),
                      "SampledVectorCdfClass<V,M>::cdfVec()",
                      "incomplete code");
  return;
}
// --------------------------------------------------
template<class V, class M>
const BaseScalarCdfClass<double>&
SampledVectorCdfClass<V,M>::cdf(unsigned int rowId) const
{
  UQ_FATAL_TEST_MACRO(rowId >= m_pdfSupport.vectorSpace().dimLocal(),
                      m_env.worldRank(),
                      "SampledVectorCdfClass<T>::cdf()",
                      "rowId is out of range");

  SampledVectorCdfClass<V,M>* tmp = const_cast<SampledVectorCdfClass<V,M>*>(this);
  return *(tmp->m_cdfs(rowId,0));
  
}
// I/O methods---------------------------------------
template <class V, class M>
void
SampledVectorCdfClass<V,M>::print(std::ostream& os) const
{
  SampledVectorCdfClass<V,M>* tmp = const_cast<SampledVectorCdfClass<V,M>*>(this);
  for (unsigned int i = 0; i < (unsigned int) m_cdfs.MyLength(); ++i) {
    os << (*tmp->m_cdfs(i,0))
       << std::endl;
  }

  return;
}
//---------------------------------------------------
template<class V, class M>
void
SampledVectorCdfClass<V,M>::subWriteContents(
  const std::string&            varNamePrefix,
  const std::string&            fileName,
  const std::string&            fileType,
  const std::set<unsigned int>& allowedSubEnvIds) const
{
  UQ_FATAL_TEST_MACRO(m_env.subRank() < 0,
                      m_env.worldRank(),
                      "SampledVectorCdfClass<V,M>::subWriteContents()",
                      "unexpected subRank");

  SampledVectorCdfClass<V,M>* tmp = const_cast<SampledVectorCdfClass<V,M>*>(this);
  char compId[16+1];
  for (unsigned int i = 0; i < (unsigned int) m_cdfs.MyLength(); ++i) {
    sprintf(compId,"%d",i);
    tmp->m_cdfs(i,0)->subWriteContents(varNamePrefix+"comp"+compId,fileName,fileType,allowedSubEnvIds);
  }

  return;
}
//---------------------------------------------------
// Method outside either class definition------------
//---------------------------------------------------
//! It calculated the maximum horizontal distances between two vector CDFs.
// 
template <class V, class M>
void
horizontalDistances(const BaseVectorCdfClass<V,M>& cdf1,
                    const BaseVectorCdfClass<V,M>& cdf2,
                    const V& epsilonVec,
                    V&       distances)
{
  for (unsigned int i = 0; i < cdf1.pdfSupport().vectorSpace().dimLocal(); ++i) {
    distances[i] = horizontalDistance(cdf1.cdf(i),
                                      cdf2.cdf(i),
                                      epsilonVec[i]);
  }

  return;
}

}  // End namespace QUESO

#endif // __UQ_VECTOR_CUMULATIVE_DISTRIBUTION_FUNCTION_H__
