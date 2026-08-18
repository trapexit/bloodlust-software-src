
#ifndef CONFIG_H
#define CONFIG_H

#define CONFIGVERSION 0x100

#define CFG_NOSOUND 0
#define CFG_SB      1
#define CFG_SB16    2
#define CFG_SBAWE   3


//structure defining configuration file
struct config
{
 int version; //version of the config file
 int crc;

 //sound blaster settings
 int soundcard;
 int sbport;
 int sbirq;
 int sbdma;
 int sbdma16;

 //input settings
 int pinput[4]; //player input devices
 inputdevicesettings ids;

};

config *new_config();
config *load_config(char *file);
void save_config(char *file,config *c);

extern config *cfg; //standard config file

#endif
