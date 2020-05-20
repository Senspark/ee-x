SRC=$(printf "target_sources(\${PROJECT_NAME} PRIVATE\n$(find . -name "*.cpp" | sed "s|^|  $PWD\/|")\n)\n");
HEADERS=$(printf "target_include_directories(\${PROJECT_NAME} PRIVATE\n$(find . -type d | sed "s|^|  $PWD\/|")\n)\n");
echo "$SRC" | cmp -s sourcelist.cmake - || echo "$SRC" > sourcelist.cmake
echo "$HEADERS" | cmp -s includelist.cmake - || echo "$HEADERS" > includelist.cmake