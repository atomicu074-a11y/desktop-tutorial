cmake_minimum_required(VERSION 3.10)
project(MiniAmazon)

# Utilisation du C++11 minimum pour votre projet
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# On liste TOUS les fichiers .cpp (pas besoin de lister les .h)
add_executable(mon_app 
    main.cpp 
    Panier.cpp 
    MiniAMAZON.cpp
)

















