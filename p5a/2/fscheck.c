#include <unistd.h>
#include <stdio.h>
#include<string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar; 

// Block 0 is unused.
// Block 1 is super block.
// Inodes start at block 2.
#define ROOTINO 1  // root i-number
#define BSIZE 512  // block size
// File system super block
struct superblock {
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // Number of data blocks
  uint ninodes;      // Number of inodes.
};
#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(uint))
#define MAXFILE (NDIRECT + NINDIRECT)
#define T_DIR  1   // Directory       
#define T_FILE 2   // File
#define T_DEV  3   // Special device
// On-disk inode structure
struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEV only)
  short minor;          // Minor device number (T_DEV only)
  short nlink;          // Number of links to inode in file system
  uint size;            // Size of file (bytes)
  uint addrs[NDIRECT+1];   // Data block addresses
};
// Inodes per block.
#define IPB           (BSIZE / sizeof(struct dinode))
// Block containing inode i
#define IBLOCK(i)     ((i) / IPB + 2)
// Bitmap bits per block
#define BPB           (BSIZE*8)
// Block containing bit for block b
#define BBLOCK(b, ninodes) (b/BPB + (ninodes)/IPB + 3)
// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14
struct dirent {
  ushort inum;
  char name[DIRSIZ];
};
// dirents per block
#define DPB (BSIZE/ sizeof(struct dirent))
struct superblock *sb;
uchar *bmap;
uchar mybmap[BSIZE]={0};
const char *dot = ".";
const char *dotdot = "..";

/* validates 
(1)- block is valid, not out of bound or anything
(2)- block is marked as 0 in the bitmap*/
void
checkbnum(int bn){
    if (bn >= sb->size || bn <= 0){
        //fprintf(stderr,"bad adres is %d\n",bn);
        fprintf(stderr,"bad address in inode.\n");
        exit(1);
    }
    uchar b = mybmap[bn/8];
    if((b >> (bn%8)) % 2){
        fprintf(stderr,"address used more than once.\n");
        exit(1);
    }
}

