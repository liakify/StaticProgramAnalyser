# CS3203 SWE Project (AY19/20 Sem 2 Team 6)

[![Build Status](https://travis-ci.com/nus-cs3203/team06-win-spa-19s2.svg?token=FcixrHMuWDoA999fe1eo&branch=master)](https://travis-ci.com/nus-cs3203/team06-win-spa-19s2)

Team 6's implementation of a Static Program Analyzer (SPA) for the SIMPLE programming language and PQL query language.

## Getting Started

### Prerequisites

- Operating System: Microsoft Windows 10
- VCS: Git
- IDE: Visual Studio 2019 (Enterprise edition recommended)
- C++ GUI framework: QT v5.11.1 (32-bit installation)

### Installation Instructions

1. Clone this Git repository locally with the URL https://github.com/CS3203-Team-6/SPA.git
2. Navigate to the folder `/Team06/Code06/` and open the solution file `StartupSPASolution.sln` in Visual Studio 2019 (any version)
3. Build the solution in Visual Studio, with either the `AutoTester` (for CLI) or `Gui` (for GUI) project set as the starting project.
    - If using the GUI setup, ensure that the project has been set to use Qt version 5.11.1.
    - This can be done by right-clicking the solution in the Solution Explorer > Change Solution's Qt version, then selecting the appropriate version.
    - The compiled executable `Gui.exe` will be placed in `\Team06\Code06\Debug`

### Build Types

It is essential that the solution is built for the correct build type, since the AutoTester executables produced by both do not exhibit the same behaviour.

The two main build types are:
- **DEBUG**: build with **no optimisations**, ensuring full semantics of the source code
- **RELEASE**: build with **moderate compiler optimisations**, improving performance but potentially causing incorrect execution where undefined behaviour exists

Since the evaluation of the SPA project is performed on the **RELEASE** build, **all testing** should be performed on only the **RELEASE** build. The key exception to this is:
- The **DEBUG** build should be used when performing unit or integration tests with code coverage analysis in Visual Studio, since the compiler optimisations introduced result in the **RELEASE** build reporting incorrect coverage metrics

Until specified otherwise, Visual Studio defaults to building the **DEBUG** build. The build type can be changed by:
- Right click on the solution file > Properties > Configuration Properties > Configuration Manager > select "Release" from the _Active solution configuration_ dropdown menu


## Testing

### System Testing

1. Navigate to the respective build output folders with your shell (Git Bash recommended for Windows); the compiled executables are here
    - **DEBUG** build: `Team06\Code06\Debug`
    - **RELEASE** build: `\Team06\Code06\Release`
2. To run the AutoTester with an input SIMPLE program file and input query file:
    ```
    ./AutoTester.exe <SIMPLE_FILE> <QUERY_FILE> <OUTPUT_FILE>
    ```
    - The output will be generated as `<OUTPUT_FILE>.xml` which may be viewed in Internet Explorer or Mozilla Firefox

### Regression Testing

The Powershell script `runAllSysTest.ps1` allows the AutoTester to be run with multiple test suites, with logging output for the status of each test suite (with failing testcases).

This script will always test the **RELEASE** build of the AutoTester if available. Otherwise, it will issue a warning that the **DEBUG** build is being tested instead.

1. Navigate to the root folder
2. Edit `runAllSysTest.ps1` to specify the list of test suites to run
    - A test suite comprises a SIMPLE source program file and a corresponding query file (if the SIMPLE program is valid)
    - For each test suite added, include the test prefix (eg. `..\calls_0`); the `..\` is needed for tests not in the `\regression_tests` directory)
    - Existing test suites can be skipped by commenting the corresponding test prefix out
3. Type in your shell the command `powershell ".\runAllSysTest.ps1"`
    - Powershell requires permission to run external scripts; this can be se twith the command `powershell -Command 'Set-ExecutionPolicy -ExecutionPolicy RemoteSigned' while running as administrator`)
4. If necessary, the full test outputs can be inspected in the `.\Team06\Test06` folder or corresponding sub-folder

### Unit Test Code Coverage Analysis

Prerequisites: Visual Studio 2019 (Enterprise edition)

The Enterprise edition of Visual Studio comes with the code coverage feature. The code coverage analysis will be run on both the unit and integration tests, and should be done only for the **DEBUG** build.

1. Click on the 'Test' menu in the toolbar, then select "Analyse Code Coverage for All Tests"
2. After all tests have run, a code coverage report will be generated and presented in the _Code Coverage Results_ window
3. To highlight coverage of individual lines in the source code, click on 'Show Code Coverage Coloring' in the _Code Coverage Results_ window

## Continuous Integration

We are employing Travis CI as our CI framework, and it automates the following tasks:
- Compilation and building of the SPA solution for the **RELEASE** build
- Execution of all unit and integration tests
- Regression testing and performance benchmarking of the **RELEASE** build of the AutoTester executable
- Detection of coding standard violations (modified [Google C++ style guide](https://google.github.io/styleguide/cppguide.html))

## Contribution Standards

The following are expectations:

- No direct commits or force-pushes are allowed to the `master` or `extensions` branches
- Use the branching workflow with merging
- No self-approval of branches
- Do not merge failing branches
- PRs should only introduce features from the Advanced SPA **OR** extensions, and not both concurrently
- PRs adding extension features should target the `extensions` branch, and include the latest commits from `master`

## Built With

* A lot of coffee and sleepless nights
