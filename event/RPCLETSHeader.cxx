#include "RPCLETSHeader.h"
#include "tools/Utils.h"
#include <iomanip>
#include "stdlib.h"

RPCLETSHeader::RPCLETSHeader(std::vector<std::ifstream*>::iterator file){
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();  
    m_dat=file;
    (*m_dat)->seekg(0,std::ios::beg);
    m_HVScan=false;
    m_ThScan=false;
    swap=false;
// read RPCLETSHeader of RPCLETSHeader and get number of ascii strings to read
    int nlines=readinizial();
    for (int l=0;l<nlines;l++) read(l+1); 
 
    covDCSstring();    
 
    read2();   
   
// patches
//PATCH 
	if(m_runNumber >=3489 && m_runNumber <=3582)m_ThScan=true;
        if(m_runNumber >=3451 && m_runNumber <=3456)m_ThScan=true;
    
  
}

RPCLETSHeader::~RPCLETSHeader(){
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->decrease(); 
}


void RPCLETSHeader::print() const{
//  std::cout<<"Ascii RPCLETSHeader part \n";
//  for (int l=0;l<m_desclines.size();l++)
//    std::cout << l << " " << m_desclines[l]<<"\t"<<m_textlines[l];


  std::cout <<  "\nRPCLETSHeader::\n------------------------- run Header start -----------------------\n";
  std::cout<<"Run Number: " <<m_runNumber<<", ";
  std::cout<<"Started: " <<StartRunDay()<<"/"<<StartRunMounth()<<"/"<<StartRunYear()<<"-"
                           <<StartRunHours()<<":"<<StartRunMiniuts()<<":"<<StartRunSeconds()<<"\n";
  std::cout<<"Run Duration: " <<RunDuration()<<" seconds, ";
  std::cout<<"Events Requested: " <<EvenSet()<<", Events Taken: "<<EvenTaken()<<"\n";
  std::cout<<"Trigger Type: " <<m_TriggerType<<", ";
  std::cout<<"TestGroup: " <<TestGroup();
  std::cout<<", HVScan Flag: " <<HVScan()<<", ";
  std::cout<<"THScan Flag: " <<ThScan()<<"\n";
  std::cout<<"Number of DCS Blocks: " <<RunDuration()/DCSPeriod()+1<<"\n";
  std::cout<<"Eltx Configuration File: " << PathConf();
  std::cout<<"\nMounted RPC's: " <<NumRPCmount()<<", their position/name is:\n";
  for(int i=0;i<NumRPCmount();i++){ 
    std::cout<<"("<<PosRPCmount(i+1)<<", "<<NameRPCmount(i+1)<<"), ";
	if((i+1)%4==0)std::cout << "\n";
  }
  std::cout << "\n";
  std::cout<<"DCS monitored RPC's: " <<NumRPCinDCS()<<", their position/name is:\n";
  for(int i=0;i<NumRPCinDCS();i++){
    std::cout<<"("<<PosRPCinDCS(i+1)<<", "<<NameRPCinDCS(i+1)<<"), ";
    if((i+1)%4==0)std::cout << "\n";
  }
  std::cout << "\n";
  std::cout << "------------------------- run Header end -------------------------\n\n" ;

}

unsigned long int RPCLETSHeader::RPCLETSHeaderSize() const {
  return m_hsizeTot;
}

unsigned long int RPCLETSHeader::RPCLETSHeaderSizeASCII() const {
  return m_hsizeascii;
}

std::string RPCLETSHeader::DescLine(unsigned long int line) const{
  if (line>m_desclines.size()){
    std::cout<<"ERROR. Max number accepted is "<<m_desclines.size()<<"\n";
    return "";
  }
  return m_desclines[line];
}

std::string RPCLETSHeader::ReadLine(unsigned long int line) const{
  if (line>m_textlines.size()){
    std::cout<<"ERROR. Max number accepted is "<<m_textlines.size()<<"\n";
    return "";
  }
  return m_textlines[line];
}

std::string RPCLETSHeader::PathConf() const{
  int find=0;
  while (m_desclines[find]!="Configuration file    ") {
    find++;
    if(find>m_desclines.size()) {
      std::cout<<"Line not found, confirm that description for this line is :\"Configuration file    \"\n, if it doesn't correspond insert the new string description as argument of the function.";
      return "";
    }
  }
  return m_textlines[find];
}

