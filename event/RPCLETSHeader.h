#ifndef RPCLETSHEADER_H
#define RPCLETSHEADER_H
/*
 * Author G.Tassielli, modified by E.Gorini for inclusion in lets
 *
 */
#include "event/Header.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class RPCLETSHeader : public Header {
 public:
  RPCLETSHeader(std::vector<std::ifstream*>::iterator file);
  ~RPCLETSHeader();
  void print() const;
  unsigned long int RPCLETSHeaderSize() const;
  unsigned long int RPCLETSHeaderSizeASCII() const;
  std::string DescLine(unsigned long int ) const;
  std::string ReadLine(unsigned long int ) const;
  std::string PathConf() const;
  std::string PathConf(std::string conf) const;
  unsigned long int invertbytes (unsigned long int );
  bool SwapStatus() const;
  unsigned long int StartRunDay() const;
  unsigned long int StartRunMounth() const;
  unsigned long int StartRunYear() const;
  unsigned long int StartRunHours() const;
  unsigned long int StartRunMiniuts() const;
  unsigned long int StartRunSeconds() const;
  unsigned long int RunDuration() const;
  unsigned long int EvenSet() const;
  unsigned long int EvenTaken() const;
  unsigned long int DCSPeriod() const;
  unsigned long int NumRPCmount() const;
  unsigned long int NumRPCinDCS() const;
  unsigned long int TestGroup() const;
  unsigned long int FirstTimeBin() const;
  unsigned long int TimeBins() const;
  unsigned long int PosRPCmount(int element) const;
  std::string NameRPCmount(int element) const;
  bool IsTriggerChamber(int ichamb);
  std::string ChamberName(int index) const;
  std::string DCSChamberName(int index) const;
  unsigned long int PosRPCinDCS(int element) const;
  std::string NameRPCinDCS(int element) const;
  bool HVScan() const;
  bool ThScan() const;
  
 private:

// variables
  std::vector<std::ifstream*>::iterator m_dat;
  unsigned long int m_maccode,m_hsizeTot,m_hsizeascii;
  unsigned long int m_day,m_mounth,m_year,m_hours,m_miniuts,m_seconds;
  unsigned long int m_RunDuration,m_EvenSet,m_EvenTaken,m_DCSPeriod;
  unsigned long int m_NumRPCmount,m_NumRPCinDCS,m_TestGroup;
  unsigned long int m_FirstTimeBin,m_TimeBins;
  bool m_HVScan,m_ThScan;
  bool swap;
  std::vector<std::string> m_desclines;
  std::vector<std::string> m_textlines;
  std::vector<std::string> m_NameRPCmount;
  std::vector<unsigned long int> m_PosRPCmount;
  std::vector<std::string> m_NameRPCinDCS;
  std::vector<unsigned long int> m_PosRPCinDCS;

// methods
  int readinizial ();
  void read (unsigned long int line);
  void covDCSstring();
  void read2();
 };
#endif // RPCLETSRPCLETSHEADER_H

