/**
 * \class HisFile
 * a singleton managing the output histogram file and the histograms list
 *
 * Histos should be created and deleted only through the methods provided by 
 * this class which keeps a list of all the active histograms. 
 * It depends from the choice of the histogramming package only through
 * the pawc_ memory allocation and the file handling.
 *
 * The main program should include this header file and declare the global
 * pawc_ common block
 *
 * float pawc_[PAWC_SIZE];
 *
 * \author D.Orestano
 * \date 20.03.00
 */ 

#ifndef HISFILE_H
#define HISFILE_H

#include <vector>
#include <map>
#include <string>

//#define GNU            /**< cfortran expects a platform definition ... */
//#define PAWC_SIZE 50000000   /**< the size of the memory allocated for HBOOK */

#include "tools/Utils.h"
#include "his/Histo.h"
#include "TFile.h"
#include "TTree.h"

class TTree;

class TFile;

class HisFile: public TFile {

   public:

      /** 
       * static method returning the singleton pointer to HisFile 
       */
//      static HisFile* getHisFile();    
//      static HisFile* getHisFile( std::string filename); 
      static HisFile* getHisFile(int Toption);
      static HisFile* getHisFile(int Toption, std::string filename); 
      /**  
       * destructor: outputs the histograms and closes the hbook file  
       */ 
      ~HisFile();

      /**
       * constructor of a 1d histo 
       */
//      void His1D(int id,char *title,int nbix,double lx,double hx);

      /**
       * constructor of a 2d histo 
       */
//      void His2D(int id,char *title,int nbix,double lx,double hx,
//                                          int nbiy,double ly,double hy);

      /**
       * constructor of a profile histo 
       */
//      void HisProf(int id,char *title,int nbix,double lx,double hx,
//                                               double ly,double hy);
      /**
       * histogram destructor: 
       * deletes the histogram and removes it from the list 
       */
//    void HisDel(int id); 
    
      /**
       * access to an histogram through the identifier
       */
//      Histo * his(int id); 
    
      /**
       * update histogram file
       */
//      void updateFile();
//      void updateFile(std::string filename);


    TFile* GetRootFile() {return t_TreeFileRoot;}

    void createTree(std::string treename, int m_debug);

    void createTree(std::string treename, std::string treeTitle, int m_debug);

    void createBranch(std::string treename, double* branch, std::string branchname, int m_debug);

    void createBranch(std::string treename, int* branch, std::string branchname, int m_debug);

    void createBranch(std::string treename, float* branch, std::string branchname, int m_debug);

    TTree* UseTree(int id) {return vectorTree[id];}

    std::vector<TTree*> GetVectorTree() {return vectorTree;} 
    
    void fillTree(std::string treename);
  

   protected:
      /**
       *  constructor: opens calib.hbook on unit 10 
       */
  //    HisFile(std::string filename); 
      HisFile(int Toption, std::string filename);


   private:

      /**
       * the singleton pointer
       */
  //    static HisFile* m_HisFile; 
      static HisFile* t_HisFile; 

      /**
       * the list of histograms
       */
      std::map<int,Histo*, std::less<int> > m_histoMap;

      TFile* t_TreeFileRoot;
      std::vector<TTree*> vectorTree;
};
#endif
