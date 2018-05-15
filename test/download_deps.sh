DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

git clone --progress --verbose https://github.com/Senspark/cocos2d-x-lite $DIR/cocos2d
sh test/cocos2d/prebuilt/libs/android/extract_libs.sh