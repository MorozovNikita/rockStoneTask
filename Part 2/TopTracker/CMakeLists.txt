
add_library(TopTracker)
target_sources(TopTracker
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    TopTracker.ixx
)
