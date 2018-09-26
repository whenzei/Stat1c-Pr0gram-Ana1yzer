# simple-spa

A Static Program Analyzer (SPA for short) is an interactive tool that automatically answers queries about programs. In this project, we design and implement an SPA for a a programming language called SIMPLE.

## Compilation Instructions
Note that this program has only been tested and thus deemed compatible with only Windows 10.

### Requirements
To build simple-spa from source, the following tools and steps are needed:

- Visual Studio 2017 (Preferably Enterprise Edition)
- Qt 5.1.1 (Can be found [here](http://mirrors.ocf.berkeley.edu/qt/official_releases/qt/5.11/5.11.1/)) 
    - During the installation of QT, please ensure that MSVC 2015 32-bit is checked. Although we are using Visual Studio 2017, binaries built for Visual Studio 2015 are compatible with it.
- Qt Visual Studio Tools 2.2.1
    - Qt Visual Studio Tools can be installed by navigating to Tools > Extensions > Updates > Online in VS 2017 and searching for Qt Visual Studio Tools.
    - Set the Qt dependencies in VS by navigating to Qt VS Tools > Qt Options > Add, then choosing the bin folder in Qt's folder.
- Add the Qt folder to `%PATH%`

### Compiling
> `$SOURCE` will be used throughout the instructions and will refer to the source directory of this project.

1. Open the solution file `StartupSPASolution.sln` located at `$SOURCE\Code08` in Visual Studio 2017
2. Make sure the active solution configuration is set to "Release" for optimal performance. To set the configuration to "Release", enter the hotkey `Alt + B`, `O` to open the configuration manager, and press `R` when the manager window appears.
3. Press the `Ctrl + Shift + B` hotkey to build the solution. The compiled project will be located at `$SOURCE\Code08\Release`

## Usage
### Running AutoTester from Visual Studio (Recommended)
After building the solution, add the required arguments for AutoTester to perform tests by right clicking the AutoTester project in VS2017's Solution Explorer and selecting "Properties". The properties page can also be accessed by pressing `Alt + Enter` while the AutoTester project is highlighted in the Solution Explorer.

From the Properties page, navigate to Configuration Properties > Debugging. Enter the provided string in the "Command Arguments" field (assuming you have not moved any folders around):

```
..\..\Tests08\Sample-Source.txt ..\..\Tests08\Sample-Queries.txt out.xml
```

After this has been done, run AutoTester and wait for the tests to be completed. A new `out.xml` file will be generated at `$SOURCE\Code08\Release` which can be opened in Firefox to view the results of the system tests. The supplied test cases should all pass.

We have also provided two addition sample programs (`Sample-Source-2.txt`, `Sample-Source-3.txt`) with their respective sample queries (`Sample-Queries-2.txt`, `Sample-Queries-3`) files for a total of 3 test suites. You can run them by changing the file names of the arguments in the "Command Arguments" field:

```
..\..\Tests08\Sample-Source-2.txt ..\..\Tests08\Sample-Queries-2.txt out-2.xml
```

```
..\..\Tests08\Sample-Source-3.txt ..\..\Tests08\Sample-Queries-3.txt out-3.xml
```

> Note that `Sample-Source-3.txt` is purposely invalid, meaning all the queries in `Sample-Queries-3.txt` should return no results.

#### AutoTester arguments
AutoTester accepts three arguments:

1. The name of the file containing the SIMPLE source
    - This file contains the SIMPLE source code that will be analyzed in test format.
2. The name of the file containing the queries
    - This file contains the queries that have to be evaluated based on the SIMPLE source code. The file may contain multiple queries, one after another. Each query is defined using 5 lines, as shown in the following example:
```
1 - comment
while w;
Select w such that Parent(w, 7)
None
5000
```
    - A description of what each line represents is as follows:
        - Query id and comment
        - declaration for PQL query
        - PQL query
        - expected result of the query
        - time limit (in ms)
    - Note that the AutoTester will compare the results returned by your SPA with the expected results shown in the file and decide if the query passed (total match) or failed (results are not matched).
3. The output file to store the results of the testing


### Running AutoTester from executable
> AutoTester is designed to be run from the command line. Double clicking the executable will not execute any tests.

To run the AutoTester from a command prompt, navigate to the parent directory of `AutoTester.exe` at `$SOURCE\Code08\Release` from the command line and input:

```bash
> AutoTester ..\..\Tests08\Sample-Source.txt ..\..\Tests08\Sample-Queries.txt ..\..\Tests08\out.xml
```

The above command runs AutoTester with the supplied tests located at `$SOURCE\Tests08`. The two alternate test suites can also be run in the same manner:

```bash
> AutoTester ..\..\Tests08\Sample-Source-2.txt ..\..\Tests08\Sample-Queries-2.txt ..\..\Tests08\out-2.xml
```

```bash
> AutoTester ..\..\Tests08\Sample-Source-3.txt ..\..\Tests08\Sample-Queries-3.txt ..\..\Tests08\out-3.xml
```


Similar to running AutoTester in VS2017, the respective `out.xml` files will be generated in `$SOURCE\Code08\Release` which can be opened in Firefox to view the results of the system tests. The supplied test cases should all pass.

### Executing the GUI
The GUI can either be run straight from VS2017 or via the executable `Gui.exe` located at `$SOURCE\Release\`.

The GUI is able to:

- load SIMPLE source files
- specify input query and
- display the result in a textbox