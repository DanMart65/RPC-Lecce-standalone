/**
 * \class RPCTrack 
 * class to store hits and track parameters  
 *
 * Class track contains 
 * 
 *   - mode of creation
 * 
 *   - quality of track
 * 
 *   - fitted parameters (both projections) 
 * 
 *   - list of hits used to create it
 *
 * \author E.Gorini, M. Primavera
 * \date 26.07.00
 */

#ifndef RPCTRACK_H
#define RPCTRACK_H

#include <vector>
#include "tools/Utils.h"
#include "his/HisFile.h" 
#include "detector/RPCCluster.h"

class RPCHit;

class RPCTrack {

 public:
 
    /**
     * the constructors 
     */
    RPCTrack(std::vector<RPCCluster *> hitList);
    RPCTrack(int score, std::vector<RPCCluster *> hitList);
    RPCTrack(std::vector<RPCCluster *> hitList, double a[2], double b[2]);
    /**
     * deep copy contructor 
     */
    RPCTrack(RPCTrack const & tk1);

    /**
     * the destructor
     */ 
    ~RPCTrack();

    void fitTrack();   /**< track fit in YZ proj */
    void fitTrack(int dir);   /**< track fit in proj "dir" */
    double computeResidual(double x, double y, 
                           double a, double b); /**< residual */ 
    double estrapPlane(int dir,double yPlane,double &sigestrap); /* estrapolates track */
    std::vector<RPCCluster *> RemoveWorstHits(float factor);		   
    bool RemoveWorstHit(float factor);
    void print();                     /**< print out track information */

    int nHits();                      /**< return the number of used hits */
    int nHits(int dir);                      /**< return the number of used hits */
    
//    void dump(int patrectype);        /**< dump track info for display */

    void fill(int baseid);               /**< fill Track histos */
    void fill(int baseid, int idsp);               /**< fill Track histos */

    /*
     * Access functions
     */
    std::vector<RPCCluster *> hitList() const;
    void linefit(std::vector<RPCCluster *> hList, int flag, double *bb, double *aa,
                                 double *sigbb, double *sigaa, double *chi2);
    double aZ() const;                         
    double aZErr() const;
    double bZ() const;
    double bZErr() const;
    double abZCorr() const;
    double aX() const;
    double aXErr() const;
    double bX() const;
    double bXErr() const;
    double abXCorr() const;
    double chisquareZ() const;
    double chisquareX() const;
    int score() const;
    bool isPresent(int Ch);

 private:

    std::vector<RPCCluster *> m_hitList; /**< the list of input hits */ 
    double m_aZ;                /**< slope in YZ projection, from fit */
    double m_aZErr;          /**< slope error in YZ projection, from fit */
    double m_bZ;                /**< offset in YZ projection, from fit */
    double m_bZErr;          /**< offset error in YZ projection, from fit */
    double m_abZCorr;        /**< correlation between slope and offset (YZ)*/
    double m_aX;                /**< slope in YX projection, from fit */
    double m_aXErr;          /**< slope error in YX projection, from fit */
    double m_bX;                /**< offset in YX projection, from fit */
    double m_bXErr;          /**< offset error in YX projection, from fit */
    double m_abXCorr;        /**< correlation between slope and offset (YX)*/
    double m_chisquareZ;        /**< chisquared of the fit in YZ projection */
    double m_chisquareX;        /**< chisquared of the fit in YX projection */
    int    m_score;             /**< quality flag from Pattern Recognition */
};
#endif //RPCTRACK_H
