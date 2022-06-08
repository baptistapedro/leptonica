#include <iostream>
#include <sstream>
#include <allheaders.h>

using namespace std;
void fuzz_jpg_file(char *fileName) {
	Pix* pixOrg = pixRead(fileName);
	Pix* pixg = pixConvertRGBToLuminance(pixOrg);
	Pixa* pixaDisp = pixaCreate(0);

	Pix* pixContrast = pixContrastNorm(NULL, pixg, 100, 100, 55, 4, 4);
	Pix* pixSharp = pixUnsharpMasking(pixg,5,0.4); //make image sharper by amplifying high frequency parts
	Pix* pixNorm = pixEqualizeTRC(NULL,pixg,0.5,1);//histogram equalization. brings out more detail
	Pix* pixEdge = pixTwoSidedEdgeFilter(pixg,L_VERTICAL_EDGES); //find edges

	pixWrite("edges.bmp",pixEdge,IFF_BMP);
	pixInvert(pixEdge,pixEdge);  //invert and binarize to bring out the edges
	Pix* pixb = pixThresholdToBinary(pixEdge,252);

	pixaAddPix(pixaDisp, pixOrg, L_CLONE);
	pixaAddPix(pixaDisp, pixContrast, L_CLONE);
	pixaAddPix(pixaDisp, pixSharp, L_CLONE);
	pixaAddPix(pixaDisp, pixNorm, L_CLONE);
	pixaAddPix(pixaDisp, pixb, L_CLONE);
	Pix* pixd = pixaDisplayTiledAndScaled(pixaDisp, 32, 600, 3, 0, 25, 2);
	pixDisplay(pixd,0,0);

	pixaDestroy(&pixaDisp);
	pixDestroy(&pixg);
	pixDestroy(&pixNorm);
	pixDestroy(&pixOrg);
	pixDestroy(&pixSharp);
	pixDestroy(&pixb);
	pixDestroy(&pixEdge);
	pixDestroy(&pixContrast);
	pixDestroy(&pixd);
}
int main(int argc, char **argv) {
 if (argc < 2) return 1;
 fuzz_jpg_file(argv[1]);
 
 return 0;
}
