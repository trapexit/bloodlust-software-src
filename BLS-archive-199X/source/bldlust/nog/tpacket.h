void InitPacket();
void ResetPacket();
void TerminatePacket();
void SendStandardPacket(unsigned char type,unsigned short data);
void SetPacketFunc(int num, void(*handler)());
//void SendPosPacket(unsigned short x, unsigned short y);
void SendPosPacket(signed char  x, unsigned char y);
void SendFramePacket(unsigned char  cm, unsigned short fp,unsigned short stat);
void SendChatPacket(char *s);
void SendEffectPacket(signed char  x, unsigned char y,unsigned char t);
//void SendAttackPacket(unsigned char  cma,unsigned char  cmb, unsigned short astat,unsigned short bstat,unsigned char f,unsigned char energy,
//       signed char  x, unsigned char y);
//void SendAttackPacket(unsigned char  cma, unsigned short fpa,unsigned short stata,
//         unsigned char  cmb, unsigned short fpb,unsigned short statb);
void SendAttackPacket(unsigned char  cma, unsigned short fpa,unsigned short stata,signed char  x, unsigned char y,
         unsigned char  cmb, unsigned short fpb,unsigned short statb,signed char  xb, unsigned char yb);
void PacketComFunc();         

//standard packet for use with most xfers
typedef struct packet
{
 public:
  unsigned char type;       //type of packet
  unsigned short data;

  unsigned char CRC;        //crc for all packet
} packet;

//standard packet for use with most xfers
typedef struct pospacket
{
 public:
  unsigned char type;       //type of packet
  signed char x;
  unsigned char y;
  unsigned char CRC;        //crc for all packet
} pospacket;


typedef struct framepacket
{
 public:
  unsigned char type;       //type of packet
  unsigned char cm;
  unsigned short frameptr;
  unsigned short stat;

  unsigned char CRC;        //crc for all packet
} framepacket;


typedef struct chatpacket
{
 public:
  unsigned char type;       //type of packet
   char s[100];
  unsigned char CRC;        //crc for all packet
} chatpacket;



typedef struct attackpacket
{
 public:
  unsigned char type;       //type of packet
  unsigned char cma;
  unsigned short frameptra;
  unsigned short stata;
  signed char xa;
  unsigned char ya;

  unsigned char cmb;
  unsigned short frameptrb;
  unsigned short statb;
  signed char xb;
  unsigned char yb;

  

  unsigned char CRC;        //crc for all packet
} attackpacket;



extern unsigned char prbuf[256]; //temp packet storage
extern unsigned char psbuf[256]; //temp packet storage
extern int packeterror;
extern int packetsends,packetrecvs;

extern int multiplay,multiinstalled,multiconnected,multimaster;
extern int pe[4];
extern int pbytes;
extern unsigned int packettime;

