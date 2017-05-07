#include "../include/BBDecTreeTool.h"
#include <iostream>
#include <sstream>

BBDecTreeTool::BBDecTreeTool(const double threshold, const std::string &paramFile)
    : m_threshold(threshold), m_paramFile(paramFile) {
  std::ifstream inFile(m_paramFile.c_str());
  if (!inFile.is_open()) {
    std::cout << "Problem with file " << m_paramFile << ": Failed to open file"
              << std::endl;
    exit(1);
  }

  unsigned int nvar(0), index(0), value(0);
  double dvalue(0);
  inFile >> nvar; // number of variables
  m_var_names.resize(nvar);
  m_splits.resize(nvar);

  // variable names
  for (unsigned int v = 0; v < nvar; ++v) {
    if (!(inFile >> m_var_names[v])) {
      std::cout << "Problem with file " << m_paramFile
                << ": Error reading in variable names" << std::endl;
      exit(1);
    }
  }

  // number of splits for each variable
  unsigned int numSplits = 1;
  for (unsigned int v = 0; v < nvar; ++v) {
    if (!(inFile >> value)) {
      std::cout << "Problem with file " << m_paramFile
                << ": Error reading no. of splits" << std::endl;
      exit(1);
    }
    m_splits[v].resize(value);
    numSplits *= value;
  }

  // split values for each variable
  for (unsigned int v = 0; v < nvar; ++v) {
    const unsigned int size = m_splits[v].size();
    for (unsigned int s = 0; s < size; ++s) {
      if (!(inFile >> dvalue)) {
        std::cout << "Problem with file " << m_paramFile
                  << ": Error reading splits" << std::endl;
        exit(1);
      }
      m_splits[v][s] = dvalue;
    }
  }

  // number of trees
  if (!(inFile >> m_ntrees)) {
    std::cout << "Problem with file " << m_paramFile
              << ": Error reading no. of trees" << std::endl;
    exit(1);
  }

  // actual values
  m_values.resize(numSplits);
  while (inFile >> index >> value) {
    if (index > numSplits) {
      std::cout << "Problem with file " << m_paramFile
                << ": Error reading values" << std::endl;
      exit(1);
    }
    m_values[index] = (unsigned short int)value;
  }
  inFile.close();

  // print info
  std::cout << "Initialized w/ Threshold = " << m_threshold
            << ", ParamFile = " << m_paramFile << " ("
            << m_ntrees << " trees," << nvar << " vars," << numSplits
            << " splits)." << std::endl;
}

// ============================================================================
int BBDecTreeTool::getVarIndex(int varIndex, double value) const {
  if (value < m_splits[varIndex][0])
    return 0;
  unsigned int size = m_splits[varIndex].size();
  for (unsigned int s = 1; s < size; s++) {
    if (value < m_splits[varIndex][s])
      return s - 1;
  }
  return size - 1;
}

// ============================================================================
int BBDecTreeTool::getIndex(const std::map<std::string, double> &vals) const {
  unsigned int size = m_splits.size();

  int ind_mult = 1, index = 0;
  for (int v = size - 1; v >= 0; v--) {
    if (v < (int)size - 1)
      ind_mult *= m_splits[v + 1].size();

    auto pval = vals.find(m_var_names[v]);
    if (pval == vals.end()) {
      std::cout << "Unable to find value for " + m_var_names[v] +
            " in dictionary of functor values, returning invalid index." << std::endl;
      index = -1;
      break;
    }

    index += this->getVarIndex(v, pval->second) * ind_mult;
  }
  return index;
}

// ============================================================================
bool BBDecTreeTool::
operator()(const std::map<std::string, double> &vals) const {
  // get response
  int index = this->getIndex(vals);
  if (index < 0 || index >= (int)m_values.size()) {
    std::cout << "BBDecTreeTool tool is misconfigured!" << std::endl;
    exit(1);
  }
  double response = m_values[index] / (double)m_ntrees;

  return response > m_threshold;
}
