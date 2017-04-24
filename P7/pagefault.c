#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"

#define RESIDENTSETSIZE 24

extern char *base;
extern int framesbegin;
extern int idproc;
extern int systemframetablesize;
extern int ptlr;

extern struct SYSTEMFRAMETABLE *systemframetable;
extern struct PROCESSPAGETABLE *ptbr;
extern struct PROCESSPAGETABLE *gprocesspagetable;
int vmadresstbl[16];
char page_in [4096], page_out[4096];

int getfreeframe();
int escribir_mem(int frame, int vframe, int mod);
int leer_mem(int pag_del_proceso, int pag_a_expulsar);
int getLRU();



// Rutina de fallos de página

int pagefault(char *vaddress)
{
    int i;
    int frame, vframe;
    int pag_a_expulsar;
    int pag_del_proceso;

    // Calcula la página del proceso
    pag_del_proceso=(int) vaddress>>12;
    // Cuenta los marcos asignados al proceso
    i=countframesassigned();
  
    // Busca un marco libre en el sistema
    if(i >= 2)
    {
    pag_a_expulsar = getLRU();  
    //checar si la pagina que esta solicitando memoria ya tiene un frame
    //si no, buscar un virtual disponible
    leer_mem(pag_del_proceso, pag_a_expulsar);  
    if(processpagetable[pag_del_proceso].framenumber != -1){    
        vframe = processpagetable[pag_del_proceso].framenumber;
    }else
        vframe = getfreevframe();
    frame = processpagetable[pag_a_expulsar].framenumber;
        systemframetable[frame].assigned=0;
    //printf("exp: %d\nin:%d\nproc: %d\nframe:%d\nvframe:%d\n", pag_a_expulsar, pag_del_proceso, idproc, frame, vframe);
    //fflush(stdin);
        escribir_mem(frame, vframe, processpagetable[pag_a_expulsar].modificado);

    //printf("###%s\n", systemframetable[0].paddress);
    processpagetable[pag_a_expulsar].framenumber = vframe;
        processpagetable[pag_a_expulsar].modificado=0;
    processpagetable[pag_a_expulsar].presente = 0;
    //return(-1); // Regresar indicando error de memoria insuficiente
    }else{
        frame=getfreeframe();
    }
    processpagetable[pag_del_proceso].presente=1;
    processpagetable[pag_del_proceso].modificado=0;
    processpagetable[pag_del_proceso].framenumber=frame;

    

    return(1); // Regresar todo bien
}


int getfreeframe()
{
    int i;
    // Busca un marco libre en el sistema
    for(i=0;i<systemframetablesize;i++)
        if(!systemframetable[i].assigned)
        {
            systemframetable[i].assigned=1;
        vmadresstbl[i] = 8*4096*i;
            break;
        }
    if(i<systemframetablesize){
        systemframetable[i].assigned=1;
    }else
        i=-1;
    return(i);
}

int getfreevframe()
{
    int i;
    // Busca un marco libre en el sistema
    for(i=8;i<16;i++)
        if(!systemframetable[i].assigned)
        {
        systemframetable[i].assigned=1;
        vmadresstbl[i] = 8*4096*i;
            break;
        }
    if(i<16){
        systemframetable[i].assigned=1;
    }else
        i=-1;
    return(i);
}

int getLRU()
{
    
    int i, pag = -1;
    for(i=0;i<processpagetablesize;i++)
        if(processpagetable[i].presente)
        {
            if(pag == -1 || processpagetable[i].tlastaccess < processpagetable[pag].tlastaccess)
        pag = i;
        }
    return(pag);
}


int escribir_mem(int frame, int vframe, int mod){
    off_t offset_orig, offset_dest, offset_temp;
    int valid, fd;
    //Calcula el offset dependiendo del frame que se pida guardar
    offset_orig = vmadresstbl[frame];
    offset_dest = vmadresstbl[vframe];
    if(mod){    
        //Abre el archivo
        fd = open("swap", O_RDWR, S_IRWXU);
        if(fd == -1){
            printf("erroropen");
            return -2;
        }
        //Mueve el offset en el archivo
        lseek(fd, offset_orig, SEEK_SET);
    

        //Escribe en el archivo
        valid = write(fd, page_out, sizeof(page_out));
            close(fd);
        if(valid == -1){
            perror("escribir");
            return -1;
        }
    }
    //swap
    offset_temp = offset_orig;
    offset_orig = offset_dest;
    offset_dest = offset_temp;
    //strcpy( systemframetable[frame].paddress, );
    return 1;
}



int leer_mem(int pag_del_proceso, int pag_a_expulsar){
    
    int frame_in, frame_ex, valid, fd;
    off_t offset_in, offset_ex;
    frame_in = processpagetable[pag_del_proceso].framenumber;
    frame_ex = processpagetable[pag_a_expulsar].framenumber;
    offset_in = vmadresstbl[frame_in];
    offset_ex = vmadresstbl[frame_ex];
    fd = open("swap", O_RDWR, S_IRWXU);
    lseek(fd, offset_in, SEEK_SET);
    valid = read(fd, page_in, sizeof(page_in));
    //Si la página está en memoria física
    if (processpagetable[pag_a_expulsar].modificado){
        strcpy(page_out, systemframetable[frame_ex].paddress);
        return 1;
    }
    //Si la página está en memoria virtual
    lseek(fd, offset_ex, SEEK_SET);
    valid = read(fd, page_out, sizeof(page_out));
    close(fd);
    if(valid == -1){
    perror("errorread");    
    return -1;
    }
    return 1;
}