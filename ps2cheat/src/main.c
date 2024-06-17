#include "include/main.h"

/*
 * Many features taken from Berion's Unfinished Artemis GUI and The Unofficial CL-LiveDebug v4
 */

/*
 *	Main function
 */
int main(int argc, char *argv[]) {
	
	
	SifInitRpc(0);
	
	TimerInit();

        init_scr();
        scr_printf("Loading modules...\n");
	
	LoadModules();

	StartGame();
	
	return 0;
}


int LoadModules(void) {
	int ret = 1;

	SifLoadFileInit();
	SifInitRpc(0);

	// Load modules off ROM
	ret += SifLoadModule("rom0:SIO2MAN", 0, NULL);
	wLoad++; // Draw_WaitMenu(wLoad, wMax);
	ret += SifLoadModule("rom0:MCMAN", 0, NULL);
	wLoad++; // Draw_WaitMenu(wLoad, wMax);
	ret += SifLoadModule("rom0:MCSERV", 0, NULL);
	wLoad++; // Draw_WaitMenu(wLoad, wMax);
	
	
	// QWERTY Module from CogSwap
	// loadmodulemem(iopmod, 1865, 0, 0);
	// wLoad++; // Draw_WaitMenu(wLoad, wMax);
	
	SifLoadFileExit();
	
	return ret;
}

/*
 * load an elf file using embedded elf loader
 * this allow to fix memory overlapping problem
 */
void LoadELF(char *elf_path, int code_len) {
	
	u8 *boot_elf;
	elf_header_t *boot_header;
	elf_pheader_t *boot_pheader;
	int i = 0;
	char *args[1];
	
	if (elf_path == NULL) return;

	
	/* The loader is embedded */
	boot_elf = (u8 *)&elf_loader;

	/* Get Elf header */
	boot_header = (elf_header_t *)boot_elf;

	/* Check elf magic */
	if ((*(u32*)boot_header->ident) != ELF_MAGIC) 
    {
        scr_printf("Invalid ELF header!\n");
		return;
    }

	/* Get program headers */
	boot_pheader = (elf_pheader_t *)(boot_elf + boot_header->phoff);
	
	/* Scan through the ELF's program headers and copy them into appropriate RAM
	 * section, then pad with zeros if needed.
	 */
	for (i = 0; i < boot_header->phnum; i++) {
		if (boot_pheader[i].type != ELF_PT_LOAD)
			continue;

		memcpy(boot_pheader[i].vaddr, boot_elf + boot_pheader[i].offset, boot_pheader[i].filesz);

		if (boot_pheader[i].memsz > boot_pheader[i].filesz)
			memset(boot_pheader[i].vaddr + boot_pheader[i].filesz, 0, boot_pheader[i].memsz - boot_pheader[i].filesz);
	}

	if (code_len != 0) {
		
        scr_printf("Loading cheat engine...\n");

		/* Setup engine */
		DI();
		ee_kmode_enter();
			memcpy((u32*)(int)EngineAddr, NCEngine, sizeof(NCEngine));						/* Install the NetCheat Engine into the kernel */
			memset((u32*)CodesAddr, 0 , code_len + 8);									/* Clear code area */
			memcpy((u32*)(CodesAddr + 8), (u32*)0x000F0000, code_len);					/* Install the codes into the kernel */
			*(u32*)((int)EngineAddr - 0x18) = (CodesAddr + 0x10); 							/* Pointer to initial code */
			*(u32*)((int)EngineAddr - 0x10) = (CodesAddr + 0x10); 							/* Pointer to current code */
			*(u32*)HookAddr = HookValue;												/* Install the kernel hook */
		ee_kmode_exit();
		EI();

        scr_printf("Done!\n");
	}
	
	/* Cleanup before launching elf loader */
	CleanUp();
	
	args[0] = elf_path;
	
	/* Execute Elf Loader */
	ExecPS2((void *)boot_header->entry, 0, 1, args);
}

