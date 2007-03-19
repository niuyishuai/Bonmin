// (C) Copyright International Business Machines (IBM) 2006
// All Rights Reserved.
// This code is published under the Common Public License.
//
// Authors :
// Pierre Bonami, IBM
//
// Date : 26/09/2006


#ifndef TNLPSolver_H
#define TNLPSolver_H
#include "IpTNLP.hpp"
#include "BonTMINLP2TNLP.hpp"

//Some declarations
#include "IpOptionsList.hpp"
#include "IpRegOptions.hpp"
#include "CoinWarmStart.hpp"

namespace Bonmin  {
/** This is a generic class for calling an NLP solver to solve a TNLP.
    A TNLPSolver is able to solve and resolve a problem, it has some options (stored
    with Ipopt OptionList structure and registeredOptions) it produces some statistics (in SolveStatisctics and sometimes some errorCodes.
*/
class TNLPSolver: public Ipopt::ReferencedObject{
 public:

  enum ReturnStatus /** Standard return statuses for a solver*/{
    iterationLimit = -3/** Solver reached iteration limit. */,
    computationError = -2/** Some error was made in the computations. */,
    notEnoughFreedom = -1/** not enough degrees of freedom.*/,
    illDefinedProblem = -4/** The solver finds that the problem is not well defined. */,
    illegalOption =-5/** An option is not valid. */,
    externalException =-6/** Some unrecovered exception occured in an external tool used by the solver. */,
    exception =-7/** Some unrocevered exception */,
    solvedOptimal = 1/** Problem solved to an optimal solution.*/,
    solvedOptimalTol =2/** Problem solved to "acceptable level of tolerance. */,
    provenInfeasible =3/** Infeasibility Proven. */,
    unbounded = 4/** Problem is unbounded.*/
  };



//#############################################################################

  /** We will throw this error when a problem is not solved.
      Eventually store the error code from solver*/
  class UnsolvedError
  {
  public:
    /** Constructor */
    UnsolvedError(int errorNum = -10000, 
                  Ipopt::SmartPtr<TMINLP2TNLP> model = NULL,
                  std::string name="")
    :
     errorNum_(errorNum),
     model_(model),
     name_(name)
    {if(name_=="") 
{
	std::cout<<"FIXME"<<std::endl;
}}
    /** Print error message.*/
    void printError(std::ostream & os);
    /** Get the string corresponding to error.*/
    virtual const std::string& errorName() const = 0;
    /** Return the name of the solver. */
    virtual const std::string& solverName() const = 0;
    /** Return error number. */
    int errorNum() const{
    return errorNum_;}
    /** destructor. */
    virtual ~UnsolvedError(){}
    /** write files with differences between input model and
        this one */
    void writeDiffFiles(const std::string prefix=std::string()) const;
  private:
    /** Error code (solver dependent). */
    int errorNum_;

    /** model_ on which error occured*/
    Ipopt::SmartPtr< TMINLP2TNLP > model_;

    /** name of the model on which error occured. */
    std::string name_;
  }
  ;

  virtual UnsolvedError * newUnsolvedError(int num,
					   Ipopt::SmartPtr<TMINLP2TNLP> problem,
					   std::string name) = 0;
 


  /// Constructor
   TNLPSolver();

  ///virtual copy constructor
  virtual Ipopt::SmartPtr<TNLPSolver> clone() = 0;

   /// Virtual destructor
   virtual ~TNLPSolver();

   /** Initialize the TNLPSolver (read options from params_file)
   */
   virtual void Initialize(std::string params_file) = 0;

   /** Initialize the TNLPSolver (read options from istream is)
   */
   virtual void Initialize(std::istream& is) = 0;

   /** @name Solve methods */
   //@{
   /// Solves a problem expresses as a TNLP 
   virtual ReturnStatus OptimizeTNLP(const Ipopt::SmartPtr<Ipopt::TNLP> & tnlp) = 0;

   /// Resolves a problem expresses as a TNLP 
   virtual ReturnStatus ReOptimizeTNLP(const Ipopt::SmartPtr<Ipopt::TNLP> & tnlp) = 0;

  /// Set the warm start in the solver
  virtual bool setWarmStart(const CoinWarmStart * warm, 
                            Ipopt::SmartPtr<TMINLP2TNLP> tnlp) = 0;

  /// Get the warm start form the solver
  virtual CoinWarmStart * getWarmStart(Ipopt::SmartPtr<TMINLP2TNLP> tnlp) const = 0;

  virtual CoinWarmStart * getEmptyWarmStart() const = 0;

  /// Enable the warm start options in the solver
  virtual void enableWarmStart() = 0;

  /// Disable the warm start options in the solver
  virtual void disableWarmStart() = 0;
   //@}

  ///Get a pointer to a journalist
  virtual Ipopt::SmartPtr<Ipopt::Journalist> Jnlst() = 0;

   ///Get a pointer to RegisteredOptions (generally used to add new ones)
   virtual Ipopt::SmartPtr<Ipopt::RegisteredOptions> RegOptions() = 0;

   /// Get the options (for getting their values).
   virtual Ipopt::SmartPtr<const Ipopt::OptionsList> Options() const = 0;

   /// Get the options (for getting and setting their values).
   virtual Ipopt::SmartPtr<Ipopt::OptionsList> Options() = 0;

   /// Register this solver options into passed roptions
   virtual void RegisterOptions(Ipopt::SmartPtr<Ipopt::RegisteredOptions> roptions) = 0;

   /// Get the CpuTime of the last optimization.
   virtual double CPUTime() = 0;

   /// Get the iteration count of the last optimization.
   virtual int IterationCount() = 0;


  /// turn off all output from the solver 
  virtual void turnOffOutput() = 0 ;
  /// turn on all output from the solver
  virtual void turnOnOutput() = 0;
  /// Get the solver name
  virtual std::string & solverName() = 0;

    /** Say if an optimization status for a problem which failed is recoverable
        (problem may be solvable).*/
  bool isRecoverable(ReturnStatus &r);

protected:
  bool zeroDimension(const Ipopt::SmartPtr<Ipopt::TNLP> &tnlp, 
		     ReturnStatus &optimization_status);
   private:
   /// There is no copy constructor for this class
   TNLPSolver(TNLPSolver &other); 
};
}
#endif

