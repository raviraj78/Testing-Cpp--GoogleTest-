if(EXISTS "/mnt/c/Users/40038384/OneDrive - LTTS/Desktop/Gtest/build/your_tests[1]_tests.cmake")
  include("/mnt/c/Users/40038384/OneDrive - LTTS/Desktop/Gtest/build/your_tests[1]_tests.cmake")
else()
  add_test(your_tests_NOT_BUILT your_tests_NOT_BUILT)
endif()