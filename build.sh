echo '------- building -------'
. $(pwd)/get_sdl.sh
echo $sdlpath
gcc --verbose -Wall -std=c99 ./src/*.c -o game $sdlparams 
