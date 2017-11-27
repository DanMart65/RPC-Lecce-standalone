mandi da fare su linux

cp /afs/le.infn.it/user/c/chiodini/public/rpcreco.tgz  .
tar -xzvf rpcreco.tgz
cd rpcreco
mkdir $HOME/bin        ( se non avete la directory bin nella $HOME)

???il seguente setup a me funziona (al momento non ne ho trovato uno senza i settaggi di atlas)

setupATLAS
asetup 20.7.4.1,AtlasProduction,slc6,64 --tags=here
setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/afs/cern.ch/sw/lcg/external/clhep/1.9.4.7/x86_64-slc6-gcc47-opt/lib


???Poi compilo


make clean
make

??? e runno. l?eseguibile finisce in $HOME/bin e si chiama lets

lets

??? e l?output e? in

per fare il merge dei file......


mv run*.dat run*.dat.ref
cat run*.dat.ref run*.bin > run*.dat




