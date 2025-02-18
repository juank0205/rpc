add_library(${PROJECT_NAME}_lib STATIC 
	httpClient.cpp
	inputManager.cpp
	application.cpp
)

target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