//Taken from uLaunchElf
int     GetStringFromCNF(u8 **CNF_p_p, u8 **name_p_p, u8 **value_p_p) {
        
        u8 *np, *vp, *tp = *CNF_p_p;

start_line:
        while((*tp<=' ') && (*tp>'\0')) tp+=1;  //Skip leading whitespace, if any
        if(*tp=='\0') return 0;                         //but exit at EOF
        np = tp;                                //Current pos is potential name
        if(*tp<'A')                             //but may be a comment line
        {                                       //We must skip a comment line
                while((*tp!='\r')&&(*tp!='\n')&&(*tp>'\0')) tp+=1;  //Seek line end
                goto start_line;                    //Go back to try next line
        }

        while((*tp>='A')||((*tp>='0')&&(*tp<='9'))) tp+=1;  //Seek name end
        if(*tp=='\0') return -1;                        //but exit at EOF

        while((*tp<=' ') && (*tp>'\0'))
                *tp++ = '\0';                       //zero&skip post-name whitespace
        if(*tp!='=') return -2;                 //exit (syntax error) if '=' missing
        *tp++ = '\0';                           //zero '=' (possibly terminating name)

        while((*tp<=' ') && (*tp>'\0')          //Skip pre-value whitespace, if any
                && (*tp!='\r') && (*tp!='\n')           //but do not pass the end of the line
                )tp+=1;                                                         
        if(*tp=='\0') return -3;                        //but exit at EOF
        vp = tp;                                //Current pos is potential value

        while((*tp!='\r')&&(*tp!='\n')&&(*tp!='\0')) tp+=1;  //Seek line end
        if(*tp!='\0') *tp++ = '\0';             //terminate value (passing if not EOF)
        while((*tp<=' ') && (*tp>'\0')) tp+=1;  //Skip following whitespace, if any

        *CNF_p_p = tp;                          //return new CNF file position
        *name_p_p = np;                         //return found variable name
        *value_p_p = vp;                        //return found variable value
        return 1;                               //return control to caller
}       //Ends GetStringFromCNF


// Taken from IGmassdumper. Parses SYSTEM.CNF

int ParseSystemCNF(char *system_cnf, char *boot_path) 
{
        int r, entrycnt, cnfsize;
        u8 *pcnf, *pcnf_start, *name, *value;
        int fd = -1;
        
        fd = open(system_cnf, O_RDONLY);
        if (fd < 0)
                return -1;              

        cnfsize = lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        
        pcnf = (char *)malloc(cnfsize);
        pcnf_start = pcnf;
        if (!pcnf) {
                close(fd);
                return -2;
        }
        
        r = read(fd, pcnf, cnfsize);    // Read CNF in one pass
        if (r != cnfsize)
                return -3;
                
        close(fd);
        pcnf[cnfsize] = 0;                              // Terminate the CNF string
        
        for (entrycnt = 0; GetStringFromCNF(&pcnf, &name, &value); entrycnt++) {
                if (!strcmp(name,"BOOT2"))  // check for BOOT2 entry
                        strcpy(boot_path, value);                       
        }
                        
        pcnf = pcnf_start;
        free(pcnf);
        
        return 1;
}

int StartGame() {
    z = 0;
    char *buffer = (char*)malloc(1024);
    memset((char*)buffer, 0, sizeof(buffer)); /* Clear buffer */
    memset((u32*)0x000F0000, 0, 0x0000FFF0); /* Clear code storage */

    char belf[40];
    int code_len = 0;
    
    scr_printf("Loading codes...\n");

    /// RECIEVE CODES
    
    code_len = sizeof(RacTimer);

    memcpy((u32*)0x000F0000, RacTimer, code_len);
    
    scr_printf("Recieved %i lines of code (%.02f kb)\n", code_len / 8, (float)code_len / 1024);

    /// START GAME
     
    ParseSystemCNF(sys_cnf, belf);
    if( (char*)belf == NULL)
    {
        scr_printf("Could not read system.cnf!\n");
        SleepThread();
    }
    
    scr_printf("Booting %s\n", (char*)belf);
        
    free((char*)buffer);
    LoadELF((char*)belf, code_len);
    // break;
	
	return 0;
}



/*
 * deInit Timers, exit services & clear screen
 */
void CleanUp(void)
{
	TimerEnd();
	
  	SifExitIopHeap();
  	SifLoadFileExit();
  	SifExitRpc();

  	FlushCache(0);
  	FlushCache(2);
	
	// Clear_Screen();
}


/*
 * asm delay function
 */
void delay(int count) {
	int i;
	int ret;
	
	for (i  = 0; i < count; i++) {
		ret = 0x01000000;
		while(ret--)
			asm("nop\nnop\nnop\nnop");
	}
}
