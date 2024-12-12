# **StressSolve**
## Software Application developed for CSCI370

StressSolve is our solution to helping students overcome stress in todays fast-paced academic environment. 
It implements a machine learning algorithm, Random Forest, to accurately predict students stress levels and give them relevant solutions to relieve their stress.

## Requirements
- Qt 6.8.0 or higher
- Microsoft Visual Studio 2022 or later (or CMake)

## Installing Qt

### Option 1: Using the Official Qt installer
1. Download the Qt installer from the official website: https://www.qt.io/download
2. Run the installer and follow the instructions
3. Select Qt 6.8 (or higher) for desktop development
4. Install

### Option 2: Using aqtinstall
1. Install Python 3.8.10 or higher
2. Install aqtinstall using ```pip install aqtinstall```
3. Install the components following the instructions given at [link](https://aqtinstall.readthedocs.io/en/latest/getting_started.html) depending on your operating system

## Installing Curl

1. Download curl of your operating system from [here](https://curl.se/download.html)
2. Extract the files to a folder

## Building in Microsoft Visual Studio

### Configuring Qt in Visual Studio
1. Open Microsoft Visual Studio 2022 or later
2. Go to Extensions -> Manage Extensions
3. Search for Qt VS Tools and install it
4. Restart Visual Studio
5. Go to Extensions -> Qt VS Tools -> Qt Versions
6. Add the path to the Qt installation directory
7. Save

### Configuring Curl in Visual Studio
1. Open Microsoft Visual Studio 2022 or later
2. Go to Project -> Properties
3. Go to C/C++ -> General -> Additional Include Directories
4. Add the path to the include folder in the curl directory
5. Go to C/C++ -> General -> Library Directories
6. Add the path to the lib folder in the curl directory

## Creating bearer token for the application
1. Go to https://console.x.ai/ and create an account
2. Go to the API section and create a new API key
3. Create a text file named "bearer_token.txt" in the root directory of the project
4. Paste the API key in the text file

### How to run
1. Clone the repository
2. Open the solution in Microsoft Visual Studio 2022 or later
3. Build the solution
4. Run the application

## Building using CMake

### How to run
1. Clone the repository
2. Create a build directory
3. Open a terminal in the build directory
4. Run ```cmake -DCMAKE_PREFIX_PATH="path\to\qt\root;path\to\curl\root"  -G "Visual Studio 17 2022" -A x64 .. && cmake --build .```
Example: ```cmake -DCMAKE_PREFIX_PATH="C:\Qt\6.2.2\msvc2019_64;C:\curl-7.79.1"  -G "Visual Studio 17 2022" -A x64 .. && cmake --build .```
5. Run the application
