#include "his/Histo.h"

/*
Histo::Histo(int id,char *title,int nbix,double lx,double hx)
{
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();
    m_id=id;
    m_dim=1;
    m_nx=nbix;
    m_ny=0;
    m_xmin=lx;
    m_xmax=hx;
    m_ymin=0.;
    m_ymax=0.;
    HBOOK1(id,title,nbix,(float)lx,(float)hx,(float)0.);
} 

Histo::Histo(int id,char *title,int nbix,double lx,double hx,
                                      int nbiy,double ly,double hy)
{
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();
    m_id=id;
    m_dim=2;
    m_nx=nbix;
    m_ny=nbiy;
    m_xmin=lx;
    m_xmax=hx;
    m_ymin=ly;
    m_ymax=hy;
    HBOOK2(id,title,nbix,(float)lx,(float)hx,nbiy,(float)ly,(float)hy,(float)0.);
} 

Histo::Histo(int id,char *title,int nbix,double lx,double hx,
                                         double ly,double hy)
{
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();
    m_id=id;
    m_dim=3;
    m_nx=nbix;
    m_xmin=lx;
    m_xmax=hx;
    m_ymin=ly;
    m_ymax=hy;
    char opt[5] = {" "};
    HBPROF(id,title,nbix,(float)lx,(float)hx,(float)ly,(float)hy,opt);
} 

Histo::~Histo()
{
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->decrease();
    HDELET(m_id);
} 

int Histo::id() const
{ 
    return m_id;
} 

int Histo::dim() const
{
    return m_dim;
}

int Histo::nx() const
{
    return m_nx;
}

int Histo::ny() const
{
    return m_ny;
}

double Histo::xmin() const
{
    return m_xmin;
}

double Histo::xmax() const
{
    return m_xmax;
}

double Histo::ymin() const
{
    return m_ymin;
}

double Histo::ymax() const
{
    return m_ymax;
}

void Histo::fill(double x, double y, double weight) 
{
    if(!this) return;
    if(m_dim == 1) { y = 0.; }
    HFILL(m_id,(float)x,(float)y,(float)weight);
}

void Histo::fit(float (* fun)(float & x), char * opt, int nPar, float * params,
                float * step, float * pMin, float * pMax, float * sigma, 
                float & chi2)
{
   if(m_dim == 2) { return; }
   HFITH(m_id,fun,opt,nPar,&params[0],&step[0],&pMin[0],&pMax[0],&sigma[0],
         (float &) chi2);
}

void Histo::fit(char * fun, char * opt, int nPar, float * params,
                float * step, float * pMin, float * pMax, float * sigma, 
                float & chi2)
{
   if(m_dim == 2) { return; }
   HFITHN(m_id,fun,opt,nPar,params[0],&step[0],&pMin[0],&pMax[0],sigma[0],
          (float &) chi2);
}

void Histo::fit(float (* fun)(float & x),int nPar,double * params,
                   float & chi,int ic,float * sig, float * cov, 
                   float * step, float * pMin, float * pMax)
{
   if(m_dim == 2) { return; }
   HFITL(m_id,fun,nPar,&params[0],(float &)chi,ic,sig[0],cov[0],
         &step[0],&pMin[0],&pMax[0]);
}

void Histo::fitg(float & norm,float & media, float & sigma, float & chi2g,  float * err)

{
//   if(m_dim == 2) { return; }
   HFITGA(m_id,norm,media,sigma, chi2g,0,&err[0]);

// if you want use fitg in the code you must write:
//histo->fitg((float &) norm,(float &) media,(float &) sigma,(float &) chi2g,&err[0]);
}
void Histo::pak(float * vector) 
{
   if(m_dim == 2) { return; }
   HPAK(m_id,&vector[0]);
}

void Histo::pake(float * vector) 
{
   if(m_dim == 2) { return; }
   HPAKE(m_id,&vector[0]);
}

void Histo::reset() 
{
   char opt[5] = {" "};
   HRESET(m_id,opt);
}

int Histo::entries() const
{
   int n;
   HNOENT(m_id,(int &)n);
   return n;
}

double Histo::events() const
{
   return HSUM(m_id);
}

double Histo::mean() const
{
   if(m_dim == 2) { return 0.; }
   int num=0;
   char * opt[] = {" "};
   int icase=1;
   return HSTATI(m_id,icase,opt,num);
}

double Histo::rms() const
{
   if(m_dim == 2) { return 0.; }
   int num=0;
   char * opt[] = {" "};
   int icase=2;
   return HSTATI(m_id,icase,opt,num);
}

double Histo::binContent(int ix) const
{
   if(m_dim == 1 || m_dim == 3) {
     return HI(m_id,ix);
   } else {
     return 0.;
   }
}

double Histo::binContent(int ix,int iy) const
{
   if(m_dim == 2) {
     return HIJ(m_id,ix,iy);
   } else {
     return HI(m_id,ix);
   }
}

double Histo::binContent(double x) const
{
   if(m_dim == 1 || m_dim == 3) {
     return HX(m_id,(float) x);
   } else {
     return 0.;
   }
}

double Histo::binContent(double x, double y) const
{
   if(m_dim == 2) {
      return HXY(m_id,(float) x,(float) y);
   } else {
     return HX(m_id,(float) x);
   }
}

int Histo::chanNumber(double x) const
{
   if(m_dim == 1 || m_dim == 3) {
     int i,j;
     float y=0;
     HXYIJ(m_id,(float)x,y,i,j);
     return i;
   } else {
     return 0; 
   }
}
*/