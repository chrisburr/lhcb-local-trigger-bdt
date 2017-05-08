#pragma once

#include "TTreeReaderValue.h"
#include <fstream>
#include <map>
#include <string>
#include <vector>

typedef std::map<std::string, TTreeReaderValue<Double_t> *> BDTVars;

class BBDecTreeTool {

public:
  virtual Double_t operator()(const BDTVars &vals) const;
  BBDecTreeTool(const std::string &paramFile);

  virtual ~BBDecTreeTool(){};

protected:
private:
  /// assignemet operator is disabled
  BBDecTreeTool &operator=(const BBDecTreeTool &);

  /// utility method to obtain index to m_values
  int getIndex(const BDTVars &vals) const;

  /// utility method to obtain split index for single variable
  int getVarIndex(int varIndex, TTreeReaderValue<Double_t> *value) const;

  // properties
  std::string m_paramFile; ///< parameter file (full path)

  // attributes
  int m_ntrees; ///< number of trees used in training
  std::vector<std::string> m_var_names;
  std::vector<std::vector<double>> m_splits; ///< variable split points
  std::vector<unsigned short int> m_values;  ///< response values
};
