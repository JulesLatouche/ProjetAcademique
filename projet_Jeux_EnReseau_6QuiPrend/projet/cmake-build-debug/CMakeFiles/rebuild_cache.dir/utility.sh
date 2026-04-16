set -e

cd /home/basti/infol3/SystemEtReseau/projet/cmake-build-debug
/cygdrive/c/Users/basti/AppData/Local/JetBrains/CLion2025.3/cygwin_cmake/bin/cmake.exe --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
