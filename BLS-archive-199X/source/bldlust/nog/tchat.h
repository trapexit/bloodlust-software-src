
typedef struct
{
char type; //type 0
char name[16];
unsigned char wins,losses;
unsigned char master;
//unsigned index;
} idpacket;    

typedef struct
{
char type; //type 2
unsigned index;
unsigned len; //position where keys should go, length of keys in packet
char t[];  //text
} textpacket;    


typedef struct
{
char type; //type 2
unsigned startctr; //position where keys should go, length of keys acked
} ackpacket;    


typedef struct
{
char type; //type 3=quit 4=challenge 5=challengeack
unsigned data;
} actionpacket;


//packet killing the other person
typedef struct
{
char type; //type 7
char data;  //how to kill them 
IPXADDRESS a;  //their address

} killpacket;    



typedef struct netplayer
{
public:
netplayer *next;
netplayer *previous;
IPXADDRESS a; //address
char name[16]; //their name
unsigned char wins,losses;
unsigned char master;
int y; //our y coordinate

int kill; //are we being killed?
int quit; //quitting?

char rchallenge; //received challenge from remote
char schallenge; //sent challenge to remote
unsigned sctimeout; //sent timeout

//RECEIVING------------------
char text[128]; //received text (to be displayed on string
int tlen,slen;  //slen=length of string in chars, tlen=length of string in pixels
int ltime,scrollleft,lw;

unsigned rctr; //counters of chars rec'v and acked by this player

unsigned timeout; //time to timeout


//SENDING----------------------------

/*

              <-------slen---------->
          
 sendbuffer: [//////*****************             ]
              |     |               |
      startctr-     |               |
      sentctr--------               |
      endctr-------------------------

 '/' character that has been physically sent
 '*' character waiting to be sent

 */
 

unsigned endctr;        //counter of characters that have been in buffer
unsigned sentctr;       //counter of characters sent to this player (w/o ack)
unsigned startctr;      //counter of sent characters acknowledged by this player

unsigned sbuflen; //characters in sending buffer
char sendbuf[100];   //buffer for keys to be sent to this player (keys that have not been acknowledged)

unsigned stime;     //last time we SEND a packet

public:
netplayer(IPXADDRESS *addr,char *,int w,int l,int master);
void Draw(int x,int y);
void Process(textpacket *t);
void Ack(unsigned a);
void ScrollLeft();
void OutputChar(char c);
void SendText();
} netplayer;