std::string RPCLETSHeader::PathConf(std::string conf) const{
  int find=0;
  while (m_desclines[find]!=conf) {
    find++;
    if(find>m_desclines.size()) {
      std::cout<<"Line not found, confirm that description for this line is : "<<conf<<"\n, if it doesn't correspond insert the new string description as argument of the function.";
      return "";
    }
  }
  return m_textlines[find];
}

unsigned long int RPCLETSHeader::invertbytes (unsigned long int dat){
  if(swap) return dat;
unsigned long int temp[4]={0,0,0,0};
unsigned long int out=0;
unsigned long int nbyte[4]={0x000000ff,0x0000ff00,0x00ff0000,0xff000000};
int i;
 for(i=0; i<4; i++) { 
  if(i==0) temp[3-i] = (dat & nbyte[i])<<24;
  if(i==1) temp[3-i] = (dat & nbyte[i])<< 8;
  if(i==2) temp[3-i] = (dat & nbyte[i])>> 8;
  if(i==3) temp[3-i] = (dat & nbyte[i])>>24; 
 }
 for(i=0;i<4;i++) {out=out | temp[i];}
 return out;
}

bool RPCLETSHeader::SwapStatus() const {
  return swap;
}

unsigned long int RPCLETSHeader::StartRunDay() const{
  return m_day;
}

unsigned long int RPCLETSHeader::StartRunMounth() const{
  return m_mounth;
}

unsigned long int RPCLETSHeader::StartRunYear() const{
  return m_year;
}

unsigned long int RPCLETSHeader::StartRunHours() const{
  return m_hours;
}

unsigned long int RPCLETSHeader::StartRunMiniuts() const{
  return m_miniuts;
}

unsigned long int RPCLETSHeader::StartRunSeconds() const{
  return m_seconds;
}

unsigned long int RPCLETSHeader::RunDuration() const{
  return m_RunDuration;
}


unsigned long int RPCLETSHeader::EvenSet() const{
  return m_EvenSet;
}

unsigned long int RPCLETSHeader::EvenTaken() const{
  return m_EvenTaken;
}

unsigned long int RPCLETSHeader::DCSPeriod() const{
  return m_DCSPeriod;
}

unsigned long int RPCLETSHeader::NumRPCmount() const{
  return m_NumRPCmount;
}

unsigned long int RPCLETSHeader::NumRPCinDCS() const{
  return m_NumRPCinDCS;
}

unsigned long int RPCLETSHeader::TestGroup() const{
  return m_TestGroup;
}

unsigned long int RPCLETSHeader::FirstTimeBin() const{
  return m_FirstTimeBin;
}

unsigned long int RPCLETSHeader::TimeBins() const{
  return m_TimeBins;
}

unsigned long int RPCLETSHeader::PosRPCmount(int element) const{
  if(element>m_NumRPCmount){
    std::cout<<"ERROR. Max number accepted is "<<m_NumRPCmount<<"\n";
    return 0;
  }
  return m_PosRPCmount[element-1];
}

std::string RPCLETSHeader::NameRPCmount(int element) const{
  if(element>m_NumRPCmount){
    std::cout<<"ERROR. Max number accepted is "<<m_NumRPCmount<<"\n";
    return "";
  }
  return m_NameRPCmount[element-1];
}

bool RPCLETSHeader::IsTriggerChamber(int ichamb){  	   
  
   for(int i=0;i<m_NumRPCmount;i++){
     if(m_PosRPCmount[i]==ichamb && m_NameRPCmount[i].substr(7,7)=="T")return true;
    // if(m_PosRPCmount[i]==ichamb && m_NameRPCmount[i].substr(7,7)=="T")return false;
   }   
   return false;
  //return true;
 
  /*
  if(ichamb==1){return false;}  
  else if(ichamb==2){return false;}   
  else if(ichamb==11){return false;} 
  else if(ichamb==12){return false;}
  else {return true;} */
}


std::string RPCLETSHeader::ChamberName(int index) const{
  for(int i=0;i<NumRPCmount();i++)if(PosRPCmount(i+1)==index)return NameRPCmount(i+1);
  return "";
}

