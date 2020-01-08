add_executable(onixs_nats_pub connection.h publisher.h main.cpp)
target_link_libraries(onixs_nats_pub ${NATS_LIB} ${CME_MDH_LIB})
