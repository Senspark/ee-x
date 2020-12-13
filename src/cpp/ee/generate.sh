SRC=$(printf "target_sources(\${PROJECT_NAME} PRIVATE\n\
$(find ./ad_colony            -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./ad_mob               -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./adjust               -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./ads                  -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./app_lovin            -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./cocos                -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./core                 -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./facebook             -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./facebook_ads         -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./firebase_crashlytics -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./firebase_performance -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./iron_source          -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./notification         -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./play                 -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./recorder             -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./store                -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./unity_ads            -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
$(find ./vungle               -type f | sed "s/^/  \${CMAKE_CURRENT_LIST_DIR}\//")\n\
)\n");
echo "$SRC" | cmp -s sourcelist.cmake - || echo "$SRC" > sourcelist.cmake