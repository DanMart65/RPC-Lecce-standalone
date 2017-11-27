/**
 * \class Histo
 * histogram object
 * it actually relies on HBOOK through cfortran.
 * Histograms can be created and deleted only through the HisFile singleton,
 * while filling, fitting etc... are public methods of this class
 * \author D.Orestano
 * \date 20.03.00
 */

#ifndef HISTO_H
#define HISTO_H

//#define DECFortran

//#define PAWC_SIZE 5000000

//#include "cfortran.h"
//#include "hbook.h"

#include "tools/Utils.h"

class HisFile;

class Histo {

   public:

//      int id() const; /**< return the identifier */

//      int dim() const; /**< return the histogram dimension */

//      int nx() const; /**< return the number of x bins */

//      int ny() const; /**< return the number of y bins */

//      double xmin() const; /**< return the lower edge along x */

//      double xmax() const; /**< return the upper edge along x */

//      double ymin() const; /**< return the lower edge along y */

//      double ymax() const; /**< return the upper edge along y */

//      void fill(double x, double y, double weight); /**< histo filling */ 

      /**  
       * fitting with user function (uses HFITH) 
       * please note that
       * a) this method requires float vectors
       * b) the user must provide a float function of (float & x)
       * c) the user must define in its code the common block HCFITD
       */
//      void fit(float (* fun)(float & x), char * opt, int nPar, float * params,
//               float * step, float * pMin, float * pMax, float * sigma,
//               float & chi2);               

      /** 
       * fitting with predefined functions (Pn,E,G ...) (uses HFITH) 
       * please note that
       * this method requires float vectors
       */
//      void fit(char * fun, char * opt, int nPar, float * params,
//               float * step, float * pMin, float * pMax, float * sigma,
//               float & chi2);

      /**
       * fitting with user function (uses HFITL) 
       */
//      void fit(float (* fun)(float & x),int nPar,double * params,
//                   float & chi,int ic,float * sig, float * cov, 
//                   float * step, float * pMin, float * pMax); 
      
//      void fitg(float & norm,float & media, float & sigma, float & chi2g,  float * err);

//      void pak(float * vector); /** calls HPAK */ 

//      void pake(float * vector); /** call HPAKE */ 

//      void reset(); /**< reset the histogram contents */

//      int entries () const; /**< return the histogram entries */ 

//      double events() const; /**< return the histogram ALLCHAN contents */

//      double mean() const; /**< return the histogram mean value (1D only) */ 

//      double rms() const; /**< return the histogram rms (1D only) */ 

//      double binContent(int ix) const; /**< return the contents of bin number 
//                                            ix (1D only) */ 
//      double binContent(int ix,int iy) const; /**< return the contents of bin 
//                                            (ix,iy) (2D) */ 
//      double binContent(double x) const; /**< return the contents of bin at 
//                                              abscissa x (1D only) */ 
//      double binContent(double x,double y) const; /**< return the contents of 
//                                                       bin at (x,y) (2D) */ 
//      int chanNumber(double x) const; /**< return bin number for x (1D) */ 

   protected:

      friend class HisFile;

      /**
       * constructor of a 1d histo 
       */
//      Histo(int id,char *title,int nbix,double lx,double hx);

      /**
       * constructor of a 2d histo 
       */
//      Histo(int id,char *title,int nbix,double lx,double hx,
//                                     int nbiy,double ly,double hy);

      /**
       * constructor of a profile histo 
       */
//      Histo(int id,char *title,int nbix,double lx,double hx,
//                                        double ly,double hy);

      /**
       * destructor
       */
//      ~Histo();


    private:
      
//      int m_id;  /**< histogram identifier */
//      int m_dim; /**< histogram dimension */ 
//      int m_nx;  /**< number of bins along x */ 
//      int m_ny;  /**< number of bins along y */ 
//      double m_xmin; /**< lower edge along x */
//      double m_xmax; /**< upper edge along x */
//      double m_ymin; /**< lower edge along y */
//      double m_ymax; /**< upper edge along y */

};

#endif //HISTO_H
