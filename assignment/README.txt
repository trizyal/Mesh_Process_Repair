# University of Leeds 2024-25
# COMP 5893M Modelling and Animation
# Assignment 1




Build instructions:


    - in the 'assignment' directory run: 
        [userid@machine assignment]$ cmake .
        [userid@machine assignment]$ make
        this should build each of the executable needed for the assignment
    
    - TASK 1
        run:
        [userid@machine assignment]$ ./face2faceindex/face2faceindex {input.tri}
        [userid@machine assignment]$ ./face2faceindex/face2faceindex ../handout_models/cube.tri
        the output .face files are saved in the facefiles directory.

        [userid@machine assignment]$ ./faceindex2directededge/faceindex2directededge {input.face}
        [userid@machine assignment]$ ./faceindex2directededge/faceindex2directededge facefiles/cube.face
        the output .diredge files are saved in diredgefiles directory.


    - TASK 2
        run:
        [userid@machine assignment]$ ./manifoldtesting/manifoldtesting {input.tri}
        [userid@machine assignment]$ ./manifoldtesting/manifoldtesting ../handout_models/cube.tri
        detailed output of manifold test are stored in manifold_test_results directory.
        and file "manifold_test_results.txt" haev the list of all the meshes with manifold test results, a edge/vertex that fails and the genus.


    -TASK 3
        [userid@machine assignment]$ ./eulerformulacalculation/eulerformulacalculation {input.tri}
        [userid@machine assignment]$ ./eulerformulacalculation/eulerformulacalculation ../handout_models/torus.tri
        this will print the genus in the terminal
        and file "manifold_test_results.txt" have the list of all the meshes with manifold test results, a edge/vertex that fails and the genus.


    - TASK 4
        run:
        [userid@machine assignment]$ ./meshrepair/meshrepair {input.tri}
        [userid@machine assignment]$ ./meshrepair/meshrepair ../handout_models/hamish_40k_edit_2.tri
        the repaired .tri file is saved in trifiles directory with the suffix '_repaired' 
        we also save the .obj file in the objfiles with the same suffix.







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
