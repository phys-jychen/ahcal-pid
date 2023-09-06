# AHCAL-PID, a Simple Tool for PID

## Author
Ji-Yuan CHEN (SJTU; <jy_chen@sjtu.edu.cn>)

## Description
This program was designed for the PID of CEPC AHCAL.  By reconstructing variables describing the topology of the hadronic and electromagnetic shower, we can perform PID with the help of BDT, using the TMVA (the **T**oolkit for **M**ulti-**V**ariate data **A**nalysis with ROOT) package.

## Usage
First of all, you can run
```shell
iSel -h[elp]
# or
iRec -h[elp]
# or
iBDT -h[elp]
```
to display help information.  For more detail, please refer to the following instructions. :p

### Selecting Hits
In MC samples or even real data, a large number of hits do not deposit energy in the scintillator.  These hits are not useful in our analysis and should be discarded.  To achieve this, run
```shell
iSel -f [file] -t [tree]
```
After the program finishes, an output file whose name has a prefix "sel" is created. The original branches have already been deleted.

### Adding Reconstructed Variables
In the directory of the ROOT file to be analyses, execute:
```shell
iRec -f [file] -t [tree]
```
The name of the output file is given a prefix "pid", and the original branches are not kept.  If you need to add some new variables or modify the definitions of some of them, please go to the file `src/Variables.cxx`.

### Performing BDT
Before you begin, make sure that the variables as well as the ROOT files listed in `bdt.cxx` are all present (you can also modify this file to meet your own needs).  Then, execute:
```shell
iBDT -r -t [tree]
```
Eventually you can see the output file containing the data obtained during training and test (`TMVAMulticlass.root`).

Possibly you need to know the performance of BDT on the validation dataset.  In this case, execute:
```shell
iBDT -v -f [file] -t [tree]
```
Then the BDT response is stored in the output ROOT file, whose name has prefix "bdt".  Possibly it is necessary to modify `src/BDT.cxx`.  Anyway, make sure that the input variables are exactly the same as those in `bdt.cxx`!

## Environment Set-up
This project requires CMake version >= 3.17.  If you are working on the cluster of INPAC, IHEP, etc., the environment can be easily set up by simply executing
```shell
source /cvmfs/sft.cern.ch/lcg/views/LCG_102/x86_64-centos7-gcc11-opt/setup.sh
```
(This command has been included in `setup.sh`.)

Then, the environment with CMake 3.20.0 and ROOT 6.26/04 is configured.  As long as no compilation errors are raised and the CMake version requirement is met, other versions on the LCG are also acceptable. :p

## Installation & Compilation
Having set up the environment, this project can be cloned from GitHub and compiled in a normal way:
```shell
git clone git@github.com:phys-jychen/ahcal-pid.git
cd ahcal-pid
mkdir build
cd build
cmake ..
make -j100    # Just do it!
source setup.sh
```

Every time you log in to the cluster, before the first time of running this program, remember to execute
```shell
source [build]/setup.sh
```

By now, the compilation have been finished.  Prepare your datasets, and have fun!

## Acknowledgement & Reference
The author would like to thank Mr Zhen WANG (TDLI/SJTU), for kindly offering his project [cepc\_hbuana](https://github.com/wangz1996/cepc_hbuana) for the convenience of going through the whole process of BDT.  In this project (AHCAL-PID), the framework of reconstructing variables and BDT are adopted from cepc\_hbuana.
