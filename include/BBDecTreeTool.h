#pragma once
// ============================================================================
// Include files
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "TTreeReaderValue.h"
// ============================================================================
/** @class BBDecTreeTool
 *  This tool applies a lookup table selection, such as the BBDT cuts for
 *  the HLT2 topological lines.
 *
 *  Its properties:
 *    - <c>"Threshold":</c>  the cut threshold.  Particles with a lookup
 *      table response greater than <c>Threshold</c> are accepted by the filter.
 *    - <c>"ParamFile":</c>  the pathname of the file that contains the lookup
 *      table definition.
 *    - <c>"ANNSvcKey":</c>  a key into the ExtraInfo block of accepted
 *      Particles where the response value is stored.
 *    - <c>"ParticleDictTool":</c>  name of the class to use for variable
 *      functor handling.  It must inherit from <c>IParticleDictTool</c>.
 *      The default value is <c>"LoKi::Hybrid::DictOfFunctors"</c>.
 *
 *  Conventions of the <c>ParamFile</c> property:
 *    - If the pathname begins with '.' or '/', the pathname is assumed to be
 *      relative to the current working directory or absolute, and the filename
 *      is not modified prior to opening.
 *    - If the pathname begins with '$', a substitution of a runtime
 *      environment variable is made prior to opening the file.
 *    - In all other cases, the designated pathname is assumed to be relative
 *      to the <c>"$PARAMFILESROOT/data/"</c> directory.
 *      <c>"$PARAMFILESROOT/data/"</c> is prepended to the pathname and an
 *      environment substitution for the <c>"$PARAMFILESROOT"</c> variable
 *      is made prior to opening the file.
 *
 *  The tool uses a private <c>LoKi::Hybrid::DictOfFunctors</c> for evaluating
 *  the values of the input variables.  It must be configured separately with
 *  a mapping of the variable names as they appear in <c>ParamFile</c> to
 *  the strings defining the corresponding LoKi functors for evaluating the
 *  variables.  The tool can be used with LoKi/Bender functors.  An example
 *  configuration in the context of a Hlt2 line follows:
 *  @code
 *      from Configurables import BBDecTreeTool
 *      from Configurables import LoKi__Hybrid__DictOfFunctors
 *      from Configurables import FilterDesktop
 *      from HltLine.HltLine import Hlt2Member
 *      from HltLine.HltLine import Hlt1Tool
 *
 *      varHandler = Hlt1Tool( type = LoKi__Hybrid__DictOfFunctors, name =
 * "VarHandler", Variables = {   "M"          :  "MM/MeV" , "DOCA"       :
 * "DOCAMAX_('',False)/mm" , "CANDIPCHI2" :
 * "BPVIPCHI2()" , "MCOR"       :  "BPVCORRM" ,
 * "FDCHI2"     :  "BPVVDCHI2" , "PT"         :
 * "PT" , "PTMIN"      :  "MINTREE(ISBASIC,PT)/MeV"
 *                              , "PTSUM"      :  "SUMTREE(PT,ISBASIC,0.0)/MeV"
 *                              }
 *
 *
 *      bdttool = Hlt1Tool(type=BBDecTreeTool,name='TrgBBDT',
 *                     Threshold=0.1,
 *                     ParamFile='Hlt2Topo2Body_BDTParams_v1r0.txt',
 *                     ParticleDictTool='LoKi::Hybrid::DictOfFunctors/'+varHandler.Name,
 *                     ANNSvcKey=6302,
 *                     tools=[varHandler])
 *
 *      cuts = "FILTER('BBDecTreeTool/TrgBBDT')"
 *      filter = Hlt2Member(FilterDesktop, 'FilterBDT', Inputs=...,
 *                          Code=cuts,tools=[bdttool])
 *  @endcode
 *
 *  @see LoKi::Cuts::FILTER
 *  @see IParticleFilter
 *  @see IParticleDictTool
 *  @see LoKi::Hybrid::DictOfFunctors
 *  @author Mike Williams (w/ help from Vanya of course!)
 *  @date 2011-01-19
 *
 *                    $Revision:$
 *  Last modification $Date:$
 *                 by $Author:$
 *
 */
class BBDecTreeTool {

public:
  virtual Double_t operator()(const std::map<std::string, TTreeReaderValue<Double_t>*> &vals) const;
  /** standard constructor
   *  @param type the actual tool type (?)
   *  @param name the tool instance name
   *  @param parent the tool parent
   */
  BBDecTreeTool(const double threshold, const std::string &paramFile);

  /// virtual & protected destructor
  virtual ~BBDecTreeTool(){};

protected:
private:
  /// assignemet operator is disabled
  BBDecTreeTool &operator=(const BBDecTreeTool &);

  /// utility method to obtain index to m_values
  int getIndex(const std::map<std::string, TTreeReaderValue<Double_t>*> &vals) const;

  /// utility method to obtain split index for single variable
  int getVarIndex(int varIndex, TTreeReaderValue<Double_t>* value) const;

  // properties
  double m_threshold;      ///< response threshold (cut) value
  std::string m_paramFile; ///< parameter file (full path)

  // attributes
  int m_ntrees; ///< number of trees used in training
  std::vector<std::string> m_var_names;
  std::vector<std::vector<double>> m_splits; ///< variable split points
  std::vector<unsigned short int> m_values;  ///< response values
};
