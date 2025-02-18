add_library(${PROJECT_NAME}_lib STATIC 
	httpServer.cpp
	routesHandler.cpp
	authManager.cpp
	repository.cpp
)

target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
