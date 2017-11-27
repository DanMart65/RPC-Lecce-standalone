int getNumChamberMeasure(TFile* fileroot);

bool isTrigger(string chamberName);

void HistoParameter(TFile* fileroot, string chamberName, string titleDirectory, float* neta, float* nphi);

void HistoParameter(TFile* fileroot, string chamberName, string titleDirectory, float* neta, float* nphi, float* lowe, float* lowp, float* highe, float* highp);


void analisi_histo(int runnumber)
{
  gROOT->Reset();

  std::cout <<runnumber <<std::endl;
  std::string FileName;
  stringstream ss ;
  ss << runnumber;
  std::string RUNNUMBER = ss.str();
  FileName="./output/Treerun"+RUNNUMBER+".root";          // ricordarsi di dare il giusto path in base a dove è posizionato il file root di cui ci serviamo

// Apertura file root
  TFile* f = (TFile*)gROOT->FindObject(FileName.c_str()); 
  if (f) f->Close();
  std::cout <<"Analizing Profile from root file: "<< FileName <<std::endl;
  TFile* fInput = new TFile(FileName.c_str(), "READ"); 

// Inizializzazione variabili
  int canvasIter = 0;
  int histoIterHP = 0;
  int histoIterRH = 0;
  int histoIterEH = 0;
  int histoIterOH = 0;
  int numChamberMeasure = 0;
 
  float nphi,neta,lowe,highe,lowp,highp;

// Recupera numero camere in misura
  numChamberMeasure = getNumChamberMeasure(fInput);


  const Int_t canvHP = 4;
  TCanvas *canvasHitProfile[canvHP];

  const Int_t canvRH = 4;
  TCanvas *canvasRawHitmap[canvRH];

  const Int_t canvEH = numChamberMeasure;
  TCanvas *canvasExpectedHitmap[canvEH];

  const Int_t canvOH = numChamberMeasure;
  TCanvas *canvasObtainedHitmap[canvOH];

  const Int_t hp = 32;
  TH1F* HistoHitProfile[hp];

  const Int_t rh = 16;
  TH2F* HistoRawHitmap[rh];

  const Int_t eh = numChamberMeasure*4;
  TH2F* HistoExpectedHitmap[eh];

  const Int_t oh = numChamberMeasure*4;
  TH2F* HistoObtainedHitmap[eh];

// Lista cartelle nel file root
  TIter next(fInput->GetListOfKeys());
  TKey *key;
  std::cout << "Directory in file " << FileName << ": " << std::endl; 
  gDirectory->GetListOfKeys()->Print();

// Loop su tutte le camere sotto analisi
  while ((key = (TKey*)next())) {

    std::string keyClass = key->GetClassName();
    std::string keyName = key->GetName();
    //std::cout << "camera: " << keyName << std::endl;
    
    // prosegue solo per le directory appartenenti a camere in uso
    if (keyClass.compare("TDirectoryFile")== 0) {
            
      fInput->cd(keyName.c_str());
      //gDirectory->pwd();

      TIter next(gDirectory->GetListOfKeys());
      TKey* key2;


      // loop su tutti tipi di profili e mappe per ogni camera
      while ((key2 = (TKey*)next())) {

          std::string keyName2 = key2->GetName();
          std::string keyTitle2 = key2->GetTitle(); 

          gDirectory->cd(keyName2.c_str());


          // Hit Profile

          if (keyTitle2.compare("Hit Profile")==0 && isTrigger(keyName)) {

            std::string TriggerName;
            if (keyName.compare("BOSA005T")==0) TriggerName = "TRIGGER RPC 1";
            if (keyName.compare("BOSA006M")==0) TriggerName = "TRIGGER RPC 2";
            if (keyName.compare("BOSA003T")==0) TriggerName = "TRIGGER RPC 11";
            if (keyName.compare("BOSA001T")==0) TriggerName = "TRIGGER RPC 12"; 
  
            canvasHitProfile[canvasIter] = new TCanvas(keyName.c_str(), TriggerName.c_str(), 900,700);
            canvasHitProfile[canvasIter]->Divide(4,2,0,0);
            
            TIter next(gDirectory->GetListOfKeys());
            TKey *key3;

            // loop per ogni gap/strip della camera
            while ((key3 = (TKey*)next())) {
             
              std::string keyName3 = key3->GetName();

              HistoParameter(fInput, keyName3, "Hit Profile", &neta, &nphi);

              TTree* myTree = (TTree*)gDirectory->Get(keyName3.c_str());
              
              double xmax;
              if (keyName3.compare(14, 3, "ETA")==0) xmax = neta;
              if (keyName3.compare(14, 3, "PHI")==0) xmax = nphi;
              
              std::string myTreeName = myTree->GetName();
              std::string stripName = myTreeName.substr(9,8);
              std::string histoName = "# of hits per strip in "+stripName;
              
              // Recupero dati dal tree e creo istogramma
              if (myTree->GetBranch("hits")) {

                HistoHitProfile[histoIterHP] = new TH1F (histoName.c_str(), myTreeName.c_str(), xmax, 1., xmax+1);

                Int_t var1;

                myTree->SetBranchAddress("hits", &var1);

                for (int i = 0; i< myTree->GetEntries(); i++) {

                  myTree->GetEntry(i);
                  HistoHitProfile[histoIterHP]->Fill(var1);  

                }

                canvasHitProfile[canvasIter]->cd((histoIterHP%8)+1);

                if (keyName3.compare(14, 3, "ETA")==0) {
                  HistoHitProfile[histoIterHP]->SetLineColor(2);
                  HistoHitProfile[histoIterHP]->SetFillColor(2);               
                }
                else if (keyName3.compare(14, 3, "PHI")==0) {
                  HistoHitProfile[histoIterHP]->SetLineColor(418);   
                  HistoHitProfile[histoIterHP]->SetFillColor(3);                
                }
                
                HistoHitProfile[histoIterHP]->SetFillStyle(3004);
                HistoHitProfile[histoIterHP]->SetStats(kFALSE);
                HistoHitProfile[histoIterHP]->Draw();

                histoIterHP++;

              }
              else {

                HistoHitProfile[histoIterHP] = new TH1F (histoName.c_str(), myTreeName.c_str(), xmax, 1., xmax+1);

                canvasHitProfile[canvasIter]->cd((histoIterHP%8)+1);

                HistoHitProfile[histoIterHP]->SetStats(kFALSE);
                HistoHitProfile[histoIterHP]->Draw();
                
                histoIterHP++;

              }
            }
          }

          // Raw Hitmap

          if (keyTitle2.compare("Raw Hitmap")==0 && isTrigger(keyName)) {

            std::string TriggerNameRH;
            std::string RawHitmapName = keyName+" Raw Hitmap";

            if (keyName.compare("BOSA005T")==0) TriggerNameRH = "TRIGGER RPC 1 - Raw Hitmap";
            if (keyName.compare("BOSA006M")==0) TriggerNameRH = "TRIGGER RPC 2 - Raw Hitmap";
            if (keyName.compare("BOSA003T")==0) TriggerNameRH = "TRIGGER RPC 11 - Raw Hitmap";
            if (keyName.compare("BOSA001T")==0) TriggerNameRH = "TRIGGER RPC 12 - Raw Hitmap"; 
            
            canvasRawHitmap[canvasIter] = new TCanvas(RawHitmapName.c_str(), TriggerNameRH.c_str(), 900,700);
            canvasRawHitmap[canvasIter]->Divide(2,2);
            gStyle->SetOptStat(1000001110);

            TIter next(gDirectory->GetListOfKeys());
            TKey *key3;

            // loop per ogni gap/strip della camera
            while ((key3 = (TKey*)next())) {
              
              std::string keyName3 = key3->GetName();

              TTree* myTree = (TTree*)gDirectory->Get(keyName3.c_str());

              std::string RawHistoName = keyName3+" Raw Hitmap";
              
              HistoParameter(fInput, keyName3, "Raw Hitmap", &neta, &nphi);

              // Recupero dati dal tree e creo istogramma
              if (myTree->GetBranchStatus("phiRawHitmap") && myTree->GetBranchStatus("etaRawHitmap")) {
                
                double phimax = myTree->GetMaximum("phiRawHitmap");
                double etamax = myTree->GetMaximum("etaRawHitmap");
                
                // caso in cui il tree è vuoto
                if (phimax < 0 && etamax < 0) {

                  HistoRawHitmap[histoIterRH] = new TH2F (RawHistoName.c_str(), RawHistoName.c_str(), nphi, 1., nphi+1, neta, 1., neta+1);

                  canvasRawHitmap[canvasIter]->cd((histoIterRH%4)+1);
                  HistoRawHitmap[histoIterRH]->SetStats(kFALSE);
                  HistoRawHitmap[histoIterRH]->Draw();

                  histoIterRH++;

                }

                else {

                  HistoRawHitmap[histoIterRH] = new TH2F (RawHistoName.c_str(), RawHistoName.c_str(), nphi, 1., nphi+1, neta, 1., neta+1);

                  Int_t varRH1;
                  Int_t varRH2;

                  myTree->SetBranchAddress("phiRawHitmap", &varRH1);
                  myTree->SetBranchAddress("etaRawHitmap", &varRH2);

                  for (int i = 0; i<myTree->GetEntries(); i++) {

                    myTree->GetEntry(i);

                    HistoRawHitmap[histoIterRH]->Fill(varRH1,varRH2);  

                  }

                  canvasRawHitmap[canvasIter]->cd((histoIterRH%4)+1);
                  
                  HistoRawHitmap[histoIterRH]->SetStats(kTRUE);
                  HistoRawHitmap[histoIterRH]->Draw();
                  
                  histoIterRH++;

                }
              }
            }
          }

          // Expected Hitmap
          if (keyTitle2.compare("Expected Hitmap")==0 && keyName.compare(7,1,"M")==0) {
  
            std::string ExpectedHitmapName = keyName+" Expected Hitmap";
            
            canvasExpectedHitmap[canvasIter] = new TCanvas(ExpectedHitmapName.c_str(), ExpectedHitmapName.c_str(), 900,700);
            canvasExpectedHitmap[canvasIter]->Divide(2,2);
            gStyle->SetOptStat(1000001110);  
       
            TIter next(gDirectory->GetListOfKeys());
            TKey *key3;

            // loop per ogni gap/strip della camera
            while ((key3 = (TKey*)next())) {

              std::string keyName3 = key3->GetName();
     
              TTree* myTree = (TTree*)gDirectory->Get(keyName3.c_str());              

              std::string myTreeName = myTree->GetName();
              ExpectedHitmapName = keyName3.substr(0,13) + "-Expected Hitmap";

              HistoParameter(fInput, keyName3, "Expected Hitmap", &neta, &nphi, &lowe, &lowp, &highe, &highp);

              // Recupero dati dal tree e creo istogramma
              if (myTree->GetBranchStatus("EstrapPhi") && myTree->GetBranchStatus("EstrapEta")) {               

                double Exphimax = myTree->GetMaximum("EstrapPhi");
                double Exetamax = myTree->GetMaximum("EstrapEta");
                
                // caso in cui il tree è vuoto
                if (Exphimax < 0 && Exetamax < 0) {

                  HistoExpectedHitmap[histoIterEH] = new TH2F (ExpectedHitmapName.c_str(), ExpectedHitmapName.c_str(), nphi*4, lowp, highp, neta*4, lowe, highe);

                  canvasExpectedHitmap[canvasIter]->cd((histoIterEH%4)+1);
                 
                  HistoExpectedHitmap[histoIterEH]->SetStats(kFALSE);
                  HistoExpectedHitmap[histoIterEH]->Draw();
                 
                  histoIterEH++;

                }

                else {

                  HistoExpectedHitmap[histoIterEH] = new TH2F (ExpectedHitmapName.c_str(), ExpectedHitmapName.c_str(), nphi*4, lowp, highp, neta*4, lowe, highe);

                  Double_t varEH1;
                  Double_t varEH2;

                  myTree->SetBranchAddress("EstrapPhi", &varEH1);
                  myTree->SetBranchAddress("EstrapEta", &varEH2);

                  for (int i = 0; i < myTree->GetEntries(); i++) {

                    myTree->GetEntry(i);

                    HistoExpectedHitmap[histoIterEH]->Fill(varEH1,varEH2,1.);  

                  }

                  canvasExpectedHitmap[canvasIter]->cd((histoIterEH%4)+1);
                 
                  HistoExpectedHitmap[histoIterEH]->SetStats(kTRUE);
                  HistoExpectedHitmap[histoIterEH]->Draw();
                 
                  histoIterEH++;

                }
              }
            }
          }

          // Obtained Hitmap
          if (keyTitle2.compare("Obtained Hitmap")==0 && keyName.compare(7,1,"M")==0) {
  
            std::string ObtainedHitmapName = keyName+" Obtained Hitmap";
            canvasObtainedHitmap[canvasIter] = new TCanvas(ObtainedHitmapName.c_str(), ObtainedHitmapName.c_str(), 900,700);
            canvasObtainedHitmap[canvasIter]->Divide(2,2);
            gStyle->SetOptStat(1000001110);           
          
            TIter next(gDirectory->GetListOfKeys());
            TKey *key3; 

            // loop per ogni gap/strip della camera
            while ((key3 = (TKey*)next())) {

              std::string keyName3 = key3->GetName();

              TTree* myTree = (TTree*)gDirectory->Get(keyName3.c_str());              

              std::string myTreeName = myTree->GetName();
              ObtainedHitmapName = keyName3.substr(0,13) + "-Obtained Hitmap";

              HistoParameter(fInput, keyName3, "Obtained Hitmap", &neta, &nphi, &lowe, &lowp, &highe, &highp);

              // Recupero dati dal tree e creo istogramma
              if (myTree->GetBranchStatus("EstrapPhi") && myTree->GetBranchStatus("EstrapEta")) {
                
                double ObExphimax = myTree->GetMaximum("EstrapPhi");
                double ObExetamax = myTree->GetMaximum("EstrapEta");
                
                // caso in cui il tree è vuoto
                if (ObExphimax < 0 && ObExetamax < 0) {

                  HistoObtainedHitmap[histoIterOH] = new TH2F (ObtainedHitmapName.c_str(), ObtainedHitmapName.c_str(), nphi*4, lowp, highp, neta*4, lowe, highe);

                  canvasObtainedHitmap[canvasIter]->cd((histoIterOH%4)+1);
                 
                  HistoObtainedHitmap[histoIterOH]->SetStats(kFALSE);
                  HistoObtainedHitmap[histoIterOH]->Draw();
                  
                  histoIterOH++;

                }

                else {

                  HistoObtainedHitmap[histoIterOH] = new TH2F (ObtainedHitmapName.c_str(), ObtainedHitmapName.c_str(), nphi*4, lowp, highp, neta*4, lowe, highe);

                  Double_t varOH1;
                  Double_t varOH2;

                  myTree->SetBranchAddress("EstrapPhi", &varOH1);
                  myTree->SetBranchAddress("EstrapEta", &varOH2);

                  for (int i = 0; i< myTree->GetEntries(); i++) {

                    myTree->GetEntry(i);

                    HistoObtainedHitmap[histoIterOH]->Fill(varOH1,varOH2,1.);  

                  }

                  canvasObtainedHitmap[canvasIter]->cd((histoIterOH%4)+1);
                  
                  HistoObtainedHitmap[histoIterOH]->SetStats(kTRUE);
                  HistoObtainedHitmap[histoIterOH]->Draw();
                  
                  histoIterOH++;

                }
              }

              else {

                HistoObtainedHitmap[histoIterOH] = new TH2F (ObtainedHitmapName.c_str(), ObtainedHitmapName.c_str(), 100, 0, 100, 100, 0, 100);

                canvasObtainedHitmap[canvasIter]->cd((histoIterOH%4)+1);
                
                HistoObtainedHitmap[histoIterOH]->SetStats(kFALSE);
                HistoObtainedHitmap[histoIterOH]->Draw();
                
                histoIterOH++;

              }
            }
          }

          // Torna alla directory relativa alla camera in esame e continua il loop sulle varie mappe e profili
          fInput->cd(keyName.c_str());
      }
    }
    canvasIter++;
  }


  // Radiografia come in vecchia macro
  std::cout << "\n==================== Radiography ====================" << std::endl;

  // Ricerca degli istogrammi Expected e rispettivi Obtained e prendo solo quelli delle camere in misura
  for (int i=0; i<oh; i++) {
             
    std::string histoRadioOb = HistoObtainedHitmap[i]->GetName();

    if ((HistoObtainedHitmap[i]->GetEntries())!=0) {
      for (int j=0; j<eh; j++) {

        std::string histoRadioExGap = HistoExpectedHitmap[j]->GetName();
        std::string histoRadioEx = histoRadioExGap.substr(0,13);
        
        if (histoRadioOb.compare(0,13,histoRadioEx)==0 && histoRadioOb.compare(7,1,"M")==0 && !isTrigger(histoRadioOb.substr(0,8))) {

            std::cout << "\nCalcolo efficienza per: " << histoRadioExGap.substr(0,13) << std::endl; 
            
            int    nbinsx_map   = HistoExpectedHitmap[j]->GetXaxis()->GetNbins();
            int    nbinsy_map   = HistoExpectedHitmap[j]->GetYaxis()->GetNbins();
            float  lowx_map     = HistoExpectedHitmap[j]->GetXaxis()->GetBinLowEdge(1);
            float  lowy_map     = HistoExpectedHitmap[j]->GetYaxis()->GetBinLowEdge(1);
            int UpBinX = HistoExpectedHitmap[j]->GetXaxis()->GetLast();
            int UpBinY = HistoExpectedHitmap[j]->GetYaxis()->GetLast();
            float  highx_map    = HistoExpectedHitmap[j]->GetXaxis()->GetBinWidth(1)*nbinsx_map+lowx_map;
            float  highy_map    = HistoExpectedHitmap[j]->GetYaxis()->GetBinWidth(1)*nbinsy_map+lowy_map; 
            float  widthx_map   = HistoExpectedHitmap[j]->GetXaxis()->GetBinWidth(1);
            float  widthy_map   = HistoExpectedHitmap[j]->GetYaxis()->GetBinWidth(1);  

            int NBinradio_phi = 60*4;
            int NBinradio_eta = 30*4;

            TH2F * h_projection = new TH2F ("Projection","Projection",NBinradio_phi,lowx_map,highx_map,NBinradio_eta ,lowy_map,highy_map);
            TH2F * h_hitontrack = new TH2F ("hitontrack","Hit on track - Efficiency",NBinradio_phi,lowx_map,highx_map,NBinradio_eta ,lowy_map,highy_map);  

            TH1F * h_projectionX = new TH1F ("ProjectionX","ProjectionX",nbinsx_map,lowx_map,highx_map);
            TH1F * h_hitontrackX = new TH1F ("hitontrackX","hitontrackX",nbinsx_map,lowx_map,highx_map);

            TH1F * h_projectionY = new TH1F ("ProjectionY","ProjectionY",nbinsy_map,lowy_map,highy_map);
            TH1F * h_hitontrackY = new TH1F ("hitontrackY","hitontrackY",nbinsy_map,lowy_map,highy_map);


            TCanvas *c11 = new TCanvas("Radiography","RADIOGRAPHY",900,900);

            gStyle->SetOptStat(1000001110);
            gStyle->SetPalette(1);

            // Rebinning
            for (int ii=1 ; ii<nbinsx_map+1 ; ii++ ) {
              for (int jj=1 ; jj<nbinsy_map+1 ; jj++ ) {
                
                float x=lowx_map+(ii-1)*widthx_map;
                float y=lowy_map+(jj-1)*widthy_map;
                float count = HistoExpectedHitmap[j]->GetBinContent(ii,jj); 
                h_projection->Fill(x,y,count);
                count = HistoObtainedHitmap[i]->GetBinContent(ii,jj); 
                h_hitontrack->Fill(x,y,count);

              }
            }   

            // Calcolo efficienza
            for (int ii=1 ; ii<NBinradio_phi+1 ; ii++ ) {
              for (int jj=1 ; jj<NBinradio_eta+1 ; jj++ ){

                float ntracks = h_projection->GetBinContent(ii,jj); 
                float nhits   = h_hitontrack->GetBinContent(ii,jj);
                float eff = 0; 
                if(ntracks) eff=nhits/ntracks;
                h_hitontrack->SetBinContent(ii,jj,eff);

              }
            }   


            // Proiezione lungo X
            for (int ii=1 ; ii<nbinsx_map+1 ; ii++ ) {
              
              float ntrackstot=0;
              float nhitstot  =0;
              
              for (int jj=1 ; jj<nbinsy_map+1 ; jj++ ){

                ntrackstot += HistoExpectedHitmap[j]->GetBinContent(ii,jj); 
                nhitstot   += HistoObtainedHitmap[i]->GetBinContent(ii,jj); 

              }
              
              float eff = 0; 
              if(ntrackstot) eff=nhitstot/ntrackstot;
              h_hitontrackX->SetBinContent(ii,eff);

            }       

            // Proiezione lungo Y
            for (int ii=1 ; ii<nbinsy_map+1 ; ii++ ) {

              float ntrackstot=0;
              float nhitstot  =0;

              for (int jj=35 ; jj<90+0*nbinsx_map+1; jj++ ){

                ntrackstot += HistoExpectedHitmap[j]->GetBinContent(jj,ii); 
                nhitstot   += HistoObtainedHitmap[i]->GetBinContent(jj,ii); 

              }

              float eff = 0; 
              if(ntrackstot) eff=nhitstot/ntrackstot;
              h_hitontrackY->SetBinContent(ii,eff);

            }   
          
            c11->Divide(1,3);

            c11->cd(1);
            h_projection->Draw("ZCOL");

            c11->cd(2);
            h_hitontrack->Draw("ZCOL");

            c11->cd(3);
            h_hitontrackY->SetTitle("Profile");
            h_hitontrackY->Draw();
            h_hitontrackY->SetLineColor(3);
            h_hitontrackX->SetLineColor(2);
            h_hitontrackX->Draw("SAME");

            // c11->Print("/afs/le.infn.it/user/c/coluccia/html/allow_listing/rpc_auger/temp/radiography_rpc11.gif");

        }
      }
    }                
  }      
}