// rough structure
// unused block | superblock | inode table | bitmap(data) | data blocks
int
main(int argc,char *argv[]){
    if(argc != 2){
        fprintf(stderr,"Usage: ./mkfs filesystemname.img\n");
        exit(1);
    }
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        fprintf(stderr,"image not found.\n");
        exit(1);
    }
    struct stat buff;
    int rc = fstat(fd, &buff);
    assert(rc == 0);
    void *img_ptr = mmap(NULL, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    sb = (struct superblock *) (img_ptr + BSIZE);

    printf("%d %d %d inodes\n", sb->size, sb->nblocks, sb->ninodes);
    mybmap[0] = mybmap[0] | (0x1 << 0); /* marking block 0 */
    mybmap[0] = mybmap[0] | (0x1 << 1); /* marking super blcok */
    
    int istat[sb->ninodes];
    /* for a consistent fs, lncount should all be 0
       index of lncount indicates inum
       value of lncount indicates num of outstanding links
       when we find a inode for a file, lncount[i] -= dip[i].nlink  */
    int lncount[sb->ninodes];
    int i,bnum,j,k;
    for(i = 0; i< sb->ninodes; i++){
        istat[i] = 0;
        lncount[i] = 0;
        bnum = 2+(i/(int)IPB);
        mybmap[bnum/8] = mybmap[bnum/8] | (0x1 << (bnum%8)); /*marking a bit*/   
    }
    
    // I think there is an empty block after inodes and b4 bitmap
    bnum++;
    mybmap[bnum/8] = mybmap[bnum/8] | (0x1 << (bnum%8)); /*marking a bit*/ 
    //this is to mark the bitmap
    bnum++;
    mybmap[bnum/8] = mybmap[bnum/8] | (0x1 << (bnum%8)); /*marking a bit*/ 
    
    struct dinode *dip = (struct dinode *) (img_ptr + 2*BSIZE);
    
    
    if(dip[1].type != T_DIR){
        fprintf(stderr,"root directory does not exist.\n");
        exit(1);
    }
    
    /*
    struct dirent *rootdent = (struct dirent *)(img_ptr+(dip[1].addrs[0])*BSIZE);
    if(dip[1].type != T_DIR || rootdent[0].inum != 1 || rootdent[1].inum != 1 ){
      printf("dot points to inode # %d\n",rootdent[0].inum);
      printf("dotdot points to inode # %d\n",rootdent[1].inum);
      fprintf(stderr,"ERROR: root directory does not exist.\n");
      exit(1);
    }
    */
    void *ptr = img_ptr + (sb->ninodes / IPB + 3) * BSIZE;
    bmap = (uchar *) ptr;
    int gotdots ;
    
    for (i = 0; i< sb->ninodes; i++){ //traverse inodes, i is inode #
        if(dip[i].type < 0 || dip[i].type > 3){
            fprintf(stderr,"bad inode.\n");
            exit(1);
        }
        if( i > 1 && dip[i].type != 0 && !istat[i]){ 
            /*current assumption - DIR inode always come before stuff in that DIR */
            fprintf(stderr,"inode marked use but not found in a directory.\n");
            exit(1);
        }
        if( i > 1 && dip[i].type == 0 && istat[i]){
            fprintf(stderr,"inode referred to in directory but marked free.\n");
            exit(1);
        }
        if(dip[i].type == T_FILE){
            lncount[i] -= dip[i].nlink;
        }
        gotdots = 0;
        bnum = 2+(i/(int)IPB);
        printf("%d type: %d @block %d\n",i, dip[i].type, bnum); 
        printf("\tnlink: %d  size: %d\n",dip[i].nlink,dip[i].size);
        

        for(j=0; j<NDIRECT+1; j++){ // traverse data blocks
            printf("\tDataBlock #: %d ",dip[i].addrs[j]);
            
            /* mybitmap construction*/
            if(j == NDIRECT && dip[i].addrs[j] != 0){ //indirect block
                bnum = dip[i].addrs[j];
                checkbnum(bnum);
                mybmap[bnum/8] = mybmap[bnum/8] | (0x1 << (bnum%8)); /*marking a bit*/
                char *buff = (char *) (img_ptr+(dip[i].addrs[j])*BSIZE);
                uint *indirect;
                indirect = (uint*)buff;
 
                for(k = 0; k < NINDIRECT; k++){
                    if(indirect[k]!=0){
                        bnum = indirect[k];
                        checkbnum(bnum);
                        mybmap[bnum/8] = mybmap[bnum/8] | (0x1 << (bnum%8)); /*marking a bit*/
                    }
                }
            }else if(dip[i].addrs[j] != 0){
                bnum = dip[i].addrs[j];
                checkbnum(bnum); 
                mybmap[bnum/8] = mybmap[bnum/8] | (0x1 << (bnum%8)); /*marking a bit*/
            }
            /* mybitmap construction*/
        
            if(dip[i].type == T_DIR && (dip[i].addrs[j])!=0){ //DIR
                struct dirent *dent = (struct dirent *)(img_ptr+(dip[i].addrs[j])*BSIZE);
                int dots[2]={0};
                int di;
                for(di=0; di < DPB; di++){ //traversing directory entries in an DIR inode
                    if(!dent[di].inum)
                        continue;
                    printf("\tData: %d-%s",dent[di].inum,dent[di].name);
                    if(!strcmp(dent[di].name,dot)){
                        dots[0]=1;
                    }else if(!strcmp(dent[di].name,dotdot)){
                        dots[1]=1;
                        /*current assumption - we already processed parent at this point */
                        if( i>1 && dent[di].inum != istat[i]){
                            fprintf(stderr,"parent directory mismatch.\n");
                            exit(1);
                        }
                        
                    }else{
                        if(dip[dent[di].inum].type == T_DIR && istat[dent[di].inum]){
                            fprintf(stderr,"directory appears more than once in file system.\n");
                            exit(1);
                        }
                        if(dip[dent[di].inum].type == T_FILE)
                            lncount[dent[di].inum]+=1;
                        istat[dent[di].inum] = i;
                    }
                }
                if((dots[0] && dots[1])){
                    gotdots = 1;
                }
                    
            }/*else if(i == 2){ //MOBY Dick
                char *buff = (char *) (img_ptr+(dip[i].addrs[j])*BSIZE);
               
                if(j==NDIRECT){ //we are in indirect block
                    uint *indirect;
                    indirect = (uint*)buff;
                    int k=0;
                    while (k<NINDIRECT && indirect[k]!=0){
                        printf("\t\tIndirectDataBlock #: %d\n",indirect[k++]);
                    }
                }else{ // direct block
                    //printf("\tData: %s\n",buff);
                }
            }*/
            printf("\n");
        }
        printf("\n");
        if(!gotdots && dip[i].type == T_DIR ){
            fprintf(stderr,"directory not properly formatted.\n");
            exit(1);
        }
    }
    
    for (i = 0; i < sb->ninodes; i++){
        if(lncount[i]){
            fprintf(stderr,"bad reference count for file.\n");
            exit(1);
        }
    }

    // acutal bit map 
    printf("actual bitmap\n");
    for (i = 0; i<BSIZE; i++){
        printf("%2x",bmap[i]);
        if((i+1)%32==0)
            printf("\n");
    }
    printf("\n");
    
    // my bit map
    printf("my bitmap\n");
    for (i = 0; i<BSIZE; i++){
        printf("%2x",mybmap[i]);
        if((i+1)%32==0)
            printf("\n");
    }
    printf("\n");
    
    // if my bit map != actual bit map, fs corruption!
    for (i = 0; i<BSIZE; i++){
        if(mybmap[i] > bmap[i]){
            fprintf(stderr,"address used by inode but marked free in bitmap.\n");
            exit(1);
        }else if(mybmap[i] < bmap[i]){
            fprintf(stderr,"bitmap marks block in use but it is not in use.\n");
            exit(1);        
        }
    }
    
    // misc. things for debugging
    printf("inode struct is %d bytes\n",(int)sizeof(struct dinode));
    printf("uint is %d bytes\n",(int)sizeof(uint));
    printf("%d inodes can fit in one block\n",(int) IPB);
    printf("%d directory entries can fit in one block\n", BSIZE/(int)sizeof(struct dirent));

    return 0;
}

