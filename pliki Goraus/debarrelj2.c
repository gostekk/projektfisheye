#include "stdio.h"
#include "string.h"
#include "jpeglib.h"                                                                                                                                                                         


#define BRD 50
#define NX 4912
#define NY 3264
#define PX 3000
#define PY 2000


typedef struct {
unsigned char r,g,b;
} BITMAP3;




BITMAP3 *image,*oimage;
float  max=0, min=255;
int x,y,r=300,x1,x2,y1,y2,dx, dy,xx, yy,NUM;





/*
	Write a JPEG file
	Quality is 0 to 100
*/
int JPEG_Write(FILE *fptr,BITMAP3 *image,int width,int height,int quality)
{
	int i,j;
	struct jpeg_compress_struct cinfo;
	JSAMPROW row_pointer[1];
	JSAMPLE *jimage = NULL;
	int row_stride;
	struct jpeg_error_mgr jerr;

	if ((jimage = malloc(width*3)) == NULL)
		return(1);

	// Error handler
   cinfo.err = jpeg_std_error(&jerr);

	// Initialize JPEG compression object.
	jpeg_create_compress(&cinfo);
	
	// Associate with output stream
	jpeg_stdio_dest(&cinfo,fptr);

	// Fill out values
   cinfo.image_width = width;
   cinfo.image_height = height;
   cinfo.input_components = 3;
   cinfo.in_color_space = JCS_RGB;

	// Default compression settings
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE); // limit to baseline-JPEG values

	// Start cmpressor
	jpeg_start_compress(&cinfo, TRUE);

	row_stride = width * 3;
	row_pointer[0] = jimage;
	j = cinfo.image_height - 1;
	while (cinfo.next_scanline < cinfo.image_height) {
		for (i=0;i<width;i++) {
			jimage[3*i  ] = image[j*width+i].r;
         jimage[3*i+1] = image[j*width+i].g;
         jimage[3*i+2] = image[j*width+i].b;
		}
      jpeg_write_scanlines(&cinfo,row_pointer,1);
		j--;
   }

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	free(jimage);
	return(TRUE);
}

/*
	Get dimensions of a JPEG image
*/
int JPEG_Info(FILE *fptr,int *width,int *height,int *depth)
{
	int row_stride;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPARRAY buffer;	

   // Error handler
   cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, fptr);

	// Read header
	jpeg_read_header(&cinfo, TRUE);

	// Read one scan line
	jpeg_start_decompress(&cinfo);
   row_stride = cinfo.output_width * cinfo.output_components;
   buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
   while (cinfo.output_scanline < cinfo.output_height) 
      jpeg_read_scanlines(&cinfo,(JSAMPARRAY)buffer,1);

	*width = cinfo.output_width;
	*height = cinfo.output_height;
	*depth = 8*cinfo.output_components;

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	rewind(fptr);

	return(TRUE);
}

/*
	Read a JPEG image
*/
int JPEG_Read(FILE *fptr,BITMAP3 *image,int *width,int *height)
{
	int i,j,n;
	int row_stride;
   struct jpeg_decompress_struct cinfo;
   struct jpeg_error_mgr jerr;
	JSAMPLE *buffer;

   // Error handler
   cinfo.err = jpeg_std_error(&jerr);

   jpeg_create_decompress(&cinfo);
   jpeg_stdio_src(&cinfo, fptr);

   // Read header
   jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

   *width = cinfo.output_width;
   *height = cinfo.output_height;

	// Can only handle RGB JPEG images at this stage
	if (cinfo.output_components != 3) 
		return(1);

	// buffer for one scan line
	row_stride = cinfo.output_width * cinfo.output_components;
	if ((buffer = malloc(row_stride * sizeof(JSAMPLE))) == NULL) 
		return(2);

	j = cinfo.output_height-1;
   while (cinfo.output_scanline < cinfo.output_height) {
      n = jpeg_read_scanlines(&cinfo,&buffer,1);
		for (i=0;i<cinfo.output_width;i++) {
			image[j*cinfo.output_width+i].r = buffer[3*i];
         image[j*cinfo.output_width+i].g = buffer[3*i+1];
         image[j*cinfo.output_width+i].b = buffer[3*i+2];

		}
		j--;
   }

	// Finish
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(buffer);

   return(0);
}














int main(int argc, char **argv)
{
int W,H, bpp,ix,iy,i,n=8,WO,HO,xd,yd,dep;

float foc=5.8/27.0,r,xf,yf,cosfi,sinfi,D,DO;




FILE *f,*fo,*fo2;
char *buf=(char*)malloc(256);
f=fopen(argv[1],"r");
strcpy(buf,"_");
strcat(buf,argv[1]);
//for (i=0; i<strlen(buf); i++) buf[i]=tolower(buf[i]);
fo=fopen(buf,"w");
if (f==NULL) { printf("can't open file\n");  return -1;}
JPEG_Info(f,&W,&H,&dep);
image=(BITMAP3*)malloc(W*H*sizeof(BITMAP3));
JPEG_Read(f,image,&W,&H);
WO=W*PX/NX;
HO=H*PY/NY;
//D=foc*sqrt(W*W+H*H)*1.3;
DO=foc*sqrt(WO*WO+HO*HO);
D=foc*sqrt(W*W+H*H)*1.25;
oimage=(BITMAP3*)malloc(WO*HO*sizeof(BITMAP3));
//#pragma omp parallel for shared(image, oimage) private(ix, j)

for (ix=0; ix<WO; ix++)
#pragma omp parallel for  shared(image, oimage) private(xf,yf,r,cosfi,sinfi,xd,yd)
for (iy=0; iy<HO; iy++)
{
xf=(float)(ix-WO/2);
yf=(float)(iy-HO/2);
r=sqrtf(xf*xf+yf*yf);
cosfi=xf/r;
sinfi=yf/r;
xf=cosfi*2*D*tanf(0.5*atan2f(r,DO));
yf=sinfi*2*D*tanf(0.5*atan2f(r,DO));
xd=(int)xf;
yd=(int)yf;
xd+=W/2;
yd+=H/2;
if ((xd>0)&&(xd<NX)&&(yd>0)&&(yd<NY)) 
 {
 oimage[iy*WO+ix].r=image[yd*W+xd].r;
 oimage[iy*WO+ix].g=image[yd*W+xd].g;
 oimage[iy*WO+ix].b=image[yd*W+xd].b;
 } else 
 {
 oimage[iy*WO+ix].r=0;
 oimage[iy*WO+ix].g=0;
 oimage[iy*WO+ix].b=0;
 }
}

JPEG_Write(fo,oimage,WO,HO,85);
free(oimage);
free(image);

    return 0;
}

