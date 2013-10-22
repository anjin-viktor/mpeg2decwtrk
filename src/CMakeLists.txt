set(PROJECT boolcalc)

project(${PROJECT})

set(SOURCES
    Expr.cpp
    Const.cpp
    Var.cpp
)


add_library(${PROJECT} ${SOURCES}) 
