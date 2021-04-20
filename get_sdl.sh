if [[ $OSTYPE == linux* ]]; then
  export sdlpath="/usr/local/include/SDL2"
  export sdlparams="-L$sdlpath -lSDL2main -lSDL2"
  echo 'penguins found'
elif [[ $OSTYPE == darwin* ]]; then
  export sdlpath="$HOME/Library/Frameworks/SDL2.framework/Headers"
  export sdlparams="-I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2"
  echo 'mac mac mac mac mac....'
fi
