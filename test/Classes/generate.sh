SRC=$(printf "target_sources(\${PROJECT_NAME} PRIVATE\n$(find . -name "*.cpp" | sed "s|^|  $PWD\/|")\n)\n");
echo "$SRC" | cmp -s sourcelist.cmake - || echo "$SRC" > sourcelist.cmake