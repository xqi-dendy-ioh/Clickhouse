add_library (bridge
    IBridge.cpp
)

target_link_libraries (bridge PRIVATE daemon dbms)
