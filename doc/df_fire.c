#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <directfb.h>

/***declaration pour les fichiers***/
FILE *fich_video = NULL;
FILE *fich_audio1 = NULL;
FILE *fich_audio2 = NULL;
FILE *fich_pti = NULL;
FILE *fich_avmem = NULL;

/*********initialisation DirectFB**********/
static IDirectFB *dfb = NULL;
static IDirectFBSurface *surf_video = NULL;
static IDirectFBSurface *title = NULL;
static int screen_width  = 0;
static int screen_height = 0;
DFBRectangle r0,r1,r2,r3,r4,r5,r6,r7,r8,r9;
static IDirectFBFont *font_video = NULL;
static IDirectFBFont *font_title = NULL;

/*********declarartion des variables*******/
static char ligne_video[50]="",ligne_pti[82]="";
static char ligne_avmem1[35]="",ligne_avmem2[35]="";
static char conv_char[8];
static int vid_int;
static char rech1[10]="", egale[10]="",rech2[35]="",rech3[35]="";
static char  chaine2[10]="VID1", chaine5[10]="Device";
static char chaine6[30]="Component = AUD[0].DecTask";
static char rech4[30]="",rech5[30]="",rech6[30]="",rech7[30]="";
static char elem1[15],elem2[15];
static char *par;
static int c1,cara,j,n;
static int y1,y2, x1,x2, y3,y4,y5;

/***********variable pour choix d'execution**************/
static int all_init=1;
static int video_init = 0;
static int audio1_init = 0;
static int audio2_init = 0;
static int pti_init = 0;
static int avmem_init = 0;

static long int set_sleep=500000;

#define DFBCHECK(x...)                                         \
  {                                                            \
    DFBResult err = x;                                         \
                                                               \
    if (err != DFB_OK)                                         \
      {                                                        \
        fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ ); \
        DirectFBErrorFatal( #x, err );                         \
      }                                                        \
  }

