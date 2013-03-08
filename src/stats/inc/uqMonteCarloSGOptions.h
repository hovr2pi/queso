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

#ifndef __UQ_MOC_SG_OPTIONS_H__
#define __UQ_MOC_SG_OPTIONS_H__

#include <uqEnvironment.h>
#include <uqSequenceStatisticalOptions.h>

#define UQ_MOC_SG_FILENAME_FOR_NO_FILE "."

// _ODV = option default value
#define UQ_MOC_SG_DATA_OUTPUT_FILE_NAME_ODV        UQ_MOC_SG_FILENAME_FOR_NO_FILE
#define UQ_MOC_SG_DATA_OUTPUT_ALLOWED_SET_ODV      ""

#define UQ_MOC_SG_PSEQ_DATA_OUTPUT_PERIOD_ODV      0
#define UQ_MOC_SG_PSEQ_DATA_OUTPUT_FILE_NAME_ODV   UQ_MOC_SG_FILENAME_FOR_NO_FILE
#define UQ_MOC_SG_PSEQ_DATA_OUTPUT_FILE_TYPE_ODV   UQ_FILE_EXTENSION_FOR_MATLAB_FORMAT
#define UQ_MOC_SG_PSEQ_DATA_OUTPUT_ALLOWED_SET_ODV ""
#define UQ_MOC_SG_PSEQ_COMPUTE_STATS_ODV           0

#define UQ_MOC_SG_QSEQ_DATA_INPUT_FILE_NAME_ODV    UQ_MOC_SG_FILENAME_FOR_NO_FILE
#define UQ_MOC_SG_QSEQ_DATA_INPUT_FILE_TYPE_ODV    UQ_FILE_EXTENSION_FOR_MATLAB_FORMAT
#define UQ_MOC_SG_QSEQ_SIZE_ODV                    100
#define UQ_MOC_SG_QSEQ_DISPLAY_PERIOD_ODV          500
#define UQ_MOC_SG_QSEQ_MEASURE_RUN_TIMES_ODV       0
#define UQ_MOC_SG_QSEQ_DATA_OUTPUT_PERIOD_ODV      0
#define UQ_MOC_SG_QSEQ_DATA_OUTPUT_FILE_NAME_ODV   UQ_MOC_SG_FILENAME_FOR_NO_FILE
#define UQ_MOC_SG_QSEQ_DATA_OUTPUT_FILE_TYPE_ODV   UQ_FILE_EXTENSION_FOR_MATLAB_FORMAT
#define UQ_MOC_SG_QSEQ_DATA_OUTPUT_ALLOWED_SET_ODV ""
#define UQ_MOC_SG_QSEQ_COMPUTE_STATS_ODV           0

class uqMcOptionsValuesClass
{
public:
#ifdef QUESO_USES_SEQUENCE_STATISTICAL_OPTIONS
  uqMcOptionsValuesClass            (const uqSsOptionsValuesClass* alternativePSsOptionsValues,
                                     const uqSsOptionsValuesClass* alternativeQSsOptionsValues);
#else
  uqMcOptionsValuesClass            ();
#endif
  uqMcOptionsValuesClass            (const uqMcOptionsValuesClass& src);
  uqMcOptionsValuesClass& operator= (const uqMcOptionsValuesClass& rhs);
 ~uqMcOptionsValuesClass            ();

  std::string                        m_dataOutputFileName;
  std::set<unsigned int>             m_dataOutputAllowedSet;

  unsigned int                       m_pseqDataOutputPeriod;
  std::string                        m_pseqDataOutputFileName;
  std::string                        m_pseqDataOutputFileType;
  std::set<unsigned int>             m_pseqDataOutputAllowedSet;
#ifdef QUESO_USES_SEQUENCE_STATISTICAL_OPTIONS
  bool                               m_pseqComputeStats;
#endif

  std::string                        m_qseqDataInputFileName;
  std::string                        m_qseqDataInputFileType;
  unsigned int                       m_qseqSize;
  unsigned int                       m_qseqDisplayPeriod;
  bool                               m_qseqMeasureRunTimes;
  unsigned int                       m_qseqDataOutputPeriod;
  std::string                        m_qseqDataOutputFileName;
  std::string                        m_qseqDataOutputFileType;
  std::set<unsigned int>             m_qseqDataOutputAllowedSet;
#ifdef QUESO_USES_SEQUENCE_STATISTICAL_OPTIONS
  bool                               m_qseqComputeStats;
#endif

private:
  void copy(const uqMcOptionsValuesClass& src);

#ifdef QUESO_USES_SEQUENCE_STATISTICAL_OPTIONS
  friend class uqMonteCarloSGOptionsClass;
  uqSsOptionsValuesClass             m_alternativePSsOptionsValues;
  uqSsOptionsValuesClass             m_alternativeQSsOptionsValues;
#endif
};

class uqMonteCarloSGOptionsClass
{
public:
  uqMonteCarloSGOptionsClass(const uqBaseEnvironmentClass& env, const char* prefix);
  uqMonteCarloSGOptionsClass(const uqBaseEnvironmentClass& env, const char* prefix, const uqMcOptionsValuesClass& alternativeOptionsValues);
 ~uqMonteCarloSGOptionsClass();

  void scanOptionsValues();
  void print            (std::ostream& os) const;

  uqMcOptionsValuesClass             m_ov;
#ifdef QUESO_USES_SEQUENCE_STATISTICAL_OPTIONS
  uqSequenceStatisticalOptionsClass* m_pseqStatisticalOptionsObj;
  uqSequenceStatisticalOptionsClass* m_qseqStatisticalOptionsObj;
#endif
  std::string                        m_prefix;

private:
  void   defineMyOptions  (po::options_description& optionsDesc) const;
  void   getMyOptionValues(po::options_description& optionsDesc);

  const uqBaseEnvironmentClass& m_env;
  po::options_description*      m_optionsDesc;

  std::string                   m_option_help;
  std::string                   m_option_dataOutputFileName;
  std::string                   m_option_dataOutputAllowedSet;

  std::string                   m_option_pseq_dataOutputPeriod;
  std::string                   m_option_pseq_dataOutputFileName;
  std::string                   m_option_pseq_dataOutputFileType;
  std::string                   m_option_pseq_dataOutputAllowedSet;
#ifdef QUESO_USES_SEQUENCE_STATISTICAL_OPTIONS
  std::string                   m_option_pseq_computeStats;
#endif

  std::string                   m_option_qseq_dataInputFileName;
  std::string                   m_option_qseq_dataInputFileType;
  std::string                   m_option_qseq_size;
  std::string                   m_option_qseq_displayPeriod;
  std::string                   m_option_qseq_measureRunTimes;
  std::string                   m_option_qseq_dataOutputPeriod;
  std::string                   m_option_qseq_dataOutputFileName;
  std::string                   m_option_qseq_dataOutputFileType;
  std::string                   m_option_qseq_dataOutputAllowedSet;
#ifdef QUESO_USES_SEQUENCE_STATISTICAL_OPTIONS
  std::string                   m_option_qseq_computeStats;
#endif
};

std::ostream& operator<<(std::ostream& os, const uqMonteCarloSGOptionsClass& obj);
#endif // __UQ_MOC_SG_OPTIONS_H__
