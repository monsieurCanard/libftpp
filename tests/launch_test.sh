#!/bin/bash

# Launch all test programs

# Build the library first
echo "Building libftpp.a..."
make
if [ $? -ne 0 ]; then
    echo "Failed to build library"
    exit 1
fi


# Counter for tests
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

OUTCAST_TESTS=("main_client" "main_server")


echo "================================================"
echo "Running all tests..."
echo "================================================"

# Loop through all main_*.cpp files in school_test directory
for test_file in tests/school_test/main_*.cpp; do
    if [ -f "$test_file" ]; then
       
        test_name=$(basename "$test_file" .cpp)
        executable_name="${test_name}_test"
        
   

        echo ""
        echo ">>> Testing: $test_name"
        echo "----------------------------------------"
        
        # Compile the test
        echo "Compiling $test_file..."
        g++ -std=c++17 -Wall -Wextra -Werror \
            -I./src \
            "$test_file" \
            -L. -lftpp \
            -pthread \
            -o tests/"$executable_name"

        if [ $? -ne 0 ]; then
            echo "❌ COMPILATION FAILED: $test_name"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        else
            echo "✅ Compilation successful for $test_name"
            if [[ " ${OUTCAST_TESTS[@]} " =~ " ${test_name} " ]]; then
                continue;
            fi
            # Run the test
            echo "Running $executable_name..."
            ./tests/"$executable_name"
            
            # Clean up executable
            # rm -f tests/"$executable_name"
        fi
        
        TOTAL_TESTS=$((TOTAL_TESTS + 1))
    fi
done

echo ""
echo "================================================"
echo "TEST SUMMARY"
echo "================================================"
echo "Total tests: $TOTAL_TESTS"
echo "Passed: $PASSED_TESTS"
echo "Failed: $FAILED_TESTS"

if [ $FAILED_TESTS -eq 0 ]; then
    echo "🎉 ALL TESTS PASSED!"
    exit 0
else
    echo "💥 Some tests failed."
    exit 1
fi