// Legge e memorizza il numero di camere considerate in misura (quelle contrassegnate con la M finale)
int getNumChamberMeasure(TFile* fileroot)
{

  TIter next(fileroot->GetListOfKeys());
  TKey *keyM;
  
  int measurechamber = 0; 
  while ((keyM = (TKey*)next())) {

    std::string typeOfCham = keyM->GetName();
    if (typeOfCham.compare(7,1,"M")==0) measurechamber++;
  }

  cout << "Numero di camere in misura:  " << measurechamber << endl;
  
  return measurechamber;
}


// Controlla se la camera indicata fa parte del gruppo di quelle considerate come trigger
bool isTrigger(string chamberName)
{
  bool trigger;

  if (chamberName.compare(7,1,"T")==0 || chamberName.compare("BOSA006M")==0) trigger = true; 
  else trigger = false;

  return trigger;
}


// Legge i valori contenuti nella cartella Histogram Parameter per ogni gap (versione per il solo numero di strip)
void HistoParameter(TFile* fileroot, string chamberName, string titleDirectory, float* neta, float* nphi)
{

  string directoryH = chamberName.substr(0,8)+"/Histogram Parameter";
  fileroot->cd(directoryH.c_str());

  string treename = chamberName.substr(0,13)+"-TUPLE";

  TTree* myTree = (TTree*)gDirectory->Get(treename.c_str());
  myTree->SetBranchAddress("neta", neta);
  myTree->SetBranchAddress("nphi", nphi);
  myTree->GetEntry(0);

// da decommentare se si vogliono avere le informazioni sui parametri degli istogrammi della gap
/*
  if (myTree) {
    cout << "\nParametri gap " << chamberName.substr(0,13) << endl;
    cout << "Strip in Eta: " << *neta << endl;
    cout << "Strip in Phi: " << *nphi << endl;  
  
  }
  */

  string oldDirectory = chamberName.substr(0,8)+"/"+titleDirectory;
  fileroot->cd(oldDirectory.c_str());
}