int main (int argc, char **argv)
{
  DFBFontDescription font_video_dsc;
  DFBFontDescription font_title_dsc;
  DFBSurfaceDescription dsc;
  DFBCHECK (DirectFBInit (&argc, &argv));
  DFBCHECK (DirectFBCreate (&dfb));
  DFBCHECK (dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN));
  dsc.flags = DSDESC_CAPS;
  dsc.caps  = DSCAPS_PRIMARY ;
  DFBCHECK (dfb->CreateSurface( dfb, &dsc, &surf_video ));
  DFBCHECK (dfb->CreateSurface( dfb, &dsc, &title ));
  DFBCHECK (surf_video->GetSize (surf_video, &screen_width, &screen_height));
  font_video_dsc.flags = DFDESC_HEIGHT;
  font_video_dsc.height = 11;
  font_title_dsc.flags = DFDESC_HEIGHT;
  font_title_dsc.height = 12;
  DFBCHECK (dfb->CreateFont (dfb, "/root/amin/decker.ttf", &font_video_dsc, &font_video));
  DFBCHECK (dfb->CreateFont (dfb, "/root/amin/decker.ttf", &font_title_dsc, &font_title));
  DFBCHECK (surf_video->Clear(surf_video, 0,0,0,0));

  DFBCHECK (surf_video->SetFont (surf_video, font_video));
  DFBCHECK (title->SetFont (title, font_title));
    
    for (n = 1; n < argc; n++) {
		  if (strcmp (argv[n], "--help") == 0){
			 printf ("DirectFB Benchmarking Demo version\n\n");
			 printf ("Usage: df_error [options]\n\n");
			 printf ("Options:\n\n");
			 printf ("By default all application are working\n\n");
			 printf ("  --time     <milliseconds>    Duration of sleep in execution.\n");
			 printf ("  --video   					 Opening file '_Statistics_' .\n");
			 printf ("  --audio1  					 Opening file '_PP0State_' .\n");
			 printf ("  --audio2  					 Opening file '_DriverState_' .\n");
			 printf ("  --pti                        Opening file '_SlotInfo_'.\n");
			 printf ("  --mem                        Opening file '_FreeStatus_'..\n");
			 printf ("  --help                       Print usage information.\n");
			 printf ("The following options allow to specify which benchmarks to run.\n");
			 printf ("If none of these are given, all benchmarks are run.\n\n");
			 all_init=0;
		  }
          else if (strcmp (argv[n], "--time") == 0) {
			  par=argv[++n];
			  set_sleep=atoi(par);
			  printf("time executed  %ld \n", set_sleep);
		  }
		  else if (strcmp (argv[n], "--video")== 0){
			  video_init=1;
			  all_init=0;
		  }
		  else if (strcmp (argv[n], "--audio1")==0){
			  audio1_init=1;
			  all_init=0;
		  }
		  else if (strcmp (argv[n], "--audio2")==0){
			  audio2_init=1;
			  all_init=0;
		  }
		  else if (strcmp (argv[n], "--pti")==0){
			  pti_init=1;
			  all_init=0;
		  }
		  else if (strcmp (argv[n], "--avmem")==0){
			  avmem_init=1;
			  all_init=0;
		  }
	}
{ 
	/*********rectangle primaire***********/
    r0.h=12; r0.w=315; r0.x=5; r0.y=5;
    r2.h=12; r2.w=315; r2.x=345; r2.y=116;
    r4.h=12; r4.w=315; r4.x=345; r4.y=217;
    r6.h=12; r6.w=315; r6.x=345; r6.y=464;
    r8.h=12; r8.w=525; r8.x=685; r8.y=5;
    
    /*********rectangle secondaire***********/
    y1=31; x1=12;
    y2=31; x2=270;
    
    r1.h=10; r1.w=310; r1.x=10; r1.y=19;
    r3.h=10; r3.w=310; r3.x=350; r3.y=141;
    r5.h=10; r5.w=310; r5.x=350; r5.y=232;
    r7.h=10; r7.w=310; r7.x=350; r7.y=478;
    r9.h=10; r9.w=520; r9.x=690; r9.y=19;
    
    y3=31 ;
    y4= 153;
    y5=244;
}
	while(1){
		
		if((video_init==1)||(all_init==1)){
			fich_video = fopen("/proc/STAPI/stvid/Statistics", "r");	
			if(fich_video != NULL){
			
				/**********partie de la recherche dans le fichier de video******/		

					c1=fgetc(fich_video);
					fscanf(fich_video,"%s\n",rech1);
					while((strcmp(rech1,chaine2)!=0)&& (c1!=EOF)){
						fscanf(fich_video,"%s\n" ,rech1);
					}

					fscanf(fich_video,"%s\n", egale);

				/******intialisation du rectangle primaire*******/
				DFBCHECK (title->SetColor (title, 0xB5, 0xFF, 0, 0x25));
				DFBCHECK (title->FillRectangle (title,r0.x ,r0.y , r0.w, r0.h));
					
				DFBCHECK (title->SetColor (title, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (title->DrawString (title, "VIDEO - Statistics", -1, 10, 18, DSTF_BOTTOMLEFT));
				/************************************************************/
				for (j=0; j<71;j++)
				{
					fgets(ligne_video,48,fich_video);
					fscanf(fich_video,"%d\n",&vid_int);
					/*****convertion de int vers chaine*****/
					sprintf(conv_char,"%d",vid_int);

					DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
					DFBCHECK (surf_video->FillRectangle (surf_video,r1.x ,r1.y , r1.w, r1.h));
					
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, ligne_video, -1, x1,  y1, DSTF_BOTTOMLEFT));
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, conv_char, -1, x2,  y2, DSTF_BOTTOMLEFT));
					
					y1+= 11;
					y2+=11;
					r1.y+=11;
					if(y1== 724){
						r1.x=350;
						r1.y=5;
						y1= 17; x1=355;
						y2=17; x2= 593;
					}
				}
				y1=31; x1=12;
				y2=31; x2=270;
				/*********rectangle secondaire***********/
				r1.h=10; r1.w=305; r1.x=10; r1.y=19;
		
			}
			else{
				printf("erreur d'ouverture du fich_video");
			}
			fclose(fich_video);
		}
		
		 if((audio1_init==1)||(all_init==1)){
			fich_audio1 = fopen("/proc/staudlx_core/PP0State", "r");	
			if(fich_audio1 != NULL){
				/******intialisation du rectangle primaire*******/
				DFBCHECK (title->SetColor (title, 0xB5, 0xFF, 0, 0x25));
				DFBCHECK (title->FillRectangle (title,r2.x ,r2.y , r2.w, r2.h));
				DFBCHECK (title->SetColor (title, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (title->DrawString (title, "AUDIO 1 - PP0State", -1, 350, 129, DSTF_BOTTOMLEFT));
				
				/***pointage au début du fichier***/
				do{
					fseek(fich_audio1,0,SEEK_SET);
					fgets(egale,7,fich_audio1);
				}while(strcmp(egale,chaine5)!=0);
				
				fseek(fich_audio1,0,SEEK_SET);
				fgets(rech4,13,fich_audio1);
				
				do{
					fgets(rech5,1,fich_audio1);
					fscanf(fich_audio1,"%c",elem1);
				}while(strcmp("=",elem1)!=0);
				
				fscanf(fich_audio1,"%s\n",rech6);

				
				DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
				DFBCHECK (surf_video->FillRectangle (surf_video,350 ,130 , r3.w, r3.h));
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, 142, DSTF_BOTTOMLEFT));
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 450,142, DSTF_BOTTOMLEFT));
				
				for (j=0; j<5;j++)
				{
					
					fgets(rech4,13,fich_audio1);
				
					do{
						fgets(rech5,1,fich_audio1);
						fscanf(fich_audio1,"%c",elem1);
					}while(strcmp("=",elem1)!=0);
					
					fscanf(fich_audio1,"%s\n",rech6);
					
					DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
					DFBCHECK (surf_video->FillRectangle (surf_video,r3.x ,r3.y , r3.w, r3.h));
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, y4, DSTF_BOTTOMLEFT));
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 450,y4, DSTF_BOTTOMLEFT));
					
					
					y4+=11;
					r3.y+=11;
				}
				y4=153;
				r3.h=10; r3.w=305; r3.x=350; r3.y=141;
			
			}
			else{
				printf("erreur d'ouverture du fich_audio1");
			}
			fclose(fich_audio1);
		}
		
		 if((audio2_init==1)||(all_init==1)){
			fich_audio2 = fopen("/proc/staudlx_core/DriverState", "r");	
			if(fich_audio2 != NULL){
				/******intialisation du rectangle primaire*******/
				DFBCHECK (title->SetColor (title, 0xB5, 0xFF, 0, 0x25));
				DFBCHECK (title->FillRectangle (title,r4.x ,r4.y , r4.w, r4.h));
				DFBCHECK (title->SetColor (title, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (title->DrawString (title, "AUDIO 2 - DriverState", -1, 350, 230, DSTF_BOTTOMLEFT));
				
				/***pointage au début du fichier***/
				do{
					fseek(fich_audio2,0,SEEK_SET);
					fscanf(fich_audio2,"%c",elem2);
					fgets(rech2,27,fich_audio2);
				}while(strcmp(rech2,chaine6)!=0);
				
				fseek(fich_audio2,1,SEEK_SET);
				
				for (j=0; j<2;j++)
				{
					fgets(rech3,27,fich_audio2);
					fscanf(fich_audio2,"\n");
					DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
					DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
					
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, rech3, -1, 355, y5, DSTF_BOTTOMLEFT));
					
					r5.y+=11;
					y5+=11;
					
					fgets(rech4,19,fich_audio2);

					do{
						fgets(recherch1,1,fich_audio2);
						fscanf(fich_audio2,"%c",elem1);
					}while(strcmp("=",elem1)!=0);
					
					fscanf(fich_audio2,"%s\n",rech6);
					
					DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
					DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
					
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, y5, DSTF_BOTTOMLEFT));
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 480,y5, DSTF_BOTTOMLEFT));
					
					r5.y+=11;
					y5+=11;
					
					fgets(rech4,15,fich_audio2);
				
					do{
						fgets(rech5,1,fich_audio2);
						fscanf(fich_audio2,"%c",elem1);
					}while(strcmp("=",elem1)!=0);
					
					fscanf(fich_audio2,"%s\n",rech6);
					
					DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
					DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
					
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, y5, DSTF_BOTTOMLEFT));
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 480,y5, DSTF_BOTTOMLEFT));
					
					r5.y+=11;
					y5+=11;
					
					fgets(rech4,16,fich_audio2);
				
					do{
						fgets(rech5,1,fich_audio2);
						fscanf(fich_audio2,"%c",elem1);
					}while(strcmp("=",elem1)!=0);
					
					fscanf(fich_audio2,"%s\n",rech6);
					
					
					DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
					DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
					
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, y5, DSTF_BOTTOMLEFT));
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 480,y5, DSTF_BOTTOMLEFT));
					
					r5.y+=11;
					y5+=11;
					
					fgets(rech4,15,fich_audio2);
				
					do{
						fgets(rech5,1,fich_audio2);
						fscanf(fich_audio2,"%c",elem1);
					}while(strcmp("=",elem1)!=0);
					
					fscanf(fich_audio2,"%s\n",rech6);
					n=sizeof(rech6);
					fscanf(fich_audio2,"%s\n",rech7);
				
					DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
					DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
					
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, y5, DSTF_BOTTOMLEFT));
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 480,y5, DSTF_BOTTOMLEFT));
					
					r5.y+=11;
					y5+=11;
				}
				fscanf(fich_audio2,"\n");
				fgets(rech3,27,fich_audio2);
				fscanf(fich_audio2,"\n");
				DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
				DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech3, -1, 355, y5, DSTF_BOTTOMLEFT));
								
				r5.y+=11;
				y5+=11;
				
				fgets(rech4,15,fich_audio2);
				
				do{
					fgets(rech5,1,fich_audio2);
					fscanf(fich_audio2,"%c",elem1);
				}while(strcmp("=",elem1)!=0);
				
				fscanf(fich_audio2,"%s\n",rech6);
				
				DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
				DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
				
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, y5, DSTF_BOTTOMLEFT));
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 480,y5, DSTF_BOTTOMLEFT));
					
				r5.y+=11;
				y5+=11;
				
				fgets(rech4,12,fich_audio2);
				do{
					fgets(rech5,1,fich_audio2);
					fscanf(fich_audio2,"%c",elem1);
				}while(strcmp("=",elem1)!=0);
				
				fscanf(fich_audio2,"%s\n",rech6);
				
				DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
				DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
				
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, y5, DSTF_BOTTOMLEFT));
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 480,y5, DSTF_BOTTOMLEFT));
					
				r5.y+=11;
				y5+=11;
					
				fgets(rech4,21,fich_audio2);
				
				do{
					fgets(rech5,1,fich_audio2);
					fscanf(fich_audio2,"%c",elem1);
				}while(strcmp("=",elem1)!=0);
					
				fscanf(fich_audio2,"%s\n",rech6);
					
				DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
				DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
					
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, y5, DSTF_BOTTOMLEFT));
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 480,y5, DSTF_BOTTOMLEFT));
					
				r5.y+=11;
				y5+=11;
				
				fscanf(fich_audio2,"\n");
				fgets(rech3,27,fich_audio2);
				fscanf(fich_audio2,"\n");
				DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
				DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech3, -1, 355, y5, DSTF_BOTTOMLEFT));
								
				r5.y+=11;
				y5+=11;
				
				fgets(rech4,15,fich_audio2);
				
				do{
					fgets(rech5,1,fich_audio2);
					fscanf(fich_audio2,"%c",elem1);
				}while(strcmp("=",elem1)!=0);
				
				fscanf(fich_audio2,"%s\n",rech6);
				
				DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
				DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
				
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, y5, DSTF_BOTTOMLEFT));
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 480,y5, DSTF_BOTTOMLEFT));
					
				r5.y+=11;
				y5+=11;
				
				fgets(rech4,12,fich_audio2);
				do{
					fgets(rech5,1,fich_audio2);
					fscanf(fich_audio2,"%c",elem1);
				}while(strcmp("=",elem1)!=0);
				
				fscanf(fich_audio2,"%s\n",rech6);
				
				DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
				DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
				
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, y5, DSTF_BOTTOMLEFT));
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 480,y5, DSTF_BOTTOMLEFT));
					
				r5.y+=11;
				y5+=11;
					
				fgets(rech4,21,fich_audio2);
				
				do{
					fgets(rech5,1,fich_audio2);
					fscanf(fich_audio2,"%c",elem1);
				}while(strcmp("=",elem1)!=0);
					
				fscanf(fich_audio2,"%s\n",rech6);

				DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
				DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
					
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, y5, DSTF_BOTTOMLEFT));
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 480,y5, DSTF_BOTTOMLEFT));
					
				r5.y+=11;
				y5+=11;
				
				fgets(rech4,24,fich_audio2);
				
				do{
					fgets(rech5,1,fich_audio2);
					fscanf(fich_audio2,"%c",elem1);
				}while(strcmp("=",elem1)!=0);
				
				fscanf(fich_audio2,"%s\n",rech6);
				
				DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
				DFBCHECK (surf_video->FillRectangle (surf_video,r5.x ,r5.y , r5.w, r5.h));
				
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech4, -1, 355, y5, DSTF_BOTTOMLEFT));
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, rech6, -1, 480,y5, DSTF_BOTTOMLEFT));
				
				fscanf(fich_audio2,"\n");

				y5=244;
				r5.h=10; r5.w=305; r5.x=350; r5.y=232;
			}
			else{
				printf("erreur d'ouverture du fich_audio2");
			}
			fclose(fich_audio2);
		}
		
		if((avmem_init==1)||(all_init==1)){
			fich_avmem = fopen("/proc/STAPI/stavmem/FreeStatus", "r");	
			if(fich_avmem != NULL){
				/******intialisation du rectangle primaire*******/
				DFBCHECK (title->SetColor (title, 0xB5, 0xFF, 0, 0x25));
				DFBCHECK (title->FillRectangle (title,r6.x ,r6.y , r6.w, r6.h));
				DFBCHECK (title->SetColor (title, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (title->DrawString (title, "AVMEM - FreeStatus", -1, 350, 477, DSTF_BOTTOMLEFT));

					cara=fgetc(fich_avmem);

					fscanf(fich_avmem,"\n");
					fgets(ligne_avmem1,32,fich_avmem);
					fscanf(fich_avmem,"\n");
					fgets(ligne_avmem2,31,fich_avmem);

				DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
				DFBCHECK (surf_video->FillRectangle (surf_video,r7.x ,r7.y , r7.w, r7.h));
								
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, ligne_avmem1, -1, 355, 490, DSTF_BOTTOMLEFT));
								
				r7.h=10; r7.w=310; r7.x=350; r7.y=478;
				r7.y+=11;

				DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
				DFBCHECK (surf_video->FillRectangle (surf_video,r7.x ,r7.y , r7.w, r7.h));
							
				DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (surf_video->DrawString (surf_video, ligne_avmem2, -1, 355, 501, DSTF_BOTTOMLEFT));
				
				r7.h=10; r7.w=310; r7.x=350; r7.y=478;
			}
			else{
				printf("erreur d'ouverture du fich_avmem");
			}
			fclose(fich_avmem);
		}
		 if((pti_init==1)||(all_init==1)){
			fich_pti = fopen("/proc/STAPI/stpti/PTI0/SlotInfo", "r");	
			if(fich_pti != NULL){
				
				fseek(fich_pti,71,SEEK_SET);
				
				/******intialisation du rectangle primaire*******/
				DFBCHECK (title->SetColor (title, 0xB5, 0xFF, 0, 0x25));
				DFBCHECK (title->FillRectangle (title,r8.x ,r8.y , r8.w, r8.h));
				DFBCHECK (title->SetColor (title, 0x44, 0x33, 0xDD, 0xA0));
				DFBCHECK (title->DrawString (title, "PTI - SlotInfo", -1, 690, 19, DSTF_BOTTOMLEFT));
				
				for (j=0; j<63;j++)
				{
					fgets(ligne_pti,82,fich_pti);
					fscanf(fich_pti,"%c",elem2);
					
					DFBCHECK (surf_video->SetColor (surf_video, 0xB5, 0xFF, 0xFF, 0));
					DFBCHECK (surf_video->FillRectangle (surf_video,r9.x ,r9.y , r9.w, r9.h));
					
					DFBCHECK (surf_video->SetColor (surf_video, 0x44, 0x33, 0xDD, 0xA0));
					DFBCHECK (surf_video->DrawString (surf_video, ligne_pti, -1, 700, y3, DSTF_BOTTOMLEFT));
					r9.y+=11;
					y3+=11;
				}
				r9.h=10; r9.w=520; r9.x=690; r9.y=19;
				y3=31 ;
				fseek(fich_pti,15700,SEEK_CUR);
				do{
					cara=fgetc(fich_pti);
				}while(cara!=EOF);
				
			}
			else{
				printf("erreur d'ouverture du fich_pti");
			}
			fclose(fich_pti);
		}
		
		usleep(set_sleep);
		DFBCHECK (surf_video->Flip (surf_video, NULL, 0 ));
		
	}	

  font_video->Release (font_video);
  surf_video->Release (surf_video);
  title->Release (title);
  dfb->Release (dfb);
	
  return 23;
}
