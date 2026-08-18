void GetFileName(char *fullname, char *name);
struct SOUND *ReadWavFile(char *file);


typedef struct block
{
 unsigned num;
 void     *d[];
} block;

typedef struct sblock
{
 unsigned num;
 SOUND     *d[];
} sblock;


//header for each entry in the volume file
typedef struct header
{
char key[4];   //must be "DSL"    
char type;     //type of data
unsigned size; //size of data
char name[9];  //name of data
} header;

//structure for open volume file
typedef struct volumefile
{
int h;            //handle of file
unsigned bytes;  //size of last bytes read

public:
char volname[9]; //name of this file
header hdr;       //last header read/written

//read functions
int open(char *filename); //open volume file
void close();    //close
void reset();
void *read();
void  read(void *t);
//block *readblock();
void ** volumefile::readblock(int *num);
void skip();

//write functions
int create(char *filename);
void writeclose();
void write(void *t,unsigned size,char type,char *name);
void writefile(char *filename);
void writemidi(char *filename);
void writescreen(char *filename);
void writepalette(char *filename);
void writerangeblock(char *path,int num,char type); //1,2,3,...num
void writelistblock(char *listfile,char type); 
void writesound(char *filename);
void writeimage(char *filename);

void writeoldimage(char *filename);
void writebbmfile(char *filename);
void writelbmfile(char *filename);

} volumefile;

extern int vdiagnose;
extern int vwerror;


//volume header
#define V_VOLHEADER 0
//pure data
#define V_RAWDATA   1
//img file new type (with ylist)
#define V_IMAGE 2
//16000hz mono signed data
#define V_SOUND 3
//type 0 midi file
#define V_MIDI  4
//color[256]
#define V_PALETTE 5
//char[64000]
#define V_SCREEN 6
//blockheader + block
#define V_BLOCK 7

//volume header
#define V_VOLEND 0xFF

