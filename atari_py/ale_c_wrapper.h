#ifndef __ALE_C_WRAPPER_H__
#define __ALE_C_WRAPPER_H__

#include <ale_interface.hpp>

#ifdef WIN32
	#define DLLEXPORT __declspec(dllexport)
#else
	#define DLLEXPORT 
#endif

extern "C" {
  // Declares int rgb_palette[256]
  #include "atari_ntsc_rgb_palette.h"
  DLLEXPORT ALEInterface *ALE_new() {return new ALEInterface();}
  DLLEXPORT void ALE_del(ALEInterface *ale){delete ale;}
  DLLEXPORT const char *getString(ALEInterface *ale, const char *key){return ale->getString(key).c_str();}
  DLLEXPORT int getInt(ALEInterface *ale,const char *key) {return ale->getInt(key);}
  DLLEXPORT bool getBool(ALEInterface *ale,const char *key){return ale->getBool(key);}
  DLLEXPORT float getFloat(ALEInterface *ale,const char *key){return ale->getFloat(key);}
  DLLEXPORT void setString(ALEInterface *ale,const char *key,const char *value){ale->setString(key,value);}
  DLLEXPORT void setInt(ALEInterface *ale,const char *key,int value){ale->setInt(key,value);}
  DLLEXPORT void setBool(ALEInterface *ale,const char *key,bool value){ale->setBool(key,value);}
  DLLEXPORT void setFloat(ALEInterface *ale,const char *key,float value){ale->setFloat(key,value);}
  DLLEXPORT void loadROM(ALEInterface *ale,const char *rom_file){ale->loadROM(rom_file);}
  DLLEXPORT int act(ALEInterface *ale,int action){return ale->act((Action)action);}
  DLLEXPORT bool game_over(ALEInterface *ale){return ale->game_over();}
  DLLEXPORT void reset_game(ALEInterface *ale){ale->reset_game();}
  DLLEXPORT void getLegalActionSet(ALEInterface *ale,int *actions){
    ActionVect action_vect = ale->getLegalActionSet();
    for(unsigned int i = 0;i < ale->getLegalActionSet().size();i++){
      actions[i] = action_vect[i];
    }
  }
  DLLEXPORT int getLegalActionSize(ALEInterface *ale){return ale->getLegalActionSet().size();}
  DLLEXPORT void getMinimalActionSet(ALEInterface *ale,int *actions){
    ActionVect action_vect = ale->getMinimalActionSet();
    for(unsigned int i = 0;i < ale->getMinimalActionSet().size();i++){
      actions[i] = action_vect[i];
    }
  }
  DLLEXPORT int getMinimalActionSize(ALEInterface *ale){return ale->getMinimalActionSet().size();}
  DLLEXPORT int getFrameNumber(ALEInterface *ale){return ale->getFrameNumber();}
  DLLEXPORT int lives(ALEInterface *ale){return ale->lives();}
  DLLEXPORT int getEpisodeFrameNumber(ALEInterface *ale){return ale->getEpisodeFrameNumber();}
  DLLEXPORT void getScreen(ALEInterface *ale,unsigned char *screen_data){
    int w = ale->getScreen().width();
    int h = ale->getScreen().height();
    pixel_t *ale_screen_data = (pixel_t *)ale->getScreen().getArray();
    memcpy(screen_data,ale_screen_data,w*h*sizeof(pixel_t));
  }
  DLLEXPORT void getRAM(ALEInterface *ale,unsigned char *ram){
    unsigned char *ale_ram = ale->getRAM().array();
    int size = ale->getRAM().size();
    memcpy(ram,ale_ram,size*sizeof(unsigned char));
  }
  DLLEXPORT int getRAMSize(ALEInterface *ale){return ale->getRAM().size();}
  DLLEXPORT int getScreenWidth(ALEInterface *ale){return ale->getScreen().width();}
  DLLEXPORT int getScreenHeight(ALEInterface *ale){return ale->getScreen().height();}

  DLLEXPORT void getScreenRGB(ALEInterface *ale, unsigned char *output_buffer){
    size_t w = ale->getScreen().width();
    size_t h = ale->getScreen().height();
    size_t screen_size = w*h;
    pixel_t *ale_screen_data = ale->getScreen().getArray();

    ale->theOSystem->colourPalette().applyPaletteRGB(output_buffer, ale_screen_data, screen_size);
  }

  DLLEXPORT void getScreenRGB2(ALEInterface *ale, unsigned char *output_buffer){
    size_t w = ale->getScreen().width();
    size_t h = ale->getScreen().height();
    size_t screen_size = w*h;
    pixel_t *ale_screen_data = ale->getScreen().getArray();

    int j = 0;
    for(int i = 0;i < screen_size;i++){
        unsigned int zrgb = rgb_palette[ale_screen_data[i]];
        output_buffer[j++] = (zrgb>>16)&0xff;
        output_buffer[j++] = (zrgb>>8)&0xff;
        output_buffer[j++] = (zrgb>>0)&0xff;
    }
  }

  DLLEXPORT void getScreenGrayscale(ALEInterface *ale, unsigned char *output_buffer){
    size_t w = ale->getScreen().width();
    size_t h = ale->getScreen().height();
    size_t screen_size = w*h;
    pixel_t *ale_screen_data = ale->getScreen().getArray();

    ale->theOSystem->colourPalette().applyPaletteGrayscale(output_buffer, ale_screen_data, screen_size);
  }

  DLLEXPORT void saveState(ALEInterface *ale){ale->saveState();}
  DLLEXPORT void loadState(ALEInterface *ale){ale->loadState();}
  DLLEXPORT ALEState* cloneState(ALEInterface *ale){return new ALEState(ale->cloneState());}
  DLLEXPORT void restoreState(ALEInterface *ale, ALEState* state){ale->restoreState(*state);}
  DLLEXPORT ALEState* cloneSystemState(ALEInterface *ale){return new ALEState(ale->cloneSystemState());}
  DLLEXPORT void restoreSystemState(ALEInterface *ale, ALEState* state){ale->restoreSystemState(*state);}
  DLLEXPORT void deleteState(ALEState* state){delete state;}
  DLLEXPORT void saveScreenPNG(ALEInterface *ale,const char *filename){ale->saveScreenPNG(filename);}

  // Encodes the state as a raw bytestream. This may have multiple '\0' characters
  // and thus should not be treated as a C string. Use encodeStateLen to find the length
  // of the buffer to pass in, or it will be overrun as this simply memcpys bytes into the buffer.
  DLLEXPORT void encodeState(ALEState *state, char *buf, int buf_len);
  DLLEXPORT int encodeStateLen(ALEState *state);
  DLLEXPORT ALEState *decodeState(const char *serialized, int len);

  // 0: Info, 1: Warning, 2: Error
  DLLEXPORT void setLoggerMode(int mode) { ale::Logger::setMode(ale::Logger::mode(mode)); }
}

#endif