// Legge i valori contenuti nella cartella Histogram Parameter per ogni gap (versione con tutti i parametri)
void HistoParameter(TFile* fileroot, string chamberName, string titleDirectory, float* neta, float* nphi, float* lowe, float* lowp, float* highe, float* highp)
{

  string directoryH = chamberName.substr(0,8)+"/Histogram Parameter";
  fileroot->cd(directoryH.c_str());

  string treename = chamberName.substr(0,13)+"-TUPLE";

  TTree* myTree = (TTree*)gDirectory->Get(treename.c_str());
  myTree->SetBranchAddress("neta", neta);
  myTree->SetBranchAddress("nphi", nphi);
  myTree->SetBranchAddress("lowe", lowe);
  myTree->SetBranchAddress("highp", highp);
  myTree->SetBranchAddress("lowp", lowp);
  myTree->SetBranchAddress("highe", highe);    
  myTree->GetEntry(0);

// da decommentare se si vogliono avere le informazioni sui parametri degli istogrammi della gap
/*
  if (myTree) {
    cout << "\nParametri gap " << chamberName.substr(0,13) << endl;
    cout << "Strip in Eta: " << *neta << endl;
    cout << "Low limit in Eta: " << *lowe << endl;
    cout << "High limit in Eta: " << *highe << endl;
    cout << "Strip in Phi: " << *nphi << endl;  
    cout << "Low limit in Phi: " << *lowp << endl;
    cout << "High limit in Phi: " << *highp << endl; 
  
  }
*/

  string oldDirectory = chamberName.substr(0,8)+"/"+titleDirectory;
  fileroot->cd(oldDirectory.c_str());
}