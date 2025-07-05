#make a bash for build project
#!/bin/bash
# remove build directory if it exists
if [ -d "build" ]; then
    rm -rf build
fi
# Set the build directory
BUILD_DIR="build"
# Create the build directory if it doesn't exist
mkdir -p $BUILD_DIR
# Navigate to the build directory
cd $BUILD_DIR
# Run CMake to configure the project
cmake ..
# Check if CMake was successful
if [ $? -ne 0 ]; then
    echo "CMake configuration failed. Please check the output for errors."
    exit 1
fi
# Build the project using make
make
# Check if the build was successful
if [ $? -ne 0 ]; then
    echo "Build failed. Please check the output for errors."
    exit 1
fi
# Print a success message
echo "Build completed successfully. You can run the executable in the build directory."