std::string RPCLETSHeader::DCSChamberName(int index) const{
  for(int i=0;i<m_NumRPCinDCS;i++)if(PosRPCinDCS(i+1)==index)return NameRPCinDCS(i+1);
  return "";
}

unsigned long int RPCLETSHeader::PosRPCinDCS(int element) const{
  if(element>m_NumRPCinDCS){
    std::cout<<"ERROR. Max number accepted is "<<m_NumRPCinDCS<<"\n";
    return 0;
  }
  return m_PosRPCinDCS[element-1];
}

std::string RPCLETSHeader::NameRPCinDCS(int element) const{
  if(element>m_NumRPCinDCS){
    std::cout<<"ERROR. Max number accepted is "<<m_NumRPCinDCS<<"\n";
    return "";
  }
  return m_NameRPCinDCS[element-1];
}

bool RPCLETSHeader::HVScan() const{
  return m_HVScan;
}

bool RPCLETSHeader::ThScan() const{
  return m_ThScan;
}

int RPCLETSHeader::readinizial (){
  
  //(*m_dat)->read((char *) &m_maccode, sizeof m_maccode);
  (*m_dat)->read((char *) &m_maccode, 4);//gabriele
//  char a;
//  cin>>a;
  if(1234==m_maccode) swap=true;
  m_maccode=invertbytes(m_maccode);
  std::cout<<swap<<"\n";
  (*m_dat)->read((char *) &m_hsizeTot, 4);
  m_hsizeTot=invertbytes(m_hsizeTot);
  (*m_dat)->read((char *) &m_hsizeascii, 4);
  m_hsizeascii=invertbytes(m_hsizeascii);
  unsigned long int hlines;
  (*m_dat)->read((char *) &hlines, 4);
  hlines=invertbytes(hlines);
  std::cout<<m_maccode<<" maccode\n";
  std::cout<<m_hsizeTot<<" sizeTot\n";
  std::cout<<m_hsizeascii<<" sizeascii\n";
  std::cout<<hlines<<" lines\n";
   
return hlines;
}

void RPCLETSHeader::read (unsigned long int line){
  int i,skipline;
  (*m_dat)->seekg(16,std::ios::beg);
  unsigned long int dimline;
  for (skipline=0;skipline<line-1;skipline++){
    (*m_dat)->read((char *) &dimline,4); 
    dimline=invertbytes(dimline);
    (*m_dat)->seekg(dimline,std::ios::cur);
  }
  
  (*m_dat)->read((char *) &dimline, 4);
  dimline=invertbytes(dimline);
  char desc[22],text[dimline-23];
  (*m_dat)->read((char *) desc ,22 );
  std::string dline;
  for(int h=0;h<22;h++){
    dline+=desc[h];
  }
  m_desclines.push_back(dline);
  (*m_dat)->seekg(1,std::ios::cur);
// questo commentato se ci sono i seekg
//  (*m_dat)->read((char *) text ,1); 
  (*m_dat)->read((char *) text ,dimline-23 );
  std::string tline;
  for(int h=0;h<dimline-23;h++){
    tline+=text[h];
  }

  m_textlines.push_back(tline);
return ;
}

void RPCLETSHeader::covDCSstring(){
  int find=0;
  while (m_desclines[find]!="Number of detectors   ") {
    find++;
    if(find>m_desclines.size()) {
      std::cout<<"Line not found, confirm that description for this line is : \"Number of detectors   \"\n, if it doesn't correspond insert the new string description as argument of the function.";
      return ;
    }
  }
  m_NumRPCinDCS=atoi(m_textlines[find].c_str());
  
  while (m_desclines[find]!="Description of detect.") {
    find++;
    if(find>m_desclines.size()) {
      std::cout<<"Line not found, confirm that description for this line is : \"Description of detect.\"\n, if it doesn't correspond insert the new string description as argument of the function.";
      return ;
    }
  }
  //  int numdcs;
  for (int numdcs=0;numdcs<m_NumRPCinDCS;numdcs++) {
    //char posrpcdcs;
    //strxfrm(&posrpcdcs,(m_textlines[find].c_str())+(numdcs-1)*7,1);
    
    m_PosRPCinDCS.push_back(atoi((m_textlines[find].substr((numdcs)*11,3)).c_str()));
 
   
    if((numdcs==9)&&(m_textlines[find].substr(3+(9)*11,8)=="BOSB049M")&&(m_textlines[find].substr(3+(8)*11,8)=="BOSB050M")){
     m_NameRPCinDCS.push_back("BOSB048M");
    }
    else{
     m_NameRPCinDCS.push_back(m_textlines[find].substr(3+(numdcs)*11,8));
    }
  }
  while (m_desclines[find]!="Scan procedure        ") {
    find++;
    if(find>m_desclines.size()) {
      std::cout<<"Line not found, confirm that description for this line is : \"Description of detect.\"\n, if it doesn't correspond insert the new string description as argument of the function.";
      return ;
    }
  }
  if (m_textlines[find].substr(0,2)=="HV")m_HVScan=true;  
  if (m_textlines[find].substr(0,2)=="Th")m_ThScan=true;
  if (m_textlines[find].substr(0,2)=="RA"){
    m_ThScan=true;
    m_HVScan=true;
  }
  return;
}

