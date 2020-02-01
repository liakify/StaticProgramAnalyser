# CS3203 SWE Project (AY19/20 Sem 2 Team 6)

Team 6's implementation of a Static Program Analyzer (SPA) for the SIMPLE programming language and PQL query language.


## Getting Started

### Prerequisites

- Operating System: Microsoft Windows 10
- VCS: Git
- IDE: Visual Studio 2019 (Enterprise edition recommended)
- C++ GUI framework: QT v5.11.1 (32-bit installation)

### Installation Instructions

1. Clone this Git repository locally with the URL https://github.com/CS3203-Team-6/SPA.git.
2. Navigate to the folder `/Team06/Code06/` and open the solution file `StartupSPASolution.sln` in the Visual Studio 2019 (any version).
3. Build the solution in Visual Studio, with either the `AutoTester` (for CLI) or `Gui` (for GUI) project set as the starting project.
    - If using the GUI setup, ensure that the project has been set to use Qt version 5.11.1.
    - This can be done by right-clicking the solution in the Solution Explorer > Change Solution's Qt version, then selecting the appropriate version.
    - The compiled executable `Gui.exe` will be placed in `\Team06\Code06\Debug`

### System Testing

1. Navigate to `\Team06\Code06\Debug` with your favourite shell (Git Bash recommended); the compiled executables are here
2. To run the AutoTester with an input SIMPLE program file and input query file:
    ```
    ./AutoTester.exe <SIMPLE_FILE> <QUERY_FILE> <OUTPUT_FILE>
    ```
    - The output will be generated as `<OUTPUT_FILE>.xml` which may be viewed in the Mozilla Firefox browser.
3. Manual testing may be conducted with the GUI by running the executable `Gui.exe`

### Automated Testing (CI)

TBD. Current consideratons: Bash scripts with VSTS or Travis CI with Windows environment

## Contribution Standards

The following are expectations:

- No direct commits or force-pushes are allowed to the `master` branch
- Use the branching workflow with merging
- No self-approval of branches
- Do not merge failing branches
- Ensure pushes to remote repo pass all pre-push hooks if any

## Built With

* A lot of coffee and sleepless nights
