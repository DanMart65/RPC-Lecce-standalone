#include "his/HisFile.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

//HisFile* HisFile::m_HisFile = 0;
HisFile* HisFile::t_HisFile = 0;

/*
HisFile* HisFile::getHisFile() 
{  
    if(!m_HisFile) {
        m_HisFile = new HisFile("calib.hbook");
    }
    return m_HisFile; 
} 
*/

HisFile* HisFile::getHisFile(int Toption) 
{  
    if(!t_HisFile) {
        t_HisFile = new HisFile(Toption, "TreeFile.root");
    }
    return t_HisFile; 
} 

HisFile* HisFile::getHisFile(int Toption, std::string filename) 
{  
    if(!t_HisFile) {
        t_HisFile = new HisFile(Toption, filename);
    }
    return t_HisFile; 
} 

/*
HisFile* HisFile::getHisFile(std::string filename) 
{  
    if(!m_HisFile) {
        m_HisFile = new HisFile(filename);
    }
    return m_HisFile; 
} 
*/

/*
HisFile::HisFile(std::string filename)
{ 

    if(m_HisFile) abort(); 

    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();   

    HLIMIT(PAWC_SIZE);
    
    int recordSize=1024;
    int istat=0;
    unsigned long int logUnit=10;
    char *hFileName={(char *)filename.c_str()};
    char hDirName[15]={"test"};
    char opt[5]={"N"};

    HROPEN(logUnit,hDirName,hFileName,opt,recordSize,istat);
    std::cout << "HisFile: opened histogram file " << filename.c_str()<< std::endl;
}
*/

HisFile::HisFile(int Toption, std::string filename)
{ 
    if(t_HisFile) abort(); 

    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();   

    t_TreeFileRoot = new TFile(filename.c_str(),"RECREATE");
    if ( t_TreeFileRoot->IsOpen() ) std::cout << "RootFile: opened successfully" << filename.c_str() << "\n" << std::endl;
    else std::cout << "File not created\n" << std::endl;
}

HisFile::~HisFile()
{  
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->decrease();
    t_HisFile->GetRootFile()->Close();                                                                            
}

/*
void HisFile::His1D(int id,char *title,int nbix,double lx,double hx) 
{
      m_histoMap[id]=new Histo(id,title,nbix,lx,hx);
}


void HisFile::His2D(int id,char *title,int nbix,double lx,double hx,
                                     int nbiy,double ly,double hy) 
{
    m_histoMap[id]=new Histo(id,title,nbix,lx,hx,nbiy,ly,hy);
}

void HisFile::HisProf(int id,char *title,int nbix,double lx,double hx,
                                         double ly,double hy) 
{
    m_histoMap[id]=new Histo(id,title,nbix,lx,hx,ly,hy);
}

void HisFile::HisDel(int id) 
{
// check this !!! this delete the objects, should delete also map element .... !!! ???? 
    if(m_histoMap[id])delete m_histoMap[id];
}

Histo * HisFile::his(int id)
{  
    if(m_histoMap[id])return m_histoMap[id];
    else {
      std::cout << "HisFile ERROR: histogram " << id <<" not found" << std::endl;
      return (Histo *)0x0;
    }
}


void HisFile::updateFile()
{
    int recordSize=1024;
    int istat=0;
    unsigned long int logUnit=10;
    char hFileName[20]={"calib.hbook"};
    char hDirName[15]={"test"};
    char opt[5]={"N"};
    char uopt[5]={"U"};
    static int icycle=0;

    HREND(hDirName);
    HROPEN(logUnit,hDirName,hFileName,uopt,recordSize,istat);

    HROUT(0,icycle,uopt);
}
void HisFile::updateFile(std::string filename)
{
    int recordSize=1024;
    int istat=0;
    unsigned long int logUnit=10;
    char *hFileName={(char *)filename.c_str()};
    char hDirName[15]={"test"};
    char opt[5]={"N"};
    char uopt[5]={"U"};
    static int icycle=0;

    HREND(hDirName);
    HROPEN(logUnit,hDirName,hFileName,uopt,recordSize,istat);

    HROUT(0,icycle,uopt);
}

*/

void HisFile::createTree(std::string treename, int m_debug) 
{   
    
    TTree *mytree = (TTree*)t_TreeFileRoot->Get(treename.c_str());

    if(!mytree) 
    {
        if (m_debug>0) std::cout << "\nCreo il tree: " << treename.c_str() << std::endl;
        mytree = new TTree(treename.c_str(), treename.c_str());
        vectorTree.push_back(mytree);
    }

}

void HisFile::createTree(std::string treename, std::string treeTitle, int m_debug) 
{   
    
    TTree *mytree = (TTree*)t_TreeFileRoot->Get(treename.c_str());

    if(!mytree) 
    {
        if (m_debug>0) std::cout << "\nCreo il tree: " << treename.c_str() << std::endl;
        mytree = new TTree(treename.c_str(), treeTitle.c_str());
        vectorTree.push_back(mytree);
    }

}  

void HisFile::createBranch(std::string treename, double* branchValue, std::string branchname, int m_debug) 
{   
    
    TTree *mytree = (TTree*)t_TreeFileRoot->Get(treename.c_str());
    std::string branchTitle = branchname + "/D";

    if(!mytree->GetBranch(branchname.c_str()))
    {   
        if (m_debug>0) std::cout << "Creo nuovo branch \"" << branchname.c_str() << "\" nel tree \"" << treename.c_str() << "\"\n" << std::endl;
        mytree->Branch(branchname.c_str(), branchValue, branchTitle.c_str());
    }
} 

void HisFile::createBranch(std::string treename, int* branchValue, std::string branchname, int m_debug) 
{   
    
    TTree *mytree = (TTree*)t_TreeFileRoot->Get(treename.c_str());
    std::string branchTitle = branchname + "/I";

    if(!mytree->GetBranch(branchname.c_str()))
    {   
        if (m_debug>0) std::cout << "Creo nuovo branch \"" << branchname.c_str() << "\" nel tree \"" << treename.c_str() << "\"\n" << std::endl;
        mytree->Branch(branchname.c_str(), branchValue, branchTitle.c_str());
    }
} 

void HisFile::createBranch(std::string treename, float* branchValue, std::string branchname, int m_debug) 
{   
    
    TTree *mytree = (TTree*)t_TreeFileRoot->Get(treename.c_str());
    std::string branchTitle = branchname + "/F";

    if(!mytree->GetBranch(branchname.c_str()))
    {   
        if (m_debug>0) std::cout << "Creo nuovo branch \"" << branchname.c_str() << "\" nel tree \"" << treename.c_str() << "\"\n" << std::endl;
        mytree->Branch(branchname.c_str(), branchValue, branchTitle.c_str());
    }
} 

void HisFile::fillTree(std::string treename)
{
    TTree *filltree = (TTree*)t_TreeFileRoot->Get(treename.c_str());
    filltree->Fill();   
}
