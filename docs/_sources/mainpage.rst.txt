Documentation for Co\[ro\]nan
=====================================

Co\[ro\]nan is an example C++20 Qt project using `conan <https://conan.io/>`_ and `CMake <https://cmake.org/>`_.
It reads the current data of Corona (Covid-19) cases for a country from the `The About Corona Covid-19 API <https://about-corona.net/documentation>`_.

*The About Corona Covid-19* provides statistics via REST API from The `World Health Organization Situation Reports <https://www.who.int/emergencies/diseases/novel-coronavirus-2019/situation-reports>`_,  `Johns Hopkins CSSE <https://coronavirus.jhu.edu/map.html>`_, `The U.S. Department of Health & Human Services <https://www.hhs.gov/>`_, `The National Health Commission of the People’s Republic of China <http://en.nhc.gov.cn/>`_, `The European Centre for Disease Prevention and Control <https://www.ecdc.europa.eu/en>`_, and `China CDC Weekly <http://weekly.chinacdc.cn/news/TrackingtheEpidemic.htm>`_.

.. image:: images/Screenshot-qt.png
  :alt: Screenshot

Development
-----------

To build the project you need python and a recent `conan version <https://conan.io/>`_, a recent version of `CMake <https://cmake.org/>`_ (>= 3.22), a modern C++ compiler (C++20 support) and `Qt 6 <https://www.qt.io/>`_ with `QtCharts <https://doc.qt.io/qt-6/qtcharts-index.html>`_.

To install all python requirements for documentation generation, build and coverage generation (including conan and cmake) use:


.. code-block:: bash

  pip3 install -r py-requirements.txt


How to build
^^^^^^^^^^^^

You have the following options to build the project

**Use build script**:

In a linux shell you can build the project with the provided ``build.sh`` script. E.g. to build in folder ``build`` execute:

.. code-block:: bash

  ./build.sh build


See ``./build.sh -h`` for more options

**CMake**:

You can also build it yourself using CMake:

* Install dependencies with conan

.. code-block:: bash

  conan install . --build=missing --settings=build_type=Debug

Because the conan `cmake_layout <https://docs.conan.io/2/reference/tools/cmake/cmake_layout.html#>`_ and the `CMakeDeps <https://docs.conan.io/2/reference/tools/cmake/cmakedeps.html>`_ generator is used, the necessary cmake files used with ``find_packages()`` are generated into ``build\<build_type>\generators``. ``CMAKE_PREFIX_PATH`` is adapted accordingly in the root ``CMakeLists.txt``.

* Without CMake Presets

.. code-block:: bash

  cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug
  cmake --build ./build


* With CMake Presets

.. code-block:: bash

  cmake -S . --preset=linux-debug-ninja
  cmake --build --preset=ninja-debug


or any other ``CMakePresets``.

Source Code formatting
^^^^^^^^^^^^^^^^^^^^^^

For source code formatting `clang-format <https://clang.llvm.org/docs/ClangFormat.html>`_ for C++ files and `cmake-format <https://pypi.org/project/cmake-format/>`_ for the CMake files are used. Run ``format_source_files.sh`` to format all C++ and CMake files.

*Note: Delete the build directory before formatting, otherwise all CMake files in the build folder will be formatted as well, what may take a while.*

CMake options
^^^^^^^^^^^^^

