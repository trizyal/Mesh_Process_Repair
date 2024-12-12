# University of Leeds 2024-25
# COMP 5893M Modelling and Animation
# Assignment 1

build instructions:
    - run 'make' in each of the subdirectories to build the executables.
    - run 'make clean' in each of the subdirectories to clean the executables.



Directories:
    utilities
        - this directory has the source code for the Mesh class and the utility functions.
        - used as a library for the other tasks.

    face2faceindex
        - this directory contains the source code and executable Task 1 (a).
        - the output files are stored in the 'facefiles' directory in the root directory (assignment).

    faceindex2directededge
        - this directory contains the source code and executable Task 1 (b).
        - the input "*.face" files are stored in the 'facefiles' directory in the root directory (assignment).
        - the output files are stored in the 'directededgefiles' directory in the root directory (assignment).

    manifoldtesting
        - this directory contains the source code and executable Task 2.
        - file 'manifold_test_results.txt' contains the results of the manifold testing.
        - the input "*.tri" files are in ../handout_models directory.
        - this program saves the non-manifold edges and vertices in corresponding files for the input '.tri' file
            in the 'manifoldtestresults' directory in the root directory (assignment).

    eulerformulacalculation
        - this directory contains the source code and executable Task 3.
        - the input "*.tri" files are in ../handout_models directory.
        - this program prints out the genus of the input '.tri' file.

    meshrepair
        - this directory contains the source code and executable Task 4.
        - the input "*.tri" files are in ../handout_models directory.
        - the output files are stored in the 'trifiles' directory in the root directory (assignment).
        - the output repaired '.tri' files are suffixed with '_repaired'.

    facefiles
        - this directory contains the input "*.face" files for Task 1.

    directededgefiles
        - this directory contains the output "*.directededge" files for Task 1.

    manifoldtestresults
        - this directory contains .txt files for each .tri file tested and stores the non-manifold edges and vertices.

    trifiles
        - this directory contains the output repaired "*.tri" files for Task 4.
