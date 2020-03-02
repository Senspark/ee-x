DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
wget https://digitalocean.cocos2d-x.org/Cocos2D-X/cocos2d-x-3.17.2.zip -O cocos2d.zip
chmod a+x cocos2d.zip
unzip ./cocos2d.zip -d ./deps 2>&1 >/dev/null
mv ./deps/cocos2d-x-3.17.2 ./test/cocos2d
