# AHCAL-PID, a Simple Tool for PID

## Author
Ji-Yuan CHEN (SJTU; <jy_chen@sjtu.edu.cn>)

## Description
This program is designed for the PID of CEPC AHCAL. By reconstructing variables describing the topology of the hadronic and electromagnetic shower, we can perform PID with the help of BDT, using the TMVA (the **T**oolkit for **M**ulti-**V**ariate data **A**nalysis with ROOT) package.

For a more intuitive understanding of the shower topology, a python program is also included for event display.

## Usage
At first, you can run
```shell
iSel -h[elp]
# or
iRec -h[elp]
# or
iBDT -h[elp]
```
to display help information of analysis, and
```shell
python display.py -h
```
for that of event display (mind the order of `display.py` and `-h`!). For more detail, please refer to the following instructions. :stuck_out_tongue:

### Selecting Hits
In MC samples or even real data, some hits do not deposit energy in the scintillator, or the energy deposition is a negative number. In these cases, the hits are misleading and not useful in our analysis; therefore, they should be discarded. To achieve this, run
```shell
iSel -f [file]
```
if the tree in the original ROOT file has default name `Calib_Hit`. Otherwise, to specify other tree names, run
```shell
iSel -f [file] -t [tree]
```

After the execution finishes, an output file whose name has a prefix “sel” is created in your current directory, regardless of where the original ROOT file is. The original branches have already been deleted to save space.

### Adding Reconstructed Variables
In any directory, execute:
```shell
iRec -f [file]
```
if the tree in the original ROOT file has default name `Calib_Hit`. Otherwise, to specify other names, run
```shell
iRec -f [file] -t [tree]
```

The name of the output file is given a prefix “rec” in your current directory, and the original branches are not kept for the sake of saving space. If you need to add some new variables or modify the definitions of some of them, please go to the file `src/Variables.cxx`.

### Performing BDT
Before you begin, make sure that the variables as well as the ROOT files listed in `bdt.cxx` are all present (you can also modify this file to meet your own needs). Then, execute:
```shell
iBDT -r
```
if the trees in the original ROOT files all have default name `Calib_Hit`. Otherwise, to specify other names, run
```shell
iBDT -r -t [tree]
```

Eventually, you can see the output file containing the data obtained during training and test (`TMVAMulticlass.root`) in your current directory.

Possibly you need to know the performance of BDT on the validation dataset. In this case, execute:
```shell
iBDT -v -f [file]
```
if the tree in the original ROOT file has default name `Calib_Hit`. Otherwise, to specify other tree names, run
```shell
iBDT -v -f [file] -t [tree]
```

Then the BDT response is stored in the output ROOT file, whose name has a prefix “bdt”, in your current directory. Possibly it is necessary to modify `src/BDT.cxx`. Anyway, make sure that the input variables are identical to those in `bdt.cxx`!

### Event Display
In the directory you have installed, run
```shell
./event_display.sh
```
to obtain a figure of event display, which will be saved in a directory assigned in this shell script.

You can modify `event_display.sh` to meet your own needs: input ROOT file, tree name, title of the figure, ID of the event, directory to save the output file, name of the output file, and instantly show the figure or not.

## Environment Set-up
This project requires CMake version >= 3.17. If you are working on the cluster of INPAC, IHEP, etc., the environment can be easily set up by simply executing
```shell
source /cvmfs/sft.cern.ch/lcg/views/LCG_102/x86_64-centos7-gcc11-opt/setup.sh
```
(This command has been included in `setup.sh`.)

Then, the environment with CMake 3.20.0 and ROOT 6.26/04 is configured. As long as neither compilation errors are raised, nor the CMake version requirement is met, other versions on the LCG are also acceptable. :stuck_out_tongue:

### Notice
This environment cannot be used for event display! Instead, the environment used while designing this program was with
- Python: 3.11.5
- Matplotlib: 3.7.2
- NumPy: 1.24.3
- UpROOT: 5.1.1

Other environments have not been fully tested.

## Installation & Compilation
Having set up the environment, this project can be cloned from GitHub and compiled as usual:
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
source <build_dir>/setup.sh
```

By now, the compilation have been finished. Prepare your datasets, and have fun! :relaxed:

## Change Log
### 5 January 2024
Added a simple python program for event display, which was adopted from [ecal-display](https://github.com/phys-jychen/ecal-display). In the event display, the energy of the hits is regarded as the fourth dimension.

### 16 January 2024
Updated event display.

## Acknowledgement & Reference
The author would like to thank Mr Zhen WANG (TDLI/SJTU), for kindly offering his project [cepc\_hbuana](https://github.com/wangz1996/cepc_hbuana) for the convenience of going through the whole process of BDT. In this project (AHCAL-PID), the framework of reconstructing variables and BDT are adopted from cepc\_hbuana.