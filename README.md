# RandomSimulationProject
6 stages before you can run our simulate tool - if you have any problem you can write me an email(yonatan786@campus.technion.ac.il):

1)First, you should download the project files from git. the files are in private git so you need write the password and the user name:

git clone "https://github.com/yonatan-maman/RandomSimulationProject.git"

user name:  randomSimulation
password:  rsProject123


2)After the files have been downloaded. you should  go into our directory files (RandomSimulationProject) , and then to llvm-project directory:

cd RandomSimulationProject/llvm-project

3)Then, make the directory to build and compile the files and go into this directory:

mkdir build
cd build

3)After this, you should build the makefiles using cmake:

cmake -DLLVM_ENABLE_PROJECTS=clang -G "Unix Makefiles" ../llvm

4)Then, you should compile the files:

make clang-format

5)After this, you should compile and create the libaries and the scripts we need to run simulate, so you need go to the main directory and run this script:

cd ../../
./createProject

6)Then, everything is ready so you can run this for other explainings:

./RS/simulate --help