void RPCLETSHeader::read2() {
//  (*m_dat)->seekg(8+m_hsizeascii,std::ios::beg);
  (*m_dat)->read((char *) &m_runNumber,4);
  m_runNumber=invertbytes(m_runNumber);
  (*m_dat)->read((char *) &m_day,4);
  m_day=invertbytes(m_day);
  (*m_dat)->read((char *) &m_mounth,4);
  m_mounth=invertbytes(m_mounth);
  (*m_dat)->read((char *) &m_year,4);
  m_year=invertbytes(m_year);
  (*m_dat)->read((char *) &m_hours,4);
  m_hours=invertbytes(m_hours);
  (*m_dat)->read((char *) &m_miniuts,4);
  m_miniuts=invertbytes(m_miniuts);
  (*m_dat)->read((char *) &m_seconds,4);
  m_seconds=invertbytes(m_seconds);
  (*m_dat)->read((char *) &m_RunDuration,4);
  m_RunDuration=invertbytes(m_RunDuration);
  (*m_dat)->read((char *) &m_TriggerType,4);

  m_TriggerType=invertbytes(m_TriggerType);
  m_TriggerType=11;

  (*m_dat)->read((char *) &m_EvenSet,4);
  m_EvenSet=invertbytes(m_EvenSet);
  (*m_dat)->read((char *) &m_EvenTaken,4);
  m_EvenTaken=invertbytes(m_EvenTaken);
  m_NumEvents=m_EvenTaken;
  (*m_dat)->read((char *) &m_DCSPeriod,4);
  m_DCSPeriod=invertbytes(m_DCSPeriod);
  (*m_dat)->read((char *) &m_TestGroup,4);
  m_TestGroup=invertbytes(m_TestGroup);
  (*m_dat)->read((char *) &m_NumRPCmount,4);
  m_NumRPCmount=invertbytes(m_NumRPCmount);
  
  std::string rpcNAME;
  for(int numRPC=0;numRPC<m_NumRPCmount;numRPC++){
    unsigned long int pos;
    (*m_dat)->read((char *) &pos,4);
    pos=invertbytes(pos);
    m_PosRPCmount.push_back(pos);
    char RPCname[8];
    (*m_dat)->read((char *) RPCname,8);
    std::string rpcname; 
   
    for (int nameRPC=0;nameRPC<8;nameRPC++) {
    // last character is M for Measure, T for Trigger      
      rpcname+=RPCname[nameRPC];
    }
    if(numRPC==8)rpcNAME=rpcname;
    if((numRPC==9)&&(rpcname=="BOSB049M")&&(rpcNAME=="BOSB050M")){
     m_NameRPCmount.push_back("BOSB048M");
    }
    else{
     m_NameRPCmount.push_back(rpcname);
    }
   
    
    
  }
  
  m_FirstTimeBin=0;
  m_TimeBins=31;
  unsigned long int ff;
  (*m_dat)->read((char *) &ff,4);
  //m_FirstTimeBin=invertbytes(ff);
  (*m_dat)->read((char *) &ff,4);
  //m_TimeBins=invertbytes(ff); 
   
  
  std::cout << "m_FirstTimeBin   m_TimeBins "  << m_FirstTimeBin<< " " << m_TimeBins<< std::endl;
  
   
  return;
}
