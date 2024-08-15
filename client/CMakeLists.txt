
# 将源代码添加到此项目的可执行文件。
add_executable(client
    src/main.cpp
    src/mainwindow.h src/mainwindow.cpp src/mainwindow.ui
)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

target_link_libraries(client PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
