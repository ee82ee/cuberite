
// ImageComposingCallback

// Declares the cImageComposingCallback class that implements a subset of cCallback for composing per-region images





#pragma once

#include "Callback.h"




/** Implements the plumbing for composing per-region images from multiple chunks.
To use this class, create a descendant that writes the image data using
SetPixel() or SetPixelURow() functions.

For the purpose of this class the image data is indexed U (horz) * V (vert), to avoid confusion with other coords.
The image is a 32bpp raw imagedata, written into a BMP file.
*/
class cImageComposingCallback :
	public cCallback
{
public:
	enum
	{
		INVALID_REGION_COORD = 99999,  ///< Used for un-assigned region coords
		IMAGE_WIDTH = 32 * 16,
		IMAGE_HEIGHT = 32 * 16,
		PIXEL_COUNT = IMAGE_WIDTH * IMAGE_HEIGHT,  ///< Total pixel count of the image data
	} ;

	cImageComposingCallback(const AString & a_FileNamePrefix);
	virtual ~cImageComposingCallback();

	// cCallback overrides:
	virtual bool OnNewRegion(int a_RegionX, int a_RegionZ) override;
	virtual void OnRegionFinished(int a_RegionX, int a_RegionZ) override;

	// New introduced overridable functions:

	/** Called when a file is about to be saved, to generate the filename */
	virtual AString GetFileName(int a_RegionX, int a_RegionZ);

	/** Called before the file is saved */
	virtual void OnBeforeImageSaved(int a_RegionX, int a_RegionZ, const AString & a_FileName) {}

	/** Called after the image is saved to a file */
	virtual void OnAfterImageSaved(int a_RegionX, int a_RegionZ, const AString & a_FileName) {}

	/** Called when a new region is beginning, to erase the image data */
	virtual void OnEraseImage(void);

	// Functions for manipulating the image:

	/** Erases the entire image with the specified color */
	void EraseImage(int a_Color);

	/** Erases the specified chunk's portion of the image with the specified color. Note that chunk coords are relative to the current region */
	void EraseChunk(int a_Color, int a_RelChunkX, int a_RelChunkZ);

	/** Returns the current region X coord */
	int GetCurrentRegionX(void) const { return m_CurrentRegionX; }

	/** Returns the current region Z coord */
	int GetCurrentRegionZ(void) const { return m_CurrentRegionZ; }

	/** Sets the pixel at the specified UV coords to the specified color */
	void SetPixel(int a_RelU, int a_RelV, int a_Color);

	/** Returns the color of the pixel at the specified UV coords; -1 if outside */
	int GetPixel(int a_RelU, int a_RelV);

	/** Sets a row of pixels. a_Pixels is expected to be a_CountU pixels wide. a_RelUStart + a_CountU is assumed less than image width */
	void SetPixelURow(int a_RelUStart, int a_RelV, int a_CountU, int * a_Pixels);

	/** "Shades" the given color based on the shade amount given
	Shade amount 0 .. 63 shades the color from black to a_Color.
	Shade amount 64 .. 127 shades the color from a_Color to white.
	All other shade amounts have undefined results.
	*/
	static int ShadeColor(int a_Color, int a_Shade);

	/** Mixes the two colors in the specified ratio; a_Ratio is between 0 and 256, 0 returning a_Src */
	static int MixColor(int a_Src, int a_Dest, int a_Ratio);

protected:
	/** Prefix for the filenames, when generated by the default GetFileName() function */
	AString m_FileNamePrefix;

	/** Coords of the currently processed region */
	int m_CurrentRegionX, m_CurrentRegionZ;

	/** Raw image data; 1 MiB worth of data, therefore unsuitable for stack allocation. [u + IMAGE_WIDTH * v] */
	int * m_ImageData;

	void SaveImage(const AString & a_FileName);
} ;
