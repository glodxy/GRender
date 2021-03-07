set(GRAPH_SRC
        BaseGraph.h
        Graph.h
        Triangle.h
        ScreenGraph/Circle.h
        ScreenGraph/Circle.cpp
        ScreenGraph/Line.h
        ScreenGraph/Line.cpp ScreenGraph/Polygon.cpp ScreenGraph/Polygon.h)

add_library(Graph STATIC ${GRAPH_SRC})