* ``DOWNLOAD_QT6``: Download and build Qt6 otherwise use installed. _Default_: ```ON```
* ``ENABLE_TESTING``: Build (and run) unittests. _Default_: ```ON```
* ``ENABLE_BUILD_WITH_TIME_TRACE``: Enable `Clang Time Trace Feature <https://www.snsystems.com/technology/tech-blog/clang-time-trace-feature>`_. *Default: ```OFF```*
* ``ENABLE_PCH``: Enable `Precompiled Headers <https://en.wikipedia.org/wiki/Precompiled_header>`_. *Default: ``OFF``*
* ``ENABLE_CACHE``: Enable caching if available, e.g. `ccache <https://ccache.dev/>`_ or `sccache <https://github.com/mozilla/sccache>`_. *Default: ``ON``*
* ``ENABLE_COVERAGE``: Enable coverage reporting for gcc/clang. *Default: ```OFF```*
* ``ENABLE_DOXYGEN``: Enable doxygen documentation build (`doxygen <https://www.doxygen.nl/index.html>`_ with `graphviz dot <https://graphviz.org/>`_ must be installed). *Default: ```ON```*
* ``ENABLE_SPHINX``: Enable sphinx documentation build (`Sphinx <https://www.sphinx-doc.org/>`_ with `Breath <https://breathe.readthedocs.io/>`_ and `sphinx-mdinclude <https://github.com/omnilib/sphinx-mdinclude>`_ must be installed). *Default: ```ON``` if Sphinx is installed ```OFF``` otherwise.*
* ``ENABLE_SANITIZER_ADDRESS``: Enable `address sanitizer <https://clang.llvm.org/docs/AddressSanitizer.html>`_. *Default: ```OFF```*
* ``ENABLE_SANITIZER_LEAK``: Enable `leak sanitizer <https://clang.llvm.org/docs/LeakSanitizer.html>`_. *Default: ```OFF```*
* ``ENABLE_SANITIZER_UNDEFINED_BEHAVIOR``: Enable `undefined behavior sanitizer <https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html>`_. *Default: ```OFF```*
* ``ENABLE_SANITIZER_THREAD``: Enable `thread sanitizer <https://clang.llvm.org/docs/ThreadSanitizer.html>`_. *Default: ```OFF```*
* ``ENABLE_SANITIZER_MEMORY``: Enable `memory sanitizer <https://clang.llvm.org/docs/MemorySanitizer.html>`_. *Default: ```OFF```*
* ``ENABLE_IPO``: Enable intermodular optimization, aka `Link Time Optimization (LTO) <https://llvm.org/docs/LinkTimeOptimization.html>`_. *Default: ```OFF```*
* ``ENABLE_CPPCHECK``: Enable static analysis with `cppcheck <http://cppcheck.sourceforge.net/>`_. *Default: ```ON``` if cppcheck is installed ```OFF``` otherwise.*
* ``ENABLE_CLANG_TIDY``: Enable static analysis with `clang-tidy <https://clang.llvm.org/extra/clang-tidy/>`_. *Default: ```ON``` if clang-tidy is installed ```OFF``` otherwise.*
* ``ENABLE_INCLUDE_WHAT_YOU_USE``: Enable static analysis with `include-what-you-use <https://include-what-you-use.org/>`_. *Default: ```ON``` if include-what-you-use is installed ```OFF``` otherwise.*
* ``ENABLE_LINK_WHAT_YOU_USE``: Enable *link what you use* cmake built-in static analysis. *Default: ```OFF```.*
* ``WARNINGS_AS_ERRORS``: Treat compiler warnings as errors. _Default: *Default: ```ON```.*

Pre-Commit Hooks
----------------

The following `Pre-commit-hooks <https://github.com/pre-commit/pre-commit-hooks>`_ are used to check and format files before committing:

* ``check-added-large-files``: Prevent giant files (> 500kB) from being committed.
* ``fix-byte-order-marker``: Forbid files which have a UTF-8 byte-order marker
* ``check-case-conflict``: Check for files with names that would conflict on a case-insensitive filesystem like MacOS HFS+ or Windows FAT.
* ``check-merge-conflict``: Check for files that contain merge conflict strings.
* ``check-symlinks``: Checks for symlinks which do not point to anything.
* ``check-yaml``: Attempts to load all yaml files to verify syntax.
* ``check-json``: Attempts to load all json files to verify syntax.
* ``check-toml``: Attempts to load all TOML files to verify syntax.
* ``check-executables-have-shebangs``: Checks that non-binary executables have a proper shebang.
* ``end-of-file-fixer``: Makes sure files end in a newline and only a newline.
* ``mixed-line-ending``: Replaces or checks mixed line ending.
* ``trailing-whitespace``: Trims trailing whitespace in markdown
* ``clang-format``: Format C++ files using clang-format
* ``cmake-format``: Format CMake files using cmake-format
* ``yamllint``: Linter for yaml files
* ``yamlfmt``: Format yaml files

Please install pre-commit hooks before committing any code.

direnv & Nix
------------

If you have `direnv <https://direnv.net/>`_ and `nix <https://nixos.org/>`_ installed everything should be setup to start upon entering the project folder. gcc (gcc10) is set as default C and C++ compiler. If you prefer clang (clang_11) change it in ``.envrc``

When using nix. The most resent cmake is available and therefore configure and build `CMakePresets <https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html>`_ can be used.
